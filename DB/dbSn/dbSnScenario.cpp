#include "dbSnScenario.h"

namespace db
{

  std::string lasturl;

  //メニュー///////////////////////////
  TSnScenario::TSnScenario(TGame* pGame, TMgUI* pMgUI, s32 myScene)
    : TScene(pGame, pMgUI, myScene)
  {
    //setlayer_f_ = FALSE;
    //goto_f_ = FALSE;
    backhome_f_ = FALSE;
  }

  TSnScenario::~TSnScenario()
  {
    FreeLayer();
  }

  //レディ
  void TSnScenario::DoReady()
  {
    //SceneBGM(mdTitle);
    
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	  pmgEO_->mgNSF_.PlayID(pmgEO_->stSeq_.GetMapMidBGM());//BGM
#else
	  pmgEO_->mgNSF_.Play(nsf_scenario);//BGM
#endif


    //メニューに戻ってきたら切断
    //if (mid::midIsConnect())
    //{
    //  mid::midDisconnectBT();
    //  mid::midDisconnectWifi();
    //}

    //全スプライトたたむ
    pmgEO_->mgFont_.ResetFontSpriteAll();

    //背景をきりかえる
    //pmgEO_->mgGrp_.SetBGTypeCourt(0, FALSE);
    //コートロード
    //pmgEO_->mgGrp_.LoadCourt(0, mid::ldm_Def);

    //カメラ初期化
    pmgEO_->mgCamera_.SetCamInit();
    pmgEO_->mgCamera_.SetCamData(TRUE, FALSE, mid::midGetDotL(), mid::midGetDotS(), 0, 0);
    pmgEO_->mgCamera_.SetCamPos(0, 0);

    // チュートリアル専用のレイヤーをセット
#ifdef __K_DEBUG_USER_DATA__
    if (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubScenario))
#else
    if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubScenario) == FALSE)
#endif
    {
      tuto_f_ = TRUE;
    }
	else
	{
		tuto_f_ = FALSE;
	}

    //レイヤーセット
    SetLayer();



    //メニューには不要
    //でreturnしてるやつをけす
  }

  void TSnScenario::DoSetLayer()
  {
    if (tuto_f_)
    {
      DoSetLayer_Tuto();
    }
    else
    {
      DoSetLayer_Normal();
    }
  }

  void TSnScenario::DoSetLayer_Normal()
  {
    frame_ = new TUIFrame(this);
    lyScBar_ = new TLyScBar(this);
    frame_->SetHedder(lyScBar_);

    scgrp_ = new TLyScGrp(this);
    lyMap_ = new TLyScMap(this);
    lyEnemy_ = new TLyScEnemy(this);
    lyTSL_ = new TLyScTSL(this);
    lyVS_ = new TLyScVS(this);
    lyTechno_ = new TLyTechno(this, this->frame_, lyScBar_);
    lyProfile_ = new TLyProfile(this, this->frame_, lyScBar_);

    pmgUI_->Init((enSceneName)GetMyScene());
    //UIMgに登録
    pmgUI_->SetUIFrame(frame_);
    pmgUI_->SetUILayer(lyScBar_);//アクションの優先度が高くなる
    pmgUI_->SetUILayer(scgrp_);
    pmgUI_->SetUILayer(lyMap_);
    pmgUI_->SetUILayer(lyEnemy_);
    pmgUI_->SetUILayer(lyTSL_);
    //pmgUI_->SetUILayer(lyCSL_);
    pmgUI_->SetUILayer(lyVS_);
    //pmgUI_->SetUILayer(lyFrame_);

    pmgUI_->SetUILayer(lyTechno_);
    pmgUI_->SetUILayer(lyProfile_);
    //UIMap
    pmgUI_->LoadLayer();
    pmgUI_->LoadBG("map_bg");//背景

    //    pmgUI_->LoadBG("dbcourt1");//背景
    //setlayer_f_ = TRUE;
    //goto_f_ = FALSE;
    //１がスタート

    //lyTSL_->stLy_.lyphase_ = 1;
    //pmgEO_->stSeq_.PushLastScene(TStSnSeq::ls_Map);//マップから来ました
    lyScBar_->Ready();

    lyMap_->Ready();
  }

  void TSnScenario::DoSetLayer_Tuto()
  {
    //if (setlayer_f_) return;

    frame_ = new TUIFrame(this);
    lyTuScBar_ = new TLyTuScBar(this);
    frame_->SetHedder(lyTuScBar_);

    scgrp_ = new TLyScGrp(this);
    lyTuMap_ = new TLyTuScMap(this);
    lyTuEnemy_ = new TLyTuScEnemy(this);
    lyTuTSL_ = new TLyTuScTSL(this);
    lyTuVS_ = new TLyTuScVS(this);
    lyTechno_ = new TLyTechno(this, this->frame_, lyTuScBar_);
    lyProfile_ = new TLyProfile(this, this->frame_, lyTuScBar_);


    pmgUI_->Init((enSceneName)GetMyScene());
    //UIMgに登録
    pmgUI_->SetUIFrame(frame_);
    pmgUI_->SetUILayer(lyTuScBar_);//アクションの優先度が高くなる
    pmgUI_->SetUILayer(scgrp_);
    pmgUI_->SetUILayer(lyTuMap_);
    pmgUI_->SetUILayer(lyTuEnemy_);
    pmgUI_->SetUILayer(lyTuTSL_);
    pmgUI_->SetUILayer(lyTuVS_);

    pmgUI_->SetUILayer(lyTechno_);
    pmgUI_->SetUILayer(lyProfile_);
    //UIMap
    pmgUI_->LoadLayer();
    pmgUI_->LoadBG("map_bg");//背景

    //setlayer_f_ = TRUE;
    //goto_f_ = FALSE;
    //１がスタート

    //lyTSL_->stLy_.lyphase_ = 1;
    //pmgEO_->stSeq_.PushLastScene(TStSnSeq::ls_Map);//マップから来ました
    lyTuScBar_->Ready();
    lyTuMap_->Ready();
  }


  void TSnScenario::DoFreeLayer()
  {
    //if (setlayer_f_ == FALSE) return;
    pmgUI_->FreeUILayer();
    pmgUI_->Init(snEND);
    Freemem(frame_);
    //setlayer_f_ = FALSE;
  }

  //試合に進む
  void TSnScenario::GoToShiai()
  {
    SetGotoNext();
    //goto_f_ = TRUE;
    backhome_f_ = FALSE;
  }
  void TSnScenario::GoToHome()
  {
    SetGotoNext();
    //goto_f_ = TRUE;
    backhome_f_ = TRUE;
  }

  //更新
  void TSnScenario::DoUpdate()
  {
    if (IsGotoNext())// || IsGotoTitle())
    {
      enSceneName next = (backhome_f_)
        ? snHome
        : snShiai;

      SetNextSceneQuick(next);

      //レイヤ解放
      FreeLayer();
    }

    if (IsSetLayerFlag() == FALSE) return;
    pmgUI_->Update();
  }

  //描画
  void TSnScenario::DoDraw()
  {
    if (IsSetLayerFlag() == FALSE) return;
    pmgUI_->Draw(TRUE);
  }
  void TSnScenario::DoGotoTitleFilter()
  {
    if (tuto_f_)
    {
      lyTuScBar_->SetFilterAll(TRUE);
    }
    else
    {
      lyScBar_->SetFilterAll(TRUE);
    }
   
    //SetNextTitle();
  }


}
