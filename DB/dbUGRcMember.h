#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcGen.h"
namespace db
{

  class TUGRcMember :public TUGRcGen
  {
  protected:
    s32 DoMakeGroupSub();
  public:
    TUIObj* body_;
    TUIObj* name_;
    TUIObj* pow_;
    TUIObj* guts_;
    TUIObj* spd_;
    TUIObj* tec_;
	TUIObj* lv_;

    //コンストラクタ
    explicit TUGRcMember(TUIGenLayer* player);
    virtual ~TUGRcMember(void);
    void SetDt(s32 teamid, s32 posno);
  };
}
