#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_BStar
  {
    mdlog_bstar_pk,//購入スター管理ＩＤ
    mdlog_bstar_id_kantoku,//保持監督ＩＤ
    mdlog_bstar_id_m_supply,//スターパックＩＤ
    mdlog_bstar_buyNum,//購入スター
    mdlog_bstar_zandaka,//残高
    mdlog_bstar_create_date,//作成日時
    mdlog_bstar_updated_date,//更新日時
    mdlog_bstar_del_f,//削除フラグ
  };
  
  const std::string LOG_BSTARCLM[] = 
  {
    "pk",//購入スター管理ＩＤ
    "id_kantoku",//保持監督ＩＤ
    "id_m_supply",//スターパックＩＤ
    "buyNum",//購入スター
    "zandaka",//残高
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_BStar: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_BStar();
    virtual ~TMdlog_BStar();
    s64 GetLineDt(s64 line, enmdlog_BStar dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_BStar dtNo);
    s64 GetPKDt(s64 pk, enmdlog_BStar dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_BStar dtNo);
  };
}
