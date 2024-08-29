#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_CpCode
  {
    mdlog_cpcode_pk,//キャンペーンコードログＩＤ
    mdlog_cpcode_id_kantoku,//監督ＩＤ
    mdlog_cpcode_id_m_campaign,//マスターキャンペーンＩＤ
    mdlog_cpcode_id_m_cpcode,//マスターキャンペーンコードＩＤ
    mdlog_cpcode_create_date,//作成日時
    mdlog_cpcode_updated_date,//更新日時
    mdlog_cpcode_del_f,//削除フラグ
  };
  
  const std::string LOG_CPCODECLM[] = 
  {
    "pk",//キャンペーンコードログＩＤ
    "id_kantoku",//監督ＩＤ
    "id_m_campaign",//マスターキャンペーンＩＤ
    "id_m_cpcode",//マスターキャンペーンコードＩＤ
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_CpCode: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_CpCode();
    virtual ~TMdlog_CpCode();
    s64 GetLineDt(s64 line, enmdlog_CpCode dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_CpCode dtNo);
    s64 GetPKDt(s64 pk, enmdlog_CpCode dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_CpCode dtNo);
  };
}
