#pragma once
#include "dbLyShGen.h"

namespace db
{
  class TLyShVSError :public TLyShGen
  {
  protected:
    TUIObj* filter_;

    void MvPage_Error();

    void Post_VSError();
    void Connect_VSError();
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyShVSError(TSnShiai* pScene);
    virtual ~TLyShVSError(void);
  };
 
}
