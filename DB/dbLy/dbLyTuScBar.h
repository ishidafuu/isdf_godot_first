#pragma once
#include "dbLyBarGen.h"
#include "dbUGSerifu.h"
#include "dbUGFinger.h"

namespace db
{
  enum enTuScLayer
  {
    tscl_Map,
    tscl_Enemy,
    tscl_TSL,
    tscl_VSFilter,
    tscl_VS,
    tscl_Shiai,
    tscl_Home,
  };

  struct TTLyTuScBarSt
  {
    enTuScLayer nowLayer_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TLyScGrp;
  class TSnScenario;
  //ヘッダフッタ////////////////////////////////
  class TLyTuScBar :public TLyBarGen
  {
  protected:
    TLyScGrp* scgrp_;
    TSnScenario* pScene_;

	TUGSerifu ugserifu_;	//セリフ
	TUGFinger ugfinger_;
	TUGFinger ugfinger2_;	// もう一つ用意
	TUGFinger ugfinger3_;	// もう一つ用意
	STRVECTOR serifuvec_;

    //オーバーロード
    void DoOpenHeader();
    void DoJumpHeader();
    void DoCloseHeader();
    void DoOpenTechno();
    void DoOpenProfile();
    void DoMoveFilter();
    void DoMoveFilterAll();
    void DoBackBtn();
    void DoUpdate(BOOL draw_f);
  public:
	  TTLyTuScBarSt stScBar_;

	  // げったー各種
	  TUGSerifu* GetSerifuWindow(void){ return &ugserifu_; }
	  TUGFinger* GetFinger(void){ return &ugfinger_; }
	  TUGFinger* GetFinger2(void){ return &ugfinger2_; }
	  TUGFinger* GetFinger3(void){ return &ugfinger3_; }
	  STRVECTOR* GetSerifuVec(void){ return &serifuvec_; }

    //コンストラクタ
    explicit TLyTuScBar(TSnScenario* pScene);
    virtual ~TLyTuScBar(void);
	void MoveLayer(enTuScLayer mvlayer);

	void DoReady(void);
  };

}
