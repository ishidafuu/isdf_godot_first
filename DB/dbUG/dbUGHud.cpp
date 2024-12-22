#include "dbUGHud.h"
#include "dbUGHeadder.h"
#include "dbUGFooter.h"
#include "dbUILayer.h"

namespace db
{

  const s32 GANBABY = -102;
  const s32 GANBABX = -152+20;
  const s32 GANBADX = 32;

  enum enAniMsg
  {
    am_Connect,
    am_Login,
    am_Auth,
    am_Search,
    am_EnWait,
  };

  //コンストラクタ
  TUGHud::TUGHud(TUIGenLayer* player) :TUGGen(player)
    , ugmsg_(player)
    , ugitemview_(player)
    , ugbtn_cancel_(player)
    , uginput_(player)
    , ugprev_(player)
    , ugbtn_tsh_(player)
		, ugmixprev_(player)
  {
    stHud_.Init();
    ugheadder_ = NULL;
    ugfooter_ = NULL;
  }

  TUGHud::~TUGHud()
  {

  }

  void TUGHud::MakeGroup(TUIObj* parent)
  {
    const s32 BARW = 512;

    field_ = MakeObj();
    field_->o_SetParent(parent);
    lastani_ = 0;

    countdown_ = MakeObj();
    countdown_->o_SetParent(field_);
    countdown_->g_SetDepth(UIDP_09MSGBOX);
    countdown_->z_SetPos(GANBABX, GANBABY);

    ganba_ = MakeObj();
    ganba_->o_SetParent(field_);
    ganba_->z_SetPos(GANBABX, GANBABY);
    ganba_->f_MakeFont("つうしんちゅう", FC2_normal);
    ganba_->g_SetDepth(UIDP_09MSGBOX);
    for (s32 i = 0; i < HTNUM; ++i)
    {
      hart_[i] = MakeObj();
      hart_[i]->o_SetParent(field_);
      hart_[i]->g_SetDepth(UIDP_09MSGBOX);
      hart_[i]->f_MakeFont("心", FC2_normal);
      hart_[i]->g_SetDraw(FALSE);


      s32 posx = (i < (HTNUM / 2))
        ? (GANBABX - 24 + (i * 8))
        : (GANBABX + GANBADX + (i * 8));

      hart_[i]->z_SetPos(posx, GANBABY);
    }

    ugbtn_cancel_.MakeGroup_FtPosBack(field_);
    ugbtn_cancel_.SetDraw(FALSE);

    ugbtn_tsh_.MakeGroup_Tokusyou(field_);
    ugbtn_tsh_.SetDraw(FALSE);

    //メッセージボックス
    ugmsg_.MakeGroup(field_);

    //ここら辺は必要なレイヤのみ作成で大ジョブか

    //アイテムビュー
    ugitemview_.MakeGroup(field_);
    //シュートプレビュー
    ugprev_.MakeGroup(field_);
    //文字入力
    uginput_.MakeGroup(field_);
		//MIXシュートプレビュー
		ugmixprev_.MakeGroup(field_);

    SetAni(FALSE, 0);
  }

  void TUGHud::SetHedder(TUGHeadder* ugheadder, TUGFooter* ugfooter)
  {
    ugheadder_ = ugheadder;
    ugfooter_ = ugfooter;
  }

  void TUGHud::SetModal(BOOL modal_f)
  {
    ugheadder_->SetModal(modal_f);
  }

