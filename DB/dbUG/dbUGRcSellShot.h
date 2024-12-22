#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGRName.h"
#include "dbUGButton.h"

namespace db
{

  enum enUGRcSellShotAct
  {
    enUGRcSellShot_Field,
    enUGRcSellShot_Eye,
  };

  struct TUGSellShotSt
  {
    s64 id_shot_;
    void Init()
    {
      Zeromem(this);
      id_shot_ = NGNUM;
    }
  };

  class TUGRcSellShot :public TUGRcNormal
  {
  protected:
    //s64 mid_gachacnt_;
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
    void DoActionSub();
  public:
    TUGSellShotSt stSellShot_;
    //TUGButton ugbtn_eye_;
    //コンストラクタ
    explicit TUGRcSellShot(TUIGenLayer* player);
    virtual ~TUGRcSellShot(void);
    void SetShotDt(s64 id_shot);
  };
}
