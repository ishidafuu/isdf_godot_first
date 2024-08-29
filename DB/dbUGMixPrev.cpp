#include "dbUGMixPrev.h"
#include "dbUILayer.h"
#include "dbScene.h"

namespace db
{
	const s32 SHOTNUM = 8;


  //コンストラクタ
	TUGMixPrev::TUGMixPrev(TUIGenLayer* player)
		:TUGGen(player)
		, ugbtn_back_(player)
		, ugbtn_cancel_(player)
		, ugbtn_ok_(player)
		, ugbtn_change_(player)
		, ugbtn_mix_shop_(player)
		, ugbtn_mix_have_(player)
		, ugbtn_select_have_(player)
		, ugpad_shot_(player)
    //, ugpad_pass_(player)
    , ugpad_jump_(player)
    , uglever_(player)
		, ugradio_shot_(player)
  {
    stPrev_.Init();
		for (s32 i2 = 0; i2 < SHOTNUM; ++i2) ugbtn_shot_.push_back(new TUGButton(player));
  }
	TUGMixPrev::~TUGMixPrev()
  {
		for (s32 i2 = 0; i2 < ugbtn_shot_.size(); ++i2) Freemem(ugbtn_shot_[i2]);
  }

	void TUGMixPrev::MakeGroup(TUIObj* parent)
  {
    const s32 BARW = 512;

    //ヘッダフッタ間フィルタ
    field_ = MakeObj();
    field_->o_SetParent(parent);
    field_->g_SetCenter(TRUE);//フィルタはフィルタサイズが小さいためセンタリングが正常に効かない
    field_->o_SetObjFilterGrp(BARW, BARW, 0xFF, 0, 0, 0);
    field_->g_SetAlp(FALSE, 0x80);
    field_->g_SetDepth(UIDP_04HEADER);//この震度で良いか後ほど確認

    court_ = MakeObj();
    court_->o_SetParent(field_);
    court_->g_SetCenter(TRUE);//フィルタはフィルタサイズが小さいためセンタリングが正常に効かない
    court_->z_SetPos(0, 50 - 40);
    court_->o_SetObjFilterGrp(BARW, 128, 0xFF, 0x38, 0x68, 0x9A);
    court_->g_SetDepth(UIDP_06BUTTONHI);

    line_ = MakeObj();
    line_->o_SetParent(court_);
    line_->g_SetCenter(TRUE);
    line_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_line));
    line_->g_SetDepth(UIDP_06BUTTONHI);

    ugbtn_back_.MakeGroup_FtPosCancel(field_);
		ugbtn_cancel_.MakeGroup_FtPosCancel(field_);
		ugbtn_cancel_.SetFont("やめる");
		ugbtn_cancel_.SetDraw(FALSE);
		ugbtn_ok_.MakeGroup_FtPos(field_, 1, "これでミックス");
		ugbtn_ok_.SetDraw(FALSE);
		ugbtn_change_.MakeGroup_FtPos(field_, 2, "ベースをかえる");
		ugbtn_change_.SetDraw(FALSE);
		/* 順番入れ替え */
		ugbtn_mix_have_.MakeGroup_FtPos(field_, 3, "てもちランダム");
		ugbtn_mix_have_.SetDraw(FALSE);
		ugbtn_select_have_.MakeGroup_FtPos(field_, 4, "てもちせんたく");
		ugbtn_select_have_.SetDraw(FALSE);

    ugpad_shot_.MakeGroup_ShotPrev(field_);
    ugpad_jump_.MakeGroup_JumpPrev(field_);
    uglever_.MakeGroup(field_, TRUE);

		ugradio_shot_.MakeGroup(field_, -160, -72);//ラジオボタン
		ugradio_shot_.SetDepth(UIDP_06BUTTONHI);
		
		for (auto& slot : slot_)
		{
			slot = MakeObj();
			slot->o_SetParent(field_);
			slot->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_rarity));
			slot->g_SetRevDepthLv2(UIDP_07BUTTONTOP);
			slot->g_SetDraw(FALSE);//けしとく
		}

		// 最初はタッチアイコンを非表示
		selectMode = enUGMixPrevSelectMode_none;
		baserarelity = 0;
		baseposition = 0;
		fromno = -1;
		tono = -1;

		changeVec_.clear();
		id_shot_mixvec_custum.clear();

    SetDraw(FALSE);
  }
  // void TUGMixPrev::Ready(s64 id_team, s64 id_shot, BOOL backbtn_f)
  // {
  //   //フッタ位置キャンセル
  //   mid::midTouchFooterPosInput(FALSE);
  //   stPrev_.mvgame_f_ = TRUE;

  //   //ゲームオブジェなど準備
  //   BOOL needlaod_f = FALSE;
  //   if (id_team <= 0)//一戦も戦ってないとゼロになる
  //   {
  //     id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(0, mdteam_pk);
  //   }
  //   
  //   if (stPrev_.id_team_prev_ != id_team)
  //   {
  //     stPrev_.id_team_prev_ = id_team;
  //     needlaod_f = TRUE;
  //   }
  //   //pGame_には直接引数を与えずpmgEO_->mgStSn_.EtcSSに渡して管理する
  //   //プレビューフラグ、シュートＩＤもEtcSSに渡す
  //   s64 mid_team = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_m_team);
  //   pmgEO_->stShiai_.SetPrev(id_team, mid_team, id_shot, pmgEO_->mgCoU_.GetCtrl());
  //   player_->pGenScene_->pGame_->Ready_Prev(needlaod_f, mid_team, id_team);

  //   ugbtn_back_.SetDraw(backbtn_f);
  //ugbtn_mix_.SetDraw(FALSE);	// 消し忘れ防止
  //ugbtn_change_.SetDraw(FALSE);
  //   SetDraw(TRUE);
  // }
  void TUGMixPrev::Ready(const TMixPrevParam& param)
  {
    //フッタ位置キャンセル
    mid::midTouchFooterPosInput(FALSE);
    stPrev_.mvgame_f_ = TRUE;

    //ゲームオブジェなど準備
    BOOL needlaod_f = FALSE;
    s64 id_team = param.id_team;
    if (id_team <= 0) id_team = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_team);
    if (id_team <= 0) id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(0, mdteam_pk);

    if ((stPrev_.id_team_prev_ != id_team) || (stPrev_.char0_ != param.char0))
    {
      stPrev_.id_team_prev_ = id_team;
      stPrev_.char0_ = param.char0;
      needlaod_f = TRUE;
    }

    //pGame_には直接引数を与えずpmgEO_->mgStSn_.EtcSSに渡して管理する
    //プレビューフラグ、シュートＩＤもEtcSSに渡す
    s64 mid_team = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_m_team);

		// キャラのグラフィックが壊れてしまうのでチームは0番に固定
		{
			id_team = NGNUM;
			mid_team = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(1, enmdm_Team::mdm_team_pk);
		}

		s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
    pmgEO_->stShiai_.SetPrev(id_kantoku, id_team, mid_team, param.id_shot, pmgEO_->mgCoU_.GetCtrl());
    player_->pGenScene_->pGame_->Ready_Prev(needlaod_f, mid_team, id_team, 
      param.char0, param.char1, param.char2, param.char3, param.char4);

    ugbtn_back_.SetDraw(!(param.nonebackbtn_f));
		ugbtn_cancel_.SetDraw(FALSE);
    ugbtn_ok_.SetDraw(!(param.nonemixbtn_f));
		ugbtn_change_.SetFont("ベースをかえる");
		ugbtn_change_.SetDraw(!(param.nonemixbtn_f));

		/* 2015/06/18 Koike Added */
		ugbtn_mix_have_.SetDraw(!(param.nonehavemixbtn_f));

    SetDraw(TRUE);

    if (param.sozaivec != NULL)
    {
      ChangeShot_Sozai(*(param.sozaivec));
    }

		ClearSlot();
		SOZAIDTVECTOR basedtvec = pmgEO_->mgCoU_.GetSozaiDataAll(param.id_shot);
		changeVec_ = pmgEO_->mgCoU_.GetSozaiDataAll(param.id_shot);
		id_shot_mixvec_custum.push_back(param.id_shot);
		for (s32 i2 = 0; i2 < SHOTNUM; ++i2)
		{
			if (basedtvec.size() <= i2) break;
			SetSlot(i2, basedtvec[i2].mid_sozai_, basedtvec[i2].rarity_, basedtvec[0].rarity_);
		}

		// ラジオボタンのリセット
		ugradio_shot_.Clear();
		// レアリティに応じたボタン数を表示
		for (s32 i2 = 0; i2 < basedtvec[0].rarity_ + 2; ++i2)
		{
			//ドロップアイコンフレーム
			ugbtn_shot_[i2]->MakeGroup_Radio(field_, ug_btn_icon, ug_btn_icon2);//ドロップアイコンの下地用意する
			ugbtn_shot_[i2]->SetDepth(UIDP_06BUTTONHI);
			ugbtn_shot_[i2]->SetModal(TRUE);
			ugbtn_shot_[i2]->SetDraw(TRUE);
			//ugbtn_shot_[i2]->SetSelect(FALSE);
			ugradio_shot_.AddButton(ugbtn_shot_[i2], 24);
			ugradio_shot_.GetSelect(i2)->SetSelect(FALSE);
		}
		ugradio_shot_.SetDraw(TRUE);

		selectMode = enUGMixPrevSelectMode_none;
		baserarelity = basedtvec[0].rarity_;
		if (baserarelity != 0) baseposition = 2;
		else baseposition = 1;
		fromno = -1;
  }
  //シュートの中身切り替え
  void TUGMixPrev::ChangeShot_Sozai(const SOZAIDTVECTOR& sozaivec)
  {
    player_->pGenScene_->pGame_->mgGO_.pmgCh_[SIDE0][0]->chCommon_.ChangeStShotPrev(sozaivec, 0, 0);
  }

	void TUGMixPrev::ClearSlot()
	{
		for (s32 i = 0; i < HSELEMMAX; ++i)
		{
			slot_[i]->g_SetDraw(FALSE);
		}
	}

  void TUGMixPrev::SetMixSozai(s64 id_sozai, BOOL isRndSet)
  {
    ResetMixSozai(id_sozai, isRndSet);
  }
  void TUGMixPrev::SetMixShot(s64 id_shot, BOOL isRndSet)
  {
    ResetMixShot(id_shot, isRndSet);
  }

  void TUGMixPrev::ChangeShot_Mixed(LONGVECTOR* id_shot_mixvec)
  {
    s64 id_shot = id_shot_mixvec->at(0);
    SOZAIDTVECTOR mixedvec;//すべてあわせたやつ
    //素材のＭＩＤとレアリティ
    SOZAIDTVECTOR basedtvec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);
    INTVECTOR mid_sozai_vec;
    //素材名
    //ugname_.SetNameSozai(basedtvec[0].mid_sozai_, basedtvec[0].rarity_, FALSE);
    s32 totalrevpow = 0;
    s32 totalrevspin = 0;
    s32 slotno = 0;
    for (auto& id_shot : *id_shot_mixvec)
    {
      //素材のＭＩＤとレアリティ
      SOZAIDTVECTOR sozaivec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);
      for (auto& sozai : sozaivec)
      {
        mixedvec.push_back(sozai);//すべてあわせたやつ
        mid_sozai_vec.push_back((sozai.mid_sozai_ * SOZAIMID) + sozai.rarity_);
        //個体値
        s32 revpow = 0;
        s32 revspin = 0;
        pmgEO_->mgCoM_.GetRevPowSpin(sozai, &revpow, &revspin);
        totalrevpow += revpow;
        totalrevspin += revspin;
        ++slotno;
      }
    }

    //基礎値
    s32 basepow = 0;
    s32 basespin = 0;
    pmgEO_->mgCoM_.GetBasePowSpin(basedtvec[0], &basepow, &basespin, FALSE);
    totalrevpow += basepow;
    totalrevspin += basespin;
    player_->pGenScene_->pGame_->mgGO_.pmgCh_[SIDE0][0]->chCommon_.ChangeStShotPrev(mixedvec, totalrevpow, totalrevspin);
		
		/* セットされた素材をセット */
		ClearSlot();
		for (s32 i = 0; i < mixedvec.size(); i++)
		{
			SetSlot(i, mixedvec[i].mid_sozai_, mixedvec[i].rarity_, mixedvec[0].rarity_);
		}

		// 一旦全部リセットする。
		changeVec_.clear();
		changeVec_ = mixedvec;

		id_shot_mixvec_custum.clear();
		for (s32 i = 0; i < id_shot_mixvec->size(); i++)
		{
			id_shot_mixvec_custum.push_back(id_shot_mixvec->at(i));
		}

		if (id_shot_mixvec->size() == 1)	ugbtn_change_.SetFont("ベースをかえる");
		else ugbtn_change_.SetFont("さいごをはずす");
  }

	void TUGMixPrev::Down()
	{
		player_->pGenScene_->pGame_->mgGO_.pmgCommon_->ResetPopupMsg();

		stPrev_.mvgame_f_ = FALSE;
		ugbtn_ok_.SetDraw(FALSE);
		ugbtn_change_.SetDraw(FALSE);
		ugbtn_mix_have_.SetDraw(FALSE);
		ugbtn_select_have_.SetDraw(FALSE);
		ugbtn_cancel_.SetDraw(FALSE);
		SetDraw(FALSE);
		//フッタ位置キャンセル解除
		mid::midTouchFooterPosInput(TRUE);
		// 最初はタッチアイコンを非表示
		changeVec_.clear();
		id_shot_mixvec_custum.clear();
		selectMode = enUGMixPrevSelectMode_none;
		baserarelity = 0;
		baseposition = 0;
		fromno = -1;
		tono = -1;
  }

  void TUGMixPrev::SetPadSmart(TMgPad* pmgPad)
  {
    //スマート
    pmgPad->SetBitSmart();
    //水平斬り入力
    mid::midSetUseHInput();
  }
  void TUGMixPrev::SetPadRetro(TMgPad* pmgPad)
  {
    //レトロ
    pmgPad->SetBitRetro();

    //四等分入力
    mid::midSetUseT1Input();
  }

  void TUGMixPrev::MvPrevGame()
  {
		//ＵＩ入力を反映（全ての操作タイプでおそらく共通）
		mid::midSetUseUIInput();

		//とりあえずSIDE0のパッド
		TMgPad* pmgPad = player_->pGenScene_->pGame_->mgGO_.pmgPad_[0];

		//強制レトロ操作
		//SetPadSmart(pmgPad);
		SetPadRetro(pmgPad);

		player_->pGenScene_->pGame_->Update();
  }


  void TUGMixPrev::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    court_->Update(draw_f);
    line_->Update(draw_f);
    ugbtn_back_.Update(draw_f);
		ugbtn_cancel_.Update(draw_f);
    ugbtn_ok_.Update(draw_f);
    ugbtn_change_.Update(draw_f);
		ugbtn_mix_have_.Update(draw_f);
		ugradio_shot_.Update(draw_f);
		ugbtn_select_have_.Update(draw_f);

		for (s32 i = 0; i < HSELEMMAX; ++i) slot_[i]->Update(draw_f);

    //if (ugbtn_ctrl_.IsSmart())
    //{
    //}
    //else if (ugbtn_ctrl_.IsRetro())
    //強制レトロ表示

		// 選択モードの時は非表示にする
		if (selectMode != enUGMixPrevSelectMode_select)
		{
			ugpad_shot_.Update(draw_f);
			//ugpad_pass_.Update(draw_f);
			ugpad_jump_.Update(draw_f);
			uglever_.Update(draw_f);

			if (draw_f == FALSE)
			{
				if (stPrev_.mvgame_f_) MvPrevGame();
			}
			else
			{
				if (stPrev_.mvgame_f_) player_->pGenScene_->pGame_->Draw();
			}
		}
	}

  void TUGMixPrev::DoAction()
  {
    if (ugbtn_back_.IsAct())
    {
      SetAct(enUGMixPrevAct_back);
			ResetField(TRUE);
    }
		else if (ugbtn_cancel_.IsAct())
		{
			SetAct(enUGMixPrevAct_cancel);
			if (selectMode == enUGMixPrevSelectMode_select) ResetField(TRUE);
		}
    else if (ugbtn_ok_.IsAct())
    {
			SetAct(enUGMixPrevAct_ok);
			if (selectMode == enUGMixPrevSelectMode_select) ResetField(TRUE);
    }
    else if (ugbtn_change_.IsAct())
    {
      // ここでボタン非表示
			if (selectMode == enUGMixPrevSelectMode_exchange)
			{
				selectMode = enUGMixPrevSelectMode_none;
				s32 icon = fromno;
				if (fromno == 1) icon = 0;
				else if (fromno == 2) icon = 1;
				ugradio_shot_.GetSelect(icon)->SetSelect(FALSE);
				ugbtn_shot_[icon]->Update(FALSE);
			}
			SetAct(enUGMixPrevAct_mix_clear);
    }
    else if (player_->pGame_->IsPrevReturn())
    {
			SetAct(enUGMixPrevAct_hit);
    }
		else if (ugbtn_mix_have_.IsAct())
		{
			ugbtn_select_have_.SetDraw(FALSE);
			// 交換モードの時に押されたら一度解除する
			if (selectMode == enUGMixPrevSelectMode_exchange) selectMode = enUGMixPrevSelectMode_none;
			SetAct(enUGMixPrevAct_have_mix);
		}
		else if (ugbtn_select_have_.IsAct())
		{
			SetAct(enUGMixPrevAct_Select_ball);
		}
		// 素材変更操作
		//else if (ugbtn_shot_[0]->IsAct())
		else if (ugbtn_shot_[0]->GetField()->b_IsEnter(FALSE))
		{
			SetAct(enUGMixPrevAct_ball_0);
			//ResetBtn(0);
		}
		else if (ugbtn_shot_[1]->GetField()->b_IsEnter(FALSE))
		{
			// ベース素材の場合は押せない
			if (baseposition != 1)
			{
				SetAct(enUGMixPrevAct_ball_1);
				//ResetBtn(1);
			}
			else
			{
				ugradio_shot_.GetSelect(1)->SetSelect(FALSE);
				ugbtn_shot_[1]->Update(FALSE);
				if (selectMode == enUGMixPrevSelectMode_exchange)
				{
					s32 icon = fromno;
					if (fromno == 1) icon = 0;
					else if (fromno == 2) icon = 1;
					ugradio_shot_.GetSelect(icon)->SetSelect(TRUE);
					ugbtn_shot_[icon]->Update(TRUE);
				}
			}
		}
		else if (ugbtn_shot_[2]->GetField()->b_IsEnter(FALSE))
		{
			// ベース素材の場合は押せない
			if (baseposition != 2)
			{
				SetAct(enUGMixPrevAct_ball_2);
				//ResetBtn(2);
			}
			else
			{
				ugradio_shot_.GetSelect(2)->SetSelect(FALSE);
				ugbtn_shot_[2]->Update(FALSE);
				if (selectMode == enUGMixPrevSelectMode_exchange)
				{
					s32 icon = fromno;
					if (fromno == 1) icon = 0;
					else if (fromno == 2) icon = 1;
					ugradio_shot_.GetSelect(icon)->SetSelect(TRUE);
					ugbtn_shot_[icon]->Update(TRUE);
				}
			}
		}
		else if (ugbtn_shot_[3]->GetField()->b_IsEnter(FALSE))
		{
			SetAct(enUGMixPrevAct_ball_3);
			//ResetBtn(3);
		}
		else if (ugbtn_shot_[4]->GetField()->b_IsEnter(FALSE))
		{
			SetAct(enUGMixPrevAct_ball_4);
			//ResetBtn(4);
		}
		else if (ugbtn_shot_[5]->GetField()->b_IsEnter(FALSE))
		{
			SetAct(enUGMixPrevAct_ball_5);
			//ResetBtn(5);
		}
		else if (ugbtn_shot_[6]->GetField()->b_IsEnter(FALSE))
		{
			SetAct(enUGMixPrevAct_ball_6);
			//ResetBtn(6);
		}
		else if (ugbtn_shot_[7]->GetField()->b_IsEnter(FALSE))
		{
			SetAct(enUGMixPrevAct_ball_7);
			//ResetBtn(7);
		}
  }
	BOOL TUGMixPrev::IsActRes(enUGMixPrevAct res)
  {
    return (GetActRes() == res);
  }


  // ランダムボタン押した時の素材シャッフル
  void TUGMixPrev::ResetMixSozai(s64 id_sozai, BOOL isRndSet)
  {
    sozaiVec_.clear();

    TSozaiData base;
    base.SetMidSozai(id_sozai);

    sozaiVec_.push_back(base);

    if (isRndSet)
    {
      s32 empty = (id_sozai % SOZAIMID) + 1;
      s32 size = pmgEO_->mgDt_.dtShRnd_.GetIntvec2Size();

      for (s32 i = 0; i < empty; i++)
      {
        /*
        ●分裂抽選・・・１／１２に当たったら　～99（分裂素材）の抽選
        ↓ハズレ
        ●形状抽選・・・１／８に当たったら　201～299（形状素材）の抽選
        ↓ハズレ
        ●波抽選・・・１／６に当たったら　1000～1999（波素材）の抽選
        ↓ハズレ
        2000～　の抽選
        */
        s32 rnd = NGNUM;
        // 分裂抽選
        rnd = mid::midGetRandMn(12);
        if (rnd == 1)
        {
          SelectMixSozai(NGNUM, 99);
          continue;
        }

        // 形状抽選
        rnd = mid::midGetRandMn(8);
        if (rnd == 1)
        {
          SelectMixSozai(201, 299);
          continue;
        }

        // 波抽選
        rnd = mid::midGetRandMn(6);
        if (rnd == 1)
        {
          SelectMixSozai(1000, 1999);
          continue;
        }

        SelectMixSozai(2000, NGNUM);
      }


      for (s32 i = 0; i < pmgEO_->mgDt_.dtShRnd_.GetIntvec2Size(); ++i)
      {
        s32 id = pmgEO_->mgDt_.dtShRnd_.GetDt(i, enDtShRnd::gs_shot_rnd_mid);
      }
    }
    ChangeShot_Sozai(sozaiVec_);
  }

  // ランダムボタン押した時の素材シャッフル
  void TUGMixPrev::ResetMixShot(s64 id_shot, BOOL isRndSet)
  {
    sozaiVec_.clear();

    SOZAIDTVECTOR shotVec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);

    sozaiVec_ = shotVec;

    if (isRndSet)
    {
      s32 empty = pmgEO_->mgCoU_.GetShotFreeSlotNum(id_shot);

      // 空きが無ければ
      // ランダムボタンを消して何もしない
      if (empty == 0)
      {
        ugbtn_ok_.SetDraw(FALSE);
        return;
      }

      s32 size = pmgEO_->mgDt_.dtShRnd_.GetIntvec2Size();

      for (s32 i = 0; i < empty; i++)
      {
        /*
        ●分裂抽選・・・１／１２に当たったら　～99（分裂素材）の抽選
        ↓ハズレ
        ●形状抽選・・・１／８に当たったら　201～299（形状素材）の抽選
        ↓ハズレ
        ●波抽選・・・１／６に当たったら　1000～1999（波素材）の抽選
        ↓ハズレ
        2000～　の抽選
        */
        s32 rnd = NGNUM;
        // 分裂抽選
        rnd = mid::midGetRandMn(12);
        if (rnd == 1)
        {
          SelectMixSozai(NGNUM, 99);
          continue;
        }

        // 形状抽選
        rnd = mid::midGetRandMn(8);
        if (rnd == 1)
        {
          SelectMixSozai(201, 299);
          continue;
        }

        // 波抽選
        rnd = mid::midGetRandMn(6);
        if (rnd == 1)
        {
          SelectMixSozai(1000, 1999);
          continue;
        }

        SelectMixSozai(2000, NGNUM);
      }


      for (s32 i = 0; i < pmgEO_->mgDt_.dtShRnd_.GetIntvec2Size(); ++i)
      {
        s32 id = pmgEO_->mgDt_.dtShRnd_.GetDt(i, enDtShRnd::gs_shot_rnd_mid);
      }
    }
    ChangeShot_Sozai(sozaiVec_);
  }

  void TUGMixPrev::SelectMixSozai(s32 lowLimit, s32 upLimit)
  {
    s32 size = pmgEO_->mgDt_.dtShRnd_.GetIntvec2Size();
    while (1)
    {
      s32 sozaiNo = mid::midGetRandMn(size);
      s32 id = pmgEO_->mgDt_.dtShRnd_.GetDt(sozaiNo, enDtShRnd::gs_shot_rnd_mid);

      if (lowLimit != NGNUM && id < lowLimit){ continue; }
      if (upLimit != NGNUM && id > upLimit){ continue; }

      TSozaiData sozai;
      sozai.SetMidSozai(id*SOZAIMID);
      sozaiVec_.push_back(sozai);
      break;
    }
  }

	void TUGMixPrev::SetSlot(s32 slotno, s64 mid_sozai, s32 rarity, s32 baserarity)
	{
		slot_[slotno]->g_SetDraw(TRUE);
		if (mid_sozai >= 0)
		{
			// ベースの素材は文字色赤
			if (slotno == 0)
			{
				//s32 fontCol = FC2_rareD + baserarity;
				slot_[slotno]->f_MakeFont_Len(pmgEO_->mgDt_.dtShElm_.GetDtStr(mid_sozai, szs_one).c_str(), FC2_red, 1);
			}
			else
			{
				slot_[slotno]->f_MakeFont_Len(pmgEO_->mgDt_.dtShElm_.GetDtStr(mid_sozai, szs_one).c_str(), FC2_normal, 1);
			}
		}
		else
		{
			slot_[slotno]->f_MakeFont_Len("？", FC2_normal, 1);
		}
		slot_[slotno]->f_FontPos(7, 0);
		slot_[slotno]->g_SetPartNo(ugr_Ball + rarity);

		// タッチ用のパネルはずれないので固定値で置いておく
		s32 BX = 8;
		s32 DX = 24;

		/*
		BCA45678　という順番で
		軌道が現れることが多いので
		現状の
		0   1   2   3   4   5   6   7
		ベ　１　２　３　４　５　６　７

		から
		１　２　べ　３　４　５　６　７
		という順番で　ボールを　配置するように変更していただけると
		*/
		s32 pos = slotno;
		switch (slotno)
		{
		case 0: // 【ベ】 0→2
			// レアリティＤの時は枠が2つしかないのでその時は1にする
			if (baserarity == 0)
			{
				pos = 1;
			}
			else
			{
				pos = 2;
			}
			break;
		case 1: // 【１】 1→0
			pos = 0;
			break;
		case 2: // 【２】 2→1
			pos = 1;
			break;
		}

		slot_[slotno]->z_SetPos(-180+(BX + (pos * DX)), -80);//レアリティによって位置が変わる
	}

	s32 TUGMixPrev::IsTouchMode(LONGVECTOR* id_shot_mixvec, s32 iconNo)
	{
		s64 id_shot = id_shot_mixvec->at(0);
		SOZAIDTVECTOR basedtvec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);
		s32 baserarity = basedtvec[0].rarity_;
		
		// ベース素材だった場合はそもそも何もしない
		if (baserarity == 0 && iconNo == 1) return selectMode = enUGMixPrevSelectMode_none;
		else if (baserarity > 0 && iconNo == 2) return selectMode = enUGMixPrevSelectMode_none;

		if (selectMode == enUGMixPrevSelectMode_none)
		{
			// ベースのレアリティが低い場合の処理
			if (iconNo == 0) fromno = 1;
			else if (iconNo == 1) fromno = 2;
			else fromno = iconNo;

			if (changeVec_.size() > fromno) selectMode = enUGMixPrevSelectMode_exchange;
			else
			{
				selectMode = enUGMixPrevSelectMode_select;
				// レイアウトの深さを変更
				ResetField(FALSE);
			}
		}
		else if (selectMode == enUGMixPrevSelectMode_exchange)
		{
			// ベースのレアリティが低い場合の処理
			if (iconNo == 0) tono = 1;
			else if (iconNo == 1) tono = 2;
			else tono = iconNo;

			// 同じアイコンの場合は交換モードのまま
			if (fromno == tono) return selectMode = enUGMixPrevSelectMode_exchange;
			
			if (changeVec_.size() > tono)
			{
				ResetBtn(iconNo);
				selectMode = enUGMixPrevSelectMode_none;
				ExchangeBall(id_shot_mixvec);
			}
			else
			{
				selectMode = enUGMixPrevSelectMode_select;
				// レイアウトの深さを変更
				ResetField(FALSE);
			}
		}
		return selectMode;
	}

	void TUGMixPrev::ExchangeBall(LONGVECTOR* id_shot_mixvec)
	{
		s64 id_shot = id_shot_mixvec->at(0);
		// 素材をセーブする
		SOZAIDTVECTOR mixedvec = changeVec_;//すべてあわせたやつ
		//素材のＭＩＤとレアリティ
		SOZAIDTVECTOR basedtvec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);
		//素材名
		s32 totalrevpow = 0;
		s32 totalrevspin = 0;

		// 一度スッキリさせる
		changeVec_.clear();
		id_shot_mixvec_custum.clear();
		s32 sozaino = 0;
		// 素材の入れ替え
		for (auto& sozai : mixedvec)
		{
			//個体値
			s32 revpow = 0;
			s32 revspin = 0;
			if (sozaino == fromno)
			{
				changeVec_.push_back(mixedvec[tono]);
				id_shot_mixvec_custum.push_back(id_shot_mixvec->at(tono));
				pmgEO_->mgCoM_.GetRevPowSpin(mixedvec[tono], &revpow, &revspin);
			}
			else if (sozaino == tono)
			{
				changeVec_.push_back(mixedvec[fromno]);
				id_shot_mixvec_custum.push_back(id_shot_mixvec->at(fromno));
				pmgEO_->mgCoM_.GetRevPowSpin(mixedvec[fromno], &revpow, &revspin);
			}
			else
			{
				changeVec_.push_back(sozai);//すべてあわせたやつ
				id_shot_mixvec_custum.push_back(id_shot_mixvec->at(sozaino));
				pmgEO_->mgCoM_.GetRevPowSpin(sozai, &revpow, &revspin);
			}
			totalrevpow += revpow;
			totalrevspin += revspin;
			sozaino++;
		}

		//基礎値
		s32 basepow = 0;
		s32 basespin = 0;
		pmgEO_->mgCoM_.GetBasePowSpin(basedtvec[0], &basepow, &basespin, FALSE);
		totalrevpow += basepow;
		totalrevspin += basespin;
		player_->pGenScene_->pGame_->mgGO_.pmgCh_[SIDE0][0]->chCommon_.ChangeStShotPrev(changeVec_, totalrevpow, totalrevspin);

		/* セットされた素材をセット */
		ClearSlot();
		for (s32 i = 0; i < changeVec_.size(); i++)
		{
			SetSlot(i, changeVec_[i].mid_sozai_, changeVec_[i].rarity_, changeVec_[0].rarity_);
		}

		if (id_shot_mixvec->size() == 1)	ugbtn_change_.SetFont("ベースをかえる");
		else ugbtn_change_.SetFont("さいごをはずす");
	}

	void TUGMixPrev::SetSelectShot(s64 id_shot)
	{
		SOZAIDTVECTOR shotVec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);
		s32 totalrevpow = 0;
		s32 totalrevspin = 0;

		for (auto& sozai : shotVec)
		{
			changeVec_.push_back(sozai);
			//個体値
			s32 revpow = 0;
			s32 revspin = 0;
			pmgEO_->mgCoM_.GetRevPowSpin(sozai, &revpow, &revspin);
			totalrevpow += revpow;
			totalrevspin += revspin;
		}

		// 素材番号を追加
		id_shot_mixvec_custum.push_back(id_shot);

		//基礎値
		s32 basepow = 0;
		s32 basespin = 0;
		pmgEO_->mgCoM_.GetBasePowSpin(shotVec[0], &basepow, &basespin, FALSE);
		totalrevpow += basepow;
		totalrevspin += basespin;
		player_->pGenScene_->pGame_->mgGO_.pmgCh_[SIDE0][0]->chCommon_.ChangeStShotPrev(changeVec_, totalrevpow, totalrevspin);

		/* セットされた素材をセット */
		ClearSlot();
		for (s32 i = 0; i < changeVec_.size(); i++)
		{
			SetSlot(i, changeVec_[i].mid_sozai_, changeVec_[i].rarity_, changeVec_[0].rarity_);
		}

		// 追加はされるのでフォントの文字は変えておく
		ugbtn_change_.SetFont("さいごをはずす");
	}

	s32 TUGMixPrev::GetTouchMode()
	{
		return selectMode;
	}

	void TUGMixPrev::ResetField(BOOL fieldflg)
	{
		if (fieldflg == TRUE)
		{
			field_->g_SetDepth(UIDP_04HEADER);
			court_->g_SetDepth(UIDP_06BUTTONHI);
			line_->g_SetDepth(UIDP_06BUTTONHI);
			ugradio_shot_.SetDepth(UIDP_06BUTTONHI);
			for (s32 i = 0; i < baserarelity + 2; ++i)
			{
				ugradio_shot_.GetSelect(i)->SetSelect(FALSE);
				ugbtn_shot_[i]->SetDepth(UIDP_06BUTTONHI);
				ugbtn_shot_[i]->Update(TRUE);
			}
			for (auto& slot : slot_) slot->g_SetRevDepthLv2(UIDP_07BUTTONTOP);
			//ugbtn_select_have_.SetDraw(TRUE);
			ugbtn_ok_.SetFont("これでミックス");
			ugbtn_back_.SetFont("ミックスやめる");
			ugbtn_mix_have_.SetDraw(TRUE);
			ugbtn_change_.SetDraw(TRUE);
			ugbtn_back_.SetDraw(TRUE);
			ugbtn_cancel_.SetDraw(FALSE);
			selectMode = enUGMixPrevSelectMode_none;
		}
		else
		{
			field_->g_SetDepth(UIDP_03_5);
			court_->g_SetDepth(UIDP_03_5);
			line_->g_SetDepth(UIDP_03_5);
			ugradio_shot_.SetDepth(UIDP_03_5);
			for (s32 i = 0; i < baserarelity + 2; ++i) ugbtn_shot_[i]->SetDepth(UIDP_03_5);
			for (auto& slot : slot_) slot->g_SetRevDepthLv2(UIDP_03_5);
			//ugbtn_select_have_.SetDraw(FALSE);
			ugbtn_ok_.SetFont("これにする");
			ugbtn_cancel_.SetFont("やめる");
			ugbtn_mix_have_.SetDraw(FALSE);
			ugbtn_change_.SetDraw(FALSE);
			ugbtn_back_.SetDraw(FALSE);
			ugbtn_cancel_.SetDraw(TRUE);
		}
	}

	LONGVECTOR TUGMixPrev::GetChangeSozaiVec()
	{
		return id_shot_mixvec_custum;
	}

	void TUGMixPrev::ResetBtn(s32 btnno)
	{
		if (selectMode == enUGMixPrevSelectMode_exchange)
		{
			for (int i = 0; i < baserarelity + 2; i++) ugradio_shot_.GetSelect(i)->SetSelect(FALSE);
			ugbtn_shot_[btnno]->Update(FALSE);
		}
	}
}
