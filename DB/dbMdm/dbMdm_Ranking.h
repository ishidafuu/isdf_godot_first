#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Ranking
  {
    mdm_ranking_pk,//ランキングＩＤ
    mdm_ranking_id_m_event,//イベントＩＤ
    mdm_ranking_scoreType,//スコアの種類　1勝数　2連勝数　3最大ダメ　4累積ダメ　5倒した数　6アイテム
    mdm_ranking_id_m_supply,//スコアアイテムＩＤ
    mdm_ranking_iconNo,//
  };
  
  enum enmdm_RankingStr
  {
    mdm_ranking_name_c64,//ランキングの名前
  };
  

  const std::string M_RANKINGCLM[] = 
  {
    "pk",//ランキングＩＤ
    "id_m_event",//イベントＩＤ
    "scoreType",//スコアの種類　1勝数　2連勝数　3最大ダメ　4累積ダメ　5倒した数　6アイテム
    "id_m_supply",//スコアアイテムＩＤ
    "iconNo",//
  };
  
  const std::string M_RANKINGCLMSTR[] = 
  {
    "name_c64",//ランキングの名前
  };
  
  class TMdm_Ranking: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Ranking();
    virtual ~TMdm_Ranking();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Ranking dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Ranking dtNo);
    std::string GetLineDtStr(s64 line, enmdm_RankingStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Ranking dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Ranking dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_RankingStr dtNo);
  };
}
