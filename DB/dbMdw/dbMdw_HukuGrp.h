#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdw_HukuGrp
  {
    mdw_hukugrp_pk,//福引きグループＩＤ
    mdw_hukugrp_fiver_f,//フィーバータイムフラグ
    mdw_hukugrp_end_f,//終了フラグ
    mdw_hukugrp_end_date,//終了日時
    mdw_hukugrp_create_date,//作成日時
    mdw_hukugrp_updated_date,//更新日時
    mdw_hukugrp_del_f,//削除フラグ
  };
  
  const std::string W_HUKUGRPCLM[] = 
  {
    "pk",//福引きグループＩＤ
    "fiver_f",//フィーバータイムフラグ
    "end_f",//終了フラグ
    "end_date",//終了日時
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdw_HukuGrp: public TModelGen
  {
  protected:

  public:
    explicit TMdw_HukuGrp();
    virtual ~TMdw_HukuGrp();
    s64 GetLineDt(s64 line, enmdw_HukuGrp dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdw_HukuGrp dtNo);
    s64 GetPKDt(s64 pk, enmdw_HukuGrp dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdw_HukuGrp dtNo);
  };
}
