#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_TokkouItem
  {
    mdm_tokkouitem_pk,//マスター特効ＩＤ
    mdm_tokkouitem_tokkouNo,//特効グループ番号
    mdm_tokkouitem_itemType,//アイテムタイプ
    mdm_tokkouitem_itemID,//アイテムＩＤ
    mdm_tokkouitem_effectType,//効果タイプ
  };
  
  const std::string M_TOKKOUITEMCLM[] = 
  {
    "pk",//マスター特効ＩＤ
    "tokkouNo",//特効グループ番号
    "itemType",//アイテムタイプ
    "itemID",//アイテムＩＤ
    "effectType",//効果タイプ
  };
  
  class TMdm_TokkouItem: public TModelGen
  {
  protected:

  public:
    explicit TMdm_TokkouItem();
    virtual ~TMdm_TokkouItem();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_TokkouItem dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_TokkouItem dtNo);
    s64 GetPKDt(s64 pk, enmdm_TokkouItem dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_TokkouItem dtNo);
  };
}
