#include "dbMdw_Event.h"
namespace db
{
  TMdw_Event::TMdw_Event():TModelGen("w_Event")
  {
  }
  TMdw_Event::~TMdw_Event(void)
  {
  }
  s64 TMdw_Event::GetLineDt(s64 line, enmdw_Event dtNo)
  {
    return GenGetLineDt(line, W_EVENTCLM[dtNo].c_str());
  }
  BOOL TMdw_Event::GetLineDtBOOL(s64 line, enmdw_Event dtNo)
  {
    return GenGetLineDtBOOL(line, W_EVENTCLM[dtNo].c_str());
  }
  s64 TMdw_Event::GetPKDt(s64 pk, enmdw_Event dtNo)
  {
    return GenGetPKDt(pk, W_EVENTCLM[dtNo].c_str());
  }
  BOOL TMdw_Event::GetPKDtBOOL(s64 pk, enmdw_Event dtNo)
  {
    return GenGetPKDtBOOL(pk, W_EVENTCLM[dtNo].c_str());
  }
}
