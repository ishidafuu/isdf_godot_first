#include "dbMdlog_ShiaiTour.h"
namespace db
{
  TMdlog_ShiaiTour::TMdlog_ShiaiTour():TModelGen("log_ShiaiTour")
  {
  }
  TMdlog_ShiaiTour::~TMdlog_ShiaiTour(void)
  {
  }
  s64 TMdlog_ShiaiTour::GetLineDt(s64 line, enmdlog_ShiaiTour dtNo)
  {
    return GenGetLineDt(line, LOG_SHIAITOURCLM[dtNo].c_str());
  }
  BOOL TMdlog_ShiaiTour::GetLineDtBOOL(s64 line, enmdlog_ShiaiTour dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_SHIAITOURCLM[dtNo].c_str());
  }
  s64 TMdlog_ShiaiTour::GetPKDt(s64 pk, enmdlog_ShiaiTour dtNo)
  {
    return GenGetPKDt(pk, LOG_SHIAITOURCLM[dtNo].c_str());
  }
  BOOL TMdlog_ShiaiTour::GetPKDtBOOL(s64 pk, enmdlog_ShiaiTour dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_SHIAITOURCLM[dtNo].c_str());
  }
}
