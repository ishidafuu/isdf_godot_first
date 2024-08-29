#include "dbMdKantoku.h"
namespace db
{
  TMdKantoku::TMdKantoku():TModelGen("Kantoku")
  {
  }
  TMdKantoku::~TMdKantoku(void)
  {
  }
  s64 TMdKantoku::GetLineDt(s64 line, enmdKantoku dtNo)
  {
    return GenGetLineDt(line, KANTOKUCLM[dtNo].c_str());
  }
  BOOL TMdKantoku::GetLineDtBOOL(s64 line, enmdKantoku dtNo)
  {
    return GenGetLineDtBOOL(line, KANTOKUCLM[dtNo].c_str());
  }
  std::string TMdKantoku::GetLineDtStr(s64 line, enmdKantokuStr dtNo)
  {
    return GenGetLineDtStr(line, KANTOKUCLMSTR[dtNo].c_str());
  }
  s64 TMdKantoku::GetPKDt(s64 pk, enmdKantoku dtNo)
  {
    return GenGetPKDt(pk, KANTOKUCLM[dtNo].c_str());
  }
  BOOL TMdKantoku::GetPKDtBOOL(s64 pk, enmdKantoku dtNo)
  {
    return GenGetPKDtBOOL(pk, KANTOKUCLM[dtNo].c_str());
  }
  std::string TMdKantoku::GetPKDtStr(s64 pk, enmdKantokuStr dtNo)
  {
    return GenGetPKDtStr(pk, KANTOKUCLMSTR[dtNo].c_str());
  }
  s64 TMdKantoku::GetMyDt(enmdKantoku dtNo)
  {
    return GenGetMyDt(KANTOKUCLM[dtNo].c_str());
  }
  BOOL TMdKantoku::GetMyDtBOOL(enmdKantoku dtNo)
  {
    return GenGetMyDtBOOL(KANTOKUCLM[dtNo].c_str());
  }
  std::string TMdKantoku::GetMyDtStr(enmdKantokuStr dtNo)
  {
    return GenGetMyDtStr(KANTOKUCLMSTR[dtNo].c_str());
  }
}
