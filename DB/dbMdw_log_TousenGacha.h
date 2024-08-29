#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdw_log_TousenGacha
  {
    mdw_log_tousengacha_pk,//ガチャ当選ログＩＤ
    mdw_log_tousengacha_id_kantoku,//監督ＩＤ
    mdw_log_tousengacha_logGrp,//ロットグループ
    mdw_log_tousengacha_id_m_gachacnt,//ガチャ中身ＩＤ
    mdw_log_tousengacha_grade,//当選等級
    mdw_log_tousengacha_create_date,//作成日時
    mdw_log_tousengacha_updated_date,//更新日時
    mdw_log_tousengacha_del_f,//削除フラグ
  };
  
  const std::string W_LOG_TOUSENGACHACLM[] = 
  {
    "pk",//ガチャ当選ログＩＤ
    "id_kantoku",//監督ＩＤ
    "logGrp",//ロットグループ
    "id_m_gachacnt",//ガチャ中身ＩＤ
    "grade",//当選等級
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdw_log_TousenGacha: public TModelGen
  {
  protected:

  public:
    explicit TMdw_log_TousenGacha();
    virtual ~TMdw_log_TousenGacha();
    s64 GetLineDt(s64 line, enmdw_log_TousenGacha dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdw_log_TousenGacha dtNo);
    s64 GetPKDt(s64 pk, enmdw_log_TousenGacha dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdw_log_TousenGacha dtNo);
  };
}
