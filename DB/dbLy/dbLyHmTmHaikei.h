#pragma once
#include "dbLyHmGen.h"
#include "dbUGLiHaikei.h"
#include "dbUGDrHaikei.h"
#include "dbUGFilter.h"
#include "dbUGHaikeiList.h"


namespace db
{
  class TLyHmTmHaikei :public TLyHmGen
  {
  protected:
    TUGFilter ugfilter_;

	s64 mid_haikei_;
	TUGHaikeiList ugHaikei_;

    void RefreshHaikeiDt(BOOL first_f);

    void MvPage_Haikei();
    void MvPage_SetHaikei();

    void Post_SetHaikei();
    void Connect_SetHaikei();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyHmTmHaikei(TSnHome* pScene);
    virtual ~TLyHmTmHaikei(void);
  };
 
}
