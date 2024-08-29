#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdShot
  {
    mdshot_pk,//シュートＩＤ
    mdshot_id_kantoku,//保持監督ＩＤ
    mdshot_state,//状態 0EMP 1ENABLE 2DOCK
    mdshot_useslot,//スロット使用数
    mdshot_lock_f,//ロックフラグ
    mdshot_indvpow,//個体値パワー
    mdshot_indvspin,//個体値スピン
    mdshot_id_m_sozai0,//マスター素材ＩＤ(１の位レア度０～６)ベース
    mdshot_id_m_sozai1,//マスター素材ＩＤ(１の位レア度０～６)
    mdshot_id_m_sozai2,//マスター素材ＩＤ(１の位レア度０～６)
    mdshot_id_m_sozai3,//マスター素材ＩＤ(１の位レア度０～６)
    mdshot_id_m_sozai4,//マスター素材ＩＤ(１の位レア度０～６)
    mdshot_id_m_sozai5,//マスター素材ＩＤ(１の位レア度０～６)
    mdshot_id_m_sozai6,//マスター素材ＩＤ(１の位レア度０～６)
    mdshot_id_m_sozai7,//マスター素材ＩＤ(１の位レア度０～６)
    mdshot_create_date,//作成日時
    mdshot_updated_date,//更新日時
    mdshot_del_f,//削除フラグ
  };
  
  enum enmdShotStr
  {
    mdshot_name_c32,//オリジナル名
  };
  

  const std::string SHOTCLM[] = 
  {
    "pk",//シュートＩＤ
    "id_kantoku",//保持監督ＩＤ
    "state",//状態 0EMP 1ENABLE 2DOCK
    "useslot",//スロット使用数
    "lock_f",//ロックフラグ
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
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  const std::string SHOTCLMSTR[] = 
  {
    "name_c32",//オリジナル名
  };
  
  class TMdShot: public TModelGen
  {
  protected:

  public:
    explicit TMdShot();
    virtual ~TMdShot();
    s64 GetLineDt(s64 line, enmdShot dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdShot dtNo);
    std::string GetLineDtStr(s64 line, enmdShotStr dtNo);
    s64 GetPKDt(s64 pk, enmdShot dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdShot dtNo);
    std::string GetPKDtStr(s64 pk, enmdShotStr dtNo);
  };
}
