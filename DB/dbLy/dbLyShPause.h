#pragma once
#include "dbLyShGen.h"

namespace db
{
  class TLyShPause :public TLyShGen
  {
  protected:
    TUIObj* filter_;

    void MvPage_Pause();

    void Post_Retire();
    void Post_VSHumRetire();
    void Post_TourRetire();
	void Post_TimeAttackRetire();
    void Connect_Retire();
    void Connect_VSHumRetire();
    void Connect_TourRetire();
	void Connect_TimeAttackRetire();
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyShPause(TSnShiai* pScene);
    virtual ~TLyShPause(void);
  };
 
}
