#include "dbUGBtnLabel.h"
#include "dbMgUI.h"

namespace db
{
  //コンストラクタ
  TUGBtnLabel::TUGBtnLabel(TUIGenLayer* player) :TUGButton(player)
    , uglabel_(player)
  {
  }
  TUGBtnLabel::~TUGBtnLabel(void)
  {
  }
  void TUGBtnLabel::DoMakeGroupSub(TUIObj* parent)
  {
    uglabel_.MakeGroup(field_);
  }
  //更新
  void TUGBtnLabel::DoUpdateSub(BOOL draw_f)
  {
    if (field_->b_IsEnter(TRUE))
    {
      uglabel_.Update(draw_f);
    }
  }

}