#include "dbMdm_Team.h"
namespace db
{
  TMdm_Team::TMdm_Team():TModelGen("m_Team")
  {
  }
  TMdm_Team::~TMdm_Team(void)
  {
  }
  void TMdm_Team::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Team", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_team_pk));
      model_.intvecmap_["rarity"].push_back(csv.GetInt(i, mdm_team_rarity));
      model_.intvecmap_["npc_f"].push_back(csv.GetInt(i, mdm_team_npc_f));
      model_.intvecmap_["com"].push_back(csv.GetInt(i, mdm_team_com));
      model_.intvecmap_["gb_f"].push_back(csv.GetInt(i, mdm_team_gb_f));
      model_.intvecmap_["id_m_haikei"].push_back(csv.GetInt(i, mdm_team_id_m_haikei));
      model_.intvecmap_["id_m_bgm"].push_back(csv.GetInt(i, mdm_team_id_m_bgm));
      model_.intvecmap_["id_m_shset"].push_back(csv.GetInt(i, mdm_team_id_m_shset));
      model_.intvecmap_["id_m_char0"].push_back(csv.GetInt(i, mdm_team_id_m_char0));
      model_.intvecmap_["id_m_char1"].push_back(csv.GetInt(i, mdm_team_id_m_char1));
      model_.intvecmap_["id_m_char2"].push_back(csv.GetInt(i, mdm_team_id_m_char2));
      model_.intvecmap_["id_m_char3"].push_back(csv.GetInt(i, mdm_team_id_m_char3));
      model_.intvecmap_["id_m_char4"].push_back(csv.GetInt(i, mdm_team_id_m_char4));
      model_.intvecmap_["id_m_char5"].push_back(csv.GetInt(i, mdm_team_id_m_char5));
      model_.intvecmap_["id_m_char6"].push_back(csv.GetInt(i, mdm_team_id_m_char6));
      model_.intvecmap_["shirCol"].push_back(csv.GetInt(i, mdm_team_shirCol));
      model_.intvecmap_["pantCol"].push_back(csv.GetInt(i, mdm_team_pantCol));
      model_.intvecmap_["sideLineCol"].push_back(csv.GetInt(i, mdm_team_sideLineCol));
      model_.intvecmap_["underLineCol"].push_back(csv.GetInt(i, mdm_team_underLineCol));
      model_.intvecmap_["shoeCol"].push_back(csv.GetInt(i, mdm_team_shoeCol));
      model_.strvecmap_["name_c32"].push_back(csv.GetStr(i, mdm_team_name_c32));
      model_.strvecmap_["text0_c64"].push_back(csv.GetStr(i, mdm_team_text0_c64));
      model_.strvecmap_["text1_c64"].push_back(csv.GetStr(i, mdm_team_text1_c64));
      model_.strvecmap_["text2_c64"].push_back(csv.GetStr(i, mdm_team_text2_c64));
      model_.strvecmap_["text3_c64"].push_back(csv.GetStr(i, mdm_team_text3_c64));
    }
    
  }
  s64 TMdm_Team::GetLineDt(s64 line, enmdm_Team dtNo)
  {
    return GenGetLineDt(line, M_TEAMCLM[dtNo].c_str());
  }
  BOOL TMdm_Team::GetLineDtBOOL(s64 line, enmdm_Team dtNo)
  {
    return GenGetLineDtBOOL(line, M_TEAMCLM[dtNo].c_str());
  }
  std::string TMdm_Team::GetLineDtStr(s64 line, enmdm_TeamStr dtNo)
  {
    return GenGetLineDtStr(line, M_TEAMCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Team::GetPKDt(s64 pk, enmdm_Team dtNo)
  {
    return GenGetPKDt(pk, M_TEAMCLM[dtNo].c_str());
  }
  BOOL TMdm_Team::GetPKDtBOOL(s64 pk, enmdm_Team dtNo)
  {
    return GenGetPKDtBOOL(pk, M_TEAMCLM[dtNo].c_str());
  }
  std::string TMdm_Team::GetPKDtStr(s64 pk, enmdm_TeamStr dtNo)
  {
    return GenGetPKDtStr(pk, M_TEAMCLMSTR[dtNo].c_str());
  }
}
