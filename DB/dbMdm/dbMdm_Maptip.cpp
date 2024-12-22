#include "dbMdm_Maptip.h"
namespace db
{
  TMdm_Maptip::TMdm_Maptip():TModelGen("m_Maptip")
  {
  }
  TMdm_Maptip::~TMdm_Maptip(void)
  {
  }
  void TMdm_Maptip::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Maptip", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_maptip_pk));
      model_.intvecmap_["id_m_event"].push_back(csv.GetInt(i, mdm_maptip_id_m_event));
      model_.intvecmap_["mapNo"].push_back(csv.GetInt(i, mdm_maptip_mapNo));
      model_.intvecmap_["posNo"].push_back(csv.GetInt(i, mdm_maptip_posNo));
      model_.intvecmap_["staminaMin"].push_back(csv.GetInt(i, mdm_maptip_staminaMin));
      model_.intvecmap_["shiaiSec"].push_back(csv.GetInt(i, mdm_maptip_shiaiSec));
      model_.intvecmap_["prob_B"].push_back(csv.GetInt(i, mdm_maptip_prob_B));
      model_.intvecmap_["prob_C"].push_back(csv.GetInt(i, mdm_maptip_prob_C));
      model_.intvecmap_["start_f"].push_back(csv.GetInt(i, mdm_maptip_start_f));
      model_.intvecmap_["warpNo"].push_back(csv.GetInt(i, mdm_maptip_warpNo));
      model_.intvecmap_["posX"].push_back(csv.GetInt(i, mdm_maptip_posX));
      model_.intvecmap_["posY"].push_back(csv.GetInt(i, mdm_maptip_posY));
      model_.intvecmap_["id_m_haikei"].push_back(csv.GetInt(i, mdm_maptip_id_m_haikei));
      model_.intvecmap_["id_m_bgm"].push_back(csv.GetInt(i, mdm_maptip_id_m_bgm));
      model_.intvecmap_["roadUp"].push_back(csv.GetInt(i, mdm_maptip_roadUp));
      model_.intvecmap_["roadDown"].push_back(csv.GetInt(i, mdm_maptip_roadDown));
      model_.intvecmap_["roadLeft"].push_back(csv.GetInt(i, mdm_maptip_roadLeft));
      model_.intvecmap_["roadRight"].push_back(csv.GetInt(i, mdm_maptip_roadRight));
      model_.intvecmap_["area"].push_back(csv.GetInt(i, mdm_maptip_area));
      model_.intvecmap_["ykkarea"].push_back(csv.GetInt(i, mdm_maptip_ykkarea));
      model_.intvecmap_["ykkroad"].push_back(csv.GetInt(i, mdm_maptip_ykkroad));
      model_.intvecmap_["bossOpen"].push_back(csv.GetInt(i, mdm_maptip_bossOpen));
      model_.intvecmap_["fc0"].push_back(csv.GetInt(i, mdm_maptip_fc0));
      model_.intvecmap_["fc1"].push_back(csv.GetInt(i, mdm_maptip_fc1));
      model_.intvecmap_["fc2"].push_back(csv.GetInt(i, mdm_maptip_fc2));
      model_.intvecmap_["fc3"].push_back(csv.GetInt(i, mdm_maptip_fc3));
      model_.intvecmap_["fc4"].push_back(csv.GetInt(i, mdm_maptip_fc4));
      model_.intvecmap_["fc5"].push_back(csv.GetInt(i, mdm_maptip_fc5));
      model_.intvecmap_["fc6"].push_back(csv.GetInt(i, mdm_maptip_fc6));
      model_.intvecmap_["fc7"].push_back(csv.GetInt(i, mdm_maptip_fc7));
      model_.intvecmap_["fp0"].push_back(csv.GetInt(i, mdm_maptip_fp0));
      model_.intvecmap_["fp1"].push_back(csv.GetInt(i, mdm_maptip_fp1));
      model_.intvecmap_["fp2"].push_back(csv.GetInt(i, mdm_maptip_fp2));
      model_.intvecmap_["fp3"].push_back(csv.GetInt(i, mdm_maptip_fp3));
      model_.intvecmap_["fp4"].push_back(csv.GetInt(i, mdm_maptip_fp4));
      model_.intvecmap_["fp5"].push_back(csv.GetInt(i, mdm_maptip_fp5));
      model_.intvecmap_["fp6"].push_back(csv.GetInt(i, mdm_maptip_fp6));
      model_.intvecmap_["fp7"].push_back(csv.GetInt(i, mdm_maptip_fp7));
      model_.intvecmap_["id_m_teamA"].push_back(csv.GetInt(i, mdm_maptip_id_m_teamA));
      model_.intvecmap_["catchA"].push_back(csv.GetInt(i, mdm_maptip_catchA));
      model_.intvecmap_["niceA"].push_back(csv.GetInt(i, mdm_maptip_niceA));
      model_.intvecmap_["id_m_drop_A"].push_back(csv.GetInt(i, mdm_maptip_id_m_drop_A));
      model_.intvecmap_["prob_sdA"].push_back(csv.GetInt(i, mdm_maptip_prob_sdA));
      model_.intvecmap_["itemType_sdA"].push_back(csv.GetInt(i, mdm_maptip_itemType_sdA));
      model_.intvecmap_["itemID_sdA"].push_back(csv.GetInt(i, mdm_maptip_itemID_sdA));
      model_.intvecmap_["itemNum_sdA"].push_back(csv.GetInt(i, mdm_maptip_itemNum_sdA));
      model_.intvecmap_["id_m_teamB"].push_back(csv.GetInt(i, mdm_maptip_id_m_teamB));
      model_.intvecmap_["catchB"].push_back(csv.GetInt(i, mdm_maptip_catchB));
      model_.intvecmap_["niceB"].push_back(csv.GetInt(i, mdm_maptip_niceB));
      model_.intvecmap_["id_m_drop_B"].push_back(csv.GetInt(i, mdm_maptip_id_m_drop_B));
      model_.intvecmap_["prob_sdB"].push_back(csv.GetInt(i, mdm_maptip_prob_sdB));
      model_.intvecmap_["itemType_sdB"].push_back(csv.GetInt(i, mdm_maptip_itemType_sdB));
      model_.intvecmap_["itemID_sdB"].push_back(csv.GetInt(i, mdm_maptip_itemID_sdB));
      model_.intvecmap_["itemNum_sdB"].push_back(csv.GetInt(i, mdm_maptip_itemNum_sdB));
      model_.intvecmap_["id_m_teamC"].push_back(csv.GetInt(i, mdm_maptip_id_m_teamC));
      model_.intvecmap_["catchC"].push_back(csv.GetInt(i, mdm_maptip_catchC));
      model_.intvecmap_["niceC"].push_back(csv.GetInt(i, mdm_maptip_niceC));
      model_.intvecmap_["id_m_drop_C"].push_back(csv.GetInt(i, mdm_maptip_id_m_drop_C));
      model_.intvecmap_["prob_sdC"].push_back(csv.GetInt(i, mdm_maptip_prob_sdC));
      model_.intvecmap_["itemType_sdC"].push_back(csv.GetInt(i, mdm_maptip_itemType_sdC));
      model_.intvecmap_["itemID_sdC"].push_back(csv.GetInt(i, mdm_maptip_itemID_sdC));
      model_.intvecmap_["itemNum_sdC"].push_back(csv.GetInt(i, mdm_maptip_itemNum_sdC));
      model_.strvecmap_["name_c64"].push_back(csv.GetStr(i, mdm_maptip_name_c64));
      model_.strvecmap_["text0_c128"].push_back(csv.GetStr(i, mdm_maptip_text0_c128));
      model_.strvecmap_["text1_c128"].push_back(csv.GetStr(i, mdm_maptip_text1_c128));
      model_.strvecmap_["text2_c128"].push_back(csv.GetStr(i, mdm_maptip_text2_c128));
      model_.strvecmap_["text3_c128"].push_back(csv.GetStr(i, mdm_maptip_text3_c128));
    }
    
  }
  s64 TMdm_Maptip::GetLineDt(s64 line, enmdm_Maptip dtNo)
  {
    return GenGetLineDt(line, M_MAPTIPCLM[dtNo].c_str());
  }
  BOOL TMdm_Maptip::GetLineDtBOOL(s64 line, enmdm_Maptip dtNo)
  {
    return GenGetLineDtBOOL(line, M_MAPTIPCLM[dtNo].c_str());
  }
  std::string TMdm_Maptip::GetLineDtStr(s64 line, enmdm_MaptipStr dtNo)
  {
    return GenGetLineDtStr(line, M_MAPTIPCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Maptip::GetPKDt(s64 pk, enmdm_Maptip dtNo)
  {
    return GenGetPKDt(pk, M_MAPTIPCLM[dtNo].c_str());
  }
  BOOL TMdm_Maptip::GetPKDtBOOL(s64 pk, enmdm_Maptip dtNo)
  {
    return GenGetPKDtBOOL(pk, M_MAPTIPCLM[dtNo].c_str());
  }
  std::string TMdm_Maptip::GetPKDtStr(s64 pk, enmdm_MaptipStr dtNo)
  {
    return GenGetPKDtStr(pk, M_MAPTIPCLMSTR[dtNo].c_str());
  }
}
