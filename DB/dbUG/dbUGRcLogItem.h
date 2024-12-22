#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGRName.h"

namespace db
{
  //獲得、消費ログ
  class TUGRcLogItem :public TUGRcNormal
  {
  protected:
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TUGRcLogItem(TUIGenLayer* player);
    virtual ~TUGRcLogItem(void);
    void SetLogGetItemDt(s64 id_itemget);
    void SetLogUseItemDt(s64 id_supplyuse);
  };
}
