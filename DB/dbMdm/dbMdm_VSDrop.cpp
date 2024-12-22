#include "dbMdm_VSDrop.h"
namespace db
{
  TMdm_VSDrop::TMdm_VSDrop():TModelGen("m_VSDrop")
  {
  }
  TMdm_VSDrop::~TMdm_VSDrop(void)
  {
  }
  void TMdm_VSDrop::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_VSDrop", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_vsdrop_pk));
      model_.intvecmap_["prob_hw"].push_back(csv.GetInt(i, mdm_vsdrop_prob_hw));
      model_.intvecmap_["prob_hl"].push_back(csv.GetInt(i, mdm_vsdrop_prob_hl));
      model_.intvecmap_["prob_cw"].push_back(csv.GetInt(i, mdm_vsdrop_prob_cw));
      model_.intvecmap_["prob_cl"].push_back(csv.GetInt(i, mdm_vsdrop_prob_cl));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_vsdrop_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_vsdrop_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_vsdrop_itemNum));
    }
    
  }
  s64 TMdm_VSDrop::GetLineDt(s64 line, enmdm_VSDrop dtNo)
  {
    return GenGetLineDt(line, M_VSDROPCLM[dtNo].c_str());
  }
  BOOL TMdm_VSDrop::GetLineDtBOOL(s64 line, enmdm_VSDrop dtNo)
  {
    return GenGetLineDtBOOL(line, M_VSDROPCLM[dtNo].c_str());
  }
  s64 TMdm_VSDrop::GetPKDt(s64 pk, enmdm_VSDrop dtNo)
  {
    return GenGetPKDt(pk, M_VSDROPCLM[dtNo].c_str());
  }
  BOOL TMdm_VSDrop::GetPKDtBOOL(s64 pk, enmdm_VSDrop dtNo)
  {
    return GenGetPKDtBOOL(pk, M_VSDROPCLM[dtNo].c_str());
  }
}
