#include "dbMdm_NaBox.h"
namespace db
{
  TMdm_NaBox::TMdm_NaBox():TModelGen("m_NaBox")
  {
  }
  TMdm_NaBox::~TMdm_NaBox(void)
  {
  }
  void TMdm_NaBox::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_NaBox", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_nabox_pk));
      model_.intvecmap_["id_m_timer"].push_back(csv.GetInt(i, mdm_nabox_id_m_timer));
      model_.intvecmap_["id_m_supply_ken"].push_back(csv.GetInt(i, mdm_nabox_id_m_supply_ken));
      model_.intvecmap_["kenNum"].push_back(csv.GetInt(i, mdm_nabox_kenNum));
      model_.intvecmap_["boxSize"].push_back(csv.GetInt(i, mdm_nabox_boxSize));
      model_.intvecmap_["iconNo"].push_back(csv.GetInt(i, mdm_nabox_iconNo));
      model_.strvecmap_["name_c64"].push_back(csv.GetStr(i, mdm_nabox_name_c64));
      model_.strvecmap_["text0_c64"].push_back(csv.GetStr(i, mdm_nabox_text0_c64));
      model_.strvecmap_["text1_c64"].push_back(csv.GetStr(i, mdm_nabox_text1_c64));
      model_.strvecmap_["text2_c64"].push_back(csv.GetStr(i, mdm_nabox_text2_c64));
      model_.strvecmap_["text3_c64"].push_back(csv.GetStr(i, mdm_nabox_text3_c64));
    }
    
  }
  s64 TMdm_NaBox::GetLineDt(s64 line, enmdm_NaBox dtNo)
  {
    return GenGetLineDt(line, M_NABOXCLM[dtNo].c_str());
  }
  BOOL TMdm_NaBox::GetLineDtBOOL(s64 line, enmdm_NaBox dtNo)
  {
    return GenGetLineDtBOOL(line, M_NABOXCLM[dtNo].c_str());
  }
  std::string TMdm_NaBox::GetLineDtStr(s64 line, enmdm_NaBoxStr dtNo)
  {
    return GenGetLineDtStr(line, M_NABOXCLMSTR[dtNo].c_str());
  }
  s64 TMdm_NaBox::GetPKDt(s64 pk, enmdm_NaBox dtNo)
  {
    return GenGetPKDt(pk, M_NABOXCLM[dtNo].c_str());
  }
  BOOL TMdm_NaBox::GetPKDtBOOL(s64 pk, enmdm_NaBox dtNo)
  {
    return GenGetPKDtBOOL(pk, M_NABOXCLM[dtNo].c_str());
  }
  std::string TMdm_NaBox::GetPKDtStr(s64 pk, enmdm_NaBoxStr dtNo)
  {
    return GenGetPKDtStr(pk, M_NABOXCLMSTR[dtNo].c_str());
  }
}
