#include "dbMdTeamCharLv.h"
namespace db
{
  TMdTeamCharLv::TMdTeamCharLv():TModelGen("TeamCharLv")
  {
  }
  TMdTeamCharLv::~TMdTeamCharLv(void)
  {
  }
  s64 TMdTeamCharLv::GetLineDt(s64 line, enmdTeamCharLv dtNo)
  {
    return GenGetLineDt(line, TEAMCHARLVCLM[dtNo].c_str());
  }
  BOOL TMdTeamCharLv::GetLineDtBOOL(s64 line, enmdTeamCharLv dtNo)
  {
    return GenGetLineDtBOOL(line, TEAMCHARLVCLM[dtNo].c_str());
  }
  s64 TMdTeamCharLv::GetPKDt(s64 pk, enmdTeamCharLv dtNo)
  {
    return GenGetPKDt(pk, TEAMCHARLVCLM[dtNo].c_str());
  }
  BOOL TMdTeamCharLv::GetPKDtBOOL(s64 pk, enmdTeamCharLv dtNo)
  {
    return GenGetPKDtBOOL(pk, TEAMCHARLVCLM[dtNo].c_str());
  }
}
