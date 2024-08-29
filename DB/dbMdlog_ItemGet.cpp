#include "dbMdlog_ItemGet.h"
namespace db
{
  TMdlog_ItemGet::TMdlog_ItemGet():TModelGen("log_ItemGet")
  {
  }
  TMdlog_ItemGet::~TMdlog_ItemGet(void)
  {
  }
  s64 TMdlog_ItemGet::GetLineDt(s64 line, enmdlog_ItemGet dtNo)
  {
    return GenGetLineDt(line, LOG_ITEMGETCLM[dtNo].c_str());
  }
  BOOL TMdlog_ItemGet::GetLineDtBOOL(s64 line, enmdlog_ItemGet dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_ITEMGETCLM[dtNo].c_str());
  }
  s64 TMdlog_ItemGet::GetPKDt(s64 pk, enmdlog_ItemGet dtNo)
  {
    return GenGetPKDt(pk, LOG_ITEMGETCLM[dtNo].c_str());
  }
  BOOL TMdlog_ItemGet::GetPKDtBOOL(s64 pk, enmdlog_ItemGet dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_ITEMGETCLM[dtNo].c_str());
  }
}
