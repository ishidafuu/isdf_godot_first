#include "dbMdm_Slot.h"
namespace db
{
  TMdm_Slot::TMdm_Slot():TModelGen("m_Slot")
  {
  }
  TMdm_Slot::~TMdm_Slot(void)
  {
  }
  void TMdm_Slot::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Slot", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_slot_pk));
      model_.intvecmap_["slot00"].push_back(csv.GetInt(i, mdm_slot_slot00));
      model_.intvecmap_["slot01"].push_back(csv.GetInt(i, mdm_slot_slot01));
      model_.intvecmap_["slot02"].push_back(csv.GetInt(i, mdm_slot_slot02));
      model_.intvecmap_["slot03"].push_back(csv.GetInt(i, mdm_slot_slot03));
      model_.intvecmap_["slot04"].push_back(csv.GetInt(i, mdm_slot_slot04));
      model_.intvecmap_["slot05"].push_back(csv.GetInt(i, mdm_slot_slot05));
      model_.intvecmap_["slot06"].push_back(csv.GetInt(i, mdm_slot_slot06));
      model_.intvecmap_["slot07"].push_back(csv.GetInt(i, mdm_slot_slot07));
      model_.intvecmap_["slot08"].push_back(csv.GetInt(i, mdm_slot_slot08));
      model_.intvecmap_["slot09"].push_back(csv.GetInt(i, mdm_slot_slot09));
      model_.intvecmap_["slot10"].push_back(csv.GetInt(i, mdm_slot_slot10));
      model_.intvecmap_["slot11"].push_back(csv.GetInt(i, mdm_slot_slot11));
      model_.intvecmap_["slot12"].push_back(csv.GetInt(i, mdm_slot_slot12));
      model_.intvecmap_["slot13"].push_back(csv.GetInt(i, mdm_slot_slot13));
      model_.intvecmap_["slot14"].push_back(csv.GetInt(i, mdm_slot_slot14));
      model_.intvecmap_["slot15"].push_back(csv.GetInt(i, mdm_slot_slot15));
      model_.intvecmap_["slot16"].push_back(csv.GetInt(i, mdm_slot_slot16));
      model_.intvecmap_["slot17"].push_back(csv.GetInt(i, mdm_slot_slot17));
      model_.intvecmap_["slot18"].push_back(csv.GetInt(i, mdm_slot_slot18));
      model_.intvecmap_["slot19"].push_back(csv.GetInt(i, mdm_slot_slot19));
      model_.intvecmap_["slot20"].push_back(csv.GetInt(i, mdm_slot_slot20));
      model_.intvecmap_["slot21"].push_back(csv.GetInt(i, mdm_slot_slot21));
      model_.intvecmap_["slot22"].push_back(csv.GetInt(i, mdm_slot_slot22));
      model_.intvecmap_["slot23"].push_back(csv.GetInt(i, mdm_slot_slot23));
      model_.intvecmap_["slot24"].push_back(csv.GetInt(i, mdm_slot_slot24));
      model_.intvecmap_["slot25"].push_back(csv.GetInt(i, mdm_slot_slot25));
      model_.intvecmap_["slot26"].push_back(csv.GetInt(i, mdm_slot_slot26));
      model_.intvecmap_["slot27"].push_back(csv.GetInt(i, mdm_slot_slot27));
      model_.intvecmap_["slot28"].push_back(csv.GetInt(i, mdm_slot_slot28));
      model_.intvecmap_["slot29"].push_back(csv.GetInt(i, mdm_slot_slot29));
      model_.intvecmap_["slot30"].push_back(csv.GetInt(i, mdm_slot_slot30));
      model_.intvecmap_["slot31"].push_back(csv.GetInt(i, mdm_slot_slot31));
      model_.intvecmap_["slot32"].push_back(csv.GetInt(i, mdm_slot_slot32));
      model_.intvecmap_["slot33"].push_back(csv.GetInt(i, mdm_slot_slot33));
      model_.intvecmap_["slot34"].push_back(csv.GetInt(i, mdm_slot_slot34));
      model_.intvecmap_["slot35"].push_back(csv.GetInt(i, mdm_slot_slot35));
      model_.intvecmap_["slot36"].push_back(csv.GetInt(i, mdm_slot_slot36));
      model_.intvecmap_["slot37"].push_back(csv.GetInt(i, mdm_slot_slot37));
      model_.intvecmap_["slot38"].push_back(csv.GetInt(i, mdm_slot_slot38));
      model_.intvecmap_["slot39"].push_back(csv.GetInt(i, mdm_slot_slot39));
      model_.intvecmap_["slot40"].push_back(csv.GetInt(i, mdm_slot_slot40));
      model_.intvecmap_["slot41"].push_back(csv.GetInt(i, mdm_slot_slot41));
      model_.intvecmap_["slot42"].push_back(csv.GetInt(i, mdm_slot_slot42));
      model_.intvecmap_["slot43"].push_back(csv.GetInt(i, mdm_slot_slot43));
      model_.intvecmap_["slot44"].push_back(csv.GetInt(i, mdm_slot_slot44));
      model_.intvecmap_["slot45"].push_back(csv.GetInt(i, mdm_slot_slot45));
      model_.intvecmap_["slot46"].push_back(csv.GetInt(i, mdm_slot_slot46));
      model_.intvecmap_["slot47"].push_back(csv.GetInt(i, mdm_slot_slot47));
      model_.intvecmap_["slot48"].push_back(csv.GetInt(i, mdm_slot_slot48));
    }
    
  }
  s64 TMdm_Slot::GetLineDt(s64 line, enmdm_Slot dtNo)
  {
    return GenGetLineDt(line, M_SLOTCLM[dtNo].c_str());
  }
  BOOL TMdm_Slot::GetLineDtBOOL(s64 line, enmdm_Slot dtNo)
  {
    return GenGetLineDtBOOL(line, M_SLOTCLM[dtNo].c_str());
  }
  s64 TMdm_Slot::GetPKDt(s64 pk, enmdm_Slot dtNo)
  {
    return GenGetPKDt(pk, M_SLOTCLM[dtNo].c_str());
  }
  BOOL TMdm_Slot::GetPKDtBOOL(s64 pk, enmdm_Slot dtNo)
  {
    return GenGetPKDtBOOL(pk, M_SLOTCLM[dtNo].c_str());
  }
}
