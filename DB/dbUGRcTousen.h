#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGRName.h"

namespace db
{
  class TUGRcTousen :public TUGRcNormal
  {
  protected:
    //s64 mid_gachacnt_;
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TUGRcTousen(TUIGenLayer* player);
    virtual ~TUGRcTousen(void);
    void SetDt_Gacha(s64 log_tousen_no);
    void SetDt_Huku(s64 log_tousen_no);
  };
}
