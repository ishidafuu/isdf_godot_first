#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{

  class TUGBtnTokusyou : public TUGButton
  {
  protected:
    void DoActionSub();
    void DoMakeGroupSub(TUIObj* parent);
  public:   
    //コンストラクタ
    explicit TUGBtnTokusyou(TUIGenLayer* player);
    virtual ~TUGBtnTokusyou(void);
    
  };

 
}
