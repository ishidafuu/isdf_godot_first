#include "dbMdMaptip.h"
namespace db
{
  TMdMaptip::TMdMaptip():TModelGen("Maptip")
  {
  }
  TMdMaptip::~TMdMaptip(void)
  {
  }
  s64 TMdMaptip::GetLineDt(s64 line, enmdMaptip dtNo)
  {
    return GenGetLineDt(line, MAPTIPCLM[dtNo].c_str());
  }
  BOOL TMdMaptip::GetLineDtBOOL(s64 line, enmdMaptip dtNo)
  {
    return GenGetLineDtBOOL(line, MAPTIPCLM[dtNo].c_str());
  }
  s64 TMdMaptip::GetPKDt(s64 pk, enmdMaptip dtNo)
  {
    return GenGetPKDt(pk, MAPTIPCLM[dtNo].c_str());
  }
  BOOL TMdMaptip::GetPKDtBOOL(s64 pk, enmdMaptip dtNo)
  {
    return GenGetPKDtBOOL(pk, MAPTIPCLM[dtNo].c_str());
  }
}
