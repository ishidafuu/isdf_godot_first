#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_Maruka
  {
    mdlog_maruka_pk,//アイテム取得ＩＤ
    mdlog_maruka_id_kantoku,//保持監督ＩＤ
    mdlog_maruka_id_m_sozai,//マスター素材ＩＤ(１の位レア度０～６)
    mdlog_maruka_indvpow,//個体値パワー
    mdlog_maruka_indvspin,//個体値スピン
    mdlog_maruka_keep_date,//あふれＢＯＸ保管日時
    mdlog_maruka_create_date,//作成日時
    mdlog_maruka_updated_date,//更新日時
    mdlog_maruka_del_f,//削除フラグ
  };
  
  const std::string LOG_MARUKACLM[] = 
  {
    "pk",//アイテム取得ＩＤ
    "id_kantoku",//保持監督ＩＤ
    "id_m_sozai",//マスター素材ＩＤ(１の位レア度０～６)
    "indvpow",//個体値パワー
    "indvspin",//個体値スピン
    "keep_date",//あふれＢＯＸ保管日時
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_Maruka: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_Maruka();
    virtual ~TMdlog_Maruka();
    s64 GetLineDt(s64 line, enmdlog_Maruka dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_Maruka dtNo);
    s64 GetPKDt(s64 pk, enmdlog_Maruka dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_Maruka dtNo);
  };
}
