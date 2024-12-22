#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Event
  {
    mdm_event_pk,//イベントＩＤ
    mdm_event_id_m_timer,//タイム管理ＩＤ
    mdm_event_eventType,//イベントタイプ
    mdm_event_mapNo,//マップ番号
    mdm_event_staminaMin,//消費スタミナ
    mdm_event_tokkouNo,//特攻チームグループ番号
    mdm_event_id_m_bgm_map,//マップＢＧＭ
    mdm_event_id_m_bgm_ready,//試合開始ジングル
    mdm_event_id_m_bgm_win,//勝利ジングル
    mdm_event_id_m_bgm_lose,//敗北ジングル
    mdm_event_helpType,//ヘルプタイプ
    mdm_event_bmpNo,//画像番号
    mdm_event_priority,//おすすめ優先度
  };
  
  enum enmdm_EventStr
  {
    mdm_event_name_c64,//イベントの名前
    mdm_event_text0_c64,//テキスト０
    mdm_event_text1_c64,//テキスト１
    mdm_event_text2_c64,//テキスト２
    mdm_event_text3_c64,//テキスト３
    mdm_event_ostext0_c64,//おすすめテキスト０
    mdm_event_ostext1_c64,//おすすめテキスト１
    mdm_event_ostext2_c64,//おすすめテキスト２
  };
  

  const std::string M_EVENTCLM[] = 
  {
    "pk",//イベントＩＤ
    "id_m_timer",//タイム管理ＩＤ
    "eventType",//イベントタイプ
    "mapNo",//マップ番号
    "staminaMin",//消費スタミナ
    "tokkouNo",//特攻チームグループ番号
    "id_m_bgm_map",//マップＢＧＭ
    "id_m_bgm_ready",//試合開始ジングル
    "id_m_bgm_win",//勝利ジングル
    "id_m_bgm_lose",//敗北ジングル
    "helpType",//ヘルプタイプ
    "bmpNo",//画像番号
    "priority",//おすすめ優先度
  };
  
  const std::string M_EVENTCLMSTR[] = 
  {
    "name_c64",//イベントの名前
    "text0_c64",//テキスト０
    "text1_c64",//テキスト１
    "text2_c64",//テキスト２
    "text3_c64",//テキスト３
    "ostext0_c64",//おすすめテキスト０
    "ostext1_c64",//おすすめテキスト１
    "ostext2_c64",//おすすめテキスト２
  };
  
  class TMdm_Event: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Event();
    virtual ~TMdm_Event();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Event dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Event dtNo);
    std::string GetLineDtStr(s64 line, enmdm_EventStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Event dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Event dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_EventStr dtNo);
  };
}
