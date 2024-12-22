#include "dbMdlog_TeamSell.h"
namespace db
{
  TMdlog_TeamSell::TMdlog_TeamSell():TModelGen("log_TeamSell")
  {
  }
  TMdlog_TeamSell::~TMdlog_TeamSell(void)
  {
  }
  s64 TMdlog_TeamSell::GetLineDt(s64 line, enmdlog_TeamSell dtNo)
  {
    return GenGetLineDt(line, LOG_TEAMSELLCLM[dtNo].c_str());
  }
  BOOL TMdlog_TeamSell::GetLineDtBOOL(s64 line, enmdlog_TeamSell dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_TEAMSELLCLM[dtNo].c_str());
  }
  s64 TMdlog_TeamSell::GetPKDt(s64 pk, enmdlog_TeamSell dtNo)
  {
    return GenGetPKDt(pk, LOG_TEAMSELLCLM[dtNo].c_str());
  }
  BOOL TMdlog_TeamSell::GetPKDtBOOL(s64 pk, enmdlog_TeamSell dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_TEAMSELLCLM[dtNo].c_str());
  }
}
