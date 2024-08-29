#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_ItemGet
  {
    mdlog_itemget_pk,//アイテム取得ＩＤ
    mdlog_itemget_id_kantoku,//保持監督ＩＤ
    mdlog_itemget_itemType,//アイテム種類
    mdlog_itemget_itemID,//アイテムＩＤ
    mdlog_itemget_itemNum,//アイテム個数
    mdlog_itemget_getType,//取得場所
    mdlog_itemget_getInfo,//取得情報
    mdlog_itemget_getInfo2,//取得情報２
    mdlog_itemget_getInfo3,//取得情報３
    mdlog_itemget_getInfo4,//取得情報４
    mdlog_itemget_outPack_f,//パックフラグ
    mdlog_itemget_create_date,//作成日時
    mdlog_itemget_updated_date,//更新日時
    mdlog_itemget_del_f,//削除フラグ
  };
  
  const std::string LOG_ITEMGETCLM[] = 
  {
    "pk",//アイテム取得ＩＤ
    "id_kantoku",//保持監督ＩＤ
    "itemType",//アイテム種類
    "itemID",//アイテムＩＤ
    "itemNum",//アイテム個数
    "getType",//取得場所
    "getInfo",//取得情報
    "getInfo2",//取得情報２
    "getInfo3",//取得情報３
    "getInfo4",//取得情報４
    "outPack_f",//パックフラグ
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_ItemGet: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_ItemGet();
    virtual ~TMdlog_ItemGet();
    s64 GetLineDt(s64 line, enmdlog_ItemGet dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_ItemGet dtNo);
    s64 GetPKDt(s64 pk, enmdlog_ItemGet dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_ItemGet dtNo);
  };
}
