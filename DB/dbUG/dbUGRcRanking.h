#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
namespace db
{

  class TUGRcRanking :public TUGRcNormal
  {
  protected:
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TUGRcRanking(TUIGenLayer* player);
    virtual ~TUGRcRanking(void);
    void SetRankingDt(s64 mid_Ranking);
  };
}
