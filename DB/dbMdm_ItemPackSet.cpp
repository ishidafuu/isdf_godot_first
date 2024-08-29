#include "dbMdm_ItemPackSet.h"
namespace db
{
  TMdm_ItemPackSet::TMdm_ItemPackSet():TModelGen("m_ItemPackSet")
  {
  }
  TMdm_ItemPackSet::~TMdm_ItemPackSet(void)
  {
  }
  void TMdm_ItemPackSet::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_ItemPackSet", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_itempackset_pk));
      model_.intvecmap_["id_m_itempack"].push_back(csv.GetInt(i, mdm_itempackset_id_m_itempack));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_itempackset_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_itempackset_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_itempackset_itemNum));
    }
    
  }
  s64 TMdm_ItemPackSet::GetLineDt(s64 line, enmdm_ItemPackSet dtNo)
  {
    return GenGetLineDt(line, M_ITEMPACKSETCLM[dtNo].c_str());
  }
  BOOL TMdm_ItemPackSet::GetLineDtBOOL(s64 line, enmdm_ItemPackSet dtNo)
  {
    return GenGetLineDtBOOL(line, M_ITEMPACKSETCLM[dtNo].c_str());
  }
  s64 TMdm_ItemPackSet::GetPKDt(s64 pk, enmdm_ItemPackSet dtNo)
  {
    return GenGetPKDt(pk, M_ITEMPACKSETCLM[dtNo].c_str());
  }
  BOOL TMdm_ItemPackSet::GetPKDtBOOL(s64 pk, enmdm_ItemPackSet dtNo)
  {
    return GenGetPKDtBOOL(pk, M_ITEMPACKSETCLM[dtNo].c_str());
  }
}
