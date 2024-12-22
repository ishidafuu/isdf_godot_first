#include "dbMdm_GachaCnt.h"
namespace db
{
  TMdm_GachaCnt::TMdm_GachaCnt():TModelGen("m_GachaCnt")
  {
  }
  TMdm_GachaCnt::~TMdm_GachaCnt(void)
  {
  }
  void TMdm_GachaCnt::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_GachaCnt", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_gachacnt_pk));
      model_.intvecmap_["cntGrpNo"].push_back(csv.GetInt(i, mdm_gachacnt_cntGrpNo));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_gachacnt_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_gachacnt_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_gachacnt_itemNum));
      model_.intvecmap_["prob"].push_back(csv.GetInt(i, mdm_gachacnt_prob));
    }
    
  }
  s64 TMdm_GachaCnt::GetLineDt(s64 line, enmdm_GachaCnt dtNo)
  {
    return GenGetLineDt(line, M_GACHACNTCLM[dtNo].c_str());
  }
  BOOL TMdm_GachaCnt::GetLineDtBOOL(s64 line, enmdm_GachaCnt dtNo)
  {
    return GenGetLineDtBOOL(line, M_GACHACNTCLM[dtNo].c_str());
  }
  s64 TMdm_GachaCnt::GetPKDt(s64 pk, enmdm_GachaCnt dtNo)
  {
    return GenGetPKDt(pk, M_GACHACNTCLM[dtNo].c_str());
  }
  BOOL TMdm_GachaCnt::GetPKDtBOOL(s64 pk, enmdm_GachaCnt dtNo)
  {
    return GenGetPKDtBOOL(pk, M_GACHACNTCLM[dtNo].c_str());
  }
}
