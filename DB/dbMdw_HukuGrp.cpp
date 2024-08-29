#include "dbMdw_HukuGrp.h"
namespace db
{
  TMdw_HukuGrp::TMdw_HukuGrp():TModelGen("w_HukuGrp")
  {
  }
  TMdw_HukuGrp::~TMdw_HukuGrp(void)
  {
  }
  s64 TMdw_HukuGrp::GetLineDt(s64 line, enmdw_HukuGrp dtNo)
  {
    return GenGetLineDt(line, W_HUKUGRPCLM[dtNo].c_str());
  }
  BOOL TMdw_HukuGrp::GetLineDtBOOL(s64 line, enmdw_HukuGrp dtNo)
  {
    return GenGetLineDtBOOL(line, W_HUKUGRPCLM[dtNo].c_str());
  }
  s64 TMdw_HukuGrp::GetPKDt(s64 pk, enmdw_HukuGrp dtNo)
  {
    return GenGetPKDt(pk, W_HUKUGRPCLM[dtNo].c_str());
  }
  BOOL TMdw_HukuGrp::GetPKDtBOOL(s64 pk, enmdw_HukuGrp dtNo)
  {
    return GenGetPKDtBOOL(pk, W_HUKUGRPCLM[dtNo].c_str());
  }
}
