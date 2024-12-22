#include "dbMdm_LogboSozai.h"
namespace db
{
  TMdm_LogboSozai::TMdm_LogboSozai():TModelGen("m_LogboSozai")
  {
  }
  TMdm_LogboSozai::~TMdm_LogboSozai(void)
  {
  }
  void TMdm_LogboSozai::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_LogboSozai", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_logbosozai_pk));
      model_.intvecmap_["logboType"].push_back(csv.GetInt(i, mdm_logbosozai_logboType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_logbosozai_itemID));
    }
    
  }
  s64 TMdm_LogboSozai::GetLineDt(s64 line, enmdm_LogboSozai dtNo)
  {
    return GenGetLineDt(line, M_LOGBOSOZAICLM[dtNo].c_str());
  }
  BOOL TMdm_LogboSozai::GetLineDtBOOL(s64 line, enmdm_LogboSozai dtNo)
  {
    return GenGetLineDtBOOL(line, M_LOGBOSOZAICLM[dtNo].c_str());
  }
  s64 TMdm_LogboSozai::GetPKDt(s64 pk, enmdm_LogboSozai dtNo)
  {
    return GenGetPKDt(pk, M_LOGBOSOZAICLM[dtNo].c_str());
  }
  BOOL TMdm_LogboSozai::GetPKDtBOOL(s64 pk, enmdm_LogboSozai dtNo)
  {
    return GenGetPKDtBOOL(pk, M_LOGBOSOZAICLM[dtNo].c_str());
  }
}
