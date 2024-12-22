#include "dbMdlog_Event.h"
namespace db
{
  TMdlog_Event::TMdlog_Event():TModelGen("log_Event")
  {
  }
  TMdlog_Event::~TMdlog_Event(void)
  {
  }
  s64 TMdlog_Event::GetLineDt(s64 line, enmdlog_Event dtNo)
  {
    return GenGetLineDt(line, LOG_EVENTCLM[dtNo].c_str());
  }
  BOOL TMdlog_Event::GetLineDtBOOL(s64 line, enmdlog_Event dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_EVENTCLM[dtNo].c_str());
  }
  s64 TMdlog_Event::GetPKDt(s64 pk, enmdlog_Event dtNo)
  {
    return GenGetPKDt(pk, LOG_EVENTCLM[dtNo].c_str());
  }
  BOOL TMdlog_Event::GetPKDtBOOL(s64 pk, enmdlog_Event dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_EVENTCLM[dtNo].c_str());
  }
}
