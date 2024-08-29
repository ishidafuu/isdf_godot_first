#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdVSDrop
  {
    mdvsdrop_pk,//対戦ドロップＩＤ
    mdvsdrop_id_kantoku,//監督ＩＤ
    mdvsdrop_id_m_vsdrop_hw,//アイテム番号対人勝利
    mdvsdrop_id_m_vsdrop_hl,//アイテム番号対人敗北
    mdvsdrop_id_m_vsdrop_cw,//アイテム番号ＣＯＭ勝利
    mdvsdrop_id_m_vsdrop_cl,//アイテム番号ＣＯＭ敗北
    mdvsdrop_create_date,//作成日時
    mdvsdrop_updated_date,//更新日時
    mdvsdrop_del_f,//削除フラグ
  };
  
  const std::string VSDROPCLM[] = 
  {
    "pk",//対戦ドロップＩＤ
    "id_kantoku",//監督ＩＤ
    "id_m_vsdrop_hw",//アイテム番号対人勝利
    "id_m_vsdrop_hl",//アイテム番号対人敗北
    "id_m_vsdrop_cw",//アイテム番号ＣＯＭ勝利
    "id_m_vsdrop_cl",//アイテム番号ＣＯＭ敗北
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdVSDrop: public TModelGen
  {
  protected:

  public:
    explicit TMdVSDrop();
    virtual ~TMdVSDrop();
    s64 GetLineDt(s64 line, enmdVSDrop dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdVSDrop dtNo);
    s64 GetPKDt(s64 pk, enmdVSDrop dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdVSDrop dtNo);
  };
}
