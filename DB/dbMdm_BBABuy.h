#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_BBABuy
  {
    mdm_bbabuy_pk,//ばばあ販売メニューＩＤ
    mdm_bbabuy_shopType,//ショップの種類
    mdm_bbabuy_priceType,//金額タイプスター１トッド2
    mdm_bbabuy_price,//価格
  };
  
  enum enmdm_BBABuyStr
  {
    mdm_bbabuy_text_c64,//テキスト
  };
  

  const std::string M_BBABUYCLM[] = 
  {
    "pk",//ばばあ販売メニューＩＤ
    "shopType",//ショップの種類
    "priceType",//金額タイプスター１トッド2
    "price",//価格
  };
  
  const std::string M_BBABUYCLMSTR[] = 
  {
    "text_c64",//テキスト
  };
  
  class TMdm_BBABuy: public TModelGen
  {
  protected:

  public:
    explicit TMdm_BBABuy();
    virtual ~TMdm_BBABuy();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_BBABuy dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_BBABuy dtNo);
    std::string GetLineDtStr(s64 line, enmdm_BBABuyStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_BBABuy dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_BBABuy dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_BBABuyStr dtNo);
  };
}
