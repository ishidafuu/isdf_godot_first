#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
#include "dbUGLabel.h"
namespace db
{

  struct TUGBtnLabelSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnLabel : public TUGButton
  {
  protected:
    void DoMakeGroupSub(TUIObj* parent);
    void DoUpdateSub(BOOL draw_f);
  public:   
    TUGLabel uglabel_;
    TUGBtnLabelSt stBtnLabel_;
    //コンストラクタ
    explicit TUGBtnLabel(TUIGenLayer* player);
    virtual ~TUGBtnLabel(void);
    //void MakeGroupLabel(TUIObj* parent);
  };

 
}
