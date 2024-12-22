#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_SupplyUse
  {
    mdlog_supplyuse_pk,//アイテム削除ＩＤ
    mdlog_supplyuse_id_kantoku,//保持監督ＩＤ
    mdlog_supplyuse_id_m_supply,//マスター消耗品ＩＤ
    mdlog_supplyuse_useType,//削除方法 0USE 1QUSE 2SELL
    mdlog_supplyuse_useNum,//使用個数
    mdlog_supplyuse_create_date,//作成日時
    mdlog_supplyuse_updated_date,//更新日時
    mdlog_supplyuse_del_f,//削除フラグ
  };
  
  const std::string LOG_SUPPLYUSECLM[] = 
  {
    "pk",//アイテム削除ＩＤ
    "id_kantoku",//保持監督ＩＤ
    "id_m_supply",//マスター消耗品ＩＤ
    "useType",//削除方法 0USE 1QUSE 2SELL
    "useNum",//使用個数
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_SupplyUse: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_SupplyUse();
    virtual ~TMdlog_SupplyUse();
    s64 GetLineDt(s64 line, enmdlog_SupplyUse dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_SupplyUse dtNo);
    s64 GetPKDt(s64 pk, enmdlog_SupplyUse dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_SupplyUse dtNo);
  };
}
