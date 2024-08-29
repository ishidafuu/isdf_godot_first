#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_TimeAttack
  {
    mdm_timeattack_pk,//タイムアタックＩＤ
    mdm_timeattack_id_m_event,//イベントＩＤ
    mdm_timeattack_id_m_team,//敵チームＩＤ
    mdm_timeattack_id_m_haikei,//背景ＩＤ
    mdm_timeattack_id_m_bgm,//ＢＧＭＩＤ
    mdm_timeattack_shiaiSec,//試合時間
    mdm_timeattack_catch,//キャッチ率（百分率）
    mdm_timeattack_nice,//ナイスキャッチ率（百分率）
    mdm_timeattack_itemType,//獲得アイテムtype
    mdm_timeattack_itemID,//獲得アイテムID
    mdm_timeattack_itemNum,//獲得個数
  };
  
  const std::string M_TIMEATTACKCLM[] = 
  {
    "pk",//タイムアタックＩＤ
    "id_m_event",//イベントＩＤ
    "id_m_team",//敵チームＩＤ
    "id_m_haikei",//背景ＩＤ
    "id_m_bgm",//ＢＧＭＩＤ
    "shiaiSec",//試合時間
    "catch",//キャッチ率（百分率）
    "nice",//ナイスキャッチ率（百分率）
    "itemType",//獲得アイテムtype
    "itemID",//獲得アイテムID
    "itemNum",//獲得個数
  };
  
  class TMdm_TimeAttack: public TModelGen
  {
  protected:

  public:
    explicit TMdm_TimeAttack();
    virtual ~TMdm_TimeAttack();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_TimeAttack dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_TimeAttack dtNo);
    s64 GetPKDt(s64 pk, enmdm_TimeAttack dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_TimeAttack dtNo);
  };
}
