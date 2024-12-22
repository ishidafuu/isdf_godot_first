#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_RkReward
  {
    mdm_rkreward_pk,//ランキング報酬ＩＤ
    mdm_rkreward_id_m_ranking,//ランキングＩＤ
    mdm_rkreward_grade,//等級
    mdm_rkreward_top,//順位上限
    mdm_rkreward_bottom,//順位下限
    mdm_rkreward_itemType,//獲得アイテムtype
    mdm_rkreward_itemID,//獲得アイテムID
    mdm_rkreward_itemNum,//獲得個数
  };
  
  enum enmdm_RkRewardStr
  {
    mdm_rkreward_comment_c512,//使用用途など
  };
  

  const std::string M_RKREWARDCLM[] = 
  {
    "pk",//ランキング報酬ＩＤ
    "id_m_ranking",//ランキングＩＤ
    "grade",//等級
    "top",//順位上限
    "bottom",//順位下限
    "itemType",//獲得アイテムtype
    "itemID",//獲得アイテムID
    "itemNum",//獲得個数
  };
  
  const std::string M_RKREWARDCLMSTR[] = 
  {
    "comment_c512",//使用用途など
  };
  
  class TMdm_RkReward: public TModelGen
  {
  protected:

  public:
    explicit TMdm_RkReward();
    virtual ~TMdm_RkReward();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_RkReward dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_RkReward dtNo);
    std::string GetLineDtStr(s64 line, enmdm_RkRewardStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_RkReward dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_RkReward dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_RkRewardStr dtNo);
  };
}
