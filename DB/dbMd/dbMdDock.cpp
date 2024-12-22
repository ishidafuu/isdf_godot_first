#include "dbMdDock.h"
namespace db
{
  TMdDock::TMdDock():TModelGen("Dock")
  {
  }
  TMdDock::~TMdDock(void)
  {
  }
  s64 TMdDock::GetLineDt(s64 line, enmdDock dtNo)
  {
    return GenGetLineDt(line, DOCKCLM[dtNo].c_str());
  }
  BOOL TMdDock::GetLineDtBOOL(s64 line, enmdDock dtNo)
  {
    return GenGetLineDtBOOL(line, DOCKCLM[dtNo].c_str());
  }
  s64 TMdDock::GetPKDt(s64 pk, enmdDock dtNo)
  {
    return GenGetPKDt(pk, DOCKCLM[dtNo].c_str());
  }
  BOOL TMdDock::GetPKDtBOOL(s64 pk, enmdDock dtNo)
  {
    return GenGetPKDtBOOL(pk, DOCKCLM[dtNo].c_str());
  }
}
