#include "dbMdlog_BStar.h"
namespace db
{
  TMdlog_BStar::TMdlog_BStar():TModelGen("log_BStar")
  {
  }
  TMdlog_BStar::~TMdlog_BStar(void)
  {
  }
  s64 TMdlog_BStar::GetLineDt(s64 line, enmdlog_BStar dtNo)
  {
    return GenGetLineDt(line, LOG_BSTARCLM[dtNo].c_str());
  }
  BOOL TMdlog_BStar::GetLineDtBOOL(s64 line, enmdlog_BStar dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_BSTARCLM[dtNo].c_str());
  }
  s64 TMdlog_BStar::GetPKDt(s64 pk, enmdlog_BStar dtNo)
  {
    return GenGetPKDt(pk, LOG_BSTARCLM[dtNo].c_str());
  }
  BOOL TMdlog_BStar::GetPKDtBOOL(s64 pk, enmdlog_BStar dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_BSTARCLM[dtNo].c_str());
  }
}
