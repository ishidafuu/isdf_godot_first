#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Supply
  {
    mdm_supply_pk,//マスター消耗品ＩＤ
    mdm_supply_priceType,//必要通貨
    mdm_supply_price,//値段MC/STAR
    mdm_supply_effect,//効果値
    mdm_supply_sell_f,//販売フラグ
    mdm_supply_gift_f,//贈答可能フラグ
    mdm_supply_iconNo,//アイコン番号
  };
  
  enum enmdm_SupplyStr
  {
    mdm_supply_name_c32,//名目
    mdm_supply_text0_c64,//テキスト０
    mdm_supply_text1_c64,//テキスト１
    mdm_supply_text2_c64,//テキスト２
  };
  

  const std::string M_SUPPLYCLM[] = 
  {
    "pk",//マスター消耗品ＩＤ
    "priceType",//必要通貨
    "price",//値段MC/STAR
    "effect",//効果値
    "sell_f",//販売フラグ
    "gift_f",//贈答可能フラグ
    "iconNo",//アイコン番号
  };
  
  const std::string M_SUPPLYCLMSTR[] = 
  {
    "name_c32",//名目
    "text0_c64",//テキスト０
    "text1_c64",//テキスト１
    "text2_c64",//テキスト２
  };
  
  class TMdm_Supply: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Supply();
    virtual ~TMdm_Supply();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Supply dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Supply dtNo);
    std::string GetLineDtStr(s64 line, enmdm_SupplyStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Supply dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Supply dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_SupplyStr dtNo);
  };
}
