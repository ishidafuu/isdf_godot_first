#include "dbUGBtnSoubi.h"
#include "dbMgUI.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGBtnSoubi::TUGBtnSoubi(TUIGenLayer* player) :TUGButton(player)
  {
  }
  TUGBtnSoubi::~TUGBtnSoubi(void)
  {
  }
  void TUGBtnSoubi::MakeGroupSoubi(TUIObj* parent, BOOL shset_f)
  {
    SetParent(parent);
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_btn_icon));//画像セット
    field_->g_SetDepth(UIDP_05BUTTON);//手前
    field_->b_SetBtn(TRUE);//ボタン化
    field_->g_SetCenter(TRUE);//中心
    field_->f_SetCenter(TRUE);//文字中心



    step_ = MakeObj();
    ball_ = MakeObj();

    step_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_soubi_step));

    step_->z_SetPos(0, 7);
    ball_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_rarity));
    step_->o_SetParent(field_);
    ball_->o_SetParent(field_);
    step_->g_SetOvOvj(ball_);
    step_->g_SetCenter(TRUE);
    ball_->g_SetCenter(TRUE);
    ball_->f_SetCenter(TRUE);

    step_->g_SetDraw(!shset_f);
    if (shset_f)
    {
      ball_->z_SetPos(0, 0);
    }
    else
    {
      ball_->z_SetPos(0, -3);
    }
  }
  void TUGBtnSoubi::SetLimY(s32 limu, s32 hhh)
  {
    field_->g_SetLimY(limu, hhh);
    step_->g_SetLimY(limu, hhh);
    ball_->g_SetLimY(limu, hhh);
  }
  //文字列セット
  void TUGBtnSoubi::SetFont(const char* msg, s32 fontcol)
  {
    ball_->f_MakeFont(msg, fontcol);
    ball_->f_SetCenter(TRUE);
  }
  void TUGBtnSoubi::SetFont(const char* msg, s32 fontcol, s32 fx, s32 fy)
  {
    ball_->f_MakeFont(msg, fontcol);
    ball_->f_FontPos(fx, fy);
    ball_->f_SetCenter(TRUE);
  }
  void TUGBtnSoubi::SetFont_Mini(const char* msg, s32 fontcol, s32 fx, s32 fy)
  {
    ball_->f_MakeFont_Mini(msg, fontcol);
    ball_->f_FontPos(fx, fy);
    ball_->f_SetCenter(FALSE);
  }
  void TUGBtnSoubi::SetBallNo(s32 ballno)
  {
    ball_->f_MakeFont(lib_str::IntToStr(ballno).c_str(), FC2_normal);
    ball_->f_FontPos(0, -1);
    ball_->f_SetCenter(TRUE);
  }
  void TUGBtnSoubi::SetCharNo(s32 mid_team, s32 slotno)
  {
    s32 posno = (slotno % DBMEMBER_ALL);
    s32 mid_char = pmgEO_->mgCoM_.GetCharID(mid_team, posno);
    ball_->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconChest_[SIDE0]);
    ball_->g_SetPartNo(posno);
    ball_->g_SetGrpPos(1, -5);
    s32 steptype = pmgEO_->mgDt_.dtChar_.GetLv(clv_Speed, mid_char);
    s32 step = SHOTSTEP[(steptype * SLOTNUM) + (slotno / DBMEMBER_ALL)];
    SetStepIcon(step);

    std::string strslot = lib_str::IntToStr(slotno + 1);
    SetFont_Mini(strslot.c_str(), FC2_normal, -12, -9);
  }

  void TUGBtnSoubi::SetStepIcon(s32 step)
  {
    if (step != NGNUM)
    {
      step_->g_SetPartNo(step);
      step_->g_SetDraw(TRUE);
    }
    else
    {
      //step_->g_SetPartNo(7);
      step_->g_SetDraw(FALSE);
    }
  }
  void TUGBtnSoubi::SetBallIcon(s32 rarity)
  {
    ball_->g_SetPartNo(ugr_Ball + rarity);
  }

  //更新
  void TUGBtnSoubi::DoUpdateSub(BOOL draw_f)
  {
    step_->Update(draw_f);
    ball_->Update(draw_f);
  }

}