#include "dbMdlog_ShiaiHum.h"
namespace db
{
  TMdlog_ShiaiHum::TMdlog_ShiaiHum():TModelGen("log_ShiaiHum")
  {
  }
  TMdlog_ShiaiHum::~TMdlog_ShiaiHum(void)
  {
  }
  s64 TMdlog_ShiaiHum::GetLineDt(s64 line, enmdlog_ShiaiHum dtNo)
  {
    return GenGetLineDt(line, LOG_SHIAIHUMCLM[dtNo].c_str());
  }
  BOOL TMdlog_ShiaiHum::GetLineDtBOOL(s64 line, enmdlog_ShiaiHum dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_SHIAIHUMCLM[dtNo].c_str());
  }
  s64 TMdlog_ShiaiHum::GetPKDt(s64 pk, enmdlog_ShiaiHum dtNo)
  {
    return GenGetPKDt(pk, LOG_SHIAIHUMCLM[dtNo].c_str());
  }
  BOOL TMdlog_ShiaiHum::GetPKDtBOOL(s64 pk, enmdlog_ShiaiHum dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_SHIAIHUMCLM[dtNo].c_str());
  }
}
