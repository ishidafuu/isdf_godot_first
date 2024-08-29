#include "dbMdm_TokkouMag.h"
namespace db
{
  TMdm_TokkouMag::TMdm_TokkouMag():TModelGen("m_TokkouMag")
  {
  }
  TMdm_TokkouMag::~TMdm_TokkouMag(void)
  {
  }
  void TMdm_TokkouMag::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_TokkouMag", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_tokkoumag_pk));
      model_.intvecmap_["magNo"].push_back(csv.GetInt(i, mdm_tokkoumag_magNo));
      model_.intvecmap_["overlap"].push_back(csv.GetInt(i, mdm_tokkoumag_overlap));
      model_.intvecmap_["mag"].push_back(csv.GetInt(i, mdm_tokkoumag_mag));
    }
    
  }
  s64 TMdm_TokkouMag::GetLineDt(s64 line, enmdm_TokkouMag dtNo)
  {
    return GenGetLineDt(line, M_TOKKOUMAGCLM[dtNo].c_str());
  }
  BOOL TMdm_TokkouMag::GetLineDtBOOL(s64 line, enmdm_TokkouMag dtNo)
  {
    return GenGetLineDtBOOL(line, M_TOKKOUMAGCLM[dtNo].c_str());
  }
  s64 TMdm_TokkouMag::GetPKDt(s64 pk, enmdm_TokkouMag dtNo)
  {
    return GenGetPKDt(pk, M_TOKKOUMAGCLM[dtNo].c_str());
  }
  BOOL TMdm_TokkouMag::GetPKDtBOOL(s64 pk, enmdm_TokkouMag dtNo)
  {
    return GenGetPKDtBOOL(pk, M_TOKKOUMAGCLM[dtNo].c_str());
  }
}
