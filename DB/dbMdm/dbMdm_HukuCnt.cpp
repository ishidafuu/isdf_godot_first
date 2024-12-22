#include "dbMdm_HukuCnt.h"
namespace db
{
  TMdm_HukuCnt::TMdm_HukuCnt():TModelGen("m_HukuCnt")
  {
  }
  TMdm_HukuCnt::~TMdm_HukuCnt(void)
  {
  }
  void TMdm_HukuCnt::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_HukuCnt", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_hukucnt_pk));
      model_.intvecmap_["id_w_hukugrp"].push_back(csv.GetInt(i, mdm_hukucnt_id_w_hukugrp));
      model_.intvecmap_["grade"].push_back(csv.GetInt(i, mdm_hukucnt_grade));
      model_.intvecmap_["num"].push_back(csv.GetInt(i, mdm_hukucnt_num));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_hukucnt_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_hukucnt_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_hukucnt_itemNum));
    }
    
  }
  s64 TMdm_HukuCnt::GetLineDt(s64 line, enmdm_HukuCnt dtNo)
  {
    return GenGetLineDt(line, M_HUKUCNTCLM[dtNo].c_str());
  }
  BOOL TMdm_HukuCnt::GetLineDtBOOL(s64 line, enmdm_HukuCnt dtNo)
  {
    return GenGetLineDtBOOL(line, M_HUKUCNTCLM[dtNo].c_str());
  }
  s64 TMdm_HukuCnt::GetPKDt(s64 pk, enmdm_HukuCnt dtNo)
  {
    return GenGetPKDt(pk, M_HUKUCNTCLM[dtNo].c_str());
  }
  BOOL TMdm_HukuCnt::GetPKDtBOOL(s64 pk, enmdm_HukuCnt dtNo)
  {
    return GenGetPKDtBOOL(pk, M_HUKUCNTCLM[dtNo].c_str());
  }
}
