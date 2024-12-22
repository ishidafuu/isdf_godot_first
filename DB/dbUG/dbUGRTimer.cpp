#include "dbUGRTimer.h"
#include "dbUILayer.h"

namespace db
{
  const s32 ICONX = -12;
  const s32 ICONY = 4;
  const s32 ICONXVAL = -8;
  const s32 ICONYVAL = 7;
  const s32 ICONXFACE = -12;
  const s32 ICONYFACE = 3;
  //コンストラクタ
  TUGRTimer::TUGRTimer(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stRTimer_.Init();

  }

  TUGRTimer::~TUGRTimer()
  {

  }
  void TUGRTimer::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    time_ = MakeObj();
    time_->o_SetParent(field_);
    time_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_time));
    time_->g_SetDraw(TRUE);
    time_->a_AddAnivec(0, 7, 60, at_Loop);//砂嵐
    time_->a_SetAniNo(0, TRUE);//砂嵐
    time_->g_SetGrpPos(-8,-2);
  }

  void TUGRTimer::SetTimer(std::string numstr)
  {
    const s32 MAXLEN = 13;
    field_->f_MakeFont_Mini(numstr.c_str(), FC2_hd_normal);
    //time_->g_SetDraw(FALSE);
  }
  void TUGRTimer::SetTimer(s64 num)
  {
    SetTimer(lib_str::IntToStr(num));
  }
  void TUGRTimer::SetRightShift()
  {
    field_->f_SetRightShift();
  }

  void TUGRTimer::SetLimY(s32 limu, s32 hhh)
  {
    field_->g_SetLimY(limu, hhh);
    time_->g_SetLimY(limu, hhh);
  }

  void TUGRTimer::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    time_->Update(draw_f);
  }



}
