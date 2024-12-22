#include "dbLyHmBar.h"
#include "dbSnHome.h"

namespace db
{

  //コンストラクタ
  TLyHmBar::TLyHmBar(TSnHome* pScene) :TLyBarGen(pScene, pScene->frame_)
    , ugfinger_(this)
	, uglock_(this)
  {
    pScene_ = pScene;
    hmgrp_ = pScene->hmGrp_;
    stHmBar_.Init();
  }

  TLyHmBar::~TLyHmBar()
  {

  }


  //オーバーロード
  void TLyHmBar::DoOpenHeader()
  {

    switch (stBar_.tabNo_)
    {
	case enFooterTabID::footer_id_home:pScene_->lyHmHome_->Idle(); break;
	case enFooterTabID::footer_id_shiai:pScene_->lyHmShiai_->Idle(); break;
	case enFooterTabID::footer_id_team:pScene_->lyHmTeam_->Idle(); break;
	case enFooterTabID::footer_id_center:pScene_->lyHmCenter_->Idle(); break;
	case enFooterTabID::footer_id_shop:pScene_->lyHmShop_->Idle(); break;
	case enFooterTabID::footer_id_event:pScene_->lyHmEvent_->Idle(); break;
    }

  }
  void TLyHmBar::DoCloseHeader()
  {
    if (stHmBar_.inside_f_)
    {
      //SetFoot_BtnHd();
      //HideBtnで対応してる
    }
    else
    {
      switch (stBar_.tabNo_)
      {
	  case enFooterTabID::footer_id_home:pScene_->lyHmHome_->Ready(); break;
	  case enFooterTabID::footer_id_shiai:pScene_->lyHmShiai_->Ready(); break;
	  case enFooterTabID::footer_id_team:pScene_->lyHmTeam_->Ready(); break;
	  case enFooterTabID::footer_id_center:pScene_->lyHmCenter_->Ready(); break;
	  case enFooterTabID::footer_id_shop:pScene_->lyHmShop_->Ready(); break;
	  case enFooterTabID::footer_id_event:pScene_->lyHmEvent_->Ready(); break;
	  }
    }
  }
  void TLyHmBar::DoJumpHeader()
  {
    if (pScene_->lyTechno_->IsReady()) pScene_->lyTechno_->Down();
    if (pScene_->lyProfile_->IsReady()) pScene_->lyProfile_->Down();
  }
  void TLyHmBar::DoOpenTechno()
  {
    pScene_->lyTechno_->Ready();
  }
  void TLyHmBar::DoOpenProfile()
  {
    pScene_->lyProfile_->Ready();
  }

  void TLyHmBar::DoMoveFilter()
  {
    //レイヤ移動によるクローズ
    for (s32 i = 0; i < tabs_.size(); ++i) tabs_[i]->Down();

    s64 mid_haikei = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei);

    switch (stBar_.nexttabNo_)
    {
	case enFooterTabID::footer_id_home://ホーム
		if (stBar_.tabNo_ != stBar_.nexttabNo_)
      {
        pmgUI_->LoadCourt(mid_haikei);//背景
        pScene_->lyHmHome_->Ready();
      }
      break;
	case enFooterTabID::footer_id_shiai://試合
		if (stBar_.tabNo_ != stBar_.nexttabNo_)
      {
        ReadyShiaiTab();
      }
      break;
	case enFooterTabID::footer_id_team://チーム
		if (stBar_.tabNo_ != stBar_.nexttabNo_)
      {
        ReadyTeamTab(mid_haikei);
      }
      break;
	case enFooterTabID::footer_id_center: //センター
		if (stBar_.tabNo_ != stBar_.nexttabNo_)
      {
        pmgUI_->LoadBG("dock_bg");//背景
        pScene_->lyHmCenter_->Ready();

        s32 centerNo = pmgEO_->stJump_.GetJumpCategoryNo();
        if (centerNo != NGNUM)
        {
          pScene_->lyHmCenter_->SetupCenter(centerNo);
        }
      }
      break;
	case enFooterTabID::footer_id_shop://ショップ
		if (stBar_.tabNo_ != stBar_.nexttabNo_)
      {
        ReadyShopTab();
      }
      break;
	case enFooterTabID::footer_id_event://イベント
		if (stBar_.tabNo_ != stBar_.nexttabNo_)
		{
			ReadyEventTab();
		}
		break;
    }

