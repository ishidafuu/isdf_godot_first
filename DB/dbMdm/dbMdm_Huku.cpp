#include "dbMdm_Huku.h"
namespace db
{
  TMdm_Huku::TMdm_Huku():TModelGen("m_Huku")
  {
  }
  TMdm_Huku::~TMdm_Huku(void)
  {
  }
  void TMdm_Huku::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Huku", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_huku_pk));
      model_.intvecmap_["id_w_hukugrp"].push_back(csv.GetInt(i, mdm_huku_id_w_hukugrp));
      model_.intvecmap_["id_m_timer"].push_back(csv.GetInt(i, mdm_huku_id_m_timer));
      model_.intvecmap_["kenNum"].push_back(csv.GetInt(i, mdm_huku_kenNum));
      model_.intvecmap_["priority"].push_back(csv.GetInt(i, mdm_huku_priority));
      model_.intvecmap_["lotNumA"].push_back(csv.GetInt(i, mdm_huku_lotNumA));
      model_.intvecmap_["lotGradeA"].push_back(csv.GetInt(i, mdm_huku_lotGradeA));
      model_.intvecmap_["lotNumB"].push_back(csv.GetInt(i, mdm_huku_lotNumB));
      model_.intvecmap_["lotGradeB"].push_back(csv.GetInt(i, mdm_huku_lotGradeB));
      model_.intvecmap_["iconNo"].push_back(csv.GetInt(i, mdm_huku_iconNo));
      model_.strvecmap_["name_c64"].push_back(csv.GetStr(i, mdm_huku_name_c64));
      model_.strvecmap_["text0_c64"].push_back(csv.GetStr(i, mdm_huku_text0_c64));
      model_.strvecmap_["text1_c64"].push_back(csv.GetStr(i, mdm_huku_text1_c64));
      model_.strvecmap_["text2_c64"].push_back(csv.GetStr(i, mdm_huku_text2_c64));
      model_.strvecmap_["text3_c64"].push_back(csv.GetStr(i, mdm_huku_text3_c64));
    }
    
  }
  s64 TMdm_Huku::GetLineDt(s64 line, enmdm_Huku dtNo)
  {
    return GenGetLineDt(line, M_HUKUCLM[dtNo].c_str());
  }
  BOOL TMdm_Huku::GetLineDtBOOL(s64 line, enmdm_Huku dtNo)
  {
    return GenGetLineDtBOOL(line, M_HUKUCLM[dtNo].c_str());
  }
  std::string TMdm_Huku::GetLineDtStr(s64 line, enmdm_HukuStr dtNo)
  {
    return GenGetLineDtStr(line, M_HUKUCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Huku::GetPKDt(s64 pk, enmdm_Huku dtNo)
  {
    return GenGetPKDt(pk, M_HUKUCLM[dtNo].c_str());
  }
  BOOL TMdm_Huku::GetPKDtBOOL(s64 pk, enmdm_Huku dtNo)
  {
    return GenGetPKDtBOOL(pk, M_HUKUCLM[dtNo].c_str());
  }
  std::string TMdm_Huku::GetPKDtStr(s64 pk, enmdm_HukuStr dtNo)
  {
    return GenGetPKDtStr(pk, M_HUKUCLMSTR[dtNo].c_str());
  }
}
