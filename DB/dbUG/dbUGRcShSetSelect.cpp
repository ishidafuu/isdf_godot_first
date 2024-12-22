#include "dbUGRcShSetSelect.h"
namespace db
{
  const s32 RECORDH = 24;
  const s32 VJUMP = 7;

  TUGRcShSetSelect::TUGRcShSetSelect(TUIGenLayer* player) : TUGRcNormal(player)
    , ugball_(player)
  {
    stShSetSelect_.Init();
  }
  TUGRcShSetSelect::~TUGRcShSetSelect(void)
  {

  }
  void TUGRcShSetSelect::DoMakeGroupSub2()
  {
    ugball_.MakeGroup(field_);
    ugball_.SetPos(13, 9);

    step_ = MakeObj();
    step_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_soubi_step));
    step_->o_SetParent(field_);
    step_->g_SetOvOvj(ugball_.GetField());
    step_->g_SetCenter(TRUE);
    step_->g_SetGrpPos(12, 20);
    step_->f_FontPos(0, 3);

    field_->b_SetBtn(FALSE);
  }
  void TUGRcShSetSelect::DoSetLimY2(s32 limu, s32 hhh)
  {
    step_->g_SetLimY(limu, hhh);
    ugball_.SetLimY(limu, hhh);
  }
  void TUGRcShSetSelect::DoUpdateSub2(BOOL draw_f)
  {
    step_->Update(draw_f);
    ugball_.Update(draw_f);
  }


  void TUGRcShSetSelect::SetBaseDt_ShSet(s32 step, s32 slot)
  {
    stShSetSelect_.slot_ = slot;//0~41
    stShSetSelect_.step_ = step;
    step_->g_SetPartNo(step);
    
    if (slot != NGNUM)
    {
      std::string strslot = lib_str::IntToStr(slot + 1);
      step_->f_MakeFont_Mini(strslot.c_str(), FC2_btn_mini);
    }
    else
    {
      step_->f_MakeFont_Mini("", FC2_btn_mini);
    }
    ClearShotDt();
  }
  void TUGRcShSetSelect::SetOpenLvDt(s64 openlv)
  {
	const s32 maxlen = 14;
    std::string openstr = "チームＬｖ" + lib_str::IntToStr(openlv) + "で＿かいほう";
    ugname_.SetName(openstr.c_str(), maxlen);
    ugname_.SetDraw(TRUE);
    SetDisable(TRUE);
  }

  void TUGRcShSetSelect::SetShotDt_ShSet(s64 id_shot, BOOL defshot_f)
  {
    stShSetSelect_.id_shot_ = id_shot;
    stShSetSelect_.def_f_ = defshot_f;
    //デフォシュートとインベントリシュートでは扱いが違う
    if (defshot_f)
    {
      stShSetSelect_.ShSetSelect_f_ = TRUE;
      ugball_.SetDraw(TRUE);
      ugball_.SetIconSozai(1, FALSE);
      ugname_.SetNameDefShot(id_shot);
      ugname_.SetDraw(TRUE);
      ugname_.SetIcon(FALSE);
    }
    else if (id_shot != 0)
    {
      stShSetSelect_.ShSetSelect_f_ = ((id_shot != NGID) && pmgEO_->mgCoU_.IsShotEnable(id_shot));
      TSozaiData sozaidt = pmgEO_->mgCoU_.GetSozaiData(id_shot, 0);
      ugball_.SetIconShot(id_shot);
      ugball_.SetDraw(TRUE);
      ugname_.SetNameShot(id_shot, FALSE);
      ugname_.SetDraw(TRUE);
      ugname_.SetIcon(FALSE);
    }
    else
    {
      ugname_.SetDraw(FALSE);
      ugball_.SetDraw(FALSE);
    }
    step_->g_SetDraw(TRUE);

  }


  void TUGRcShSetSelect::ClearShotDt()
  {
    stShSetSelect_.ShSetSelect_f_ = FALSE;
    stShSetSelect_.id_shot_ = NGNUM;
    step_->g_SetDraw(TRUE);
    ugname_.SetDraw(FALSE);
  }

  void TUGRcShSetSelect::DoActionSub()
  {

  }
}
