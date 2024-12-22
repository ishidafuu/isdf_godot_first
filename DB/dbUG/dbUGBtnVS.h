#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
#include "dbUGRName.h"

namespace db
{

  struct TUGBtnVSSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnVS : public TUGButton
  {
  public:   
    //コンストラクタ
    explicit TUGBtnVS(TUIGenLayer* player);
    virtual ~TUGBtnVS(void);
    void MakeGroupVS(TUIObj* parent, base::TGrPart* pgrpart);
  protected:
    void DoUpdateSub(BOOL draw_f);
    TUGRName ugname_;
    TUGBtnVSSt stBtnVS_;
  };

 
}
