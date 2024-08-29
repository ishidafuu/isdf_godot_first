#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Giftbox
  {
    mdm_giftbox_pk,//贈り物ボックスＩＤ
    mdm_giftbox_id_m_timer,//タイマー管理ＩＤ
    mdm_giftbox_itemType,//獲得アイテムtype
    mdm_giftbox_itemID,//獲得アイテムID
    mdm_giftbox_itemNum,//獲得個数
  };
  
  enum enmdm_GiftboxStr
  {
    mdm_giftbox_name_c64,//名称
    mdm_giftbox_text0_c64,//テキスト０
    mdm_giftbox_text1_c64,//テキスト１
    mdm_giftbox_text2_c64,//テキスト２
  };
  

  const std::string M_GIFTBOXCLM[] = 
  {
    "pk",//贈り物ボックスＩＤ
    "id_m_timer",//タイマー管理ＩＤ
    "itemType",//獲得アイテムtype
    "itemID",//獲得アイテムID
    "itemNum",//獲得個数
  };
  
  const std::string M_GIFTBOXCLMSTR[] = 
  {
    "name_c64",//名称
    "text0_c64",//テキスト０
    "text1_c64",//テキスト１
    "text2_c64",//テキスト２
  };
  
  class TMdm_Giftbox: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Giftbox();
    virtual ~TMdm_Giftbox();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Giftbox dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Giftbox dtNo);
    std::string GetLineDtStr(s64 line, enmdm_GiftboxStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Giftbox dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Giftbox dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_GiftboxStr dtNo);
  };
}
