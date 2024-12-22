#include "dbMdKantokuSub.h"
namespace db
{
  TMdKantokuSub::TMdKantokuSub():TModelGen("KantokuSub")
  {
  }
  TMdKantokuSub::~TMdKantokuSub(void)
  {
  }
  s64 TMdKantokuSub::GetLineDt(s64 line, enmdKantokuSub dtNo)
  {
    return GenGetLineDt(line, KANTOKUSUBCLM[dtNo].c_str());
  }
  BOOL TMdKantokuSub::GetLineDtBOOL(s64 line, enmdKantokuSub dtNo)
  {
    return GenGetLineDtBOOL(line, KANTOKUSUBCLM[dtNo].c_str());
  }
  std::string TMdKantokuSub::GetLineDtStr(s64 line, enmdKantokuSubStr dtNo)
  {
    return GenGetLineDtStr(line, KANTOKUSUBCLMSTR[dtNo].c_str());
  }
  s64 TMdKantokuSub::GetPKDt(s64 pk, enmdKantokuSub dtNo)
  {
    return GenGetPKDt(pk, KANTOKUSUBCLM[dtNo].c_str());
  }
  BOOL TMdKantokuSub::GetPKDtBOOL(s64 pk, enmdKantokuSub dtNo)
  {
    return GenGetPKDtBOOL(pk, KANTOKUSUBCLM[dtNo].c_str());
  }
  std::string TMdKantokuSub::GetPKDtStr(s64 pk, enmdKantokuSubStr dtNo)
  {
    return GenGetPKDtStr(pk, KANTOKUSUBCLMSTR[dtNo].c_str());
  }
  s64 TMdKantokuSub::GetMyDt(enmdKantokuSub dtNo)
  {
    return GenGetMyDt(KANTOKUSUBCLM[dtNo].c_str());
  }
  BOOL TMdKantokuSub::GetMyDtBOOL(enmdKantokuSub dtNo)
  {
    return GenGetMyDtBOOL(KANTOKUSUBCLM[dtNo].c_str());
  }
  std::string TMdKantokuSub::GetMyDtStr(enmdKantokuSubStr dtNo)
  {
    return GenGetMyDtStr(KANTOKUSUBCLMSTR[dtNo].c_str());
  }
}
