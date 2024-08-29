#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdTeam
  {
    mdteam_pk,//ユーザーチームＩＤ
    mdteam_id_kantoku,//保持監督ＩＤ
    mdteam_id_m_team,//クライアントマスターチームＩＤ
    mdteam_id_shset,//シュートセットＩＤ
    mdteam_overlap,//限界突破回数
    mdteam_useNum,//使用回数
    mdteam_useSec,//最終使用日時（秒）
    mdteam_nigun_f,//二軍フラグ
    mdteam_id_shot00,//slot00
    mdteam_id_shot01,//slot01
    mdteam_id_shot02,//slot02
    mdteam_id_shot03,//slot03
    mdteam_id_shot04,//slot04
    mdteam_id_shot05,//slot05
    mdteam_id_shot06,//slot06
    mdteam_id_shot07,//slot07
    mdteam_id_shot08,//slot08
    mdteam_id_shot09,//slot09
    mdteam_id_shot10,//slot10
    mdteam_id_shot11,//slot11
    mdteam_id_shot12,//slot12
    mdteam_id_shot13,//slot13
    mdteam_id_shot14,//slot14
    mdteam_id_shot15,//slot15
    mdteam_id_shot16,//slot16
    mdteam_id_shot17,//slot17
    mdteam_id_shot18,//slot18
    mdteam_id_shot19,//slot19
    mdteam_id_shot20,//slot20
    mdteam_id_shot21,//slot21
    mdteam_id_shot22,//slot22
    mdteam_id_shot23,//slot23
    mdteam_id_shot24,//slot24
    mdteam_id_shot25,//slot25
    mdteam_id_shot26,//slot26
    mdteam_id_shot27,//slot27
    mdteam_id_shot28,//slot28
    mdteam_id_shot29,//slot29
    mdteam_id_shot30,//slot30
    mdteam_id_shot31,//slot31
    mdteam_id_shot32,//slot32
    mdteam_id_shot33,//slot33
    mdteam_id_shot34,//slot34
    mdteam_id_shot35,//slot35
    mdteam_id_shot36,//slot36
    mdteam_id_shot37,//slot37
    mdteam_id_shot38,//slot38
    mdteam_id_shot39,//slot39
    mdteam_id_shot40,//slot40
    mdteam_id_shot41,//slot41
    mdteam_id_shot42,//slot42
    mdteam_id_shot43,//slot43
    mdteam_id_shot44,//slot44
    mdteam_id_shot45,//slot45
    mdteam_id_shot46,//slot46
    mdteam_id_shot47,//slot47
    mdteam_id_shot48,//slot48
    mdteam_create_date,//作成日時
    mdteam_updated_date,//更新日時
    mdteam_del_f,//削除フラグ
  };
  
  const std::string TEAMCLM[] = 
  {
    "pk",//ユーザーチームＩＤ
    "id_kantoku",//保持監督ＩＤ
    "id_m_team",//クライアントマスターチームＩＤ
    "id_shset",//シュートセットＩＤ
    "overlap",//限界突破回数
    "useNum",//使用回数
    "useSec",//最終使用日時（秒）
    "nigun_f",//二軍フラグ
    "id_shot00",//slot00
    "id_shot01",//slot01
    "id_shot02",//slot02
    "id_shot03",//slot03
    "id_shot04",//slot04
    "id_shot05",//slot05
    "id_shot06",//slot06
    "id_shot07",//slot07
    "id_shot08",//slot08
    "id_shot09",//slot09
    "id_shot10",//slot10
    "id_shot11",//slot11
    "id_shot12",//slot12
    "id_shot13",//slot13
    "id_shot14",//slot14
    "id_shot15",//slot15
    "id_shot16",//slot16
    "id_shot17",//slot17
    "id_shot18",//slot18
    "id_shot19",//slot19
    "id_shot20",//slot20
    "id_shot21",//slot21
    "id_shot22",//slot22
    "id_shot23",//slot23
    "id_shot24",//slot24
    "id_shot25",//slot25
    "id_shot26",//slot26
    "id_shot27",//slot27
    "id_shot28",//slot28
    "id_shot29",//slot29
    "id_shot30",//slot30
    "id_shot31",//slot31
    "id_shot32",//slot32
    "id_shot33",//slot33
    "id_shot34",//slot34
    "id_shot35",//slot35
    "id_shot36",//slot36
    "id_shot37",//slot37
    "id_shot38",//slot38
    "id_shot39",//slot39
    "id_shot40",//slot40
    "id_shot41",//slot41
    "id_shot42",//slot42
    "id_shot43",//slot43
    "id_shot44",//slot44
    "id_shot45",//slot45
    "id_shot46",//slot46
    "id_shot47",//slot47
    "id_shot48",//slot48
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdTeam: public TModelGen
  {
  protected:

  public:
    explicit TMdTeam();
    virtual ~TMdTeam();
    s64 GetLineDt(s64 line, enmdTeam dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdTeam dtNo);
    s64 GetPKDt(s64 pk, enmdTeam dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdTeam dtNo);
  };
}
