#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdDock
  {
    mddock_pk,//ドックＩＤ
    mddock_id_kantoku,//保持監督ＩＤ
    mddock_state,//状態 0FREE 1GOUSEI
    mddock_start_date,//開始日時
    mddock_end_date,//完成日時
    mddock_id_shot0,//slot00
    mddock_id_shot1,//slot01
    mddock_id_shot2,//slot02
    mddock_id_shot3,//slot03
    mddock_id_shot4,//slot04
    mddock_id_shot5,//slot05
    mddock_id_shot6,//slot06
    mddock_id_shot7,//slot07
    mddock_create_date,//作成日時
    mddock_updated_date,//更新日時
    mddock_del_f,//削除フラグ
  };
  
  const std::string DOCKCLM[] = 
  {
    "pk",//ドックＩＤ
    "id_kantoku",//保持監督ＩＤ
    "state",//状態 0FREE 1GOUSEI
    "start_date",//開始日時
    "end_date",//完成日時
    "id_shot0",//slot00
    "id_shot1",//slot01
    "id_shot2",//slot02
    "id_shot3",//slot03
    "id_shot4",//slot04
    "id_shot5",//slot05
    "id_shot6",//slot06
    "id_shot7",//slot07
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdDock: public TModelGen
  {
  protected:

  public:
    explicit TMdDock();
    virtual ~TMdDock();
    s64 GetLineDt(s64 line, enmdDock dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdDock dtNo);
    s64 GetPKDt(s64 pk, enmdDock dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdDock dtNo);
  };
}
