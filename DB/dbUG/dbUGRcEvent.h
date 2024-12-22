#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcGen.h"
namespace db
{

  class TUGRcEvent :public TUGRcGen
  {
  protected:
    s64 mid_event_;
    s32 DoMakeGroupSub();
  public:
    TUIObj* banner_;
    TUIObj* name_;
    //TUIObj* nokori_;

    //コンストラクタ
    explicit TUGRcEvent(TUIGenLayer* player);
    virtual ~TUGRcEvent(void);
    void SetDt(s64 mid_event);
    s64 GetMIDEvent();
  };
}
