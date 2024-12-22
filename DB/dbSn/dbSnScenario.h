#pragma once
#include "dbScene.h"
#include "dbLyList_Scenario.h"

namespace db
{

  //モードセレクト///////////////////////////
  class TSnScenario : public TScene
  {
  public:
    TUIFrame* frame_;
    TLyScGrp* scgrp_;
    TLyScMap* lyMap_;
    TLyScEnemy* lyEnemy_;
    TLyScTSL* lyTSL_;
    TLyScVS* lyVS_;
    TLyScBar* lyScBar_;
    TLyTechno* lyTechno_;
    TLyProfile* lyProfile_;

    // チュートリアル用
    TLyTuScBar*	lyTuScBar_;
    TLyTuScMap* lyTuMap_;
    TLyTuScEnemy* lyTuEnemy_;
    TLyTuScTSL* lyTuTSL_;
    TLyTuScVS* lyTuVS_;

    explicit TSnScenario(TGame* pGame, TMgUI* pMgUI, s32 myScene);
    virtual ~TSnScenario(void);

    void GoToShiai();
    void GoToHome();
  protected:
    //継承
    void DoSetLayer();
    void DoFreeLayer();
    void DoGotoTitleFilter();
    void DoReady();
    void DoUpdate();
    void DoDraw();
    //BOOL connect_f_;
    //BOOL setlayer_f_;
    //BOOL goto_f_;
    BOOL backhome_f_;
    BOOL tuto_f_;
    //void SetLayer();
    //void FreeLayer();

    void SetLayerTuto(void);	// サブチュートリアル専用の世界

    void DoSetLayer_Normal();
    void DoSetLayer_Tuto();
  };
}
