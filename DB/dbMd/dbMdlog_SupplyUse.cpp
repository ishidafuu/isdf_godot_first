#include "dbMdlog_SupplyUse.h"
namespace db
{
  TMdlog_SupplyUse::TMdlog_SupplyUse():TModelGen("log_SupplyUse")
  {
  }
  TMdlog_SupplyUse::~TMdlog_SupplyUse(void)
  {
  }
  s64 TMdlog_SupplyUse::GetLineDt(s64 line, enmdlog_SupplyUse dtNo)
  {
    return GenGetLineDt(line, LOG_SUPPLYUSECLM[dtNo].c_str());
  }
  BOOL TMdlog_SupplyUse::GetLineDtBOOL(s64 line, enmdlog_SupplyUse dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_SUPPLYUSECLM[dtNo].c_str());
  }
  s64 TMdlog_SupplyUse::GetPKDt(s64 pk, enmdlog_SupplyUse dtNo)
  {
    return GenGetPKDt(pk, LOG_SUPPLYUSECLM[dtNo].c_str());
  }
  BOOL TMdlog_SupplyUse::GetPKDtBOOL(s64 pk, enmdlog_SupplyUse dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_SUPPLYUSECLM[dtNo].c_str());
  }
}
