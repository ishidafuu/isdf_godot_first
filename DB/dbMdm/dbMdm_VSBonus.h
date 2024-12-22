#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_VSBonus
  {
    mdm_vsbonus_pk,//対戦ボーナスＩＤ
    mdm_vsbonus_bonusNum,//もらえる回数
    mdm_vsbonus_itemType,//アイテムタイプ
    mdm_vsbonus_itemID,//アイテムＩＤ
    mdm_vsbonus_itemNum,//アイテム個数
  };
  
  enum enmdm_VSBonusStr
  {
    mdm_vsbonus_msg_c128,//表示メッセージ
    mdm_vsbonus_endmsg_c128,//すべて獲得後メッセージ
  };
  

  const std::string M_VSBONUSCLM[] = 
  {
    "pk",//対戦ボーナスＩＤ
    "bonusNum",//もらえる回数
    "itemType",//アイテムタイプ
    "itemID",//アイテムＩＤ
    "itemNum",//アイテム個数
  };
  
  const std::string M_VSBONUSCLMSTR[] = 
  {
    "msg_c128",//表示メッセージ
    "endmsg_c128",//すべて獲得後メッセージ
  };
  
  class TMdm_VSBonus: public TModelGen
  {
  protected:

  public:
    explicit TMdm_VSBonus();
    virtual ~TMdm_VSBonus();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_VSBonus dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_VSBonus dtNo);
    std::string GetLineDtStr(s64 line, enmdm_VSBonusStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_VSBonus dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_VSBonus dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_VSBonusStr dtNo);
  };
}
