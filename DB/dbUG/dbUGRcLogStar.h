#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGRName.h"

namespace db
{
  //獲得、消費ログ
  class TUGRcLogStar :public TUGRcNormal
  {
  protected:
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TUGRcLogStar(TUIGenLayer* player);
    virtual ~TUGRcLogStar(void);
    void SetLogBuyStarDt(s64 id_log_bstar);
    void SetLogUseStarDt(s64 id_bstar, BOOL bstar_f, BOOL fstar_f);
  };
}
