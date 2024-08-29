#include "dbMdm_Gacha.h"
namespace db
{
  TMdm_Gacha::TMdm_Gacha():TModelGen("m_Gacha")
  {
  }
  TMdm_Gacha::~TMdm_Gacha(void)
  {
  }
  void TMdm_Gacha::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Gacha", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_gacha_pk));
      model_.intvecmap_["id_m_timer"].push_back(csv.GetInt(i, mdm_gacha_id_m_timer));
      model_.intvecmap_["id_m_supply_ken"].push_back(csv.GetInt(i, mdm_gacha_id_m_supply_ken));
      model_.intvecmap_["kenNum"].push_back(csv.GetInt(i, mdm_gacha_kenNum));
      model_.intvecmap_["lotNumA"].push_back(csv.GetInt(i, mdm_gacha_lotNumA));
      model_.intvecmap_["lotGrpNoA"].push_back(csv.GetInt(i, mdm_gacha_lotGrpNoA));
      model_.intvecmap_["lotGradeA"].push_back(csv.GetInt(i, mdm_gacha_lotGradeA));
      model_.intvecmap_["lotNumB"].push_back(csv.GetInt(i, mdm_gacha_lotNumB));
      model_.intvecmap_["lotGrpNoB"].push_back(csv.GetInt(i, mdm_gacha_lotGrpNoB));
      model_.intvecmap_["lotGradeB"].push_back(csv.GetInt(i, mdm_gacha_lotGradeB));
      model_.intvecmap_["box_f"].push_back(csv.GetInt(i, mdm_gacha_box_f));
      model_.intvecmap_["bonus_f"].push_back(csv.GetInt(i, mdm_gacha_bonus_f));
      model_.intvecmap_["bonusNumA"].push_back(csv.GetInt(i, mdm_gacha_bonusNumA));
      model_.intvecmap_["bonusGrpA"].push_back(csv.GetInt(i, mdm_gacha_bonusGrpA));
      model_.intvecmap_["bonusGradeA"].push_back(csv.GetInt(i, mdm_gacha_bonusGradeA));
      model_.intvecmap_["bonusNumB"].push_back(csv.GetInt(i, mdm_gacha_bonusNumB));
      model_.intvecmap_["bonusGrpB"].push_back(csv.GetInt(i, mdm_gacha_bonusGrpB));
      model_.intvecmap_["bonusGradeB"].push_back(csv.GetInt(i, mdm_gacha_bonusGradeB));
      model_.intvecmap_["step_f"].push_back(csv.GetInt(i, mdm_gacha_step_f));
      model_.intvecmap_["id_m_Gacha_stepNext"].push_back(csv.GetInt(i, mdm_gacha_id_m_Gacha_stepNext));
      model_.intvecmap_["id_m_Gacha_stepReset"].push_back(csv.GetInt(i, mdm_gacha_id_m_Gacha_stepReset));
      model_.intvecmap_["stepLvUp_f"].push_back(csv.GetInt(i, mdm_gacha_stepLvUp_f));
      model_.intvecmap_["id_m_gacha_stepLvUp"].push_back(csv.GetInt(i, mdm_gacha_id_m_gacha_stepLvUp));
      model_.intvecmap_["point_f"].push_back(csv.GetInt(i, mdm_gacha_point_f));
      model_.intvecmap_["id_m_gacha_point"].push_back(csv.GetInt(i, mdm_gacha_id_m_gacha_point));
      model_.intvecmap_["omake_f"].push_back(csv.GetInt(i, mdm_gacha_omake_f));
      model_.intvecmap_["id_m_gacha_omake"].push_back(csv.GetInt(i, mdm_gacha_id_m_gacha_omake));
      model_.intvecmap_["retry_f"].push_back(csv.GetInt(i, mdm_gacha_retry_f));
      model_.intvecmap_["iconNo"].push_back(csv.GetInt(i, mdm_gacha_iconNo));
      model_.strvecmap_["name_c64"].push_back(csv.GetStr(i, mdm_gacha_name_c64));
      model_.strvecmap_["text0_c64"].push_back(csv.GetStr(i, mdm_gacha_text0_c64));
      model_.strvecmap_["text1_c64"].push_back(csv.GetStr(i, mdm_gacha_text1_c64));
      model_.strvecmap_["text2_c64"].push_back(csv.GetStr(i, mdm_gacha_text2_c64));
      model_.strvecmap_["text3_c64"].push_back(csv.GetStr(i, mdm_gacha_text3_c64));
    }
    
  }
  s64 TMdm_Gacha::GetLineDt(s64 line, enmdm_Gacha dtNo)
  {
    return GenGetLineDt(line, M_GACHACLM[dtNo].c_str());
  }
  BOOL TMdm_Gacha::GetLineDtBOOL(s64 line, enmdm_Gacha dtNo)
  {
    return GenGetLineDtBOOL(line, M_GACHACLM[dtNo].c_str());
  }
  std::string TMdm_Gacha::GetLineDtStr(s64 line, enmdm_GachaStr dtNo)
  {
    return GenGetLineDtStr(line, M_GACHACLMSTR[dtNo].c_str());
  }
  s64 TMdm_Gacha::GetPKDt(s64 pk, enmdm_Gacha dtNo)
  {
    return GenGetPKDt(pk, M_GACHACLM[dtNo].c_str());
  }
  BOOL TMdm_Gacha::GetPKDtBOOL(s64 pk, enmdm_Gacha dtNo)
  {
    return GenGetPKDtBOOL(pk, M_GACHACLM[dtNo].c_str());
  }
  std::string TMdm_Gacha::GetPKDtStr(s64 pk, enmdm_GachaStr dtNo)
  {
    return GenGetPKDtStr(pk, M_GACHACLMSTR[dtNo].c_str());
  }
}
