#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_SpSyouhin
  {
    mdm_spsyouhin_pk,//ショップメニューＩＤ
    mdm_spsyouhin_shopType,//ショップの種類
    mdm_spsyouhin_itemType,//アイテムタイプ
    mdm_spsyouhin_itemID,//アイテムＩＤ
    mdm_spsyouhin_itemNum,//アイテム個数
  };
  
  const std::string M_SPSYOUHINCLM[] = 
  {
    "pk",//ショップメニューＩＤ
    "shopType",//ショップの種類
    "itemType",//アイテムタイプ
    "itemID",//アイテムＩＤ
    "itemNum",//アイテム個数
  };
  
  class TMdm_SpSyouhin: public TModelGen
  {
  protected:

  public:
    explicit TMdm_SpSyouhin();
    virtual ~TMdm_SpSyouhin();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_SpSyouhin dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_SpSyouhin dtNo);
    s64 GetPKDt(s64 pk, enmdm_SpSyouhin dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_SpSyouhin dtNo);
  };
}
