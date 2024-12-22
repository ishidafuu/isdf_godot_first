#include "dbMdShot.h"
namespace db
{
  TMdShot::TMdShot():TModelGen("Shot")
  {
  }
  TMdShot::~TMdShot(void)
  {
  }
  s64 TMdShot::GetLineDt(s64 line, enmdShot dtNo)
  {
    return GenGetLineDt(line, SHOTCLM[dtNo].c_str());
  }
  BOOL TMdShot::GetLineDtBOOL(s64 line, enmdShot dtNo)
  {
    return GenGetLineDtBOOL(line, SHOTCLM[dtNo].c_str());
  }
  std::string TMdShot::GetLineDtStr(s64 line, enmdShotStr dtNo)
  {
    return GenGetLineDtStr(line, SHOTCLMSTR[dtNo].c_str());
  }
  s64 TMdShot::GetPKDt(s64 pk, enmdShot dtNo)
  {
    return GenGetPKDt(pk, SHOTCLM[dtNo].c_str());
  }
  BOOL TMdShot::GetPKDtBOOL(s64 pk, enmdShot dtNo)
  {
    return GenGetPKDtBOOL(pk, SHOTCLM[dtNo].c_str());
  }
  std::string TMdShot::GetPKDtStr(s64 pk, enmdShotStr dtNo)
  {
    return GenGetPKDtStr(pk, SHOTCLMSTR[dtNo].c_str());
  }
}
