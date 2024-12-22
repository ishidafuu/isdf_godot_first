#include "dbMdw_HukuCnt.h"
namespace db
{
  TMdw_HukuCnt::TMdw_HukuCnt():TModelGen("w_HukuCnt")
  {
  }
  TMdw_HukuCnt::~TMdw_HukuCnt(void)
  {
  }
  s64 TMdw_HukuCnt::GetLineDt(s64 line, enmdw_HukuCnt dtNo)
  {
    return GenGetLineDt(line, W_HUKUCNTCLM[dtNo].c_str());
  }
  BOOL TMdw_HukuCnt::GetLineDtBOOL(s64 line, enmdw_HukuCnt dtNo)
  {
    return GenGetLineDtBOOL(line, W_HUKUCNTCLM[dtNo].c_str());
  }
  s64 TMdw_HukuCnt::GetPKDt(s64 pk, enmdw_HukuCnt dtNo)
  {
    return GenGetPKDt(pk, W_HUKUCNTCLM[dtNo].c_str());
  }
  BOOL TMdw_HukuCnt::GetPKDtBOOL(s64 pk, enmdw_HukuCnt dtNo)
  {
    return GenGetPKDtBOOL(pk, W_HUKUCNTCLM[dtNo].c_str());
  }
}
