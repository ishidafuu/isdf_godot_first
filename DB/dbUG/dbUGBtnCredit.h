#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{

  struct TUGBtnCreditSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnCredit : public TUGButton
  {
  protected:
    TUIObj* cb_star_;
    TUIObj* cb_todd_;
    void DoUpdateSub(BOOL draw_f);
  public:   
    TUGBtnCreditSt stBtnCredit_;
    //コンストラクタ
    explicit TUGBtnCredit(TUIGenLayer* player);
    virtual ~TUGBtnCredit(void);
    void MakeGroupCredit(TUIObj* parent);
    void Refresh();
  };

 
}
