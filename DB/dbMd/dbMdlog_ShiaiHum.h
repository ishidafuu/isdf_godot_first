#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_ShiaiHum
  {
    mdlog_shiaihum_pk,//試合ＩＤ
    mdlog_shiaihum_id_kantoku,//監督ＩＤ
    mdlog_shiaihum_id_kantoku_en,//敵監督ＩＤ
    mdlog_shiaihum_id_team,//使用チームＩＤ
    mdlog_shiaihum_id_m_team,//使用チームマスターＩＤ
    mdlog_shiaihum_id_m_event,//イベントＩＤ
    mdlog_shiaihum_wifi_f,//ワイファイ
    mdlog_shiaihum_roomtype,//部屋タイプ
    mdlog_shiaihum_result,//結果 0NONE 1WIN 2LOSE 3DRAW 4RETIRE
    mdlog_shiaihum_killNum,//倒した
    mdlog_shiaihum_deathNum,//倒された
    mdlog_shiaihum_maxDam,//最大ダメージ
    mdlog_shiaihum_totalDam,//合計ダメージ
    mdlog_shiaihum_shiaiTime,//試合時間
    mdlog_shiaihum_errcode,//エラーコード
    mdlog_shiaihum_create_date,//作成日時
    mdlog_shiaihum_updated_date,//更新日時
    mdlog_shiaihum_del_f,//削除フラグ
  };
  
  const std::string LOG_SHIAIHUMCLM[] = 
  {
    "pk",//試合ＩＤ
    "id_kantoku",//監督ＩＤ
    "id_kantoku_en",//敵監督ＩＤ
    "id_team",//使用チームＩＤ
    "id_m_team",//使用チームマスターＩＤ
    "id_m_event",//イベントＩＤ
    "wifi_f",//ワイファイ
    "roomtype",//部屋タイプ
    "result",//結果 0NONE 1WIN 2LOSE 3DRAW 4RETIRE
    "killNum",//倒した
    "deathNum",//倒された
    "maxDam",//最大ダメージ
    "totalDam",//合計ダメージ
    "shiaiTime",//試合時間
    "errcode",//エラーコード
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_ShiaiHum: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_ShiaiHum();
    virtual ~TMdlog_ShiaiHum();
    s64 GetLineDt(s64 line, enmdlog_ShiaiHum dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_ShiaiHum dtNo);
    s64 GetPKDt(s64 pk, enmdlog_ShiaiHum dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_ShiaiHum dtNo);
  };
}
