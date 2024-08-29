#include "dbMdm_Drop.h"
namespace db
{
  TMdm_Drop::TMdm_Drop():TModelGen("m_Drop")
  {
  }
  TMdm_Drop::~TMdm_Drop(void)
  {
  }
  void TMdm_Drop::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Drop", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_drop_pk));
      model_.intvecmap_["itemType_0"].push_back(csv.GetInt(i, mdm_drop_itemType_0));
      model_.intvecmap_["itemID_0"].push_back(csv.GetInt(i, mdm_drop_itemID_0));
      model_.intvecmap_["itemNum_0"].push_back(csv.GetInt(i, mdm_drop_itemNum_0));
      model_.intvecmap_["itemType_1"].push_back(csv.GetInt(i, mdm_drop_itemType_1));
      model_.intvecmap_["itemID_1"].push_back(csv.GetInt(i, mdm_drop_itemID_1));
      model_.intvecmap_["itemNum_1"].push_back(csv.GetInt(i, mdm_drop_itemNum_1));
      model_.intvecmap_["itemType_2"].push_back(csv.GetInt(i, mdm_drop_itemType_2));
      model_.intvecmap_["itemID_2"].push_back(csv.GetInt(i, mdm_drop_itemID_2));
      model_.intvecmap_["itemNum_2"].push_back(csv.GetInt(i, mdm_drop_itemNum_2));
      model_.intvecmap_["itemType_Cmp"].push_back(csv.GetInt(i, mdm_drop_itemType_Cmp));
      model_.intvecmap_["itemID_Cmp"].push_back(csv.GetInt(i, mdm_drop_itemID_Cmp));
      model_.intvecmap_["itemNum_Cmp"].push_back(csv.GetInt(i, mdm_drop_itemNum_Cmp));
    }
    
  }
  s64 TMdm_Drop::GetLineDt(s64 line, enmdm_Drop dtNo)
  {
    return GenGetLineDt(line, M_DROPCLM[dtNo].c_str());
  }
  BOOL TMdm_Drop::GetLineDtBOOL(s64 line, enmdm_Drop dtNo)
  {
    return GenGetLineDtBOOL(line, M_DROPCLM[dtNo].c_str());
  }
  s64 TMdm_Drop::GetPKDt(s64 pk, enmdm_Drop dtNo)
  {
    return GenGetPKDt(pk, M_DROPCLM[dtNo].c_str());
  }
  BOOL TMdm_Drop::GetPKDtBOOL(s64 pk, enmdm_Drop dtNo)
  {
    return GenGetPKDtBOOL(pk, M_DROPCLM[dtNo].c_str());
  }
}
