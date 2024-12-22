#include "dbUGBtnCourt.h"
#include "dbMgUI.h"
#include "dbUIGenLayer.h"
#include "dbGame.h"

namespace db
{

  //コンストラクタ
  TUGBtnCourt::TUGBtnCourt(TUIGenLayer* player) :TUGButton(player)
  {
    stBtnCourt_.Init();
  }
  TUGBtnCourt::~TUGBtnCourt(void)
  {
  }
  void TUGBtnCourt::MakeGroupCourt(TUIObj* parent)
  {
    GenMakeGroup(parent, TRUE);//センタリングする
    //SetDepth(UIDP_05BUTTON);
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_btn_icon));
    field_->b_SetModal(TRUE);
    field_->g_SetPartNo(0);//0番画像
    field_->f_MakeFont("コート", FC2_normal);
    field_->f_FontPos(-24, 0);
    ugricon_.SetIcon(ugi_ScAway);
  }
  BOOL TUGBtnCourt::IsHome()
  {
    return (stBtnCourt_.home_f_);
  }

  //更新
  void TUGBtnCourt::DoUpdateSub(BOOL draw_f)
  {

  }

  //更新
  void TUGBtnCourt::DoActionSub()
  {
    if (IsAct() == FALSE) return;

    stBtnCourt_.home_f_ = !(stBtnCourt_.home_f_);
    //アイコン切り替え
    if (stBtnCourt_.home_f_)
    {
      ugricon_.SetIcon(ugi_ScHome);
    }
    else
    {
      ugricon_.SetIcon(ugi_ScAway);
    }
  }

}