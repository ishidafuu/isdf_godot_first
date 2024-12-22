#include "dbMdlog_ShotDel.h"
namespace db
{
  TMdlog_ShotDel::TMdlog_ShotDel():TModelGen("log_ShotDel")
  {
  }
  TMdlog_ShotDel::~TMdlog_ShotDel(void)
  {
  }
  s64 TMdlog_ShotDel::GetLineDt(s64 line, enmdlog_ShotDel dtNo)
  {
    return GenGetLineDt(line, LOG_SHOTDELCLM[dtNo].c_str());
  }
  BOOL TMdlog_ShotDel::GetLineDtBOOL(s64 line, enmdlog_ShotDel dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_SHOTDELCLM[dtNo].c_str());
  }
  s64 TMdlog_ShotDel::GetPKDt(s64 pk, enmdlog_ShotDel dtNo)
  {
    return GenGetPKDt(pk, LOG_SHOTDELCLM[dtNo].c_str());
  }
  BOOL TMdlog_ShotDel::GetPKDtBOOL(s64 pk, enmdlog_ShotDel dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_SHOTDELCLM[dtNo].c_str());
  }
}
