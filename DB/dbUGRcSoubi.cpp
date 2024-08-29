#include "dbUGRcSoubi.h"
namespace db
{
  const s32 RECORDH = 24;
  const s32 VJUMP = 7;

  TUGRcSoubi::TUGRcSoubi(TUIGenLayer* player) : TUGRcNormal(player)
    , ugbtn_detach_(player)
    //, ugbtn_eye_(player)
  {
    stSoubi_.Init();
  }
  TUGRcSoubi::~TUGRcSoubi(void)
  {

  }
  void TUGRcSoubi::DoMakeGroupSub2()
  {
    ugbtn_detach_.MakeGroupSoubi(field_, FALSE);
    ugbtn_detach_.SetPos(12, 12);

    step_ = MakeObj();
    step_->o_SetParent(field_);
    step_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_soubi_step));
    step_->g_SetCenter(TRUE);
    step_->z_SetPos(12, 19);
    step_->f_SetCenter(TRUE);
    step_->f_FontPos(0, -11);


  }
  void TUGRcSoubi::DoSetLimY2(s32 limu, s32 hhh)
  {
    ugbtn_detach_.SetLimY(limu, hhh);
    step_->g_SetLimY(limu, hhh);
  }
  void TUGRcSoubi::DoUpdateSub2(BOOL draw_f)
  {
    ugbtn_detach_.Update(draw_f);
    step_->Update(draw_f);
  }

  void TUGRcSoubi::SetBaseDt(s32 step, s32 slot)
  {

    stSoubi_.slot_ = slot;//0~41
    stSoubi_.step_ = step;
    
    if (step != VJUMP)
    {
      step_->g_SetPartNo(step);
      std::string strstep = lib_str::IntToStr(step + 1);
      step_->f_MakeFont(strstep.c_str(), FC2_btn_normal);
      ugbtn_detach_.SetBallNo(step + 1);
    }
    else
    {
      step_->g_SetPartNo(VJUMP);
      step_->f_MakeFont("０", FC2_btn_normal);
      ugbtn_detach_.SetFont("Ｊ", FC2_normal, 0, -1);
    }

    ClearShotDt();
  }
  void TUGRcSoubi::SetBaseDt_ShSet(s32 step, s32 slot)
  {
    //stSoubi_.step_ = step;//0~6
    stSoubi_.slot_ = slot;//0~41
    stSoubi_.step_ = step;
    step_->g_SetPartNo(step);
    
    if (slot != NGNUM)
    {
      std::string strslot = lib_str::IntToStr(slot + 1);
      step_->f_MakeFont(strslot.c_str(), FC2_btn_normal);
    }
    else
    {
      step_->f_MakeFont("Ｓ", FC2_btn_normal);
    }
    ClearShotDt();
  }
  void TUGRcSoubi::SetOpenLvDt(s64 openlv)
  {
	const s32 maxlen = 14;
    std::string openstr = "チームＬｖ" + lib_str::IntToStr(openlv) + "で＿かいほう";
    ugname_.SetName(openstr.c_str(), maxlen);
    ugname_.SetDraw(TRUE);
    SetDisable(TRUE);
  }
  void TUGRcSoubi::SetShotDt(s64 id_shot, BOOL defshot_f)
  {
    stSoubi_.id_shot_ = id_shot;
    stSoubi_.def_f_ = defshot_f;
    //デフォシュートとインベントリシュートでは扱いが違う
    if (defshot_f)
    {
      stSoubi_.soubi_f_ = TRUE;
      ugname_.SetNameDefShot(id_shot);
      ugname_.SetDraw(TRUE);
      s32 mid_sozai = 0;
      step_->g_SetDraw(TRUE);
      ugbtn_detach_.SetDraw(FALSE);
      field_->b_SetBtn(FALSE);
      SetDisable(TRUE);
    }
    else if (id_shot != 0)
    {
      stSoubi_.soubi_f_ = ((id_shot != NGID) && pmgEO_->mgCoU_.IsShotEnable(id_shot));
      ugname_.SetNameShot(id_shot, FALSE);
      ugname_.SetDraw(stSoubi_.soubi_f_);
      TSozaiData sozaidt = pmgEO_->mgCoU_.GetSozaiData(id_shot, 0);
      ugbtn_detach_.SetBallIcon(sozaidt.rarity_);//外すボタン
      ugbtn_detach_.SetStepIcon(stSoubi_.step_);
      ugbtn_detach_.SetDraw(stSoubi_.soubi_f_);
      step_->g_SetDraw(!stSoubi_.soubi_f_);
      field_->b_SetBtn(TRUE);
    }
    ugname_.SetIcon(FALSE);
  }
  void TUGRcSoubi::SwitchEdit(BOOL edit_f)
  {
    if (edit_f)
    {
      if (stSoubi_.def_f_)
      {
        step_->g_SetDraw(TRUE);
        ugbtn_detach_.SetDraw(FALSE);
        SetDisable(TRUE);
      }
      else
      {
        ugbtn_detach_.SetDraw(stSoubi_.soubi_f_);
        step_->g_SetDraw(!stSoubi_.soubi_f_);
      }
    }
    else
    {
      step_->g_SetDraw(TRUE);
      ugbtn_detach_.SetDraw(FALSE);
    }
  }

  void TUGRcSoubi::SetShotDt_ShSet(s64 id_shot, BOOL defshot_f)
  {
    stSoubi_.id_shot_ = id_shot;
    stSoubi_.def_f_ = defshot_f;
    //デフォシュートとインベントリシュートでは扱いが違う
    if (defshot_f)
    {
      stSoubi_.soubi_f_ = TRUE;
      ugname_.SetNameDefShot(id_shot);
    }
    else if (id_shot != 0)
    {
      stSoubi_.soubi_f_ = ((id_shot != NGID) && pmgEO_->mgCoU_.IsShotEnable(id_shot));
      ugname_.SetNameShot(id_shot, FALSE);
    }
    field_->b_SetBtn(FALSE);
    ugname_.SetDraw(TRUE);
    ugname_.SetIcon(FALSE);
    step_->g_SetDraw(TRUE);
    ugbtn_detach_.SetDraw(FALSE);
  }


  void TUGRcSoubi::ClearShotDt()
  {
    stSoubi_.soubi_f_ = FALSE;
    stSoubi_.id_shot_ = NGNUM;
    step_->g_SetDraw(TRUE);
    ugname_.SetDraw(FALSE);
    ugbtn_detach_.SetDraw(FALSE);
    //ugbtn_eye_.SetDraw(FALSE);
  }
  s32 TUGRcSoubi::GetSlot()
  {
    return stSoubi_.slot_;
  }
  s32 TUGRcSoubi::GetShotID()
  {
    return stSoubi_.id_shot_;
  }

  void TUGRcSoubi::DoActionSub()
  {
    if (ugbtn_detach_.IsAct())
    {
      SetAct(enUGRcSoubi_Detach);//はずす
    }
    else if ((ugbtn_detach_.GetField()->b_IsEnter(TRUE) == FALSE))//外すボタンが押されてなければ
    {
      if (field_->b_IsDecide())//レコード
      {
        SetAct(enUGRcSoubi_Field);
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

  BOOL TUGRcSoubi::IsActRes(enUGRcSoubiAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }
}
