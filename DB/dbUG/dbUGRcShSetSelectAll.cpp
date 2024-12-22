#include "dbUGRcShSetSelectAll.h"
namespace db
{
  const s32 RECORDH = 24;

  TUGRcShSetSelectAll::TUGRcShSetSelectAll(TUIGenLayer* player) : TUGRcNormal(player)
    //, ugbtn_detach_(player)
  {
    stShSetSelect_.Init();
  }
  TUGRcShSetSelectAll::~TUGRcShSetSelectAll(void)
  {
  
  }
  void TUGRcShSetSelectAll::DoMakeGroupSub2()
  {
    slot_ = MakeObj();
    slot_->o_SetParent(field_);
    slot_->f_FontPos(0, 4);

    chest_ = MakeObj();
    chest_->o_SetParent(field_);
    chest_->g_SetCenter(TRUE);
    chest_->f_SetCenter(TRUE);
    chest_->g_SetGrpPos(13, 8);

    step_ = MakeObj();
    step_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_soubi_step));
    step_->o_SetParent(field_);
    step_->g_SetOvOvj(chest_);
    step_->g_SetCenter(TRUE);
    step_->g_SetGrpPos(12, 20);

    field_->b_SetBtn(FALSE);
  }
  void TUGRcShSetSelectAll::DoSetLimY2(s32 limu, s32 hhh)
  {
    slot_->g_SetLimY(limu, hhh);
    step_->g_SetLimY(limu, hhh);
    chest_->g_SetLimY(limu, hhh);
  }
  void TUGRcShSetSelectAll::DoUpdateSub2(BOOL draw_f)
  {
    slot_->Update(draw_f);
    step_->Update(draw_f);
    chest_->Update(draw_f);
  }

  void TUGRcShSetSelectAll::SetOpenLvDt(s64 openlv)
  {
	const s32 maxlen = 14;
    std::string openstr = "チームＬｖ" + lib_str::IntToStr(openlv) + "で＿かいほう";
    ugname_.SetName(openstr.c_str(), maxlen);
    ugname_.SetDraw(TRUE);
    SetDisable(TRUE);


  }
  void TUGRcShSetSelectAll::SetBaseDt(s32 slot, s32 mid_team)
  {
    stShSetSelect_.slot_ = slot;//0~41
    stShSetSelect_.mid_team_ = mid_team;
    std::string strslot = lib_str::IntToStr(slot + 1);
    slot_->f_MakeFont_Mini(strslot.c_str(), FC2_btn_normal);

    s32 posno = (slot % DBMEMBER_ALL);
    s32 mid_char = pmgEO_->mgCoM_.GetCharID(mid_team, posno);
    chest_->o_SetObjGrPart(pmgEO_->mgGrp_.pgr_IconChest_[SIDE0]);
    chest_->g_SetPartNo(posno);

    s32 steptype = pmgEO_->mgDt_.dtChar_.GetLv(clv_Speed, mid_char);
    s32 step = SHOTSTEP[(steptype * SLOTNUM) + (slot / DBMEMBER_ALL)];
    step_->g_SetPartNo(step);
    step_->g_SetDraw(TRUE);

  }
  void TUGRcShSetSelectAll::SetShotDt(s64 id_shot)
  {
    stShSetSelect_.id_shot_ = id_shot;
    
    stShSetSelect_.ShSetSelect_f_ = ((id_shot != NGID) && pmgEO_->mgCoU_.IsShotEnable(id_shot));
    ugname_.SetNameShot(id_shot, FALSE);
    ugname_.SetDraw(stShSetSelect_.ShSetSelect_f_);
    ugname_.SetIcon(FALSE);
    TSozaiData sozaidt =  pmgEO_->mgCoU_.GetSozaiData(id_shot, 0);
  }

  //s32 TUGRcShSetSelectAll::GetSlot()
  //{
  //  return stShSetSelect_.slot_;
  //}
  //s32 TUGRcShSetSelectAll::GetShotID()
  //{
  //  return stShSetSelect_.id_shot_;
  //}

  void TUGRcShSetSelectAll::DoActionSub()
  {
  }
}
