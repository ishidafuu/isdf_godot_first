#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Team
  {
    mdm_team_pk,//マスターチームＩＤ
    mdm_team_rarity,//レアリティ
    mdm_team_npc_f,//ＮＰＣフラグ
    mdm_team_com,//COM思考
    mdm_team_gb_f,//GBフラグ
    mdm_team_id_m_haikei,//背景ＩＤ
    mdm_team_id_m_bgm,//ＢＧＭＩＤ
    mdm_team_id_m_shset,//シュートセットＩＤ
    mdm_team_id_m_char0,//メンバー0
    mdm_team_id_m_char1,//メンバー1
    mdm_team_id_m_char2,//メンバー2
    mdm_team_id_m_char3,//メンバー3
    mdm_team_id_m_char4,//メンバー4
    mdm_team_id_m_char5,//メンバー5
    mdm_team_id_m_char6,//メンバー6
    mdm_team_shirCol,//シャツ色
    mdm_team_pantCol,//シャツ色
    mdm_team_sideLineCol,//サイドライン色
    mdm_team_underLineCol,//ライン色
    mdm_team_shoeCol,//シューズ色
  };
  
  enum enmdm_TeamStr
  {
    mdm_team_name_c32,//チーム名
    mdm_team_text0_c64,//テキスト０
    mdm_team_text1_c64,//テキスト１
    mdm_team_text2_c64,//テキスト２
    mdm_team_text3_c64,//テキスト３
  };
  

  const std::string M_TEAMCLM[] = 
  {
    "pk",//マスターチームＩＤ
    "rarity",//レアリティ
    "npc_f",//ＮＰＣフラグ
    "com",//COM思考
    "gb_f",//GBフラグ
    "id_m_haikei",//背景ＩＤ
    "id_m_bgm",//ＢＧＭＩＤ
    "id_m_shset",//シュートセットＩＤ
    "id_m_char0",//メンバー0
    "id_m_char1",//メンバー1
    "id_m_char2",//メンバー2
    "id_m_char3",//メンバー3
    "id_m_char4",//メンバー4
    "id_m_char5",//メンバー5
    "id_m_char6",//メンバー6
    "shirCol",//シャツ色
    "pantCol",//シャツ色
    "sideLineCol",//サイドライン色
    "underLineCol",//ライン色
    "shoeCol",//シューズ色
  };
  
  const std::string M_TEAMCLMSTR[] = 
  {
    "name_c32",//チーム名
    "text0_c64",//テキスト０
    "text1_c64",//テキスト１
    "text2_c64",//テキスト２
    "text3_c64",//テキスト３
  };
  
  class TMdm_Team: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Team();
    virtual ~TMdm_Team();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Team dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Team dtNo);
    std::string GetLineDtStr(s64 line, enmdm_TeamStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Team dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Team dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_TeamStr dtNo);
  };
}