  //がんばってますアピール
  void TUGHud::SetAni(BOOL ani_f, s32 anitype)
  {
    if (ani_f)
    {
      s32 dx = 0;

      if (lastani_ != anitype)
      {
        lastani_ = anitype;
        switch (anitype) {
        case am_Login:
          ganba_->f_MakeFont("ログインちゅう", FC2_normal);
          break;
        case am_Auth:
          ganba_->f_MakeFont("にんしょうちゅう", FC2_normal);
          dx = 8;
          break;
        case am_Search:
          ganba_->f_MakeFont("けんさくちゅう", FC2_normal);
          break;
        case am_EnWait:
          ganba_->f_MakeFont("あいてまちちゅう", FC2_normal);
          dx = 8;
          break;
        default:
          ganba_->f_MakeFont("つうしんちゅう", FC2_normal);
          break;
        }
        for (s32 i = 0; i < HTNUM; ++i)
        {
          s32 posx = (i < (HTNUM / 2))
            ? (GANBABX - 24 + (i * 8))
            : (GANBABX + GANBADX + dx + (i * 8));

          hart_[i]->z_SetPos(posx, GANBABY);
          hart_[i]->g_SetDraw(i == 0);
        }
      }
      else
      {
        for (s32 i = 0; i < HTNUM; ++i) hart_[i]->g_SetDraw((i % (HTNUM / 2)) == 0);
      }
    }
    stHud_.ani_f_ = ani_f;
    stHud_.ani_c_ = 0;
    stHud_.ani2_c_ = 0;
    ganba_->g_SetDraw(ani_f);
    for (s32 i = 0; i < HTNUM; ++i)hart_[i]->g_SetDraw(FALSE);

    countdown_->g_SetDraw(FALSE);
  }
  //カウントダウン
  void TUGHud::SetCount(s32 count)
  {
    countdown_->f_MakeFont(lib_str::IntToStr(count).c_str(), FC2_normal);
    countdown_->g_SetDraw(TRUE);
  }
  void TUGHud::SetCountEnd()
  {
    countdown_->g_SetDraw(FALSE);
  }
  void TUGHud::GenMessage()
  {
    SetAni(FALSE, 0);
    ugmsg_.SetDraw(TRUE);
    stHud_.msg_f_ = TRUE;
    //フッタのボタンを消す
    if (ugfooter_ != NULL) ugfooter_->HideBtn(TRUE);
  }
  void TUGHud::SetErrorMessage(s32 code, const char* msg)
  {

    if (code == NGNUM)
    {
      //std::string codestr = "コード：";
      //codestr += lib_str::IntToStr(code);
      STRVECTOR strvec = { "せつぞくできませんでした心",  msg };
      ugmsg_.SetData(&strvec, FALSE);
    }
    else
    {
      //STRVECTOR strvec = { "心＿なめんなよ＿心", msg };
      STRVECTOR strvec = { msg };
      ugmsg_.SetData(&strvec, FALSE);
    }


    GenMessage();
  }
  void TUGHud::SetMessage(BOOL cancel_f, BOOL tsh_f, STRVECTOR* strvec)
  {
    ugmsg_.SetData(strvec, cancel_f);
    //ugbtn_cancel_.SetDraw(FALSE);
    ugbtn_tsh_.SetDraw(tsh_f);
    GenMessage();
  }
  void TUGHud::SetMessageTodd(STRVECTOR* strvec, s32 num, BOOL get_f)
  {
    ugmsg_.SetUseTodd(strvec, num, get_f);
    //ugbtn_cancel_.SetDraw(TRUE);
    //ugbtn_tsh_.SetDraw(TRUE);
    GenMessage();
  }
  void TUGHud::SetMessageStar(STRVECTOR* strvec, s32 num)
  {
    ugmsg_.SetUseStar(strvec, num);
    //ugbtn_cancel_.SetDraw(TRUE);
    ugbtn_tsh_.SetDraw(TRUE);
    GenMessage();
  }
  void TUGHud::SetMessageStarSupply(s32 mid_supply, BOOL quse_f)
  {
    std::string strsupply = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_name_c32);
    s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply, mdm_supply_price);
    STRVECTOR strvec = { strsupply + "＿を", "こうにゅうします" };
    ugmsg_.SetUseStar(&strvec, price);
    //ugbtn_cancel_.SetDraw(TRUE);
    ugbtn_tsh_.SetDraw(TRUE);
    GenMessage();

    //まみのこうしん必要フラグ
    pmgEO_->stSeq_.PushNeedMami();
  }
  void TUGHud::SetMessageStarGacha(s32 mid_gacha)
  {
    std::string strgacha = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDtStr(mid_gacha, mdm_gacha_name_c64);
    s64 mid_supply_ken = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_id_m_supply_ken);
    s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply_ken, mdm_supply_price);
    STRVECTOR strvec = { strgacha + "＿を", "まわします" };
    ugmsg_.SetUseStar(&strvec, price);
    ugbtn_cancel_.SetDraw(TRUE);
    ugbtn_tsh_.SetDraw(TRUE);
    GenMessage();

    //まみのこうしん必要フラグ
    pmgEO_->stSeq_.PushNeedMami();
  }
  void TUGHud::SetMessageStarNaBox(s32 mid_nabox, BOOL otona_f)
  {
    std::string strnabox = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(mid_nabox, mdm_nabox_name_c64);
    s64 mid_supply_ken = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_id_m_supply_ken);
    s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply_ken, mdm_supply_price);

    if (otona_f)
    {
      STRVECTOR strvec = { strnabox + "＿を", "すべて＿ひきます" };
      s32 id_nabox = pmgEO_->mgCoU_.GetNaBox_id(mid_nabox);
      s32 opennum = pmgEO_->mgCoU_.mdNaBox_.GetPKDt(id_nabox, mdnabox_openNum);
      s32 boxsize = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_boxSize);
      s32 otonaprice = (price * (boxsize - opennum));
      ugmsg_.SetUseStar(&strvec, otonaprice);
    }
    else
    {
      STRVECTOR strvec = { strnabox + "＿を", "１かい＿ひきます" };
      ugmsg_.SetUseStar(&strvec, price);
    }
    //ugbtn_cancel_.SetDraw(TRUE);
    ugbtn_tsh_.SetDraw(TRUE);
    GenMessage();

    //まみのこうしん必要フラグ
    pmgEO_->stSeq_.PushNeedMami();
  }

  void TUGHud::SetGKErrorMessage()
  {
    std::string codestr = "エラーコード：";
    std::string errtypestr = "";
    if (mid::midGetGKErrorCode() > 0)
    {
      codestr += lib_str::IntToStr(mid::midGetGKErrorCode());
      errtypestr = "ゲームセンターエラー";
    }
    else if (mid::midGetSyncErrorCode() > 0)
    {
      codestr += lib_str::IntToStr(mid::midGetSyncErrorCode());
      errtypestr = "つうしんどうきエラー";
    }
    else
    {
      codestr += lib_str::IntToStr(mid::midGetSyncErrorCode());
      errtypestr = "せつぞくエラー";
    }
    STRVECTOR strvec = { errtypestr, codestr, mid::midGetErrorMessage() };
    ugmsg_.SetData(&strvec, FALSE);
    ugbtn_cancel_.SetDraw(FALSE);//キャンセルボタンは不要
    GenMessage();
  }

  void TUGHud::SetAniStart()
  {
    //アニメーション開始
    SetAni(TRUE, am_Connect);
  }
  void TUGHud::SetAniStart_Login()
  {
    //アニメーション開始
    SetAni(TRUE, am_Login);
  }
  void TUGHud::SetAniStart_Auth()
  {
    //アニメーション開始
    SetAni(TRUE, am_Auth);
  }
  void TUGHud::SetAniStart_EnWait()
  {
    //アニメーション開始
    SetAni(TRUE, am_EnWait);
  }
  void TUGHud::SetAniEnd()
  {
    SetAni(FALSE, 0);
  }
  void TUGHud::SetMsgEnd()
  {
    ugmsg_.SetDraw(FALSE);
    stHud_.msg_f_ = FALSE;
    ugbtn_cancel_.SetDraw(FALSE);
    ugbtn_tsh_.SetDraw(FALSE);

    //フッタのボタンを戻す
    if (ugfooter_ != NULL) ugfooter_->HideBtn(FALSE);
  }
  void TUGHud::GenSetItemView()
  {
    SetAni(FALSE, 0);
    stHud_.itemview_f_ = TRUE;
    ugitemview_.SetDraw(TRUE);
  }
  void TUGHud::SetItemView_GetLog(BOOL msgok_f, BOOL leftpos_f, BOOL filter_f)
{
    GenSetItemView();
	ugitemview_.Refresh_GetLog(msgok_f, leftpos_f, filter_f);
  }
  void TUGHud::SetItemView_GetLogSingle(s32 logno, BOOL msgok_f, BOOL leftpos_f)
  {
    GenSetItemView();
    ugitemview_.Refresh_GetLogSingle(logno, msgok_f, leftpos_f);
  }
  void TUGHud::SetItemView_GachaCnt(s64 mid_gachacnt)
  {
    GenSetItemView();
    ugitemview_.Refresh_GachaCnt(mid_gachacnt);
  }
  void TUGHud::SetItemView_GachaInit()
  {
    ugitemview_.ClearGachaLv();
  }
  void TUGHud::SetItemView_GachaGet(s32 logno)
  {
    GenSetItemView();
    ugitemview_.Refresh_GachaGet(logno);
  }
  void TUGHud::SetItemView_NaBoxCnt(s64 mid_naboxcnt)
  {
    GenSetItemView();
    ugitemview_.Refresh_NaBoxCnt(mid_naboxcnt);
  }
  void TUGHud::SetItemView_HukuCnt(s64 mid_hukucnt)
  {
    GenSetItemView();
    ugitemview_.Refresh_HukuCnt(mid_hukucnt);
  }
  void TUGHud::SetItemView_Reward(s64 mid_ranking, s32 rank, BOOL msgok_f, BOOL leftpos_f, BOOL filter_f)
  {
	  GenSetItemView();
	  ugitemview_.Refresh_Reward(mid_ranking, rank, msgok_f, leftpos_f, filter_f);
  }

  void TUGHud::SetItemView_View(s64 itemType, s64 itemID, s64 itemNum, BOOL msgok_f, BOOL leftpos_f)
  {
    GenSetItemView();
    ugitemview_.Refresh_View(itemType, itemID, itemNum, msgok_f, leftpos_f);
  }
  void TUGHud::SetItemView_Shot(SOZAIDTVECTOR sozai, BOOL leftpos_f)
  {
	  GenSetItemView();
	  ugitemview_.Refresh_Shot(sozai, leftpos_f);
  }
  void TUGHud::SetItemView_SozaiList(SOZAIDTVECTOR sozai, BOOL leftpos_f)
  {
	  GenSetItemView();
	  ugitemview_.Refresh_SozaiList(sozai, leftpos_f);
  }
  void TUGHud::SetItemView_Drop(s64 itemType, s64 itemID, s64 itemNum, s32 tokkouNum, s32 tokkouMag, BOOL comp_f)
  {
    GenSetItemView();
    ugitemview_.Refresh_Drop(itemType, itemID, itemNum, tokkouNum, tokkouMag, comp_f);
  }

  void TUGHud::SetItemViewEnd()
  {
    stHud_.itemview_f_ = FALSE;
    ugitemview_.SetDraw(FALSE);
  }
  void TUGHud::SetItemViewDraw(BOOL draw_f)
  {
	  stHud_.itemview_f_ = draw_f;
	  ugitemview_.SetDraw(draw_f);
  }
  //void TUGHud::SetPrev(s64 id_team, s64 id_shot, BOOL backbtn_f)
  //{
  //  stHud_.prev_f_ = TRUE;
  //  ugprev_.Ready(id_team, id_shot, backbtn_f);
  //}
  void TUGHud::SetPrev(const TPrevParam& param)
  {
	  stHud_.prev_f_ = TRUE;
    ugprev_.Ready(param);
  }
  void TUGHud::SetPrev_Mixed(LONGVECTOR* id_shot_mixvec)
  {
    stHud_.prev_f_ = TRUE;
    TPrevParam param;
    param.Init();
		/* 2015/06/19 Koike Added */
		// そのままいく場合は「ミックスれい」は表示しない
    //param.id_team = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_team);
		param.nonemixbtn_f = TRUE;
		param.nonehavemixbtn_f = TRUE;
    ugprev_.Ready(param);
    ugprev_.ChangeShot_Mixed(id_shot_mixvec);
  }
	/* 2015/06/19 Koike Added */
	void TUGHud::SetPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec)
	{
		//stHud_.prev_f_ = TRUE;
		//TPrevParam param;
		/* 2015/06/19 Koike Added */
		// そのままいく場合は「ミックスれい」は表示しない
		//param.id_team = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_team);
		ugprev_.ChangeShot_Mixed(id_shot_mixvec);
	}

  void TUGHud::SetPrevChangeShot(const SOZAIDTVECTOR& sozaivec)
  {
    ugprev_.ChangeShot_Sozai(sozaivec);
  }
  void TUGHud::SetPrevMixSozai(s64 id_sozai, BOOL isRndSet)
  {
	  ugprev_.SetMixSozai(id_sozai, isRndSet);
  }
  void TUGHud::SetPrevMixShot(s64 id_shot, BOOL isRndSet)
  {
	  ugprev_.SetMixShot(id_shot, isRndSet);
  }

  void TUGHud::SetPrevEnd()
  {
    stHud_.prev_f_ = FALSE;
    ugprev_.Down();
  }
	//-------------------------------------------------------
	void TUGHud::SetMixPrev(const TMixPrevParam& param)
	{
		stHud_.prev_f_ = TRUE;
		ugmixprev_.Ready(param);
	}
	void TUGHud::SetMixPrev_Mixed(LONGVECTOR* id_shot_mixvec)
	{
		stHud_.prev_f_ = TRUE;
		TMixPrevParam param;
		param.Init();
		/* 2015/06/19 Koike Added */
		// そのままいく場合は「ミックスれい」は表示しない
		//param.id_team = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_team);
		param.nonemixbtn_f = TRUE;
		param.nonehavemixbtn_f = TRUE;
		ugmixprev_.Ready(param);
		ugmixprev_.ChangeShot_Mixed(id_shot_mixvec);
	}
	/* 2015/06/19 Koike Added */
	void TUGHud::SetMixPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec)
	{
		//stHud_.prev_f_ = TRUE;
		//TPrevParam param;
		/* 2015/06/19 Koike Added */
		// そのままいく場合は「ミックスれい」は表示しない
		//param.id_team = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_team);
		ugmixprev_.ChangeShot_Mixed(id_shot_mixvec);
	}
	void TUGHud::SetMixPrevChangeShot(const SOZAIDTVECTOR& sozaivec)
	{
		ugmixprev_.ChangeShot_Sozai(sozaivec);
	}
	void TUGHud::SetMixPrevMixSozai(s64 id_sozai, BOOL isRndSet)
	{
		ugmixprev_.SetMixSozai(id_sozai, isRndSet);
	}
	void TUGHud::SetMixPrevMixShot(s64 id_shot, BOOL isRndSet)
	{
		ugmixprev_.SetMixShot(id_shot, isRndSet);
	}
	void TUGHud::SetMixPrevEnd()
	{
		stHud_.prev_f_ = FALSE;
		ugmixprev_.Down();
	}
	s32 TUGHud::SetMixPrevTouchMode(LONGVECTOR* id_shot_mixvec, s32 iconNo)
	{
		return ugmixprev_.IsTouchMode(id_shot_mixvec, iconNo);
	}
	void TUGHud::SetMixPrevSlectShot(s64 id_shot)
	{
		ugmixprev_.SetSelectShot(id_shot);
	}
	LONGVECTOR TUGHud::GetMixPrevChangeVec()
	{
		return ugmixprev_.GetChangeSozaiVec();
	}
	void TUGHud::ResetField(BOOL fieldflg)
	{
		ugmixprev_.ResetField(fieldflg);
	}
	//-------------------------------------------------------

	void TUGHud::SetInput(s32 outw, BOOL alponly_f)
  {
    stHud_.input_f_ = TRUE;
    uginput_.Refresh(outw, alponly_f);
  }
  void TUGHud::SetInput(s32 outw, BOOL alponly_f, INTVECTOR defmcvec)
  {
    stHud_.input_f_ = TRUE;
    uginput_.Refresh(outw, alponly_f, defmcvec);
  }
  void TUGHud::SetVSWait_Search()
  {
    stHud_.vswait_f_ = TRUE;
    //アニメーション開始
    SetAni(TRUE, am_Search);
  }
  void TUGHud::SetVSWait_EnWait()
  {
    stHud_.vswait_f_ = TRUE;
    //アニメーション開始
    SetAni(TRUE, am_EnWait);
  }
  void TUGHud::SetVSWaitEnd()
  {
    stHud_.vswait_f_ = FALSE;
    SetAniEnd();
  }

  std::string TUGHud::OutputString()
  {
    return uginput_.OutputString();
  }
  void TUGHud::SetInputEnd()
  {
    stHud_.input_f_ = FALSE;
    uginput_.SetDraw(FALSE);
  }
  BOOL TUGHud::IsAni()
  {
    return stHud_.ani_f_;
  }
  BOOL TUGHud::IsMsg()
  {
    return stHud_.msg_f_;
  }
  BOOL TUGHud::IsItemView()
  {
    return stHud_.itemview_f_;
  }
  BOOL TUGHud::IsPrev()
  {
    return stHud_.prev_f_;
  }
  BOOL TUGHud::IsInput()
  {
    return stHud_.input_f_;
  }
  void TUGHud::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ganba_->Update(draw_f);
    countdown_->Update(draw_f);
    for (s32 i = 0; i < HTNUM; ++i) hart_[i]->Update(draw_f);
    ugmsg_.Update(draw_f);
    ugitemview_.Update(draw_f);
    ugprev_.Update(draw_f);
    uginput_.Update(draw_f);
    ugbtn_cancel_.Update(draw_f);
    ugbtn_tsh_.Update(draw_f);
		ugmixprev_.Update(draw_f);

    if (draw_f == FALSE)
    {
      //メッセージボックスおされた
      if (ugmsg_.IsAct())
      {
        if (ugmsg_.IsActRes(enUGMsgBoxAct_OK))
        {
          SetAct(enUGHudAct_OK);
        }
        else if (ugmsg_.IsActRes(enUGMsgBoxAct_NG))
        {
          SetAct(enUGHudAct_Cancel);
        }
      }
      else if (ugbtn_cancel_.IsAct())//キャンセル押された
      {
        SetAct(enUGHudAct_Cancel);
      }
      else if (ugitemview_.IsAct())//アイテムビュー終了
      {
        SetItemViewEnd();
        SetAct(enUGHudAct_ItemView);
      }
      else if (ugprev_.IsAct())//プレビュー終了
      {
        if (ugprev_.IsActRes(enUGPrevAct_back))
        {
          SetPrevEnd();
          SetAct(enUGHudAct_PrevBack);
        }
				else if (ugprev_.IsActRes(enUGPrevAct_mix))
				{
					SetAct(enUGHudAct_PrevMix);
				}
				else if (ugprev_.IsActRes(enUGPrevAct_mix_clear))
				{
					SetAct(enUGHudAct_PrevMixClear);
				}
				else if (ugprev_.IsActRes(enUGPrevAct_hit))
        {
          SetAct(enUGHudAct_PrevHit);
        }
				else if (ugprev_.IsActRes(enUGPrevAct_have_mix))
				{
					SetAct(enUGHudAct_PrevHaveMix);
				}
      }
			/* 2015/07/02 Koike Added */
			else if (ugmixprev_.IsAct())
			{
				if (ugmixprev_.IsActRes(enUGMixPrevAct_back))
				{
					SetMixPrevEnd();
					SetAct(enUGHudAct_PrevBack);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_cancel))
				{
					SetAct(enUGHudAct_MixPrevCancel);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_ok))
				{
					SetAct(enUGHudAct_MixPrevSelectOK);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_have_mix))
				{
					SetAct(enUGHudAct_PrevHaveMix);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_mix_clear))
				{
					SetAct(enUGHudAct_MixPrevUndo);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_ball_0))
				{
					SetAct(enUGHudAct_MixPrevBall0);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_ball_1))
				{
					SetAct(enUGHudAct_MixPrevBall1);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_ball_2))
				{
					SetAct(enUGHudAct_MixPrevBall2);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_ball_3))
				{
					SetAct(enUGHudAct_MixPrevBall3);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_ball_4))
				{
					SetAct(enUGHudAct_MixPrevBall4);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_ball_5))
				{
					SetAct(enUGHudAct_MixPrevBall5);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_ball_6))
				{
					SetAct(enUGHudAct_MixPrevBall6);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_ball_7))
				{
					SetAct(enUGHudAct_MixPrevBall7);
				}
				else if (ugmixprev_.IsActRes(enUGMixPrevAct_Select_ball))
				{
					SetAct(enUGHudAct_MixPrevSelect);
				}
			}
      else if (uginput_.IsAct())//インプット終了
      {
        if (uginput_.IsActRes(enUGInputAct_OK)) SetAct(enUGHudAct_InputOK);
        else if (uginput_.IsActRes(enUGInputAct_Back)) SetAct(enUGHudAct_InputCancel);
        SetInputEnd();
      }

      if (stHud_.ani_f_)
      {
        if (stHud_.ani_c_ % 24 == 0)
        {
          for (s32 i = 0; i < HTNUM; ++i)
          {
            hart_[i]->g_SetDraw((stHud_.ani2_c_ % (HTNUM / 2)) == (i % (HTNUM / 2)));
          }
          ++stHud_.ani2_c_;
        }
        ++stHud_.ani_c_;
      }
    }
  }

  BOOL TUGHud::IsActRes(enUGHudAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

}
