#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGRName.h"

namespace db
{
  //懐かしボックスの筐体情報
  class TUGRcHuku :public TUGRcNormal
  {
  protected:
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TUGRcHuku(TUIGenLayer* player);
    virtual ~TUGRcHuku(void);
    void SetHukuDt(s64 mid_Huku);
  };
}
