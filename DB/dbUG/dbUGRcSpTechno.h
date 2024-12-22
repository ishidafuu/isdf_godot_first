#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
namespace db
{

  class TUGRcSpTechno :public TUGRcNormal
  {
  protected:
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TUGRcSpTechno(TUIGenLayer* player);
    virtual ~TUGRcSpTechno(void);
    void SetTechnoDt(s64 mid_spTechno);
    void SetGachaDt(s64 mid_gacha);
  };
}
