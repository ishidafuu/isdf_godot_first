#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_CharLvUp
  {
    mdlog_charlvup_pk,//キャラレベルアップログＩＤ
    mdlog_charlvup_id_kantoku,//保持監督ＩＤ
    mdlog_charlvup_id_m_team,//チームマスタＩＤ
    mdlog_charlvup_char_no,//キャラＮＯ
    mdlog_charlvup_addExp,//加算経験値
    mdlog_charlvup_create_date,//作成日時
    mdlog_charlvup_updated_date,//更新日時
    mdlog_charlvup_del_f,//削除フラグ
  };
  
  const std::string LOG_CHARLVUPCLM[] = 
  {
    "pk",//キャラレベルアップログＩＤ
    "id_kantoku",//保持監督ＩＤ
    "id_m_team",//チームマスタＩＤ
    "char_no",//キャラＮＯ
    "addExp",//加算経験値
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_CharLvUp: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_CharLvUp();
    virtual ~TMdlog_CharLvUp();
    s64 GetLineDt(s64 line, enmdlog_CharLvUp dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_CharLvUp dtNo);
    s64 GetPKDt(s64 pk, enmdlog_CharLvUp dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_CharLvUp dtNo);
  };
}
