#pragma once
#include "dbLyBarGen.h"

namespace db
{
  enum enScLayer
  {
    scl_Map,
    scl_Enemy,
    scl_TSL,
    scl_VSFilter,
    scl_VS,
    scl_Shiai,
    scl_Home,
  };

  struct TTLyScBarSt
  {
    enScLayer nowLayer_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //struct TTLyScBarSt
  //{
  //  enScLayer nowLayer_;
  //  s32 filter_c_;
  //  s32 filter2_c_;
  //  s32 nexttabNo_;
  //  s32 tabNo_;
  //  BOOL hud_f_;
  //  s32 hudtype_;
  //  s32 refresh_c_;
  //  BOOL barmove_f_;
  //  
  //  BOOL mvtechno_f_;
  //  BOOL mvprofile_f_;

  //  void Init()
  //  {
  //    Zeromem(this);
  //  }
  //};

  class TLyScGrp;
  class TSnScenario;
  //ヘッダフッタ////////////////////////////////
  class TLyScBar :public TLyBarGen
  {
  protected:
    TLyScGrp* scgrp_;
    TSnScenario* pScene_;

    //オーバーロード
    void DoOpenHeader();
    void DoCloseHeader();
    void DoJumpHeader();
    void DoOpenTechno();
    void DoOpenProfile();
    void DoMoveFilter();
    void DoMoveFilterAll();
    void DoBackBtn();
  public:
    TTLyScBarSt stScBar_;

    //コンストラクタ
    explicit TLyScBar(TSnScenario* pScene);
    virtual ~TLyScBar(void);
    void MoveLayer(enScLayer mvlayer);
  };

}
