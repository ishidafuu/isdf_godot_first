#include "dbMdm_Tips.h"
namespace db
{
  TMdm_Tips::TMdm_Tips():TModelGen("m_Tips")
  {
  }
  TMdm_Tips::~TMdm_Tips(void)
  {
  }
  void TMdm_Tips::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Tips", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_tips_pk));
      model_.intvecmap_["id_m_timer"].push_back(csv.GetInt(i, mdm_tips_id_m_timer));
      model_.intvecmap_["osType"].push_back(csv.GetInt(i, mdm_tips_osType));
      model_.intvecmap_["avtHair"].push_back(csv.GetInt(i, mdm_tips_avtHair));
      model_.intvecmap_["avtFace"].push_back(csv.GetInt(i, mdm_tips_avtFace));
      model_.intvecmap_["avtHairCol"].push_back(csv.GetInt(i, mdm_tips_avtHairCol));
      model_.intvecmap_["avtSkinCol"].push_back(csv.GetInt(i, mdm_tips_avtSkinCol));
      model_.intvecmap_["avtEyeCol"].push_back(csv.GetInt(i, mdm_tips_avtEyeCol));
      model_.intvecmap_["avtAccCol"].push_back(csv.GetInt(i, mdm_tips_avtAccCol));
      model_.intvecmap_["avtClotheCol"].push_back(csv.GetInt(i, mdm_tips_avtClotheCol));
      model_.intvecmap_["avtShirCol"].push_back(csv.GetInt(i, mdm_tips_avtShirCol));
      model_.intvecmap_["avtPantCol"].push_back(csv.GetInt(i, mdm_tips_avtPantCol));
      model_.intvecmap_["avtSideLineCol"].push_back(csv.GetInt(i, mdm_tips_avtSideLineCol));
      model_.intvecmap_["avtUnderLineCol"].push_back(csv.GetInt(i, mdm_tips_avtUnderLineCol));
      model_.intvecmap_["avtShoeCol"].push_back(csv.GetInt(i, mdm_tips_avtShoeCol));
      model_.intvecmap_["avtItem"].push_back(csv.GetInt(i, mdm_tips_avtItem));
      model_.intvecmap_["gb_f"].push_back(csv.GetInt(i, mdm_tips_gb_f));
      model_.intvecmap_["personType"].push_back(csv.GetInt(i, mdm_tips_personType));
      model_.intvecmap_["moveType"].push_back(csv.GetInt(i, mdm_tips_moveType));
      model_.intvecmap_["speedType"].push_back(csv.GetInt(i, mdm_tips_speedType));
      model_.intvecmap_["selectPercent"].push_back(csv.GetInt(i, mdm_tips_selectPercent));
      model_.strvecmap_["name_c32"].push_back(csv.GetStr(i, mdm_tips_name_c32));
      model_.strvecmap_["text0_c128"].push_back(csv.GetStr(i, mdm_tips_text0_c128));
      model_.strvecmap_["text1_c128"].push_back(csv.GetStr(i, mdm_tips_text1_c128));
      model_.strvecmap_["text2_c128"].push_back(csv.GetStr(i, mdm_tips_text2_c128));
      model_.strvecmap_["text3_c128"].push_back(csv.GetStr(i, mdm_tips_text3_c128));
      model_.strvecmap_["text4_c128"].push_back(csv.GetStr(i, mdm_tips_text4_c128));
      model_.strvecmap_["text5_c128"].push_back(csv.GetStr(i, mdm_tips_text5_c128));
    }
    
  }
  s64 TMdm_Tips::GetLineDt(s64 line, enmdm_Tips dtNo)
  {
    return GenGetLineDt(line, M_TIPSCLM[dtNo].c_str());
  }
  BOOL TMdm_Tips::GetLineDtBOOL(s64 line, enmdm_Tips dtNo)
  {
    return GenGetLineDtBOOL(line, M_TIPSCLM[dtNo].c_str());
  }
  std::string TMdm_Tips::GetLineDtStr(s64 line, enmdm_TipsStr dtNo)
  {
    return GenGetLineDtStr(line, M_TIPSCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Tips::GetPKDt(s64 pk, enmdm_Tips dtNo)
  {
    return GenGetPKDt(pk, M_TIPSCLM[dtNo].c_str());
  }
  BOOL TMdm_Tips::GetPKDtBOOL(s64 pk, enmdm_Tips dtNo)
  {
    return GenGetPKDtBOOL(pk, M_TIPSCLM[dtNo].c_str());
  }
  std::string TMdm_Tips::GetPKDtStr(s64 pk, enmdm_TipsStr dtNo)
  {
    return GenGetPKDtStr(pk, M_TIPSCLMSTR[dtNo].c_str());
  }
}
