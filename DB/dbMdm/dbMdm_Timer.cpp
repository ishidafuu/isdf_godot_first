#include "dbMdm_Timer.h"
namespace db
{
  TMdm_Timer::TMdm_Timer():TModelGen("m_Timer")
  {
  }
  TMdm_Timer::~TMdm_Timer(void)
  {
  }
  void TMdm_Timer::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Timer", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_timer_pk));
      model_.intvecmap_["noticeSt_date"].push_back(csv.GetInt(i, mdm_timer_noticeSt_date));
      model_.intvecmap_["noticeEd_date"].push_back(csv.GetInt(i, mdm_timer_noticeEd_date));
      model_.intvecmap_["eventSt_date"].push_back(csv.GetInt(i, mdm_timer_eventSt_date));
      model_.intvecmap_["eventEd_date"].push_back(csv.GetInt(i, mdm_timer_eventEd_date));
      model_.intvecmap_["resultSt_date"].push_back(csv.GetInt(i, mdm_timer_resultSt_date));
      model_.intvecmap_["resultEd_date"].push_back(csv.GetInt(i, mdm_timer_resultEd_date));
      model_.intvecmap_["weekday"].push_back(csv.GetInt(i, mdm_timer_weekday));
      model_.strvecmap_["comment_c512"].push_back(csv.GetStr(i, mdm_timer_comment_c512));
    }
    
  }
  s64 TMdm_Timer::GetLineDt(s64 line, enmdm_Timer dtNo)
  {
    return GenGetLineDt(line, M_TIMERCLM[dtNo].c_str());
  }
  BOOL TMdm_Timer::GetLineDtBOOL(s64 line, enmdm_Timer dtNo)
  {
    return GenGetLineDtBOOL(line, M_TIMERCLM[dtNo].c_str());
  }
  std::string TMdm_Timer::GetLineDtStr(s64 line, enmdm_TimerStr dtNo)
  {
    return GenGetLineDtStr(line, M_TIMERCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Timer::GetPKDt(s64 pk, enmdm_Timer dtNo)
  {
    return GenGetPKDt(pk, M_TIMERCLM[dtNo].c_str());
  }
  BOOL TMdm_Timer::GetPKDtBOOL(s64 pk, enmdm_Timer dtNo)
  {
    return GenGetPKDtBOOL(pk, M_TIMERCLM[dtNo].c_str());
  }
  std::string TMdm_Timer::GetPKDtStr(s64 pk, enmdm_TimerStr dtNo)
  {
    return GenGetPKDtStr(pk, M_TIMERCLMSTR[dtNo].c_str());
  }
}
