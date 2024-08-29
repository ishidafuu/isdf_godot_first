#pragma once
#include "dbLyShGen.h"


namespace db
{
  

  class TLyShRetry :public TLyShGen
  {
  protected:
    TUIObj* filter_;
    s32 msgwait_c_;

    void MsgRetry();
    void MvPage_Retry();
    void MvPage_Error();
    void MvPage_Out();

    void Post_Retry();
    void Post_RetryStar();
    void Post_Retire();
	void Post_TimeAttackRetire();
    void Connect_Retry();
    void Connect_RetryStar();
    void Connect_Retire();
	void Connect_TimeAttackRetire(void);

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyShRetry(TSnShiai* pScene);
    virtual ~TLyShRetry(void);
    void Idle();
  };
 
}
