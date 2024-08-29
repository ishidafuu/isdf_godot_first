#include "dbMdm_Connect.h"
namespace db
{
  TMdm_Connect::TMdm_Connect():TModelGen("m_Connect")
  {
  }
  TMdm_Connect::~TMdm_Connect(void)
  {
  }
  void TMdm_Connect::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Connect", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_connect_pk));
      model_.intvecmap_["delay"].push_back(csv.GetInt(i, mdm_connect_delay));
      model_.intvecmap_["buf"].push_back(csv.GetInt(i, mdm_connect_buf));
      model_.intvecmap_["senditv"].push_back(csv.GetInt(i, mdm_connect_senditv));
      model_.intvecmap_["rollitv"].push_back(csv.GetInt(i, mdm_connect_rollitv));
      model_.intvecmap_["poscheck_f"].push_back(csv.GetInt(i, mdm_connect_poscheck_f));
      model_.intvecmap_["staminaMin"].push_back(csv.GetInt(i, mdm_connect_staminaMin));
      model_.intvecmap_["teamStamina_f"].push_back(csv.GetInt(i, mdm_connect_teamStamina_f));
    }
    
  }
  s64 TMdm_Connect::GetLineDt(s64 line, enmdm_Connect dtNo)
  {
    return GenGetLineDt(line, M_CONNECTCLM[dtNo].c_str());
  }
  BOOL TMdm_Connect::GetLineDtBOOL(s64 line, enmdm_Connect dtNo)
  {
    return GenGetLineDtBOOL(line, M_CONNECTCLM[dtNo].c_str());
  }
  s64 TMdm_Connect::GetPKDt(s64 pk, enmdm_Connect dtNo)
  {
    return GenGetPKDt(pk, M_CONNECTCLM[dtNo].c_str());
  }
  BOOL TMdm_Connect::GetPKDtBOOL(s64 pk, enmdm_Connect dtNo)
  {
    return GenGetPKDtBOOL(pk, M_CONNECTCLM[dtNo].c_str());
  }
}
