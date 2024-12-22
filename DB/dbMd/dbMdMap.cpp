#include "dbMdMap.h"
namespace db
{
  TMdMap::TMdMap():TModelGen("Map")
  {
  }
  TMdMap::~TMdMap(void)
  {
  }
  s64 TMdMap::GetLineDt(s64 line, enmdMap dtNo)
  {
    return GenGetLineDt(line, MAPCLM[dtNo].c_str());
  }
  BOOL TMdMap::GetLineDtBOOL(s64 line, enmdMap dtNo)
  {
    return GenGetLineDtBOOL(line, MAPCLM[dtNo].c_str());
  }
  s64 TMdMap::GetPKDt(s64 pk, enmdMap dtNo)
  {
    return GenGetPKDt(pk, MAPCLM[dtNo].c_str());
  }
  BOOL TMdMap::GetPKDtBOOL(s64 pk, enmdMap dtNo)
  {
    return GenGetPKDtBOOL(pk, MAPCLM[dtNo].c_str());
  }
}
