#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{
  struct TUGRBlinkSt
  {
    BOOL blink_f_;
    s32 blink_c_;
    void Init()
    {
      Zeromem(this);
    }
  };
  class TUGRBlink : public TUGGen
  {
  public:
    explicit TUGRBlink(TUIGenLayer* player);
    virtual ~TUGRBlink(void);
    void MakeGroup(TUIObj* parent);
    void SetLimY(s32 limu, s32 hhh);
    void SetMessage(const std::string& msg, s32 fontcol, BOOL blink_f);
    void SetMessage_mini(const std::string& msg, s32 fontcol, BOOL blink_f);
  protected:
    void DoUpdate(BOOL draw_f);
    TUGRBlinkSt stBrink_;
  };

}