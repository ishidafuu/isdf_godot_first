#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdre_Refresh
  {
    mdre_refresh_pk,//ID
    mdre_refresh_mami_f,//まみ更新
    mdre_refresh_maruka_f,//まるか更新
    mdre_refresh_newestinfo,//最新おしらせ日時
    mdre_refresh_create_date,//作成日時
    mdre_refresh_updated_date,//更新日時
    mdre_refresh_del_f,//削除フラグ
  };
  
  const std::string RE_REFRESHCLM[] = 
  {
    "pk",//ID
    "mami_f",//まみ更新
    "maruka_f",//まるか更新
    "newestinfo",//最新おしらせ日時
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdre_Refresh: public TModelGen
  {
  protected:

  public:
    explicit TMdre_Refresh();
    virtual ~TMdre_Refresh();
    s64 GetLineDt(s64 line, enmdre_Refresh dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdre_Refresh dtNo);
    s64 GetPKDt(s64 pk, enmdre_Refresh dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdre_Refresh dtNo);
  };
}
