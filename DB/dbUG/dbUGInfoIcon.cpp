#include "dbUGInfoIcon.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGInfoIcon::TUGInfoIcon(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stInfoIcon_.Init();

  }

  TUGInfoIcon::~TUGInfoIcon()
  {

  }
  void TUGInfoIcon::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);

    icon_ = MakeObj();
    icon_->o_SetParent(field_);
    icon_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_item));
    icon_->g_SetPartNo(6);
    icon_->z_SetPos(0, 7);
    icon_->g_SetUnder();
    icon_->g_SetDepth(UIDP_01BGOBJ);
    icon_->g_SetRevDepthLv(1);
    icon_->g_SetDraw(FALSE);//デフォはアイコン無し

  }

  //アイコン表示リフレッシュ
  void TUGInfoIcon::Refresh()
  {
    icon_->g_SetDraw(pmgEO_->stSeq_.IsNewInfo());
  }
  void TUGInfoIcon::DoUpdate(BOOL draw_f)
  {
    icon_->Update(draw_f);
  }

}
