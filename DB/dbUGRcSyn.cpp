#include "dbUGRcSyn.h"
namespace db
{
  const s32 RECORDH = 24;
  const s32 VJUMP = 7;

  TUGRcSyn::TUGRcSyn(TUIGenLayer* player) : TUGRcNormal(player)
  {
    stSyn_.Init();
  }
  TUGRcSyn::~TUGRcSyn(void)
  {

  }
  void TUGRcSyn::DoMakeGroupSub2()
  {

    //step_ = MakeObj();
    //step_->o_SetParent(field_);
    //step_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_soubi_step));
    //step_->g_SetCenter(TRUE);
    //step_->z_SetPos(12, 19);
    //step_->f_SetCenter(TRUE);
    //step_->f_FontPos(0, -11);


    chest_ = MakeObj();
    //chest_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_rarity));
    chest_->o_SetParent(field_);
    chest_->g_SetCenter(TRUE);
    chest_->f_SetCenter(TRUE);
    chest_->g_SetGrpPos(13, 10);

  }
  void TUGRcSyn::DoSetLimY2(s32 limu, s32 hhh)
  {
    //step_->g_SetLimY(limu, hhh);
    chest_->g_SetLimY(limu, hhh);
  }
  void TUGRcSyn::DoUpdateSub2(BOOL draw_f)
  {
    //step_->Update(draw_f);
    chest_->Update(draw_f);
  }

  void TUGRcSyn::SetShotDt(s32 posno, s32 charid)
  {
    stSyn_.charid_ = charid;
    //デフォシュートとインベントリシュートでは扱いが違う
    s32 defshot = pmgEO_->mgDt_.dtChar_.GetLv(clv_DefShotID, charid);
    ugname_.SetNameDefShot(defshot);
    ugname_.SetDraw(TRUE);
    //step_->g_SetDraw(TRUE);
    //field_->b_SetBtn(FALSE);
    //SetDisable(TRUE);
    ugname_.SetIcon(FALSE);

    //s32 posno = (stShSetSelect_.slot_ % DBMEMBER_ALL);
    //s32 mid_char = pmgEO_->mgCoM_.GetCharID(mid_team, posno);
    chest_->o_SetObjGrPart(pmgEO_->mgGrp_.pgr_IconChest_[SIDE0]);
    chest_->g_SetPartNo(posno);

    SetCommonID(defshot);
  }


  s32 TUGRcSyn::GetCharID()
  {
    return stSyn_.charid_;
  }
  s32 TUGRcSyn::GetDefShotID()
  {
    return stSyn_.defshotid_;
  }

  void TUGRcSyn::DoActionSub()
  {

  }
}
