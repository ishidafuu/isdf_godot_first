#include "dbPoBank.h"

namespace db
{
  TPostBank::TPostBank(TMgEngineObj* pmgEO):TPoGen(pmgEO)
  {
  }

  TPostBank::~TPostBank(void)
  {
  }

  //送信パック初期化
  void TPostBank::Ready_create(s64 mid_supply, s32 techno)
  {
    ClearMap();
    sendmap_.longmap_["mid_supply"] = mid_supply;
    sendmap_.longmap_["techno"] = techno;
    sendmap_.longmap_["mobacoin"] = (mid::midIsADR())
        ? 1
        : 0;
    sendmap_.longmap_["sandbox"] = (mid::midIsSandbox())
        ? 1
        : 0;

    StartConnect("bank_create");
  }

  void TPostBank::Ready_close(const char* tid)
  {
    ClearMap();
    sendmap_.strmap_["tid"] = tid;
    sendmap_.longmap_["sandbox"] = (mid::midIsSandbox())
        ? 1
        : 0;
    StartConnect("bank_close");
  }
  void TPostBank::Ready_cancel(const char* tid)
  {
    ClearMap();
    sendmap_.strmap_["tid"] = tid;
    sendmap_.longmap_["sandbox"] = (mid::midIsSandbox())
        ? 1
        : 0;
    StartConnect("bank_cancel");
  }

}
