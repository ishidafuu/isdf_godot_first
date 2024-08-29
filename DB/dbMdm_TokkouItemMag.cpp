#include "dbMdm_TokkouItemMag.h"
namespace db
{
  TMdm_TokkouItemMag::TMdm_TokkouItemMag():TModelGen("m_TokkouItemMag")
  {
  }
  TMdm_TokkouItemMag::~TMdm_TokkouItemMag(void)
  {
  }
  void TMdm_TokkouItemMag::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_TokkouItemMag", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_tokkouitemmag_pk));
      model_.intvecmap_["tokkouNo"].push_back(csv.GetInt(i, mdm_tokkouitemmag_tokkouNo));
      model_.intvecmap_["effectType"].push_back(csv.GetInt(i, mdm_tokkouitemmag_effectType));
      model_.intvecmap_["haveNum"].push_back(csv.GetInt(i, mdm_tokkouitemmag_haveNum));
      model_.intvecmap_["effect"].push_back(csv.GetInt(i, mdm_tokkouitemmag_effect));
    }
    
  }
  s64 TMdm_TokkouItemMag::GetLineDt(s64 line, enmdm_TokkouItemMag dtNo)
  {
    return GenGetLineDt(line, M_TOKKOUITEMMAGCLM[dtNo].c_str());
  }
  BOOL TMdm_TokkouItemMag::GetLineDtBOOL(s64 line, enmdm_TokkouItemMag dtNo)
  {
    return GenGetLineDtBOOL(line, M_TOKKOUITEMMAGCLM[dtNo].c_str());
  }
  s64 TMdm_TokkouItemMag::GetPKDt(s64 pk, enmdm_TokkouItemMag dtNo)
  {
    return GenGetPKDt(pk, M_TOKKOUITEMMAGCLM[dtNo].c_str());
  }
  BOOL TMdm_TokkouItemMag::GetPKDtBOOL(s64 pk, enmdm_TokkouItemMag dtNo)
  {
    return GenGetPKDtBOOL(pk, M_TOKKOUITEMMAGCLM[dtNo].c_str());
  }
}
