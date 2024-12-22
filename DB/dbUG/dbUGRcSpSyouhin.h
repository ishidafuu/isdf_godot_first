#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
namespace db
{

  class TUGRcSpSyouhin :public TUGRcNormal
  {
  protected:
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TUGRcSpSyouhin(TUIGenLayer* player);
    virtual ~TUGRcSpSyouhin(void);
    void SetSyouhinDt(s64 mid_spsyouhin);
    void SetBuySozaiDt(s64 mid_spsyouhin, s64 mid_bbabuy);
    void SetGachaDt(s64 mid_gacha);
  };
}
