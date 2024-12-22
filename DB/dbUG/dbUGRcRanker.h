#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
namespace db
{

  class TUGRcRanker :public TUGRcNormal
  {
  protected:
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TUGRcRanker(TUIGenLayer* player);
    virtual ~TUGRcRanker(void);
    void SetRankerDt(s64 id_ranker,s32 lineno, s32 id_ranking);
  };
}
