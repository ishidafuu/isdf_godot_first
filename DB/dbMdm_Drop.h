#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Drop
  {
    mdm_drop_pk,//マスタードロップＩＤ
    mdm_drop_itemType_0,//獲得アイテムタイプ
    mdm_drop_itemID_0,//獲得アイテムＩＤ
    mdm_drop_itemNum_0,//獲得アイテム個数
    mdm_drop_itemType_1,//獲得アイテムタイプ
    mdm_drop_itemID_1,//獲得アイテムＩＤ
    mdm_drop_itemNum_1,//獲得アイテム個数
    mdm_drop_itemType_2,//獲得アイテムタイプ
    mdm_drop_itemID_2,//獲得アイテムＩＤ
    mdm_drop_itemNum_2,//獲得アイテム個数
    mdm_drop_itemType_Cmp,//コンプアイテムタイプ
    mdm_drop_itemID_Cmp,//コンプアイテムＩＤ
    mdm_drop_itemNum_Cmp,//コンプアイテム個数
  };
  
  const std::string M_DROPCLM[] = 
  {
    "pk",//マスタードロップＩＤ
    "itemType_0",//獲得アイテムタイプ
    "itemID_0",//獲得アイテムＩＤ
    "itemNum_0",//獲得アイテム個数
    "itemType_1",//獲得アイテムタイプ
    "itemID_1",//獲得アイテムＩＤ
    "itemNum_1",//獲得アイテム個数
    "itemType_2",//獲得アイテムタイプ
    "itemID_2",//獲得アイテムＩＤ
    "itemNum_2",//獲得アイテム個数
    "itemType_Cmp",//コンプアイテムタイプ
    "itemID_Cmp",//コンプアイテムＩＤ
    "itemNum_Cmp",//コンプアイテム個数
  };
  
  class TMdm_Drop: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Drop();
    virtual ~TMdm_Drop();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Drop dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Drop dtNo);
    s64 GetPKDt(s64 pk, enmdm_Drop dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Drop dtNo);
  };
}
