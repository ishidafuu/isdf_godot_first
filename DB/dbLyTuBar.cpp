#include "dbLyTuBar.h"
#include "dbSnTutorial.h"
//=========================================================================
// 
// チュートリアルシーンクラス
//
//=========================================================================
namespace db
{

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  TLyTuBar::TLyTuBar(TSnTutorial* pScene) :TLyBarGen(pScene, pScene->frame_)
  {
    pScene_ = pScene;
    tugrp_ = pScene->tuGrp_;
    stTuBar_.Init();
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  TLyTuBar::~TLyTuBar()
  {

  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBar::DoOpenHeader(void)
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBar::DoCloseHeader(void)
  {
    switch (stBar_.tabNo_)
    {
    case tab_Test:pScene_->lyTuTest_->Ready(); break;
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBar::DoJumpHeader(void)
  {

  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBar::DoOpenTechno(void)
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBar::DoOpenProfile(void)
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBar::DoMoveFilter(void)
  {
    //レイヤ移動によるクローズ
    for (s32 i = 0; i < tabs_.size(); ++i) tabs_[i]->Down();

    s64 mid_haikei = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei);

    switch (stBar_.nexttabNo_)
    {
    case 0://ホーム
      if (stBar_.tabNo_ != tab_Test)
      {
        pmgUI_->LoadCourt(mid_haikei);//背景
        pScene_->lyTuTest_->Ready();
      }
      break;
    }

    stBar_.tabNo_ = stBar_.nexttabNo_;
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBar::DoMoveFilterAll(void)
  {
    //レイヤ移動によるクローズ
    for (s32 i = 0; i < tabs_.size(); ++i) tabs_[i]->Down();

    switch (stTuBar_.tutotab_)
    {
    case tab_Shop://ホームからショップへ
      pScene_->lyTuHome_->SetTutoEnd();
      break;
    case tab_Center: //ショップからセンターへ
      pScene_->lyTuShop_->SetTutoEnd();
      break;
    case tab_End://センターから終了へ
      pScene_->lyTuCenter_->SetTutoEnd();
      break;
    }
  }


  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBar::ReadyTutorial(void)
  {
    //まずBarのReady
    Ready();

    s64 mid_haikei = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei);
    mid_haikei = 1;
    pmgUI_->LoadCourt(mid_haikei);//背景
    pScene_->lyTuTest_->Ready();
    stBar_.tabNo_ = tab_Test;

    ugfooter_.ugradio_.SetSelect(stBar_.tabNo_);
  }

  void TLyTuBar::GotoTab(enTutoTab tutotab)
  {
    SetFilterAll_AutoOff();
    stTuBar_.tutotab_ = tutotab;
  }

}