    stBar_.tabNo_ = stBar_.nexttabNo_;

  }
  void TLyHmBar::DoMoveFilterAll()
  {
    if (pScene_->lyHmShVS_->IsMove())
    {
      if (pScene_->lyHmShVS_->IsGotoVS() == FALSE)
      {
        pScene_->GotoNextScene();
      }
      else
      {
        pScene_->SetNextVSShiai();
        ugfilter_.OffFilter(2, FALSE);
        pScene_->lyHmShVS_->SetVS();
      }
    }
	else if (pScene_->lyHmEvEvent_->IsMove())
	{
		if (pScene_->lyHmEvEvent_->IsGotoVS() == FALSE)
		{
			pScene_->GotoNextScene();
		}
		else
		{
			pScene_->SetNextVSShiai();
			ugfilter_.OffFilter(2, FALSE);
			pScene_->lyHmEvEvent_->SetVS();
		}
	}
	else
	{
		pScene_->GotoNextScene();
	}
  }
  void TLyHmBar::DoLoadSub()
  {
    ugfinger_.MakeGroup(field_);
    ugfinger_.SetRevUD(TRUE);
    f32 px = 0;
    f32 py = 0;
    ugfooter_.tabbtn_[1]->GetField()->z_GetGlobalPos(&px, &py);
    //pUGFinger_->SetPos(px, py-30);
    ugfinger_.SetMotionLookPoint(px, py - 50);
    ugfinger_.SetDraw(IsTutoFinger());


	// チュートリアルが全て終了していない
#ifdef __K_DEBUG_USER_DATA__
	if (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubEventOpen))	
#else
	if (!pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubEventOpen))
#endif
	{
		TUGButton* btn = ugfooter_.tabbtn_[enFooterTabID::footer_id_event];
		uglock_.MakeGroup(btn->GetField());
		btn->SetDisable(TRUE);
	}
  }
  void TLyHmBar::DoUpdateSub(BOOL draw_f)
  {
    if (IsTutoFinger())
    {
      ugfinger_.Update(draw_f);
    }

	// チュートリアルが全て終了していない
#ifdef __K_DEBUG_USER_DATA__
	if (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubEventOpen))
#else
	if (!pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubEventOpen))
#endif
	{
		uglock_.Update(draw_f);
	}
  }

  void TLyHmBar::ReadyHome()
  {
    //まずBarのReady
    Ready();

    switch (pScene_->GetLastScene())
    {
    case snScenario: FromMap(); break;//シナリオから戻ってきた
    case snShiai:FromShiai(); break;//疑似対人、ＶＳなど
    case snTutorial:FromTuto(); break;//合成チュート終了後
    default://タイトルなど
		// 最強チーム、最強シュートの更新
		// 処理が重いのでタイトルから入ってきたときに１回だけ行う
		pmgEO_->RefreshStateTbl();
		FromOther();
		break;
    }

    ugfooter_.ugradio_.SetSelect(stBar_.tabNo_);
  }

  void TLyHmBar::FromMap()
  {
	  // 0の時は通常シナリオマップなので試合タブへ
	  if (pmgEO_->stSeq_.GetMidEvent() == 0)
	  {
		  ReadyShiaiTab();
	  }
	  // それ以外はイベントマップなのでイベントタブへ
	  else
	  {
		  ReadyEventTab();
	  }
  }

  void TLyHmBar::SetDrawFinger(BOOL draw_f)
  {
    ugfinger_.SetDraw(draw_f);
  }

  void TLyHmBar::FromShiai()
  {
	  // イベントのとき
	  if (pmgEO_->stSeq_.IsEventTimeAttack() ||
		  pmgEO_->stSeq_.IsEventTour())
	  {
		  pmgUI_->LoadBG("vs_bg");//背
		  pScene_->lyHmEvent_->Ready();
		  stBar_.tabNo_ = enFooterTabID::footer_id_event;
		  
		  // フラグを落としておく
		  pmgEO_->stSeq_.SetEventTimeAttack(FALSE);
		  pmgEO_->stSeq_.SetEventTour(FALSE);
	  }
	  else
	  {
		  pmgUI_->LoadBG("vs_bg");//背
		  pScene_->lyHmShiai_->Ready();
		  //pScene_->lyHmShiai_->Idle();
		  //pScene_->lyHmShVS_->Ready();
		  stBar_.tabNo_ = enFooterTabID::footer_id_shiai;
	  }
  }

  void TLyHmBar::FromTuto()
  {
    pmgUI_->LoadBG("dock_bg");//背
    pScene_->lyHmCenter_->Ready();
	stBar_.tabNo_ = enFooterTabID::footer_id_center;
  }

  void TLyHmBar::FromOther()
  {
    s64 mid_haikei = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei);
#ifdef __K_DEBUG_USER_DATA__
    if (mid_haikei == 0){ mid_haikei = 1; }
#endif
    pmgUI_->LoadCourt(mid_haikei);//背
    pScene_->lyHmHome_->Ready();
	stBar_.tabNo_ = enFooterTabID::footer_id_home;
  }

  void TLyHmBar::ReadyShiaiTab()
  {
    pmgUI_->LoadBG("vs_bg");//背
    pScene_->lyHmShiai_->Ready();
	stBar_.tabNo_ = enFooterTabID::footer_id_shiai;

#ifdef __K_DEBUG_USER_DATA__
    if (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubEventOpen))
