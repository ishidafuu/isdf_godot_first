#include "dbMdlog_ShiaiMap.h"
namespace db
{
  TMdlog_ShiaiMap::TMdlog_ShiaiMap():TModelGen("log_ShiaiMap")
  {
  }
  TMdlog_ShiaiMap::~TMdlog_ShiaiMap(void)
  {
  }
  s64 TMdlog_ShiaiMap::GetLineDt(s64 line, enmdlog_ShiaiMap dtNo)
  {
    return GenGetLineDt(line, LOG_SHIAIMAPCLM[dtNo].c_str());
  }
  BOOL TMdlog_ShiaiMap::GetLineDtBOOL(s64 line, enmdlog_ShiaiMap dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_SHIAIMAPCLM[dtNo].c_str());
  }
  s64 TMdlog_ShiaiMap::GetPKDt(s64 pk, enmdlog_ShiaiMap dtNo)
  {
    return GenGetPKDt(pk, LOG_SHIAIMAPCLM[dtNo].c_str());
  }
  BOOL TMdlog_ShiaiMap::GetPKDtBOOL(s64 pk, enmdlog_ShiaiMap dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_SHIAIMAPCLM[dtNo].c_str());
  }
}
