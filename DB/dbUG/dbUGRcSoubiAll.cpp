#include "dbUGRcSoubiAll.h"
namespace db
{
  const s32 RECORDH = 24;

  TUGRcSoubiAll::TUGRcSoubiAll(TUIGenLayer* player) : TUGRcNormal(player)
    , ugbtn_detach_(player)
  {
    stSoubiAll_.Init();
  }
  TUGRcSoubiAll::~TUGRcSoubiAll(void)
  {
  
  }
  void TUGRcSoubiAll::DoMakeGroupSub2()
  {
    ugbtn_detach_.MakeGroupSoubi(field_, TRUE);
    ugbtn_detach_.SetPos(12, 12);

    slot_ = MakeObj();
    slot_->o_SetParent(field_);
    //slot_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_soubi_step));
    //slot_->g_SetCenter(TRUE);
    //slot_->z_SetPos(12, 19);
    //slot_->f_SetCenter(TRUE);
    //slot_->f_FontPos(0,-11);

    slot_->f_FontPos(0, 4);


    chest_ = MakeObj();
    //chest_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_rarity));
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
  
  }
  void TUGRcSoubiAll::DoSetLimY2(s32 limu, s32 hhh)
  {
    ugbtn_detach_.SetLimY(limu, hhh);
    slot_->g_SetLimY(limu, hhh);
    step_->g_SetLimY(limu, hhh);
    chest_->g_SetLimY(limu, hhh);
  }
  void TUGRcSoubiAll::DoUpdateSub2(BOOL draw_f)
  {
    ugbtn_detach_.Update(draw_f);
    slot_->Update(draw_f);
    chest_->Update(draw_f);
    step_->Update(draw_f);
  }

  void TUGRcSoubiAll::SetOpenLvDt(s64 openlv)
  {
	const s32 maxlen = 14;
    std::string openstr = "チームＬｖ" + lib_str::IntToStr(openlv) + "で＿かいほう";
    ugname_.SetName(openstr.c_str(), maxlen);
    ugname_.SetDraw(TRUE);
    SetDisable(TRUE);
  }
  void TUGRcSoubiAll::SetBaseDt(s32 slot, s32 mid_team)
  {
    //stSoubiAll_.slot_ = slot;//0~41
    //std::string strslot = lib_str::IntToStr(slot + 1);
    //slot_->f_MakeFont(strslot.c_str(), FC2_btn_normal);
    //ugbtn_detach_.SetFont_Mini(strslot.c_str(), FC2_btn_normal, -12, -9);
    //ClearShotDt();
    stSoubiAll_.slot_ = slot;//0~41
    stSoubiAll_.mid_team_ = mid_team;
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
  void TUGRcSoubiAll::SetShotDt(s64 id_shot, s32 mid_team)
  {
    stSoubiAll_.id_shot_ = id_shot;
    stSoubiAll_.mid_team_ = mid_team;
    stSoubiAll_.SoubiAll_f_ = ((id_shot != NGID) && pmgEO_->mgCoU_.IsShotEnable(id_shot));
    ugname_.SetNameShot(id_shot, FALSE);
    ugname_.SetDraw(stSoubiAll_.SoubiAll_f_);
    ugname_.SetIcon(FALSE);
    TSozaiData sozaidt =  pmgEO_->mgCoU_.GetSozaiData(id_shot, 0);
    ugbtn_detach_.SetCharNo(mid_team, stSoubiAll_.slot_);//キャラアイコン
    ugbtn_detach_.SetDraw(stSoubiAll_.SoubiAll_f_);
    slot_->g_SetDraw(!stSoubiAll_.SoubiAll_f_);
    chest_->g_SetDraw(!stSoubiAll_.SoubiAll_f_);
    step_->g_SetDraw(!stSoubiAll_.SoubiAll_f_);
  }
  void TUGRcSoubiAll::ClearShotDt()
  {
    stSoubiAll_.SoubiAll_f_ = FALSE;
    stSoubiAll_.id_shot_ = NGNUM;
    slot_->g_SetDraw(TRUE);
    chest_->g_SetDraw(TRUE);
    step_->g_SetDraw(TRUE);
    ugname_.SetDraw(FALSE);
    ugbtn_detach_.SetDraw(FALSE);
  }
  s32 TUGRcSoubiAll::GetSlot()
  {
    return stSoubiAll_.slot_;
  }
  s32 TUGRcSoubiAll::GetShotID()
  {
    return stSoubiAll_.id_shot_;
  }

  void TUGRcSoubiAll::DoActionSub()
  {
    if (ugbtn_detach_.IsAct())
    {
      SetAct(enUGRcSoubiAll_Detach);//はずす
    }
    else if ((ugbtn_detach_.GetField()->b_IsEnter(TRUE) == FALSE))//外すボタンが押されてなければ
    {
      if (field_->b_IsDecide())//レコード
      {
        SetAct(enUGRcSoubiAll_Field);
        field_->g_Flash();//とりあえずここで明るく
      }
      else if (field_->b_IsEnter(TRUE))
      {
        field_->g_Bright();//とりあえずここで明るく
      }
      else
      {
        field_->g_EfReset();//とりあえずここで戻す
      }
    }
  }

  BOOL TUGRcSoubiAll::IsActRes(enUGRcSoubiAllAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }
}
