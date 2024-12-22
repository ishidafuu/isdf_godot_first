#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGButton.h"

namespace db
{
  
  struct TUGRcSynSt
  {
    s32 charid_;
    s32 defshotid_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGRcSyn :public TUGRcNormal
  {
  public:
    //コンストラクタ
    explicit TUGRcSyn(TUIGenLayer* player);
    virtual ~TUGRcSyn(void);
    void SetShotDt(s32 posno, s32 charid);
    void ClearShotDt();
    s32 GetCharID();
    s32 GetDefShotID();
  protected:
    void DoActionSub();
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
    TUGRcSynSt stSyn_;
    //TUIObj* step_;
    TUIObj* chest_;
  };

}
