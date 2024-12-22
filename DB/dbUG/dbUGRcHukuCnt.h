#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGRName.h"

namespace db
{

  class TUGRcHukuCnt :public TUGRcNormal
  {
  protected:
    //s64 mid_gachacnt_;
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    //TUIObj* subget_;

    //コンストラクタ
    explicit TUGRcHukuCnt(TUIGenLayer* player);
    virtual ~TUGRcHukuCnt(void);
    void SetHukuCntDt(s32 mid_Hukucnt);
  };
}