#else
    if ((pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubEventOpen) == FALSE))
#endif
    {
      pScene_->lySTuShiai_->Ready();
      SetFoot_Tab();
      // チュートリアル中は操作させない
      pmgUI_->SetModal(TRUE);
      ugheadder_.SetModal(FALSE);//ヘッダーがモーダル状態でも聴くのを止める
    }
    else
    {
      s32 shiaiBtnNo = pmgEO_->stJump_.GetJumpCategoryNo();
      if (shiaiBtnNo > 0)
      {
        pScene_->lyHmShiai_->SetupShiai(shiaiBtnNo);
      }
    }
  }
  void TLyHmBar::ReadyEventTab()
  {
	  pmgUI_->LoadBG("vs_bg");//背
	  pScene_->lyHmEvent_->Ready();
	  stBar_.tabNo_ = enFooterTabID::footer_id_event;

#ifdef __K_DEBUG_USER_DATA__
	  if (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubEventOpen))
#else
	  if ((pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubEventOpen) == FALSE))
#endif
	  {
		  //pScene_->lySTuShiai_->Ready();
		  //SetFoot_Tab();
		  // チュートリアル中は操作させない
		  //pmgUI_->SetModal(TRUE);
		  //ugheadder_.SetModal(FALSE);//ヘッダーがモーダル状態でも聴くのを止める
	  }
	  else
	  {
		  s32 eventBtnNo = pmgEO_->stJump_.GetJumpCategoryNo();
		  if (eventBtnNo > 0)
		  {
			  pScene_->lyHmEvent_->SetupEvent(eventBtnNo);
		  }
	  }
  }
  BOOL TLyHmBar::IsTutoFinger()
  {
    BOOL res = FALSE;
#ifdef __K_DEBUG_USER_DATA__
    if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Gousei) &&
      !kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubScenario))
#else
    if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Gousei) == TRUE &&
      pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubScenario) == FALSE)
#endif
    {
      res = TRUE;
    }
    return res;
  }
  void TLyHmBar::ReadyTeamTab(s32 mid_haikei)
  {

    pmgUI_->LoadCourt(mid_haikei);//背景
    pScene_->lyHmTeam_->Ready();

#ifdef __K_DEBUG_USER_DATA__
    if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubScenario) &&	// シナリオマップのサブチュートリアルが終わってなければとりあえず入れる
      !kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubTeam))		// チームのサブチュートリアルが終わってたらもちろん入れる

#else
    if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubScenario) == TRUE &&
      pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubTeam) == FALSE)
#endif
    {
      pScene_->lySTuTeam_->Ready();
      SetFoot_Tab();
      // チュートリアル中は操作させない
      pmgUI_->SetModal(TRUE);
      ugheadder_.SetModal(FALSE);//ヘッダーがモーダル状態でも聴くのを止める
    }
  }

  void TLyHmBar::ReadyShopTab(void)
  {
    pmgUI_->LoadBG("shop_bg");//背景
    pScene_->lyHmShop_->Ready();

    s32 shopNo = pmgEO_->stJump_.GetJumpCategoryNo();
    if (shopNo > 0)
    {
      pScene_->lyHmShop_->SetupShop(shopNo);
    }
  }

  //レイヤの中に入ってる状態
  void TLyHmBar::SwitchInside(BOOL inside_f)
  {
    stHmBar_.inside_f_ = inside_f;
  }

  void TLyHmBar::JumpFooter(s32 tabno)
  {
    s32 shiaiBtnNo = pmgEO_->stJump_.GetJumpCategoryNo();
    if (shiaiBtnNo != NGNUM)
    {
      // 1:シナリオマップ
		if (tabno == enFooterTabID::footer_id_shiai)
      {
        if (shiaiBtnNo == 1)
        {
          pScene_->lyHmBar_->SetFilterAll(FALSE);
          pScene_->SetNextScenario();
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
		  pmgEO_->stSeq_.SetMapMidBGM(nsf_scenario);
#endif
          //pmgEO_->mgStSn_.EtcSS.SetShiaiType_Normal();//
          pmgEO_->stSeq_.SetIDEvent(0, 0);// .EtcSS.SetEventID(0, 0);

          // ジャンプが終わったらジャンプデータはクリアしておく
          pmgEO_->stJump_.ClearJumpAreaData();
          return;
        }
      }
    }

    TLyBarGen::JumpFooter(tabno);

    // チームタブは深い階層に潜らないのでここでクリア
	if (tabno == enFooterTabID::footer_id_team)
    {
      // ジャンプが終わったらジャンプデータはクリアしておく
      pmgEO_->stJump_.ClearJumpAreaData();
    }
  }
}