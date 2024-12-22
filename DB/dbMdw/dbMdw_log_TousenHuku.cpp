#include "dbMdw_log_TousenHuku.h"
namespace db
{
  TMdw_log_TousenHuku::TMdw_log_TousenHuku():TModelGen("w_log_TousenHuku")
  {
  }
  TMdw_log_TousenHuku::~TMdw_log_TousenHuku(void)
  {
  }
  s64 TMdw_log_TousenHuku::GetLineDt(s64 line, enmdw_log_TousenHuku dtNo)
  {
    return GenGetLineDt(line, W_LOG_TOUSENHUKUCLM[dtNo].c_str());
  }
  BOOL TMdw_log_TousenHuku::GetLineDtBOOL(s64 line, enmdw_log_TousenHuku dtNo)
  {
    return GenGetLineDtBOOL(line, W_LOG_TOUSENHUKUCLM[dtNo].c_str());
  }
  s64 TMdw_log_TousenHuku::GetPKDt(s64 pk, enmdw_log_TousenHuku dtNo)
  {
    return GenGetPKDt(pk, W_LOG_TOUSENHUKUCLM[dtNo].c_str());
  }
  BOOL TMdw_log_TousenHuku::GetPKDtBOOL(s64 pk, enmdw_log_TousenHuku dtNo)
  {
    return GenGetPKDtBOOL(pk, W_LOG_TOUSENHUKUCLM[dtNo].c_str());
  }
}
