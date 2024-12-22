#include "dbMdVSDrop.h"
namespace db
{
  TMdVSDrop::TMdVSDrop():TModelGen("VSDrop")
  {
  }
  TMdVSDrop::~TMdVSDrop(void)
  {
  }
  s64 TMdVSDrop::GetLineDt(s64 line, enmdVSDrop dtNo)
  {
    return GenGetLineDt(line, VSDROPCLM[dtNo].c_str());
  }
  BOOL TMdVSDrop::GetLineDtBOOL(s64 line, enmdVSDrop dtNo)
  {
    return GenGetLineDtBOOL(line, VSDROPCLM[dtNo].c_str());
  }
  s64 TMdVSDrop::GetPKDt(s64 pk, enmdVSDrop dtNo)
  {
    return GenGetPKDt(pk, VSDROPCLM[dtNo].c_str());
  }
  BOOL TMdVSDrop::GetPKDtBOOL(s64 pk, enmdVSDrop dtNo)
  {
    return GenGetPKDtBOOL(pk, VSDROPCLM[dtNo].c_str());
  }
}
