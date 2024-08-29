#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGRName.h"
#include "dbUGButton.h"

namespace db
{

  enum enUGRcMarukaAct
  {
    enUGRcMaruka_Field,
    enUGRcMaruka_Eye,
  };

  struct TUGMarukaSt
  {
    s64 id_maruka_;
    s64 mid_maruka_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGRcMaruka :public TUGRcNormal
  {
  protected:
    //s64 mid_gachacnt_;
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
    void DoActionSub();
  public:
    TUGMarukaSt stMaruka_;
    //TUGButton ugbtn_eye_;
    //コンストラクタ
    explicit TUGRcMaruka(TUIGenLayer* player);
    virtual ~TUGRcMaruka(void);
    void SetSozaiDt(s64 id_maruka);
  };
}
