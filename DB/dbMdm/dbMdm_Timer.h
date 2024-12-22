#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Timer
  {
    mdm_timer_pk,//タイム管理ＩＤ
    mdm_timer_noticeSt_date,//予告開始日時
    mdm_timer_noticeEd_date,//予告終了日時
    mdm_timer_eventSt_date,//イベント開始日時
    mdm_timer_eventEd_date,//イベント終了日時
    mdm_timer_resultSt_date,//結果開始日時
    mdm_timer_resultEd_date,//結果終了日時
    mdm_timer_weekday,//曜日指定
  };
  
  enum enmdm_TimerStr
  {
    mdm_timer_comment_c512,//使用用途など
  };
  

  const std::string M_TIMERCLM[] = 
  {
    "pk",//タイム管理ＩＤ
    "noticeSt_date",//予告開始日時
    "noticeEd_date",//予告終了日時
    "eventSt_date",//イベント開始日時
    "eventEd_date",//イベント終了日時
    "resultSt_date",//結果開始日時
    "resultEd_date",//結果終了日時
    "weekday",//曜日指定
  };
  
  const std::string M_TIMERCLMSTR[] = 
  {
    "comment_c512",//使用用途など
  };
  
  class TMdm_Timer: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Timer();
    virtual ~TMdm_Timer();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Timer dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Timer dtNo);
    std::string GetLineDtStr(s64 line, enmdm_TimerStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Timer dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Timer dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_TimerStr dtNo);
  };
}
