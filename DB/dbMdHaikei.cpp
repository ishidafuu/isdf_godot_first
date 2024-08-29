#include "dbMdHaikei.h"
namespace db
{
  TMdHaikei::TMdHaikei():TModelGen("Haikei")
  {
  }
  TMdHaikei::~TMdHaikei(void)
  {
  }
  s64 TMdHaikei::GetLineDt(s64 line, enmdHaikei dtNo)
  {
    return GenGetLineDt(line, HAIKEICLM[dtNo].c_str());
  }
  BOOL TMdHaikei::GetLineDtBOOL(s64 line, enmdHaikei dtNo)
  {
    return GenGetLineDtBOOL(line, HAIKEICLM[dtNo].c_str());
  }
  s64 TMdHaikei::GetPKDt(s64 pk, enmdHaikei dtNo)
  {
    return GenGetPKDt(pk, HAIKEICLM[dtNo].c_str());
  }
  BOOL TMdHaikei::GetPKDtBOOL(s64 pk, enmdHaikei dtNo)
  {
    return GenGetPKDtBOOL(pk, HAIKEICLM[dtNo].c_str());
  }
}
