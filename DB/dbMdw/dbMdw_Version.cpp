#include "dbMdw_Version.h"
namespace db
{
  TMdw_Version::TMdw_Version():TModelGen("w_Version")
  {
  }
  TMdw_Version::~TMdw_Version(void)
  {
  }
  s64 TMdw_Version::GetLineDt(s64 line, enmdw_Version dtNo)
  {
    return GenGetLineDt(line, W_VERSIONCLM[dtNo].c_str());
  }
  BOOL TMdw_Version::GetLineDtBOOL(s64 line, enmdw_Version dtNo)
  {
    return GenGetLineDtBOOL(line, W_VERSIONCLM[dtNo].c_str());
  }
  s64 TMdw_Version::GetPKDt(s64 pk, enmdw_Version dtNo)
  {
    return GenGetPKDt(pk, W_VERSIONCLM[dtNo].c_str());
  }
  BOOL TMdw_Version::GetPKDtBOOL(s64 pk, enmdw_Version dtNo)
  {
    return GenGetPKDtBOOL(pk, W_VERSIONCLM[dtNo].c_str());
  }
}
