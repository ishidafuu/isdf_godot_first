#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdw_log_TousenHuku
  {
    mdw_log_tousenhuku_pk,//ふくびき当選ログＩＤ
    mdw_log_tousenhuku_id_kantoku,//監督ＩＤ
    mdw_log_tousenhuku_id_w_hukugrp,//ふくびきグループＩＤ
    mdw_log_tousenhuku_id_m_hukucnt,//ふくびき中身ＩＤ
    mdw_log_tousenhuku_grade,//当選等級
    mdw_log_tousenhuku_create_date,//作成日時
    mdw_log_tousenhuku_updated_date,//更新日時
    mdw_log_tousenhuku_del_f,//削除フラグ
  };
  
  const std::string W_LOG_TOUSENHUKUCLM[] = 
  {
    "pk",//ふくびき当選ログＩＤ
    "id_kantoku",//監督ＩＤ
    "id_w_hukugrp",//ふくびきグループＩＤ
    "id_m_hukucnt",//ふくびき中身ＩＤ
    "grade",//当選等級
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdw_log_TousenHuku: public TModelGen
  {
  protected:

  public:
    explicit TMdw_log_TousenHuku();
    virtual ~TMdw_log_TousenHuku();
    s64 GetLineDt(s64 line, enmdw_log_TousenHuku dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdw_log_TousenHuku dtNo);
    s64 GetPKDt(s64 pk, enmdw_log_TousenHuku dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdw_log_TousenHuku dtNo);
  };
}
