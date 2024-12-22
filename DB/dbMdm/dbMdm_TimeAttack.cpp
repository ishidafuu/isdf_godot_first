#include "dbMdm_TimeAttack.h"
namespace db
{
  TMdm_TimeAttack::TMdm_TimeAttack():TModelGen("m_TimeAttack")
  {
  }
  TMdm_TimeAttack::~TMdm_TimeAttack(void)
  {
  }
  void TMdm_TimeAttack::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_TimeAttack", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_timeattack_pk));
      model_.intvecmap_["id_m_event"].push_back(csv.GetInt(i, mdm_timeattack_id_m_event));
      model_.intvecmap_["id_m_team"].push_back(csv.GetInt(i, mdm_timeattack_id_m_team));
      model_.intvecmap_["id_m_haikei"].push_back(csv.GetInt(i, mdm_timeattack_id_m_haikei));
      model_.intvecmap_["id_m_bgm"].push_back(csv.GetInt(i, mdm_timeattack_id_m_bgm));
      model_.intvecmap_["shiaiSec"].push_back(csv.GetInt(i, mdm_timeattack_shiaiSec));
      model_.intvecmap_["catch"].push_back(csv.GetInt(i, mdm_timeattack_catch));
      model_.intvecmap_["nice"].push_back(csv.GetInt(i, mdm_timeattack_nice));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_timeattack_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_timeattack_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_timeattack_itemNum));
    }
    
  }
  s64 TMdm_TimeAttack::GetLineDt(s64 line, enmdm_TimeAttack dtNo)
  {
    return GenGetLineDt(line, M_TIMEATTACKCLM[dtNo].c_str());
  }
  BOOL TMdm_TimeAttack::GetLineDtBOOL(s64 line, enmdm_TimeAttack dtNo)
  {
    return GenGetLineDtBOOL(line, M_TIMEATTACKCLM[dtNo].c_str());
  }
  s64 TMdm_TimeAttack::GetPKDt(s64 pk, enmdm_TimeAttack dtNo)
  {
    return GenGetPKDt(pk, M_TIMEATTACKCLM[dtNo].c_str());
  }
  BOOL TMdm_TimeAttack::GetPKDtBOOL(s64 pk, enmdm_TimeAttack dtNo)
  {
    return GenGetPKDtBOOL(pk, M_TIMEATTACKCLM[dtNo].c_str());
  }
}
