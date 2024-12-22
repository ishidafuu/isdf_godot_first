#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdSupply
  {
    mdsupply_pk,//アイテムＩＤ
    mdsupply_id_kantoku,//保持監督ＩＤ
    mdsupply_id_m_supply,//マスター消耗品ＩＤ
    mdsupply_supplyNum,//個数
    mdsupply_create_date,//作成日時
    mdsupply_updated_date,//更新日時
    mdsupply_del_f,//削除フラグ
  };
  
  const std::string SUPPLYCLM[] = 
  {
    "pk",//アイテムＩＤ
    "id_kantoku",//保持監督ＩＤ
    "id_m_supply",//マスター消耗品ＩＤ
    "supplyNum",//個数
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdSupply: public TModelGen
  {
  protected:

  public:
    explicit TMdSupply();
    virtual ~TMdSupply();
    s64 GetLineDt(s64 line, enmdSupply dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdSupply dtNo);
    s64 GetPKDt(s64 pk, enmdSupply dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdSupply dtNo);
  };
}
