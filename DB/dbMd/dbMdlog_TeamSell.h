#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_TeamSell
  {
    mdlog_teamsell_pk,//チーム売却ログＩＤ
    mdlog_teamsell_id_kantoku,//保持監督ＩＤ
    mdlog_teamsell_id_m_team,//チームマスタＩＤ
    mdlog_teamsell_sell_price,//売却価格
    mdlog_teamsell_team_lv,//チームレベル
    mdlog_teamsell_char0_lv,//キャラ0レベル
    mdlog_teamsell_char0_exp,//キャラ0経験値
    mdlog_teamsell_char1_lv,//キャラ1レベル
    mdlog_teamsell_char1_exp,//キャラ1経験値
    mdlog_teamsell_char2_lv,//キャラ2レベル
    mdlog_teamsell_char2_exp,//キャラ2経験値
    mdlog_teamsell_char3_lv,//キャラ3レベル
    mdlog_teamsell_char3_exp,//キャラ3経験値
    mdlog_teamsell_char4_lv,//キャラ4レベル
    mdlog_teamsell_char4_exp,//キャラ4経験値
    mdlog_teamsell_char5_lv,//キャラ5レベル
    mdlog_teamsell_char5_exp,//キャラ5経験値
    mdlog_teamsell_char6_lv,//キャラ6レベル
    mdlog_teamsell_char6_exp,//キャラ6経験値
    mdlog_teamsell_create_date,//作成日時
    mdlog_teamsell_updated_date,//更新日時
    mdlog_teamsell_del_f,//削除フラグ
  };
  
  const std::string LOG_TEAMSELLCLM[] = 
  {
    "pk",//チーム売却ログＩＤ
    "id_kantoku",//保持監督ＩＤ
    "id_m_team",//チームマスタＩＤ
    "sell_price",//売却価格
    "team_lv",//チームレベル
    "char0_lv",//キャラ0レベル
    "char0_exp",//キャラ0経験値
    "char1_lv",//キャラ1レベル
    "char1_exp",//キャラ1経験値
    "char2_lv",//キャラ2レベル
    "char2_exp",//キャラ2経験値
    "char3_lv",//キャラ3レベル
    "char3_exp",//キャラ3経験値
    "char4_lv",//キャラ4レベル
    "char4_exp",//キャラ4経験値
    "char5_lv",//キャラ5レベル
    "char5_exp",//キャラ5経験値
    "char6_lv",//キャラ6レベル
    "char6_exp",//キャラ6経験値
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_TeamSell: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_TeamSell();
    virtual ~TMdlog_TeamSell();
    s64 GetLineDt(s64 line, enmdlog_TeamSell dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_TeamSell dtNo);
    s64 GetPKDt(s64 pk, enmdlog_TeamSell dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_TeamSell dtNo);
  };
}
