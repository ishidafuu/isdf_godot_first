#include "dbMdInvite.h"
namespace db
{
  TMdInvite::TMdInvite():TModelGen("Invite")
  {
  }
  TMdInvite::~TMdInvite(void)
  {
  }
  s64 TMdInvite::GetLineDt(s64 line, enmdInvite dtNo)
  {
    return GenGetLineDt(line, INVITECLM[dtNo].c_str());
  }
  BOOL TMdInvite::GetLineDtBOOL(s64 line, enmdInvite dtNo)
  {
    return GenGetLineDtBOOL(line, INVITECLM[dtNo].c_str());
  }
  s64 TMdInvite::GetPKDt(s64 pk, enmdInvite dtNo)
  {
    return GenGetPKDt(pk, INVITECLM[dtNo].c_str());
  }
  BOOL TMdInvite::GetPKDtBOOL(s64 pk, enmdInvite dtNo)
  {
    return GenGetPKDtBOOL(pk, INVITECLM[dtNo].c_str());
  }
}
