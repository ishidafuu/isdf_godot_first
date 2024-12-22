#include "dbUGRBlink.h"


namespace db
{
  //コンストラクタ
  TUGRBlink::TUGRBlink(TUIGenLayer* player) :TUGGen(player)
  {
  }

  TUGRBlink::~TUGRBlink()
  {

  }

  void TUGRBlink::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    //field_->g_SetCenter();
    //field_->f_SetCenter(TRUE);
  }
  void TUGRBlink::SetMessage(const std::string& msg, s32 fontcol, BOOL blink_f)
  {
    field_->f_MakeFont(msg.c_str(), fontcol);
    stBrink_.Init();
    stBrink_.blink_f_ = blink_f;
  }
  void TUGRBlink::SetMessage_mini(const std::string& msg, s32 fontcol, BOOL blink_f)
  {
    field_->f_MakeFont_Mini(msg.c_str(), fontcol);
    stBrink_.Init();
    stBrink_.blink_f_ = blink_f;
  }
  void TUGRBlink::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    if (draw_f == FALSE)
    {
      if (stBrink_.blink_f_)
      {
        const s32 BRKITV = 80;
        const s32 BRKITV_HF = BRKITV / 2;
        lib_num::IncLoop(&stBrink_.blink_c_, 0, BRKITV);
        field_->g_SetDraw((stBrink_.blink_c_ < BRKITV_HF));
      }
    }
  }

  void TUGRBlink::SetLimY(s32 limu, s32 hhh)
  {
    field_->g_SetLimY(limu, hhh);
  }

}
