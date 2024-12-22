#include "dbUGVS.h"


namespace db
{
  //コンストラクタ
  TUGVS::TUGVS(TUIGenLayer* player) :TUGGen(player)
  {
  }

  TUGVS::~TUGVS()
  {

  }

  void TUGVS::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    field_->g_SetRevDepthLv(1);
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_vs_vsset));
    field_->z_SetPos(0, -72);
    field_->g_SetCenter(TRUE);
  }
  void TUGVS::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
  }

  void TUGVS::DoAction()
  {
  }
}
