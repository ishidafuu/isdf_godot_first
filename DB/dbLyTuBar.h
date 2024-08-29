#pragma once
#include "dbLyBarGen.h"
//=========================================================================
// 
// チュートリアルシーンクラス
//
//=========================================================================
namespace db
{
  struct TTLyTuBarSt
  {
    s32 tutotab_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TLyTuGrp;
  class TSnTutorial;

  class TLyTuBar :public TLyBarGen
  {
  public:
    enum enTutoTab
    {
      tab_Home,
      tab_Shop,
      tab_Center,
      tab_End,
      tab_Test,
    };
    //コンストラクタ
    explicit TLyTuBar(TSnTutorial* pScene);
    virtual ~TLyTuBar(void);
    void ReadyTutorial(void);
    void GotoTab(enTutoTab tutotab);
  protected:
    TLyTuGrp* tugrp_;
    TSnTutorial* pScene_;
    TTLyTuBarSt stTuBar_;
    //オーバーロード
    void DoOpenHeader(void);
    void DoCloseHeader(void);
    void DoJumpHeader(void);
    void DoOpenTechno(void);
    void DoOpenProfile(void);
    void DoMoveFilter(void);
    void DoMoveFilterAll(void);
  };
}
