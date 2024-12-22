#include "dbMdBGM.h"
namespace db
{
  TMdBGM::TMdBGM():TModelGen("BGM")
  {
  }
  TMdBGM::~TMdBGM(void)
  {
  }
  s64 TMdBGM::GetLineDt(s64 line, enmdBGM dtNo)
  {
    return GenGetLineDt(line, BGMCLM[dtNo].c_str());
  }
  BOOL TMdBGM::GetLineDtBOOL(s64 line, enmdBGM dtNo)
  {
    return GenGetLineDtBOOL(line, BGMCLM[dtNo].c_str());
  }
  s64 TMdBGM::GetPKDt(s64 pk, enmdBGM dtNo)
  {
    return GenGetPKDt(pk, BGMCLM[dtNo].c_str());
  }
  BOOL TMdBGM::GetPKDtBOOL(s64 pk, enmdBGM dtNo)
  {
    return GenGetPKDtBOOL(pk, BGMCLM[dtNo].c_str());
  }
}
