#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{

  struct TUGBtnCourtSt
  {
    BOOL home_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnCourt : public TUGButton
  {
  protected:
    //TUIObj* gb_ggz_;
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
  public:   
    TUGBtnCourtSt stBtnCourt_;
    //コンストラクタ
    explicit TUGBtnCourt(TUIGenLayer* player);
    virtual ~TUGBtnCourt(void);
    void MakeGroupCourt(TUIObj* parent);

    BOOL IsHome();
  };

 
}
