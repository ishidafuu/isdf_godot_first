#include "dbMdm_VSBonus.h"
namespace db
{
  TMdm_VSBonus::TMdm_VSBonus():TModelGen("m_VSBonus")
  {
  }
  TMdm_VSBonus::~TMdm_VSBonus(void)
  {
  }
  void TMdm_VSBonus::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_VSBonus", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_vsbonus_pk));
      model_.intvecmap_["bonusNum"].push_back(csv.GetInt(i, mdm_vsbonus_bonusNum));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_vsbonus_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_vsbonus_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_vsbonus_itemNum));
      model_.strvecmap_["msg_c128"].push_back(csv.GetStr(i, mdm_vsbonus_msg_c128));
      model_.strvecmap_["endmsg_c128"].push_back(csv.GetStr(i, mdm_vsbonus_endmsg_c128));
    }
    
  }
  s64 TMdm_VSBonus::GetLineDt(s64 line, enmdm_VSBonus dtNo)
  {
    return GenGetLineDt(line, M_VSBONUSCLM[dtNo].c_str());
  }
  BOOL TMdm_VSBonus::GetLineDtBOOL(s64 line, enmdm_VSBonus dtNo)
  {
    return GenGetLineDtBOOL(line, M_VSBONUSCLM[dtNo].c_str());
  }
  std::string TMdm_VSBonus::GetLineDtStr(s64 line, enmdm_VSBonusStr dtNo)
  {
    return GenGetLineDtStr(line, M_VSBONUSCLMSTR[dtNo].c_str());
  }
  s64 TMdm_VSBonus::GetPKDt(s64 pk, enmdm_VSBonus dtNo)
  {
    return GenGetPKDt(pk, M_VSBONUSCLM[dtNo].c_str());
  }
  BOOL TMdm_VSBonus::GetPKDtBOOL(s64 pk, enmdm_VSBonus dtNo)
  {
    return GenGetPKDtBOOL(pk, M_VSBONUSCLM[dtNo].c_str());
  }
  std::string TMdm_VSBonus::GetPKDtStr(s64 pk, enmdm_VSBonusStr dtNo)
  {
    return GenGetPKDtStr(pk, M_VSBONUSCLMSTR[dtNo].c_str());
  }
}
