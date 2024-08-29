#include "dbMdm_NaBoxCnt.h"
namespace db
{
  TMdm_NaBoxCnt::TMdm_NaBoxCnt():TModelGen("m_NaBoxCnt")
  {
  }
  TMdm_NaBoxCnt::~TMdm_NaBoxCnt(void)
  {
  }
  void TMdm_NaBoxCnt::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_NaBoxCnt", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_naboxcnt_pk));
      model_.intvecmap_["id_m_nabox"].push_back(csv.GetInt(i, mdm_naboxcnt_id_m_nabox));
      model_.intvecmap_["boxNo"].push_back(csv.GetInt(i, mdm_naboxcnt_boxNo));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_naboxcnt_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_naboxcnt_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_naboxcnt_itemNum));
    }
    
  }
  s64 TMdm_NaBoxCnt::GetLineDt(s64 line, enmdm_NaBoxCnt dtNo)
  {
    return GenGetLineDt(line, M_NABOXCNTCLM[dtNo].c_str());
  }
  BOOL TMdm_NaBoxCnt::GetLineDtBOOL(s64 line, enmdm_NaBoxCnt dtNo)
  {
    return GenGetLineDtBOOL(line, M_NABOXCNTCLM[dtNo].c_str());
  }
  s64 TMdm_NaBoxCnt::GetPKDt(s64 pk, enmdm_NaBoxCnt dtNo)
  {
    return GenGetPKDt(pk, M_NABOXCNTCLM[dtNo].c_str());
  }
  BOOL TMdm_NaBoxCnt::GetPKDtBOOL(s64 pk, enmdm_NaBoxCnt dtNo)
  {
    return GenGetPKDtBOOL(pk, M_NABOXCNTCLM[dtNo].c_str());
  }
}
