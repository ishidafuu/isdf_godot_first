#include "dbUGBtnTokusyou.h"
#include "dbMgUI.h"

namespace db
{
  //コンストラクタ
  TUGBtnTokusyou::TUGBtnTokusyou(TUIGenLayer* player) :TUGButton(player)
  {
  }
  TUGBtnTokusyou::~TUGBtnTokusyou(void)
  {
  }
  void TUGBtnTokusyou::DoMakeGroupSub(TUIObj* parent)
  {
    GetField()->b_SetModal(TRUE);
    SetDepth(UIDP_09MSGBOX);
  }

  void TUGBtnTokusyou::DoActionSub()
  {
    if (IsAct())
    {
      mid::midTokusyouhou();//特賞法
    }
  }

}