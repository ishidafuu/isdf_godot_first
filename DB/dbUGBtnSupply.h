#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{

  struct TUGBtnSupplySt
  {
    s32 supplytype_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnSupply : public TUGButton
  {
  protected:
    //TUIObj* gb_ggz_;
    void GenMakeGroupSupply(TUIObj* parent, s32 stype, s32 mid_supply);
    void DoUpdateSub(BOOL draw_f);
  public:   
    TUGBtnSupplySt stBtnSupply_;
    //コンストラクタ
    explicit TUGBtnSupply(TUIGenLayer* player);
    virtual ~TUGBtnSupply(void);
    void MakeGroupSupply_GGZ(TUIObj* parent);
    void MakeGroupSupply_MGK(TUIObj* parent);
    void MakeGroupSupply_FKT(TUIObj* parent);
    void Refresh();
  };

 
}
