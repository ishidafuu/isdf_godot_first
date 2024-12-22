#include "dbMdTeam.h"
namespace db
{
  TMdTeam::TMdTeam():TModelGen("Team")
  {
  }
  TMdTeam::~TMdTeam(void)
  {
  }
  s64 TMdTeam::GetLineDt(s64 line, enmdTeam dtNo)
  {
    return GenGetLineDt(line, TEAMCLM[dtNo].c_str());
  }
  BOOL TMdTeam::GetLineDtBOOL(s64 line, enmdTeam dtNo)
  {
    return GenGetLineDtBOOL(line, TEAMCLM[dtNo].c_str());
  }
  s64 TMdTeam::GetPKDt(s64 pk, enmdTeam dtNo)
  {
    return GenGetPKDt(pk, TEAMCLM[dtNo].c_str());
  }
  BOOL TMdTeam::GetPKDtBOOL(s64 pk, enmdTeam dtNo)
  {
    return GenGetPKDtBOOL(pk, TEAMCLM[dtNo].c_str());
  }
}
