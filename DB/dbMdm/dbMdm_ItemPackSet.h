#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_ItemPackSet
  {
    mdm_itempackset_pk,//マスターアイテムパックセットＩＤ
    mdm_itempackset_id_m_itempack,//マスターアイテムパックＩＤ
    mdm_itempackset_itemType,//アイテムタイプ
    mdm_itempackset_itemID,//アイテムＩＤ
    mdm_itempackset_itemNum,//アイテム個数
  };
  
  const std::string M_ITEMPACKSETCLM[] = 
  {
    "pk",//マスターアイテムパックセットＩＤ
    "id_m_itempack",//マスターアイテムパックＩＤ
    "itemType",//アイテムタイプ
    "itemID",//アイテムＩＤ
    "itemNum",//アイテム個数
  };
  
  class TMdm_ItemPackSet: public TModelGen
  {
  protected:

  public:
    explicit TMdm_ItemPackSet();
    virtual ~TMdm_ItemPackSet();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_ItemPackSet dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_ItemPackSet dtNo);
    s64 GetPKDt(s64 pk, enmdm_ItemPackSet dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_ItemPackSet dtNo);
  };
}
