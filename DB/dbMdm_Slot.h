#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Slot
  {
    mdm_slot_pk,//マスタースロットオープンＩＤ
    mdm_slot_slot00,//slot00解放
    mdm_slot_slot01,//slot01解放
    mdm_slot_slot02,//slot02解放
    mdm_slot_slot03,//slot03解放
    mdm_slot_slot04,//slot04解放
    mdm_slot_slot05,//slot05解放
    mdm_slot_slot06,//slot06解放
    mdm_slot_slot07,//slot07解放
    mdm_slot_slot08,//slot08解放
    mdm_slot_slot09,//slot09解放
    mdm_slot_slot10,//slot10解放
    mdm_slot_slot11,//slot11解放
    mdm_slot_slot12,//slot12解放
    mdm_slot_slot13,//slot13解放
    mdm_slot_slot14,//slot14解放
    mdm_slot_slot15,//slot15解放
    mdm_slot_slot16,//slot16解放
    mdm_slot_slot17,//slot17解放
    mdm_slot_slot18,//slot18解放
    mdm_slot_slot19,//slot19解放
    mdm_slot_slot20,//slot20解放
    mdm_slot_slot21,//slot21解放
    mdm_slot_slot22,//slot22解放
    mdm_slot_slot23,//slot23解放
    mdm_slot_slot24,//slot24解放
    mdm_slot_slot25,//slot25解放
    mdm_slot_slot26,//slot26解放
    mdm_slot_slot27,//slot27解放
    mdm_slot_slot28,//slot28解放
    mdm_slot_slot29,//slot29解放
    mdm_slot_slot30,//slot30解放
    mdm_slot_slot31,//slot31解放
    mdm_slot_slot32,//slot32解放
    mdm_slot_slot33,//slot33解放
    mdm_slot_slot34,//slot34解放
    mdm_slot_slot35,//slot35解放
    mdm_slot_slot36,//slot36解放
    mdm_slot_slot37,//slot37解放
    mdm_slot_slot38,//slot38解放
    mdm_slot_slot39,//slot39解放
    mdm_slot_slot40,//slot40解放
    mdm_slot_slot41,//slot41解放
    mdm_slot_slot42,//slot42解放
    mdm_slot_slot43,//slot43解放
    mdm_slot_slot44,//slot44解放
    mdm_slot_slot45,//slot45解放
    mdm_slot_slot46,//slot46解放
    mdm_slot_slot47,//slot47解放
    mdm_slot_slot48,//slot48解放
  };
  
  const std::string M_SLOTCLM[] = 
  {
    "pk",//マスタースロットオープンＩＤ
    "slot00",//slot00解放
    "slot01",//slot01解放
    "slot02",//slot02解放
    "slot03",//slot03解放
    "slot04",//slot04解放
    "slot05",//slot05解放
    "slot06",//slot06解放
    "slot07",//slot07解放
    "slot08",//slot08解放
    "slot09",//slot09解放
    "slot10",//slot10解放
    "slot11",//slot11解放
    "slot12",//slot12解放
    "slot13",//slot13解放
    "slot14",//slot14解放
    "slot15",//slot15解放
    "slot16",//slot16解放
    "slot17",//slot17解放
    "slot18",//slot18解放
    "slot19",//slot19解放
    "slot20",//slot20解放
    "slot21",//slot21解放
    "slot22",//slot22解放
    "slot23",//slot23解放
    "slot24",//slot24解放
    "slot25",//slot25解放
    "slot26",//slot26解放
    "slot27",//slot27解放
    "slot28",//slot28解放
    "slot29",//slot29解放
    "slot30",//slot30解放
    "slot31",//slot31解放
    "slot32",//slot32解放
    "slot33",//slot33解放
    "slot34",//slot34解放
    "slot35",//slot35解放
    "slot36",//slot36解放
    "slot37",//slot37解放
    "slot38",//slot38解放
    "slot39",//slot39解放
    "slot40",//slot40解放
    "slot41",//slot41解放
    "slot42",//slot42解放
    "slot43",//slot43解放
    "slot44",//slot44解放
    "slot45",//slot45解放
    "slot46",//slot46解放
    "slot47",//slot47解放
    "slot48",//slot48解放
  };
  
  class TMdm_Slot: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Slot();
    virtual ~TMdm_Slot();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Slot dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Slot dtNo);
    s64 GetPKDt(s64 pk, enmdm_Slot dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Slot dtNo);
  };
}
