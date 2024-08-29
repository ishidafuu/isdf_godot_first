#include "dbMdm_Giftbox.h"
namespace db
{
  TMdm_Giftbox::TMdm_Giftbox():TModelGen("m_Giftbox")
  {
  }
  TMdm_Giftbox::~TMdm_Giftbox(void)
  {
  }
  void TMdm_Giftbox::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Giftbox", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_giftbox_pk));
      model_.intvecmap_["id_m_timer"].push_back(csv.GetInt(i, mdm_giftbox_id_m_timer));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_giftbox_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_giftbox_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_giftbox_itemNum));
      model_.strvecmap_["name_c64"].push_back(csv.GetStr(i, mdm_giftbox_name_c64));
      model_.strvecmap_["text0_c64"].push_back(csv.GetStr(i, mdm_giftbox_text0_c64));
      model_.strvecmap_["text1_c64"].push_back(csv.GetStr(i, mdm_giftbox_text1_c64));
      model_.strvecmap_["text2_c64"].push_back(csv.GetStr(i, mdm_giftbox_text2_c64));
    }
    
  }
  s64 TMdm_Giftbox::GetLineDt(s64 line, enmdm_Giftbox dtNo)
  {
    return GenGetLineDt(line, M_GIFTBOXCLM[dtNo].c_str());
  }
  BOOL TMdm_Giftbox::GetLineDtBOOL(s64 line, enmdm_Giftbox dtNo)
  {
    return GenGetLineDtBOOL(line, M_GIFTBOXCLM[dtNo].c_str());
  }
  std::string TMdm_Giftbox::GetLineDtStr(s64 line, enmdm_GiftboxStr dtNo)
  {
    return GenGetLineDtStr(line, M_GIFTBOXCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Giftbox::GetPKDt(s64 pk, enmdm_Giftbox dtNo)
  {
    return GenGetPKDt(pk, M_GIFTBOXCLM[dtNo].c_str());
  }
  BOOL TMdm_Giftbox::GetPKDtBOOL(s64 pk, enmdm_Giftbox dtNo)
  {
    return GenGetPKDtBOOL(pk, M_GIFTBOXCLM[dtNo].c_str());
  }
  std::string TMdm_Giftbox::GetPKDtStr(s64 pk, enmdm_GiftboxStr dtNo)
  {
    return GenGetPKDtStr(pk, M_GIFTBOXCLMSTR[dtNo].c_str());
  }
}
