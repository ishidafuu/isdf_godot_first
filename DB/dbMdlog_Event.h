#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_Event
  {
    mdlog_event_pk,//イベントＩＤ
    mdlog_event_id_kantoku,//保持監督ＩＤ
    mdlog_event_mbgID,//モバゲーＩＤ
    mdlog_event_id_m_event,//マスターイベントＩＤ
    mdlog_event_number,//第何回
    mdlog_event_win,//勝ち数
    mdlog_event_wins,//連勝数
    mdlog_event_maxdam,//最大ダメージ
    mdlog_event_totaldam,//トータルダメージ
    mdlog_event_kill,//倒した数
    mdlog_event_itemnum,//獲得アイテム
    mdlog_event_nowwins,//現在連勝数
    mdlog_event_play_f,//挑戦中フラグ
    mdlog_event_create_date,//作成日時
    mdlog_event_updated_date,//更新日時
    mdlog_event_del_f,//削除フラグ
  };
  
  const std::string LOG_EVENTCLM[] = 
  {
    "pk",//イベントＩＤ
    "id_kantoku",//保持監督ＩＤ
    "mbgID",//モバゲーＩＤ
    "id_m_event",//マスターイベントＩＤ
    "number",//第何回
    "win",//勝ち数
    "wins",//連勝数
    "maxdam",//最大ダメージ
    "totaldam",//トータルダメージ
    "kill",//倒した数
    "itemnum",//獲得アイテム
    "nowwins",//現在連勝数
    "play_f",//挑戦中フラグ
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_Event: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_Event();
    virtual ~TMdlog_Event();
    s64 GetLineDt(s64 line, enmdlog_Event dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_Event dtNo);
    s64 GetPKDt(s64 pk, enmdlog_Event dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_Event dtNo);
  };
}
