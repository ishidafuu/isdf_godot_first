#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdGiftbox
  {
    mdgiftbox_pk,//贈り物ボックスＩＤ
    mdgiftbox_id_kantoku,//監督ＩＤ
    mdgiftbox_itemType,//獲得アイテムtype
    mdgiftbox_itemID,//獲得アイテムID
    mdgiftbox_itemNum,//獲得アイテム個数
    mdgiftbox_limit_date,//受取期限
    mdgiftbox_getType,//取得場所
    mdgiftbox_getInfo,//取得情報
    mdgiftbox_getInfo2,//取得情報２
    mdgiftbox_getInfo3,//取得情報３
    mdgiftbox_getInfo4,//取得情報４
    mdgiftbox_create_date,//作成日時
    mdgiftbox_updated_date,//更新日時
    mdgiftbox_del_f,//削除フラグ
  };
  
  const std::string GIFTBOXCLM[] = 
  {
    "pk",//贈り物ボックスＩＤ
    "id_kantoku",//監督ＩＤ
    "itemType",//獲得アイテムtype
    "itemID",//獲得アイテムID
    "itemNum",//獲得アイテム個数
    "limit_date",//受取期限
    "getType",//取得場所
    "getInfo",//取得情報
    "getInfo2",//取得情報２
    "getInfo3",//取得情報３
    "getInfo4",//取得情報４
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdGiftbox: public TModelGen
  {
  protected:

  public:
    explicit TMdGiftbox();
    virtual ~TMdGiftbox();
    s64 GetLineDt(s64 line, enmdGiftbox dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdGiftbox dtNo);
    s64 GetPKDt(s64 pk, enmdGiftbox dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdGiftbox dtNo);
  };
}
