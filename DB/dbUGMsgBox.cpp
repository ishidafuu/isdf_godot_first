#include "dbUGMsgBox.h"

namespace db
{
  const s32 MSGMAX = 3;
  const s32 BBX = 44;
  const s32 BBY = 27;
  //コンストラクタ
  TUGMsgBox::TUGMsgBox(TUIGenLayer* player) :TUGGen(player)
    , ugbtn_ok_(player)
    , ugbtn_ng_(player)
    , ugcredit_(player)
  {
    stMsg_.Init();
  }

  TUGMsgBox::~TUGMsgBox()
  {

  }

  void TUGMsgBox::MakeGroup(TUIObj* parent)
  {
    const s32 BARW = 512;

    //ヘッダフッタ間フィルタ
    field_ = MakeObj();
    field_->o_SetParent(parent);
    field_->g_SetCenter(TRUE);
    field_->o_SetObjFilterGrp(BARW, BARW, 0xFF, 0, 0, 0);
    field_->g_SetAlp(FALSE, 0x80);
    field_->g_SetDepth(UIDP_08MSGFILTER);

    panel_ = MakeObj();
    panel_->o_SetParent(field_);
    //panel_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_msg_frame));
    panel_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_msg_frame));


    ugbtn_ok_.MakeGroup(panel_, ug_btn_ok);
    ugbtn_ok_.SetPos(-BBX, BBY);
    ugbtn_ok_.GetField()->b_SetModal(TRUE);
    ugbtn_ok_.SetFont("おーけー！", FC2_btn_normal);

    ugbtn_ng_.MakeGroup(panel_, ug_btn_ok);
    ugbtn_ng_.SetPos(+BBX, BBY);
    ugbtn_ng_.GetField()->b_SetModal(TRUE);
    ugbtn_ng_.SetFont("ダメダメ！", FC2_btn_normal);

    panel_->g_SetCenter(TRUE);
    panel_->g_SetDepth(UIDP_09MSGBOX);

    const s32 BY = -29;
    const s32 DY = 16;
    for (s32 i = 0; i < MSGMAX; ++i)
    {
      TUIObj* newtext = MakeObj();
      newtext->o_SetParent(panel_);
      newtext->z_SetPos(0, BY + (i * DY));
      newtext->f_SetCenter(TRUE);
      text_.push_back(newtext);
    }
    ugcredit_.MakeGroup(panel_);
    ugcredit_.SetPos(0, BY + ((MSGMAX - 1)* DY));
    SetDraw(FALSE);
  }

  void TUGMsgBox::SetData(STRVECTOR* strvec, BOOL ngbtn_f)
  {
    for (s32 i = 0; i < text_.size(); ++i) text_[i]->g_SetDraw(FALSE);

    s32 i2 = 0;
    for (s32 i = 0; i < strvec->size(); ++i)
    {
      text_[i2]->f_MakeFont(strvec->at(i).c_str(), FC2_normal);
      text_[i2]->g_SetDraw(TRUE);
      ++i2;
    }
    ugcredit_.SetDraw(FALSE);
    
    ugbtn_ng_.SetDraw(ngbtn_f);
    s32 posx = (ngbtn_f)
      ? -BBX
      : 0;
    ugbtn_ok_.SetPos(posx, BBY);
  }
  void TUGMsgBox::SetUseTodd(STRVECTOR* strvec, s32 num, BOOL get_f)
  {
    SetData(strvec, TRUE);
    ugcredit_.SetUseTodd(num, get_f);
    ugcredit_.SetDraw(TRUE);
  }
  void TUGMsgBox::SetUseStar(STRVECTOR* strvec, s32 num)
  {
    SetData(strvec, TRUE);
    ugcredit_.SetUseStar(num);
    ugcredit_.SetDraw(TRUE);
  }

  void TUGMsgBox::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    panel_->Update(draw_f);
    for (s32 i = 0; i < text_.size(); ++i) text_[i]->Update(draw_f);
    ugcredit_.Update(draw_f);
    ugbtn_ok_.Update(draw_f);
    ugbtn_ng_.Update(draw_f);
  }

  void TUGMsgBox::DoAction()
  {
    if (ugbtn_ok_.IsAct())
    {
      SetAct(enUGMsgBoxAct_OK);
    }
    else if (ugbtn_ng_.IsAct())
    {
      SetAct(enUGMsgBoxAct_NG);
    }
  }

  BOOL TUGMsgBox::IsActRes(enUGMsgBoxAct res) //返値ゲット
  {
    return (GetActRes() == res);
  }

}
