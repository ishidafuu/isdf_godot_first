#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdlog_ShiaiTmat
  {
    mdlog_shiaitmat_pk,//試合ＩＤ
    mdlog_shiaitmat_id_kantoku,//監督ＩＤ
    mdlog_shiaitmat_id_team,//使用チームＩＤ
    mdlog_shiaitmat_id_m_team,//使用チームマスターＩＤ
    mdlog_shiaitmat_id_m_event,//イベントＩＤ
    mdlog_shiaitmat_result,//結果 0NONE 1WIN 2LOSE 3DRAW 4RETIRE
    mdlog_shiaitmat_killNum,//倒した
    mdlog_shiaitmat_deathNum,//倒された
    mdlog_shiaitmat_maxDam,//最大ダメージ
    mdlog_shiaitmat_totalDam,//合計ダメージ
    mdlog_shiaitmat_shiaiTime,//試合時間
    mdlog_shiaitmat_create_date,//作成日時
    mdlog_shiaitmat_updated_date,//更新日時
    mdlog_shiaitmat_del_f,//削除フラグ
  };
  
  const std::string LOG_SHIAITMATCLM[] = 
  {
    "pk",//試合ＩＤ
    "id_kantoku",//監督ＩＤ
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
  
  class TMdlog_ShiaiTmat: public TModelGen
  {
  protected:

  public:
    explicit TMdlog_ShiaiTmat();
    virtual ~TMdlog_ShiaiTmat();
    s64 GetLineDt(s64 line, enmdlog_ShiaiTmat dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdlog_ShiaiTmat dtNo);
    s64 GetPKDt(s64 pk, enmdlog_ShiaiTmat dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdlog_ShiaiTmat dtNo);
  };
}
