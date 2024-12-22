#include "dbUGLabel.h"


namespace db
{
  //コンストラクタ
  TUGLabel::TUGLabel(TUIGenLayer* player) : TUGGen(player)//アクションなし
    , ugname_(player)
  {
    stLabel_.Init();
  }

  TUGLabel::~TUGLabel()
  {

  }
  void TUGLabel::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);

    //field_->p_SetPop();
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_lab_word));
    field_->g_SetDepth(UIDP_09MSGBOX);
    labelend_ = MakeObj();
    labelend_->o_SetParent(parent);//親を自分のフィールドに変えたので位置を修正する
    labelend_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_lab_end));
    labelend_->g_SetDepth(UIDP_09MSGBOX);

    ugname_.MakeGroup(field_);
    ugname_.SetLabel(TRUE);
    ugname_.SetPos(8, 3);
  }

  void TUGLabel::Refresh()
  {
    stLabel_.www_ = (ugname_.GetNameLen() * GRIDSIZE / 3) + (GRIDSIZE * 2);
    //s32 asdf = parent_->g_GetSize().width_;
    //s32 posx = ((parent_->g_GetSize().width_ - stLabel_.www_) / 2) + 10;
    s32 posx = (- stLabel_.www_ / 2);
    const s32 posy = -(GRIDSIZE * 4);

    field_->g_SetSrcW(stLabel_.www_);
    field_->z_SetPos(posx, posy);
    labelend_->z_SetPos(posx + stLabel_.www_, posy);
  }
  void TUGLabel::DoUpdate(BOOL draw_f)
  {
    //文字列長さ０は無効
    if (ugname_.GetNameLen() == 0) return;

    if (stLabel_.www_ != ugname_.GetNameLen()) Refresh();

    field_->Update(draw_f);
    labelend_->Update(draw_f);
    ugname_.Update(draw_f);
  }

}
