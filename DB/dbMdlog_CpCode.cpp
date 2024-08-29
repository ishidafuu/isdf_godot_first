#include "dbMdlog_CpCode.h"
namespace db
{
  TMdlog_CpCode::TMdlog_CpCode():TModelGen("log_CpCode")
  {
  }
  TMdlog_CpCode::~TMdlog_CpCode(void)
  {
  }
  s64 TMdlog_CpCode::GetLineDt(s64 line, enmdlog_CpCode dtNo)
  {
    return GenGetLineDt(line, LOG_CPCODECLM[dtNo].c_str());
  }
  BOOL TMdlog_CpCode::GetLineDtBOOL(s64 line, enmdlog_CpCode dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_CPCODECLM[dtNo].c_str());
  }
  s64 TMdlog_CpCode::GetPKDt(s64 pk, enmdlog_CpCode dtNo)
  {
    return GenGetPKDt(pk, LOG_CPCODECLM[dtNo].c_str());
  }
  BOOL TMdlog_CpCode::GetPKDtBOOL(s64 pk, enmdlog_CpCode dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_CPCODECLM[dtNo].c_str());
  }
}
