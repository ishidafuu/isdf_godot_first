#include "dbMdlog_Maruka.h"
namespace db
{
  TMdlog_Maruka::TMdlog_Maruka():TModelGen("log_Maruka")
  {
  }
  TMdlog_Maruka::~TMdlog_Maruka(void)
  {
  }
  s64 TMdlog_Maruka::GetLineDt(s64 line, enmdlog_Maruka dtNo)
  {
    return GenGetLineDt(line, LOG_MARUKACLM[dtNo].c_str());
  }
  BOOL TMdlog_Maruka::GetLineDtBOOL(s64 line, enmdlog_Maruka dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_MARUKACLM[dtNo].c_str());
  }
  s64 TMdlog_Maruka::GetPKDt(s64 pk, enmdlog_Maruka dtNo)
  {
    return GenGetPKDt(pk, LOG_MARUKACLM[dtNo].c_str());
  }
  BOOL TMdlog_Maruka::GetPKDtBOOL(s64 pk, enmdlog_Maruka dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_MARUKACLM[dtNo].c_str());
  }
}
