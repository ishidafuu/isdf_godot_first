#include "dbUGShPad.h"
#include "dbUIGenLayer.h"
#include "dbGame.h"

namespace db
{

  //ボタン種類
  enum enPadBtnType
  {
    pbt_Shot,
    pbt_Pass,
    pbt_Jump,
  };

  //コンストラクタ
  TUGShPad::TUGShPad(TUIGenLayer* player) :TUGGen(player)
  {
    stShPad_.Init();
  }

  TUGShPad::~TUGShPad()
  {
  }
  void TUGShPad::GenMakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    field_->g_SetDepth(UIDP_07BUTTONTOP);
    field_->g_SetCenter(TRUE);
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_pad_btn));
    field_->g_SetAlp(TRUE, 128);


    dfbal_ = MakeObj();
    dfbal_->g_SetCenter(TRUE);
    dfbal_->f_SetCenter(TRUE);
    dfbal_->o_SetParent(field_);
    dfbal_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_btn_balloon));
    dfbal_->z_SetPos(0, 32);
    dfbal_->g_SetAlp(TRUE, 160);

    ofbal_ = MakeObj();
    ofbal_->g_SetCenter(TRUE);
    ofbal_->f_SetCenter(TRUE);
    ofbal_->o_SetParent(field_);
    ofbal_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_btn_balloon));
    ofbal_->z_SetPos(0, 32);
    ofbal_->g_SetAlp(TRUE, 160);
  }
  void TUGShPad::MakeGroup_Shot(TUIObj* parent)
  {
    GenMakeGroup(parent);
    ofbal_->f_MakeFont("シュート", FC2_normal);
    dfbal_->f_MakeFont("キャッチ", FC2_normal);
    stShPad_.btntype_ = pbt_Shot;
    SetPos(56, 64);
  }
  void TUGShPad::MakeGroup_ShotPrev(TUIObj* parent)
  {
    GenMakeGroup(parent);
    stShPad_.btntype_ = pbt_Shot;
    SetPos(56, 56);
    stShPad_.prev_f_ = TRUE;
  }
  void TUGShPad::MakeGroup_Pass(TUIObj* parent)
  {
    GenMakeGroup(parent);
    ofbal_->f_MakeFont("パス", FC2_normal);
    dfbal_->f_MakeFont("ドッジ", FC2_normal);
    stShPad_.btntype_ = pbt_Pass;
    SetPos(136, 64);
  }
  void TUGShPad::MakeGroup_Jump(TUIObj* parent)
  {
    GenMakeGroup(parent);
    ofbal_->f_MakeFont("ジャンプ", FC2_normal);
    dfbal_->f_MakeFont("ジャンプ", FC2_normal);
    stShPad_.btntype_ = pbt_Jump;
    SetPos(-136, -40);
    dfbal_->z_SetPos(0, -32);
    ofbal_->z_SetPos(0, -32);
  }

  void TUGShPad::MakeGroup_JumpPrev(TUIObj* parent)
  {
    GenMakeGroup(parent);
    stShPad_.btntype_ = pbt_Jump;
    SetPos(-136, -40);
    stShPad_.prev_f_ = TRUE;
  }
  void TUGShPad::SetMsgType(enPadMsgType msgtype)
  {
    stShPad_.msgtype_ = msgtype;

  }

  void TUGShPad::Refresh()
  {

    BOOL ofside_f = FALSE;
    BOOL draw_f = TRUE;
    switch (stShPad_.msgtype_)
    {
    case pmt_Normal: ofside_f = (player_->pGame_->mgSG_.stTm_[mid::midGetMyIdx()].ofside_f); break;
    case pmt_TutoOf: ofside_f = TRUE; break;
    case pmt_TutoDf: ofside_f = FALSE; break;
    case pmt_Invisible: draw_f = FALSE; break;
    }

    ofbal_->g_SetDraw(ofside_f && draw_f && (stShPad_.prev_f_ == FALSE));
    dfbal_->g_SetDraw((!ofside_f) && draw_f && (stShPad_.prev_f_ == FALSE));

    TMgPad* pmgPad = player_->pGame_->mgGO_.pmgPad_[mid::midGetMyIdx()];
    s32 partno = 0;
    switch (stShPad_.btntype_)
    {
    case pbt_Shot: if (pmgPad->IsShot2()) partno = 1; break;
    case pbt_Pass: if (pmgPad->IsPass2()) partno = 1; break;
    case pbt_Jump: if (pmgPad->IsJump2()) partno = 1; break;
    }
    field_->g_SetPartNo(partno);
  }

  void TUGShPad::DoUpdate(BOOL draw_f)
  {
    Refresh();
    field_->Update(draw_f);
    ofbal_->Update(draw_f);
    dfbal_->Update(draw_f);
  }

}
