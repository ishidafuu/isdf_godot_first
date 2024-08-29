#include "dbMdre_Refresh.h"
namespace db
{
  TMdre_Refresh::TMdre_Refresh():TModelGen("re_Refresh")
  {
  }
  TMdre_Refresh::~TMdre_Refresh(void)
  {
  }
  s64 TMdre_Refresh::GetLineDt(s64 line, enmdre_Refresh dtNo)
  {
    return GenGetLineDt(line, RE_REFRESHCLM[dtNo].c_str());
  }
  BOOL TMdre_Refresh::GetLineDtBOOL(s64 line, enmdre_Refresh dtNo)
  {
    return GenGetLineDtBOOL(line, RE_REFRESHCLM[dtNo].c_str());
  }
  s64 TMdre_Refresh::GetPKDt(s64 pk, enmdre_Refresh dtNo)
  {
    return GenGetPKDt(pk, RE_REFRESHCLM[dtNo].c_str());
  }
  BOOL TMdre_Refresh::GetPKDtBOOL(s64 pk, enmdre_Refresh dtNo)
  {
    return GenGetPKDtBOOL(pk, RE_REFRESHCLM[dtNo].c_str());
  }
}
