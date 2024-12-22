#include "dbUGLock.h"


namespace db
{
  //コンストラクタ
  TUGLock::TUGLock(TUIGenLayer* player) :TUGGen(player)
  {
  }

  TUGLock::~TUGLock()
  {

  }

  void TUGLock::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    //field_->g_SetRevDepthLv(1);
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_lock));
    //field_->z_SetPos(0, -72);
    field_->g_SetCenter(TRUE);
  }
  void TUGLock::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
  }

  void TUGLock::DoAction()
  {
  }
}
