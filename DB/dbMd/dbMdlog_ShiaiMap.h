#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_ShiaiMap
  {
    mdlog_shiaimap_pk,//試合ＩＤ
    mdlog_shiaimap_id_kantoku,//監督ＩＤ
    mdlog_shiaimap_mapNo,//マップＮｏ
    mdlog_shiaimap_id_m_maptip,//マップチップＩＤ
    mdlog_shiaimap_id_m_event,//イベントＩＤ
    mdlog_shiaimap_id_team,//使用チームＩＤ
    mdlog_shiaimap_id_m_team,//使用チームマスターＩＤ
    mdlog_shiaimap_result,//結果 0NONE 1WIN 2LOSE 3DRAW 4RETIRE
    mdlog_shiaimap_retryNum,//復活
    mdlog_shiaimap_killNum,//倒した
    mdlog_shiaimap_deathNum,//倒された
    mdlog_shiaimap_maxDam,//最大ダメージ
    mdlog_shiaimap_totalDam,//合計ダメージ
    mdlog_shiaimap_shiaiTime,//試合時間
    mdlog_shiaimap_create_date,//作成日時
    mdlog_shiaimap_updated_date,//更新日時
    mdlog_shiaimap_del_f,//削除フラグ
  };
  
  const std::string LOG_SHIAIMAPCLM[] = 
  {
    "pk",//試合ＩＤ
    "id_kantoku",//監督ＩＤ
    "mapNo",//マップＮｏ
    "id_m_maptip",//マップチップＩＤ
    "id_m_event",//イベントＩＤ
    "id_team",//使用チームＩＤ
    "id_m_team",//使用チームマスターＩＤ
    "result",//結果 0NONE 1WIN 2LOSE 3DRAW 4RETIRE
    "retryNum",//復活
    "killNum",//倒した
    "deathNum",//倒された
    "maxDam",//最大ダメージ
    "totalDam",//合計ダメージ
    "shiaiTime",//試合時間
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_ShiaiMap: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_ShiaiMap();
    virtual ~TMdlog_ShiaiMap();
    s64 GetLineDt(s64 line, enmdlog_ShiaiMap dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_ShiaiMap dtNo);
    s64 GetPKDt(s64 pk, enmdlog_ShiaiMap dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_ShiaiMap dtNo);
  };
}
