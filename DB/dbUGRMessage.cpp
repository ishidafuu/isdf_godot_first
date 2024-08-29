#include "dbUGRMessage.h"

namespace db
{
  const s32 MSGMAX = 4;
  //コンストラクタ
  TUGRMessage::TUGRMessage(TUIGenLayer* player) :TUGGen(player)
  {
    stMsg_.Init();
  }

  TUGRMessage::~TUGRMessage()
  {

  }

  void TUGRMessage::MakeGroup(TUIObj* parent, BOOL titlebar_f)
  {
    SetParent(parent);
    //field_ = MakeObj();
    //field_->o_SetParent(parent);
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_msg_frame));
    if (titlebar_f)
    {
      stMsg_.title_f_ = TRUE;
      field_->g_SetPartNo(1);
      title_ = MakeObj();
      title_->o_SetParent(field_);
      title_->z_SetPos(7, 5);
    }

    const s32 BX = 7;
    const s32 BY = (titlebar_f) ? 14 : 4;
    const s32 DY = 16;
    for (s32 i = 0; i < MSGMAX; ++i)
    {
      TUIObj* newtext = MakeObj();
      newtext->o_SetParent(field_);
      newtext->z_SetPos(BX, BY + (i * DY));
      text_.push_back(newtext);
    }

    SetDraw(FALSE);
  }
  void TUGRMessage::MakeGroup_Mini(TUIObj* parent, BOOL titlebar_f)
  {
	  MakeGroup(parent, titlebar_f);
	  field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_msg_frame_mini));
  }

  void TUGRMessage::SetData(STRVECTOR* strvec)
  {

    for (s32 i = 0; i < text_.size(); ++i) text_[i]->g_SetDraw(FALSE);

    s32 i2 = 0;
    for (s32 i = 0; i < strvec->size(); ++i)
    {
      if (stMsg_.title_f_ && (i == 0))
      {
        title_->f_MakeFont_Mini(strvec->at(i).c_str(), FC2_mini);
        title_->g_SetDraw(TRUE);
      }
      else
      {
        text_[i2]->f_MakeFont(strvec->at(i).c_str(), FC2_normal);
        text_[i2]->g_SetDraw(TRUE);
        ++i2;
      }
    }
  }
  void TUGRMessage::SetEvent(s32 mid_event)
  {
	  if (stMsg_.title_f_)
	  {
		  title_->f_MakeFont_Mini(pmgEO_->mgCoU_.GetEventDate(mid_event).c_str(), FC2_mini);
	  }
    for (s32 i = 0; i < MSGMAX; ++i)
    {
      enmdm_EventStr textno = (enmdm_EventStr)(mdm_event_text0_c64 + i);
      text_[i]->f_MakeFont(pmgEO_->mgCoM_.mdm_Event_.GetPKDtStr(mid_event, textno).c_str(), FC2_normal);
    }
  }


  void TUGRMessage::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    for (s32 i = 0; i < text_.size(); ++i) text_[i]->Update(draw_f);
    if (stMsg_.title_f_)
    {
      title_->Update(draw_f);
    }
  }

  void TUGRMessage::DoAction()
  {

  }
}
