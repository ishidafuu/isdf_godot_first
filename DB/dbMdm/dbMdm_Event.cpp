#include "dbMdm_Event.h"
namespace db
{
  TMdm_Event::TMdm_Event():TModelGen("m_Event")
  {
  }
  TMdm_Event::~TMdm_Event(void)
  {
  }
  void TMdm_Event::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Event", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_event_pk));
      model_.intvecmap_["id_m_timer"].push_back(csv.GetInt(i, mdm_event_id_m_timer));
      model_.intvecmap_["eventType"].push_back(csv.GetInt(i, mdm_event_eventType));
      model_.intvecmap_["mapNo"].push_back(csv.GetInt(i, mdm_event_mapNo));
      model_.intvecmap_["staminaMin"].push_back(csv.GetInt(i, mdm_event_staminaMin));
      model_.intvecmap_["tokkouNo"].push_back(csv.GetInt(i, mdm_event_tokkouNo));
      model_.intvecmap_["id_m_bgm_map"].push_back(csv.GetInt(i, mdm_event_id_m_bgm_map));
      model_.intvecmap_["id_m_bgm_ready"].push_back(csv.GetInt(i, mdm_event_id_m_bgm_ready));
      model_.intvecmap_["id_m_bgm_win"].push_back(csv.GetInt(i, mdm_event_id_m_bgm_win));
      model_.intvecmap_["id_m_bgm_lose"].push_back(csv.GetInt(i, mdm_event_id_m_bgm_lose));
      model_.intvecmap_["helpType"].push_back(csv.GetInt(i, mdm_event_helpType));
      model_.intvecmap_["bmpNo"].push_back(csv.GetInt(i, mdm_event_bmpNo));
      model_.intvecmap_["priority"].push_back(csv.GetInt(i, mdm_event_priority));
      model_.strvecmap_["name_c64"].push_back(csv.GetStr(i, mdm_event_name_c64));
      model_.strvecmap_["text0_c64"].push_back(csv.GetStr(i, mdm_event_text0_c64));
      model_.strvecmap_["text1_c64"].push_back(csv.GetStr(i, mdm_event_text1_c64));
      model_.strvecmap_["text2_c64"].push_back(csv.GetStr(i, mdm_event_text2_c64));
      model_.strvecmap_["text3_c64"].push_back(csv.GetStr(i, mdm_event_text3_c64));
      model_.strvecmap_["ostext0_c64"].push_back(csv.GetStr(i, mdm_event_ostext0_c64));
      model_.strvecmap_["ostext1_c64"].push_back(csv.GetStr(i, mdm_event_ostext1_c64));
      model_.strvecmap_["ostext2_c64"].push_back(csv.GetStr(i, mdm_event_ostext2_c64));
    }
    
  }
  s64 TMdm_Event::GetLineDt(s64 line, enmdm_Event dtNo)
  {
    return GenGetLineDt(line, M_EVENTCLM[dtNo].c_str());
  }
  BOOL TMdm_Event::GetLineDtBOOL(s64 line, enmdm_Event dtNo)
  {
    return GenGetLineDtBOOL(line, M_EVENTCLM[dtNo].c_str());
  }
  std::string TMdm_Event::GetLineDtStr(s64 line, enmdm_EventStr dtNo)
  {
    return GenGetLineDtStr(line, M_EVENTCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Event::GetPKDt(s64 pk, enmdm_Event dtNo)
  {
    return GenGetPKDt(pk, M_EVENTCLM[dtNo].c_str());
  }
  BOOL TMdm_Event::GetPKDtBOOL(s64 pk, enmdm_Event dtNo)
  {
    return GenGetPKDtBOOL(pk, M_EVENTCLM[dtNo].c_str());
  }
  std::string TMdm_Event::GetPKDtStr(s64 pk, enmdm_EventStr dtNo)
  {
    return GenGetPKDtStr(pk, M_EVENTCLMSTR[dtNo].c_str());
  }
}
