#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Campaign
  {
    mdm_campaign_pk,//キャンペーンＩＤ
    mdm_campaign_id_m_timer,//タイム管理ＩＤ
    mdm_campaign_itemType,//アイテム種類
    mdm_campaign_itemID,//アイテムID
    mdm_campaign_itemNum,//アイテム個数
    mdm_campaign_inputType,//入力形式
    mdm_campaign_inputLen,//入力文字数
    mdm_campaign_commonCode_f,//全員共通コードフラグ
    mdm_campaign_multiGet_f,//複数取得可能フラグ
  };
  
  enum enmdm_CampaignStr
  {
    mdm_campaign_name_c64,//キャンペーンの名前
  };
  

  const std::string M_CAMPAIGNCLM[] = 
  {
    "pk",//キャンペーンＩＤ
    "id_m_timer",//タイム管理ＩＤ
    "itemType",//アイテム種類
    "itemID",//アイテムID
    "itemNum",//アイテム個数
    "inputType",//入力形式
    "inputLen",//入力文字数
    "commonCode_f",//全員共通コードフラグ
    "multiGet_f",//複数取得可能フラグ
  };
  
  const std::string M_CAMPAIGNCLMSTR[] = 
  {
    "name_c64",//キャンペーンの名前
  };
  
  class TMdm_Campaign: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Campaign();
    virtual ~TMdm_Campaign();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Campaign dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Campaign dtNo);
    std::string GetLineDtStr(s64 line, enmdm_CampaignStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Campaign dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Campaign dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_CampaignStr dtNo);
  };
}
