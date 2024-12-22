#include "dbUGRText.h"

namespace db
{
  //コンストラクタ
  TUGRText::TUGRText(TUIGenLayer* player) :TUGGen(player)
  {
    stText_.Init();
  }

  TUGRText::~TUGRText()
  {

  }

  void TUGRText::MakeGroup(TUIObj* parent, s32 linenum)
  {
    SetParent(parent);
    //field_ = MakeObj();
    //field_->o_SetParent(parent);
    //field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_msg_frame));

    //const s32 BX = 7;
    //const s32 BY = 14;
    const s32 DY = 16;
    for (s32 i = 0; i < linenum; ++i)
    {
      TUIObj* newtext = MakeObj();
      newtext->o_SetParent(field_);
      newtext->z_SetPos(0, (i * DY));
      text_.push_back(newtext);
    }
    SetDraw(TRUE);
  }

  void TUGRText::SetData(const STRVECTOR& textvec)
  {
    for (s32 i = 0; i < text_.size(); ++i) text_[i]->g_SetDraw(FALSE);

    for (s32 i = 0; i < textvec.size(); ++i)
    {
      text_[i]->f_MakeFont(textvec[i].c_str(), FC2_normal);
      text_[i]->g_SetDraw(TRUE);
    }
  }

  void TUGRText::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    for (s32 i = 0; i < text_.size(); ++i) text_[i]->Update(draw_f);
  }

  void TUGRText::DoAction()
  {

  }
}
