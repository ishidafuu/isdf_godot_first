#include "dbMdm_Tokkou.h"
namespace db
{
  TMdm_Tokkou::TMdm_Tokkou():TModelGen("m_Tokkou")
  {
  }
  TMdm_Tokkou::~TMdm_Tokkou(void)
  {
  }
  void TMdm_Tokkou::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Tokkou", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_tokkou_pk));
      model_.intvecmap_["tokkouNo"].push_back(csv.GetInt(i, mdm_tokkou_tokkouNo));
      model_.intvecmap_["id_m_team"].push_back(csv.GetInt(i, mdm_tokkou_id_m_team));
      model_.intvecmap_["magNo"].push_back(csv.GetInt(i, mdm_tokkou_magNo));
    }
    
  }
  s64 TMdm_Tokkou::GetLineDt(s64 line, enmdm_Tokkou dtNo)
  {
    return GenGetLineDt(line, M_TOKKOUCLM[dtNo].c_str());
  }
  BOOL TMdm_Tokkou::GetLineDtBOOL(s64 line, enmdm_Tokkou dtNo)
  {
    return GenGetLineDtBOOL(line, M_TOKKOUCLM[dtNo].c_str());
  }
  s64 TMdm_Tokkou::GetPKDt(s64 pk, enmdm_Tokkou dtNo)
  {
    return GenGetPKDt(pk, M_TOKKOUCLM[dtNo].c_str());
  }
  BOOL TMdm_Tokkou::GetPKDtBOOL(s64 pk, enmdm_Tokkou dtNo)
  {
    return GenGetPKDtBOOL(pk, M_TOKKOUCLM[dtNo].c_str());
  }
}
