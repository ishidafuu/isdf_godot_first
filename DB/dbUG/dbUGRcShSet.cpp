#include "dbUGRcShSet.h"
namespace db
{
  const s32 RECORDH = 24;

  TUGRcShSet::TUGRcShSet(TUIGenLayer* player) : TUGRcNormal(player)
    , ugbtn_detach_(player)
  {
    stShSet_.Init();
  }
  TUGRcShSet::~TUGRcShSet(void)
  {
  
  }
  void TUGRcShSet::DoMakeGroupSub2()
  {
    ugbtn_detach_.MakeGroupSoubi(field_, TRUE);
    ugbtn_detach_.SetPos(12, 12);

    slot_ = MakeObj();
    slot_->o_SetParent(field_);
    //slot_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_soubi_step));
    slot_->g_SetCenter(TRUE);
    slot_->z_SetPos(12, 19);
    slot_->f_SetCenter(TRUE);
    slot_->f_FontPos(0,-11);

  
  }
  void TUGRcShSet::DoSetLimY2(s32 limu, s32 hhh)
  {
    ugbtn_detach_.SetLimY(limu, hhh);
    slot_->g_SetLimY(limu, hhh);
  }
  void TUGRcShSet::DoUpdateSub2(BOOL draw_f)
  {
    ugbtn_detach_.Update(draw_f);
    slot_->Update(draw_f);
  }

  void TUGRcShSet::SetBaseDt(s32 slot)
  {
    stShSet_.slot_ = slot;//0~41
    std::string strslot = lib_str::IntToStr(slot + 1);
    slot_->f_MakeFont(strslot.c_str(), FC2_btn_normal);
    ugbtn_detach_.SetFont(strslot.c_str(), FC2_btn_normal, 0, -1);
    ClearShotDt();
  }
  void TUGRcShSet::SetShotDt(s64 id_shot)
  {
    stShSet_.id_shot_ = id_shot;

    stShSet_.ShSet_f_ = ((id_shot != NGID) && pmgEO_->mgCoU_.IsShotEnable(id_shot));
    ugname_.SetNameShot(id_shot, FALSE);
    ugname_.SetDraw(stShSet_.ShSet_f_);
    ugname_.SetIcon(FALSE);
    TSozaiData sozaidt =  pmgEO_->mgCoU_.GetSozaiData(id_shot, 0);
    ugbtn_detach_.SetBallIcon(sozaidt.rarity_);
    slot_->g_SetDraw(!stShSet_.ShSet_f_);
    ugbtn_detach_.SetDraw(stShSet_.ShSet_f_);
    
  }
  void TUGRcShSet::ClearShotDt()
  {
    stShSet_.ShSet_f_ = FALSE;
    stShSet_.id_shot_ = NGNUM;
    slot_->g_SetDraw(TRUE);
    ugname_.SetDraw(FALSE);
    ugbtn_detach_.SetDraw(FALSE);
  }
  s32 TUGRcShSet::GetSlot()
  {
    return stShSet_.slot_;
  }
  s32 TUGRcShSet::GetShotID()
  {
    return stShSet_.id_shot_;
  }

  void TUGRcShSet::DoActionSub()
  {
    if (ugbtn_detach_.IsAct())
    {
      SetAct(enUGRcShSet_Detach);//はずす
    }
    else if ((ugbtn_detach_.GetField()->b_IsEnter(TRUE) == FALSE))//外すボタンが押されてなければ
    {
      if (field_->b_IsDecide())//レコード
      {
        SetAct(enUGRcShSet_Field);
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

  BOOL TUGRcShSet::IsActRes(enUGRcShSetAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }
}
