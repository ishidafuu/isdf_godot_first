#include "dbUGInputTip.h"


namespace db
{
  //コンストラクタ
  TUGInputTip::TUGInputTip(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stInputTip_.Init();
  }
  TUGInputTip::~TUGInputTip()
  {

  }
  void TUGInputTip::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    //field_->p_SetPop();
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_lab_inputtip));
    field_->g_SetDepth(UIDP_09MSGBOX);
    field_->f_FontPos(7, 2);
    field_->z_SetPos(0, -(GRIDSIZE * 3));
  }
  void TUGInputTip::SetFontMC(s32 mojicode)
  {
    field_->f_MakeFontMC(mojicode, FC2_hl_normal);
  }
  void TUGInputTip::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
  }

}
