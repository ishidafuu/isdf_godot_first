#include "dbMdSupply.h"
namespace db
{
  TMdSupply::TMdSupply():TModelGen("Supply")
  {
  }
  TMdSupply::~TMdSupply(void)
  {
  }
  s64 TMdSupply::GetLineDt(s64 line, enmdSupply dtNo)
  {
    return GenGetLineDt(line, SUPPLYCLM[dtNo].c_str());
  }
  BOOL TMdSupply::GetLineDtBOOL(s64 line, enmdSupply dtNo)
  {
    return GenGetLineDtBOOL(line, SUPPLYCLM[dtNo].c_str());
  }
  s64 TMdSupply::GetPKDt(s64 pk, enmdSupply dtNo)
  {
    return GenGetPKDt(pk, SUPPLYCLM[dtNo].c_str());
  }
  BOOL TMdSupply::GetPKDtBOOL(s64 pk, enmdSupply dtNo)
  {
    return GenGetPKDtBOOL(pk, SUPPLYCLM[dtNo].c_str());
  }
}
