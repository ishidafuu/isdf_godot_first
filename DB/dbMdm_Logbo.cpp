#include "dbMdm_Logbo.h"
namespace db
{
  TMdm_Logbo::TMdm_Logbo():TModelGen("m_Logbo")
  {
  }
  TMdm_Logbo::~TMdm_Logbo(void)
  {
  }
  void TMdm_Logbo::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Logbo", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_logbo_pk));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_logbo_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_logbo_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_logbo_itemNum));
    }
    
  }
  s64 TMdm_Logbo::GetLineDt(s64 line, enmdm_Logbo dtNo)
  {
    return GenGetLineDt(line, M_LOGBOCLM[dtNo].c_str());
  }
  BOOL TMdm_Logbo::GetLineDtBOOL(s64 line, enmdm_Logbo dtNo)
  {
    return GenGetLineDtBOOL(line, M_LOGBOCLM[dtNo].c_str());
  }
  s64 TMdm_Logbo::GetPKDt(s64 pk, enmdm_Logbo dtNo)
  {
    return GenGetPKDt(pk, M_LOGBOCLM[dtNo].c_str());
  }
  BOOL TMdm_Logbo::GetPKDtBOOL(s64 pk, enmdm_Logbo dtNo)
  {
    return GenGetPKDtBOOL(pk, M_LOGBOCLM[dtNo].c_str());
  }
}
