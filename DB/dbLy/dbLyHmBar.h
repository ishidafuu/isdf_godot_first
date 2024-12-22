#pragma once
#include "dbLyBarGen.h"
#include "dbUGFinger.h"
#include "dbUGLock.h"

namespace db
{
  struct TTLyHmBarSt
  {
    BOOL inside_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TLyHmGrp;
  class TSnHome;
  class TUGFinger;

  //ヘッダフッタ////////////////////////////////
  class TLyHmBar :public TLyBarGen
  {
  public:
    TTLyHmBarSt stHmBar_;

    //コンストラクタ
    explicit TLyHmBar(TSnHome* pScene);
    virtual ~TLyHmBar(void);
    void ReadyHome();

    void FromOther();
    void FromTuto();
    void FromShiai();
    void FromMap();

	void JumpFooter(s32 tabno);	// オーバーロード

    void SwitchInside(BOOL inside_f);

    // サブチュートリアル誘導指カーソル
    void SetDrawFinger(BOOL draw_f);
    BOOL IsTutoFinger();
  protected:
    TLyHmGrp* hmgrp_;
    TSnHome* pScene_;
    TUGFinger ugfinger_;
	TUGLock uglock_;

    //オーバーロード
    void DoOpenHeader();
    void DoCloseHeader();
    void DoJumpHeader();
    void DoOpenTechno();
    void DoOpenProfile();
    void DoMoveFilter();

    void ReadyTeamTab(s32 mid_haikei);
    void ReadyShiaiTab();
	void ReadyEventTab();
	void ReadyShopTab(void);

    void DoMoveFilterAll();
    void DoLoadSub();
    void DoUpdateSub(BOOL draw_f);
  };

}
