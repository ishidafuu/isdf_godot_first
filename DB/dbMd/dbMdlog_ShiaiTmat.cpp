#include "dbMdlog_ShiaiTmat.h"
namespace db
{
  TMdlog_ShiaiTmat::TMdlog_ShiaiTmat():TModelGen("log_ShiaiTmat")
  {
  }
  TMdlog_ShiaiTmat::~TMdlog_ShiaiTmat(void)
  {
  }
  s64 TMdlog_ShiaiTmat::GetLineDt(s64 line, enmdlog_ShiaiTmat dtNo)
  {
    return GenGetLineDt(line, LOG_SHIAITMATCLM[dtNo].c_str());
  }
  BOOL TMdlog_ShiaiTmat::GetLineDtBOOL(s64 line, enmdlog_ShiaiTmat dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_SHIAITMATCLM[dtNo].c_str());
  }
  s64 TMdlog_ShiaiTmat::GetPKDt(s64 pk, enmdlog_ShiaiTmat dtNo)
  {
    return GenGetPKDt(pk, LOG_SHIAITMATCLM[dtNo].c_str());
  }
  BOOL TMdlog_ShiaiTmat::GetPKDtBOOL(s64 pk, enmdlog_ShiaiTmat dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_SHIAITMATCLM[dtNo].c_str());
  }
}
