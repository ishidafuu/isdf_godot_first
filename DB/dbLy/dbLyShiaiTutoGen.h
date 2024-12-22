#pragma once
#include "dbConst_Tuto.h"
#include "dbLyShiaiGen.h"
#include "dbUGTutoMsgRetro.h"
#include "dbUGFinger.h"

namespace db
{
  class TSnShiai;

  struct TShTutoGenSt
  {
    s32 okNum_;
    s32 ngNum_;
    BOOL okclear_f_;
    s32 ngwait_c_;
    //s32 lastpg_;
    //BOOL gameset_f_;
    s32 pagetype_;
    BOOL normal_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  class TLyShiaiTutoGen :public TLyShiaiGen
  {
  protected:
    enum enTutoPageType
    {
      tpt_Info,
      tpt_Act,
      tpt_End,
    };
    BOOL IncActNum(s32 actindex);
    void SetPageData(s32 pageno, enSendTutoAct act, s32 pagetype);

    void MvPage_Info();
    void MvPage_Act();
    void MvPage_End();

    //void MvPage_GameAct();
    //void MvPage_Fin();
    void MvPage_Idle();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    //void DoIdle();
    void DoRestart();
    //void DoChangePage(s32 pageno);
    void DoUpdate(BOOL draw_f);

    virtual void DoLoadSub(){};
    virtual void DoReadySub(){};
    virtual void DoUpdateSub(BOOL draw_f){};
  public:
    TShTutoGenSt stTuto_;
    TUGFinger ugfinger_;
    TUGTutoMsgGen* ugtuto_;
    //TUGButton ugbtn_skip_;
    //コンストラクタ
    explicit TLyShiaiTutoGen(TSnShiai* pScene, TUGShiai* ugshiai);
    virtual ~TLyShiaiTutoGen(void);
  };

}
