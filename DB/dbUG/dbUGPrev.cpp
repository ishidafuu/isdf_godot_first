#include "dbUGPrev.h"
#include "dbUILayer.h"
#include "dbScene.h"

namespace db
{



  //コンストラクタ
  TUGPrev::TUGPrev(TUIGenLayer* player)
    :TUGGen(player)
    , ugbtn_back_(player)
    , ugbtn_mix_(player)
    , ugbtn_mix_clear_(player)
    , ugpad_shot_(player)
    //, ugpad_pass_(player)
    , ugpad_jump_(player)
    , uglever_(player)
		, ugbtn_have_mix_(player)
  {
    stPrev_.Init();
  }
  TUGPrev::~TUGPrev()
  {
  }

  void TUGPrev::MakeGroup(TUIObj* parent)
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

    ugbtn_back_.MakeGroup_FtPosBack(field_);
    ugbtn_mix_.MakeGroup_FtPos(field_, 1, "ミックスれい");
    ugbtn_mix_.SetDraw(FALSE);
    ugbtn_mix_clear_.MakeGroup_FtPos(field_, 3, "デフォルト");
    ugbtn_mix_clear_.SetDraw(FALSE);
		/* 順番入れ替え */
		ugbtn_have_mix_.MakeGroup_FtPos(field_, 2, "てもちランダム");
		ugbtn_have_mix_.SetDraw(FALSE);

    ugpad_shot_.MakeGroup_ShotPrev(field_);
    ugpad_jump_.MakeGroup_JumpPrev(field_);
    uglever_.MakeGroup(field_, TRUE);

    SetDraw(FALSE);
  }
  // void TUGPrev::Ready(s64 id_team, s64 id_shot, BOOL backbtn_f)
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
  //ugbtn_mix_clear_.SetDraw(FALSE);
  //   SetDraw(TRUE);
  // }
  void TUGPrev::Ready(const TPrevParam& param)
  {
    //フッタ位置キャンセル
    mid::midTouchFooterPosInput(FALSE);
    stPrev_.mvgame_f_ = TRUE;

    //ゲームオブジェなど準備
    BOOL needlaod_f = FALSE;
	s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
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

    pmgEO_->stShiai_.SetPrev(id_kantoku, id_team, mid_team, param.id_shot, pmgEO_->mgCoU_.GetCtrl());
    player_->pGenScene_->pGame_->Ready_Prev(needlaod_f, mid_team, id_team, 
      param.char0, param.char1, param.char2, param.char3, param.char4);

    ugbtn_back_.SetDraw(!(param.nonebackbtn_f));
    ugbtn_mix_.SetDraw(!(param.nonemixbtn_f));
    ugbtn_mix_clear_, SetDraw(FALSE);

		/* 2015/06/18 Koike Added */
		ugbtn_have_mix_.SetDraw(!(param.nonehavemixbtn_f));

    SetDraw(TRUE);

    if (param.sozaivec != NULL)
    {
      ChangeShot_Sozai(*(param.sozaivec));
    }
  }
  //シュートの中身切り替え
  void TUGPrev::ChangeShot_Sozai(const SOZAIDTVECTOR& sozaivec)
  {
    player_->pGenScene_->pGame_->mgGO_.pmgCh_[SIDE0][0]->chCommon_.ChangeStShotPrev(sozaivec, 0, 0);
  }


  void TUGPrev::SetMixSozai(s64 id_sozai, BOOL isRndSet)
  {
    ResetMixSozai(id_sozai, isRndSet);
  }
  void TUGPrev::SetMixShot(s64 id_shot, BOOL isRndSet)
  {
    ResetMixShot(id_shot, isRndSet);
  }



  void TUGPrev::ChangeShot_Mixed(LONGVECTOR* id_shot_mixvec)
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
  }

  void TUGPrev::Down()
  {
	  player_->pGenScene_->pGame_->mgGO_.pmgCommon_->ResetPopupMsg();

    stPrev_.mvgame_f_ = FALSE;
    ugbtn_mix_.SetDraw(FALSE);
    ugbtn_mix_clear_.SetDraw(FALSE);
		ugbtn_have_mix_.SetDraw(FALSE);
    SetDraw(FALSE);
    //フッタ位置キャンセル解除
    mid::midTouchFooterPosInput(TRUE);
  }

  void TUGPrev::SetPadSmart(TMgPad* pmgPad)
  {
    //スマート
    pmgPad->SetBitSmart();
    //水平斬り入力
    mid::midSetUseHInput();
  }
  void TUGPrev::SetPadRetro(TMgPad* pmgPad)
  {
    //レトロ
    pmgPad->SetBitRetro();

    //四等分入力
    mid::midSetUseT1Input();
  }

  void TUGPrev::MvPrevGame()
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


  void TUGPrev::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    court_->Update(draw_f);
    line_->Update(draw_f);
    ugbtn_back_.Update(draw_f);
    ugbtn_mix_.Update(draw_f);
    ugbtn_mix_clear_.Update(draw_f);
		ugbtn_have_mix_.Update(draw_f);

    //if (ugbtn_ctrl_.IsSmart())
    //{
    //}
    //else if (ugbtn_ctrl_.IsRetro())
    //強制レトロ表示
    {
      ugpad_shot_.Update(draw_f);
      //ugpad_pass_.Update(draw_f);
      ugpad_jump_.Update(draw_f);
      uglever_.Update(draw_f);
    }

    if (draw_f == FALSE)
    {
      if (stPrev_.mvgame_f_) MvPrevGame();
    }
    else
    {
      if (stPrev_.mvgame_f_) player_->pGenScene_->pGame_->Draw();
    }
  }

  void TUGPrev::DoAction()
  {
    if (ugbtn_back_.IsAct())
    {
      SetAct(enUGPrevAct_back);
    }
    else if (ugbtn_mix_.IsAct())
    {
      ugbtn_mix_clear_.SetDraw(TRUE);	// ここでボタン表示
      SetAct(enUGPrevAct_mix);
    }
    else if (ugbtn_mix_clear_.IsAct())
    {
      // ここでボタン非表示
      ugbtn_mix_clear_.SetDraw(FALSE);
      SetAct(enUGPrevAct_mix_clear);
    }
    else if (player_->pGame_->IsPrevReturn())
    {
      SetAct(enUGPrevAct_hit);
    }
		else if (ugbtn_have_mix_.IsAct())
		{
			ugbtn_mix_clear_.SetDraw(TRUE);	// ここでボタン表示
			SetAct(enUGPrevAct_have_mix);
		}
  }
  BOOL TUGPrev::IsActRes(enUGPrevAct res)
  {
    return (GetActRes() == res);
  }


  // ランダムボタン押した時の素材シャッフル
  void TUGPrev::ResetMixSozai(s64 id_sozai, BOOL isRndSet)
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
  void TUGPrev::ResetMixShot(s64 id_shot, BOOL isRndSet)
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
        ugbtn_mix_.SetDraw(FALSE);
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

  void TUGPrev::SelectMixSozai(s32 lowLimit, s32 upLimit)
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
}
