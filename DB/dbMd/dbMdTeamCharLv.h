#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdTeamCharLv
  {
    mdteamcharlv_pk,//チーム選手レベルＩＤ
    mdteamcharlv_id_kantoku,//保持監督ＩＤ
    mdteamcharlv_id_m_team,//チームマスタＩＤ
    mdteamcharlv_char0_lv,//キャラ０Lv
    mdteamcharlv_char0_exp,//キャラ０経験値
    mdteamcharlv_char1_lv,//キャラ１Lv
    mdteamcharlv_char1_exp,//キャラ１経験値
    mdteamcharlv_char2_lv,//キャラ２Lv
    mdteamcharlv_char2_exp,//キャラ２経験値
    mdteamcharlv_char3_lv,//キャラ３Lv
    mdteamcharlv_char3_exp,//キャラ３経験値
    mdteamcharlv_char4_lv,//キャラ４Lv
    mdteamcharlv_char4_exp,//キャラ４経験値
    mdteamcharlv_char5_lv,//キャラ５Lv
    mdteamcharlv_char5_exp,//キャラ５経験値
    mdteamcharlv_char6_lv,//キャラ６Lv
    mdteamcharlv_char6_exp,//キャラ６経験値
    mdteamcharlv_create_date,//作成日時
    mdteamcharlv_updated_date,//更新日時
    mdteamcharlv_del_f,//削除フラグ
  };
  
  const std::string TEAMCHARLVCLM[] = 
  {
    "pk",//チーム選手レベルＩＤ
    "id_kantoku",//保持監督ＩＤ
    "id_m_team",//チームマスタＩＤ
    "char0_lv",//キャラ０Lv
    "char0_exp",//キャラ０経験値
    "char1_lv",//キャラ１Lv
    "char1_exp",//キャラ１経験値
    "char2_lv",//キャラ２Lv
    "char2_exp",//キャラ２経験値
    "char3_lv",//キャラ３Lv
    "char3_exp",//キャラ３経験値
    "char4_lv",//キャラ４Lv
    "char4_exp",//キャラ４経験値
    "char5_lv",//キャラ５Lv
    "char5_exp",//キャラ５経験値
    "char6_lv",//キャラ６Lv
    "char6_exp",//キャラ６経験値
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdTeamCharLv: public TModelGen
  {
  protected:

  public:
    explicit TMdTeamCharLv();
    virtual ~TMdTeamCharLv();
    s64 GetLineDt(s64 line, enmdTeamCharLv dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdTeamCharLv dtNo);
    s64 GetPKDt(s64 pk, enmdTeamCharLv dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdTeamCharLv dtNo);
  };
}
