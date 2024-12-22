#include "dbMdm_BBABuy.h"
namespace db
{
  TMdm_BBABuy::TMdm_BBABuy():TModelGen("m_BBABuy")
  {
  }
  TMdm_BBABuy::~TMdm_BBABuy(void)
  {
  }
  void TMdm_BBABuy::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_BBABuy", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_bbabuy_pk));
      model_.intvecmap_["shopType"].push_back(csv.GetInt(i, mdm_bbabuy_shopType));
      model_.intvecmap_["priceType"].push_back(csv.GetInt(i, mdm_bbabuy_priceType));
      model_.intvecmap_["price"].push_back(csv.GetInt(i, mdm_bbabuy_price));
      model_.strvecmap_["text_c64"].push_back(csv.GetStr(i, mdm_bbabuy_text_c64));
    }
    
  }
  s64 TMdm_BBABuy::GetLineDt(s64 line, enmdm_BBABuy dtNo)
  {
    return GenGetLineDt(line, M_BBABUYCLM[dtNo].c_str());
  }
  BOOL TMdm_BBABuy::GetLineDtBOOL(s64 line, enmdm_BBABuy dtNo)
  {
    return GenGetLineDtBOOL(line, M_BBABUYCLM[dtNo].c_str());
  }
  std::string TMdm_BBABuy::GetLineDtStr(s64 line, enmdm_BBABuyStr dtNo)
  {
    return GenGetLineDtStr(line, M_BBABUYCLMSTR[dtNo].c_str());
  }
  s64 TMdm_BBABuy::GetPKDt(s64 pk, enmdm_BBABuy dtNo)
  {
    return GenGetPKDt(pk, M_BBABUYCLM[dtNo].c_str());
  }
  BOOL TMdm_BBABuy::GetPKDtBOOL(s64 pk, enmdm_BBABuy dtNo)
  {
    return GenGetPKDtBOOL(pk, M_BBABUYCLM[dtNo].c_str());
  }
  std::string TMdm_BBABuy::GetPKDtStr(s64 pk, enmdm_BBABuyStr dtNo)
  {
    return GenGetPKDtStr(pk, M_BBABUYCLMSTR[dtNo].c_str());
  }
}
