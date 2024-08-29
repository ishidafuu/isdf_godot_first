#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_ShiaiTour
  {
    mdlog_shiaitour_pk,//試合ＩＤ
    mdlog_shiaitour_id_kantoku,//監督ＩＤ
    mdlog_shiaitour_id_kantoku_en,//敵監督ＩＤ
    mdlog_shiaitour_id_team,//使用チームＩＤ
    mdlog_shiaitour_id_m_team,//使用チームマスターＩＤ
    mdlog_shiaitour_id_m_event,//イベントＩＤ
    mdlog_shiaitour_result,//結果 0NONE 1WIN 2LOSE 3DRAW 4RETIRE
    mdlog_shiaitour_killNum,//倒した
    mdlog_shiaitour_deathNum,//倒された
    mdlog_shiaitour_maxDam,//最大ダメージ
    mdlog_shiaitour_totalDam,//合計ダメージ
    mdlog_shiaitour_shiaiTime,//試合時間
    mdlog_shiaitour_create_date,//作成日時
    mdlog_shiaitour_updated_date,//更新日時
    mdlog_shiaitour_del_f,//削除フラグ
  };
  
  const std::string LOG_SHIAITOURCLM[] = 
  {
    "pk",//試合ＩＤ
    "id_kantoku",//監督ＩＤ
    "id_kantoku_en",//敵監督ＩＤ
    "id_team",//使用チームＩＤ
    "id_m_team",//使用チームマスターＩＤ
    "id_m_event",//イベントＩＤ
    "result",//結果 0NONE 1WIN 2LOSE 3DRAW 4RETIRE
    "killNum",//倒した
    "deathNum",//倒された
    "maxDam",//最大ダメージ
    "totalDam",//合計ダメージ
    "shiaiTime",//試合時間
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdlog_ShiaiTour: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_ShiaiTour();
    virtual ~TMdlog_ShiaiTour();
    s64 GetLineDt(s64 line, enmdlog_ShiaiTour dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_ShiaiTour dtNo);
    s64 GetPKDt(s64 pk, enmdlog_ShiaiTour dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_ShiaiTour dtNo);
  };
}
