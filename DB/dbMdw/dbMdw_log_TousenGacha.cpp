#include "dbMdw_log_TousenGacha.h"
namespace db
{
  TMdw_log_TousenGacha::TMdw_log_TousenGacha():TModelGen("w_log_TousenGacha")
  {
  }
  TMdw_log_TousenGacha::~TMdw_log_TousenGacha(void)
  {
  }
  s64 TMdw_log_TousenGacha::GetLineDt(s64 line, enmdw_log_TousenGacha dtNo)
  {
    return GenGetLineDt(line, W_LOG_TOUSENGACHACLM[dtNo].c_str());
  }
  BOOL TMdw_log_TousenGacha::GetLineDtBOOL(s64 line, enmdw_log_TousenGacha dtNo)
  {
    return GenGetLineDtBOOL(line, W_LOG_TOUSENGACHACLM[dtNo].c_str());
  }
  s64 TMdw_log_TousenGacha::GetPKDt(s64 pk, enmdw_log_TousenGacha dtNo)
  {
    return GenGetPKDt(pk, W_LOG_TOUSENGACHACLM[dtNo].c_str());
  }
  BOOL TMdw_log_TousenGacha::GetPKDtBOOL(s64 pk, enmdw_log_TousenGacha dtNo)
  {
    return GenGetPKDtBOOL(pk, W_LOG_TOUSENGACHACLM[dtNo].c_str());
  }
}
