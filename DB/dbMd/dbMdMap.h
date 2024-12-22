#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdMap
  {
    mdmap_pk,//マップＩＤ
    mdmap_id_kantoku,//監督ＩＤ
    mdmap_mapNo,//マップ番号
    mdmap_id_m_maptip,//マップチップＩＤ
    mdmap_id_m_event,//イベントＩＤ
    mdmap_enemyNo,//クエスト敵番号
    mdmap_dropNo,//アイテム番号
    mdmap_shiainum,//対戦数
    mdmap_winnum,//勝利数
    mdmap_end_f,//試合終了フラグ
    mdmap_create_date,//作成日時
    mdmap_updated_date,//更新日時
    mdmap_del_f,//削除フラグ
  };
  
  const std::string MAPCLM[] = 
  {
    "pk",//マップＩＤ
    "id_kantoku",//監督ＩＤ
    "mapNo",//マップ番号
    "id_m_maptip",//マップチップＩＤ
    "id_m_event",//イベントＩＤ
    "enemyNo",//クエスト敵番号
    "dropNo",//アイテム番号
    "shiainum",//対戦数
    "winnum",//勝利数
    "end_f",//試合終了フラグ
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdMap: public TModelGen
  {
  protected:

  public:
    explicit TMdMap();
    virtual ~TMdMap();
    s64 GetLineDt(s64 line, enmdMap dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdMap dtNo);
    s64 GetPKDt(s64 pk, enmdMap dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdMap dtNo);
  };
}
