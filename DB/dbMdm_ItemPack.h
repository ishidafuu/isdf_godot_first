#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_ItemPack
  {
    mdm_itempack_pk,//アイテムパックＩＤ
    mdm_itempack_priceType,//必要通貨
    mdm_itempack_price,//値段MC/STAR
    mdm_itempack_iconNo,//アイコン番号
  };
  
  enum enmdm_ItemPackStr
  {
    mdm_itempack_name_c64,//アイテムパックの名前
    mdm_itempack_text0_c64,//テキスト０
    mdm_itempack_text1_c64,//テキスト１
    mdm_itempack_text2_c64,//テキスト２
  };
  

  const std::string M_ITEMPACKCLM[] = 
  {
    "pk",//アイテムパックＩＤ
    "priceType",//必要通貨
    "price",//値段MC/STAR
    "iconNo",//アイコン番号
  };
  
  const std::string M_ITEMPACKCLMSTR[] = 
  {
    "name_c64",//アイテムパックの名前
    "text0_c64",//テキスト０
    "text1_c64",//テキスト１
    "text2_c64",//テキスト２
  };
  
  class TMdm_ItemPack: public TModelGen
  {
  protected:

  public:
    explicit TMdm_ItemPack();
    virtual ~TMdm_ItemPack();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_ItemPack dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_ItemPack dtNo);
    std::string GetLineDtStr(s64 line, enmdm_ItemPackStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_ItemPack dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_ItemPack dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_ItemPackStr dtNo);
  };
}
