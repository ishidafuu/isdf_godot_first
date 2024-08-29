#include "dbMdm_Supply.h"
namespace db
{
  TMdm_Supply::TMdm_Supply():TModelGen("m_Supply")
  {
  }
  TMdm_Supply::~TMdm_Supply(void)
  {
  }
  void TMdm_Supply::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Supply", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_supply_pk));
      model_.intvecmap_["priceType"].push_back(csv.GetInt(i, mdm_supply_priceType));
      model_.intvecmap_["price"].push_back(csv.GetInt(i, mdm_supply_price));
      model_.intvecmap_["effect"].push_back(csv.GetInt(i, mdm_supply_effect));
      model_.intvecmap_["sell_f"].push_back(csv.GetInt(i, mdm_supply_sell_f));
      model_.intvecmap_["gift_f"].push_back(csv.GetInt(i, mdm_supply_gift_f));
      model_.intvecmap_["iconNo"].push_back(csv.GetInt(i, mdm_supply_iconNo));
      model_.strvecmap_["name_c32"].push_back(csv.GetStr(i, mdm_supply_name_c32));
      model_.strvecmap_["text0_c64"].push_back(csv.GetStr(i, mdm_supply_text0_c64));
      model_.strvecmap_["text1_c64"].push_back(csv.GetStr(i, mdm_supply_text1_c64));
      model_.strvecmap_["text2_c64"].push_back(csv.GetStr(i, mdm_supply_text2_c64));
    }
    
  }
  s64 TMdm_Supply::GetLineDt(s64 line, enmdm_Supply dtNo)
  {
    return GenGetLineDt(line, M_SUPPLYCLM[dtNo].c_str());
  }
  BOOL TMdm_Supply::GetLineDtBOOL(s64 line, enmdm_Supply dtNo)
  {
    return GenGetLineDtBOOL(line, M_SUPPLYCLM[dtNo].c_str());
  }
  std::string TMdm_Supply::GetLineDtStr(s64 line, enmdm_SupplyStr dtNo)
  {
    return GenGetLineDtStr(line, M_SUPPLYCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Supply::GetPKDt(s64 pk, enmdm_Supply dtNo)
  {
    return GenGetPKDt(pk, M_SUPPLYCLM[dtNo].c_str());
  }
  BOOL TMdm_Supply::GetPKDtBOOL(s64 pk, enmdm_Supply dtNo)
  {
    return GenGetPKDtBOOL(pk, M_SUPPLYCLM[dtNo].c_str());
  }
  std::string TMdm_Supply::GetPKDtStr(s64 pk, enmdm_SupplyStr dtNo)
  {
    return GenGetPKDtStr(pk, M_SUPPLYCLMSTR[dtNo].c_str());
  }
}
