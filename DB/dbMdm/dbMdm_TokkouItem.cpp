#include "dbMdm_TokkouItem.h"
namespace db
{
  TMdm_TokkouItem::TMdm_TokkouItem():TModelGen("m_TokkouItem")
  {
  }
  TMdm_TokkouItem::~TMdm_TokkouItem(void)
  {
  }
  void TMdm_TokkouItem::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_TokkouItem", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_tokkouitem_pk));
      model_.intvecmap_["tokkouNo"].push_back(csv.GetInt(i, mdm_tokkouitem_tokkouNo));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_tokkouitem_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_tokkouitem_itemID));
      model_.intvecmap_["effectType"].push_back(csv.GetInt(i, mdm_tokkouitem_effectType));
    }
    
  }
  s64 TMdm_TokkouItem::GetLineDt(s64 line, enmdm_TokkouItem dtNo)
  {
    return GenGetLineDt(line, M_TOKKOUITEMCLM[dtNo].c_str());
  }
  BOOL TMdm_TokkouItem::GetLineDtBOOL(s64 line, enmdm_TokkouItem dtNo)
  {
    return GenGetLineDtBOOL(line, M_TOKKOUITEMCLM[dtNo].c_str());
  }
  s64 TMdm_TokkouItem::GetPKDt(s64 pk, enmdm_TokkouItem dtNo)
  {
    return GenGetPKDt(pk, M_TOKKOUITEMCLM[dtNo].c_str());
  }
  BOOL TMdm_TokkouItem::GetPKDtBOOL(s64 pk, enmdm_TokkouItem dtNo)
  {
    return GenGetPKDtBOOL(pk, M_TOKKOUITEMCLM[dtNo].c_str());
  }
}
