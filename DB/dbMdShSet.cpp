#include "dbMdShSet.h"
namespace db
{
  TMdShSet::TMdShSet():TModelGen("ShSet")
  {
  }
  TMdShSet::~TMdShSet(void)
  {
  }
  s64 TMdShSet::GetLineDt(s64 line, enmdShSet dtNo)
  {
    return GenGetLineDt(line, SHSETCLM[dtNo].c_str());
  }
  BOOL TMdShSet::GetLineDtBOOL(s64 line, enmdShSet dtNo)
  {
    return GenGetLineDtBOOL(line, SHSETCLM[dtNo].c_str());
  }
  std::string TMdShSet::GetLineDtStr(s64 line, enmdShSetStr dtNo)
  {
    return GenGetLineDtStr(line, SHSETCLMSTR[dtNo].c_str());
  }
  s64 TMdShSet::GetPKDt(s64 pk, enmdShSet dtNo)
  {
    return GenGetPKDt(pk, SHSETCLM[dtNo].c_str());
  }
  BOOL TMdShSet::GetPKDtBOOL(s64 pk, enmdShSet dtNo)
  {
    return GenGetPKDtBOOL(pk, SHSETCLM[dtNo].c_str());
  }
  std::string TMdShSet::GetPKDtStr(s64 pk, enmdShSetStr dtNo)
  {
    return GenGetPKDtStr(pk, SHSETCLMSTR[dtNo].c_str());
  }
}
