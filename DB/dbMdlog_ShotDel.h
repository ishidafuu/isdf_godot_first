#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_ShotDel
  {
    mdlog_shotdel_pk,//アイテム取得ＩＤ
    mdlog_shotdel_id_kantoku,//保持監督ＩＤ
    mdlog_shotdel_useslot,//スロット使用数
    mdlog_shotdel_indvpow,//個体値パワー
    mdlog_shotdel_indvspin,//個体値スピン
    mdlog_shotdel_id_m_sozai0,//マスター素材ＩＤ(１の位レア度０～６)ベース
    mdlog_shotdel_id_m_sozai1,//マスター素材ＩＤ(１の位レア度０～６)
    mdlog_shotdel_id_m_sozai2,//マスター素材ＩＤ(１の位レア度０～６)
    mdlog_shotdel_id_m_sozai3,//マスター素材ＩＤ(１の位レア度０～６)
    mdlog_shotdel_id_m_sozai4,//マスター素材ＩＤ(１の位レア度０～６)
    mdlog_shotdel_id_m_sozai5,//マスター素材ＩＤ(１の位レア度０～６)
    mdlog_shotdel_id_m_sozai6,//マスター素材ＩＤ(１の位レア度０～６)
    mdlog_shotdel_id_m_sozai7,//マスター素材ＩＤ(１の位レア度０～６)
    mdlog_shotdel_deltype,//状態 0SELL 1OVERKEEP
    mdlog_shotdel_create_date,//作成日時
    mdlog_shotdel_updated_date,//更新日時
    mdlog_shotdel_del_f,//削除フラグ
  };
  
  const std::string LOG_SHOTDELCLM[] = 
  {
    "pk",//アイテム取得ＩＤ
    "id_kantoku",//保持監督ＩＤ
    "useslot",//スロット使用数
    "indvpow",//個体値パワー
    "indvspin",//個体値スピン
    "id_m_sozai0",//マスター素材ＩＤ(１の位レア度０～６)ベース
    "id_m_sozai1",//マスター素材ＩＤ(１の位レア度０～６)
    "id_m_sozai2",//マスター素材ＩＤ(１の位レア度０～６)
    "id_m_sozai3",//マスター素材ＩＤ(１の位レア度０～６)
    "id_m_sozai4",//マスター素材ＩＤ(１の位レア度０～６)
    "id_m_sozai5",//マスター素材ＩＤ(１の位レア度０～６)
    "id_m_sozai6",//マスター素材ＩＤ(１の位レア度０～６)
    "id_m_sozai7",//マスター素材ＩＤ(１の位レア度０～６)
    "deltype",//状態 0SELL 1OVERKEEP
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_ShotDel: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_ShotDel();
    virtual ~TMdlog_ShotDel();
    s64 GetLineDt(s64 line, enmdlog_ShotDel dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_ShotDel dtNo);
    s64 GetPKDt(s64 pk, enmdlog_ShotDel dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_ShotDel dtNo);
  };
}
