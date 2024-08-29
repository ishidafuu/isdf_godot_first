#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdw_Event
  {
    mdw_event_pk,//イベントＩＤ
    mdw_event_id_m_event,//イベントＩＤ
    mdw_event_number,//第何回
    mdw_event_start_date,//開始日時
    mdw_event_end_date,//終了日時
    mdw_event_once_f,//１回きり
    mdw_event_open_f,//開催中
    mdw_event_priority,//ソート優先度
    mdw_event_result_date,//結果表示終了日時
    mdw_event_create_date,//作成日時
    mdw_event_updated_date,//更新日時
    mdw_event_del_f,//削除フラグ
  };
  
  const std::string W_EVENTCLM[] = 
  {
    "pk",//イベントＩＤ
    "id_m_event",//イベントＩＤ
    "number",//第何回
    "start_date",//開始日時
    "end_date",//終了日時
    "once_f",//１回きり
    "open_f",//開催中
    "priority",//ソート優先度
    "result_date",//結果表示終了日時
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdw_Event: public TModelGen
  {
  protected:

  public:
    explicit TMdw_Event();
    virtual ~TMdw_Event();
    s64 GetLineDt(s64 line, enmdw_Event dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdw_Event dtNo);
    s64 GetPKDt(s64 pk, enmdw_Event dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdw_Event dtNo);
  };
}
