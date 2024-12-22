#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGRName.h"

namespace db
{

  class TUGRcProb :public TUGRcNormal
  {
  protected:
    //s64 mid_gachacnt_;
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TUGRcProb(TUIGenLayer* player);
    virtual ~TUGRcProb(void);
    void SetDt(s32 mid_GachaGrp, s32 prob);
  };
}
