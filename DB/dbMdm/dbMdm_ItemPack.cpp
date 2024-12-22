#include "dbMdm_ItemPack.h"
namespace db
{
  TMdm_ItemPack::TMdm_ItemPack():TModelGen("m_ItemPack")
  {
  }
  TMdm_ItemPack::~TMdm_ItemPack(void)
  {
  }
  void TMdm_ItemPack::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_ItemPack", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_itempack_pk));
      model_.intvecmap_["priceType"].push_back(csv.GetInt(i, mdm_itempack_priceType));
      model_.intvecmap_["price"].push_back(csv.GetInt(i, mdm_itempack_price));
      model_.intvecmap_["iconNo"].push_back(csv.GetInt(i, mdm_itempack_iconNo));
      model_.strvecmap_["name_c64"].push_back(csv.GetStr(i, mdm_itempack_name_c64));
      model_.strvecmap_["text0_c64"].push_back(csv.GetStr(i, mdm_itempack_text0_c64));
      model_.strvecmap_["text1_c64"].push_back(csv.GetStr(i, mdm_itempack_text1_c64));
      model_.strvecmap_["text2_c64"].push_back(csv.GetStr(i, mdm_itempack_text2_c64));
    }
    
  }
  s64 TMdm_ItemPack::GetLineDt(s64 line, enmdm_ItemPack dtNo)
  {
    return GenGetLineDt(line, M_ITEMPACKCLM[dtNo].c_str());
  }
  BOOL TMdm_ItemPack::GetLineDtBOOL(s64 line, enmdm_ItemPack dtNo)
  {
    return GenGetLineDtBOOL(line, M_ITEMPACKCLM[dtNo].c_str());
  }
  std::string TMdm_ItemPack::GetLineDtStr(s64 line, enmdm_ItemPackStr dtNo)
  {
    return GenGetLineDtStr(line, M_ITEMPACKCLMSTR[dtNo].c_str());
  }
  s64 TMdm_ItemPack::GetPKDt(s64 pk, enmdm_ItemPack dtNo)
  {
    return GenGetPKDt(pk, M_ITEMPACKCLM[dtNo].c_str());
  }
  BOOL TMdm_ItemPack::GetPKDtBOOL(s64 pk, enmdm_ItemPack dtNo)
  {
    return GenGetPKDtBOOL(pk, M_ITEMPACKCLM[dtNo].c_str());
  }
  std::string TMdm_ItemPack::GetPKDtStr(s64 pk, enmdm_ItemPackStr dtNo)
  {
    return GenGetPKDtStr(pk, M_ITEMPACKCLMSTR[dtNo].c_str());
  }
}
