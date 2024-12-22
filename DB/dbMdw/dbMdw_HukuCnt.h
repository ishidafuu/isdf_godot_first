#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdw_HukuCnt
  {
    mdw_hukucnt_pk,//ふくびきＩＤ
    mdw_hukucnt_id_w_hukugrp,//福引き番号
    mdw_hukucnt_restNum,//のこり本数
    mdw_hukucnt_create_date,//作成日時
    mdw_hukucnt_updated_date,//更新日時
    mdw_hukucnt_del_f,//削除フラグ
  };
  
  const std::string W_HUKUCNTCLM[] = 
  {
    "pk",//ふくびきＩＤ
    "id_w_hukugrp",//福引き番号
    "restNum",//のこり本数
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdw_HukuCnt: public TModelGen
  {
  protected:

  public:
    explicit TMdw_HukuCnt();
    virtual ~TMdw_HukuCnt();
    s64 GetLineDt(s64 line, enmdw_HukuCnt dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdw_HukuCnt dtNo);
    s64 GetPKDt(s64 pk, enmdw_HukuCnt dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdw_HukuCnt dtNo);
  };
}
