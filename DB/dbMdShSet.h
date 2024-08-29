#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdShSet
  {
    mdshset_pk,//シュートセットＩＤ
    mdshset_id_kantoku,//保持監督ＩＤ
    mdshset_id_shot00,//slot00
    mdshset_id_shot01,//slot01
    mdshset_id_shot02,//slot02
    mdshset_id_shot03,//slot03
    mdshset_id_shot04,//slot04
    mdshset_id_shot05,//slot05
    mdshset_id_shot06,//slot06
    mdshset_id_shot07,//slot07
    mdshset_id_shot08,//slot08
    mdshset_id_shot09,//slot09
    mdshset_id_shot10,//slot10
    mdshset_id_shot11,//slot11
    mdshset_id_shot12,//slot12
    mdshset_id_shot13,//slot13
    mdshset_id_shot14,//slot14
    mdshset_id_shot15,//slot15
    mdshset_id_shot16,//slot16
    mdshset_id_shot17,//slot17
    mdshset_id_shot18,//slot18
    mdshset_id_shot19,//slot19
    mdshset_id_shot20,//slot20
    mdshset_id_shot21,//slot21
    mdshset_id_shot22,//slot22
    mdshset_id_shot23,//slot23
    mdshset_id_shot24,//slot24
    mdshset_id_shot25,//slot25
    mdshset_id_shot26,//slot26
    mdshset_id_shot27,//slot27
    mdshset_id_shot28,//slot28
    mdshset_id_shot29,//slot29
    mdshset_id_shot30,//slot30
    mdshset_id_shot31,//slot31
    mdshset_id_shot32,//slot32
    mdshset_id_shot33,//slot33
    mdshset_id_shot34,//slot34
    mdshset_id_shot35,//slot35
    mdshset_id_shot36,//slot36
    mdshset_id_shot37,//slot37
    mdshset_id_shot38,//slot38
    mdshset_id_shot39,//slot39
    mdshset_id_shot40,//slot40
    mdshset_id_shot41,//slot41
    mdshset_id_shot42,//slot42
    mdshset_id_shot43,//slot43
    mdshset_id_shot44,//slot44
    mdshset_id_shot45,//slot45
    mdshset_id_shot46,//slot46
    mdshset_id_shot47,//slot47
    mdshset_id_shot48,//slot48
    mdshset_create_date,//作成日時
    mdshset_updated_date,//更新日時
    mdshset_del_f,//削除フラグ
  };
  
  enum enmdShSetStr
  {
    mdshset_name_c32,//オリジナル名
  };
  

  const std::string SHSETCLM[] = 
  {
    "pk",//シュートセットＩＤ
    "id_kantoku",//保持監督ＩＤ
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
  
  const std::string SHSETCLMSTR[] = 
  {
    "name_c32",//オリジナル名
  };
  
  class TMdShSet: public TModelGen
  {
  protected:

  public:
    explicit TMdShSet();
    virtual ~TMdShSet();
    s64 GetLineDt(s64 line, enmdShSet dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdShSet dtNo);
    std::string GetLineDtStr(s64 line, enmdShSetStr dtNo);
    s64 GetPKDt(s64 pk, enmdShSet dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdShSet dtNo);
    std::string GetPKDtStr(s64 pk, enmdShSetStr dtNo);
  };
}
