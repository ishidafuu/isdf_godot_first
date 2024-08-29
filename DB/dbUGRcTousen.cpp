#include "dbUGRcTousen.h"
namespace db
{
  TUGRcTousen::TUGRcTousen(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcTousen::~TUGRcTousen(void)
  {
  
  }
  void TUGRcTousen::DoMakeGroupSub2()
  {
    SetShopRecord();
    //subget_->o_SetParent(field_);
  }
  void TUGRcTousen::DoSetLimY2(s32 limu, s32 hhh)
  {
    //subget_->g_SetLimY(limu, hhh);
  }
  void TUGRcTousen::DoUpdateSub2(BOOL draw_f)
  {
    //subget_->Update(draw_f);
  }

  void TUGRcTousen::SetDt_Gacha(s64 log_tousen_no)
  {
    //とりあえず監督ＩＤ保存
    s64 id_kantoku = pmgEO_->mgCoU_.mdw_log_TousenGacha_.GetLineDt(log_tousen_no, mdw_log_tousengacha_id_kantoku);
    SetCommonID(id_kantoku);
    ugname_.SetNameTousen(log_tousen_no);
    s64 grade = pmgEO_->mgCoU_.mdw_log_TousenGacha_.GetLineDt(log_tousen_no, mdw_log_tousengacha_grade);
    s64 createdate = pmgEO_->mgCoU_.mdw_log_TousenGacha_.GetLineDt(log_tousen_no, mdw_log_tousengacha_create_date);
    std::string datestr = base::UnixtimeToDateString(createdate, TRUE);
    SetSubValue(datestr.c_str());
  }

  void TUGRcTousen::SetDt_Huku(s64 log_tousen_no)
  {
    //とりあえず監督ＩＤ保存
    s64 id_kantoku = pmgEO_->mgCoU_.mdw_log_TousenHuku_.GetLineDt(log_tousen_no, mdw_log_tousenhuku_id_kantoku);
    SetCommonID(id_kantoku);
    ugname_.SetNameTousen(log_tousen_no);
    s64 createdate = pmgEO_->mgCoU_.mdw_log_TousenHuku_.GetLineDt(log_tousen_no, mdw_log_tousenhuku_create_date);
    std::string datestr = base::UnixtimeToDateString(createdate, TRUE);
    SetSubValue(datestr.c_str());
  }
}
