#include "dbMdm_LvUp.h"
namespace db
{
  TMdm_LvUp::TMdm_LvUp():TModelGen("m_LvUp")
  {
  }
  TMdm_LvUp::~TMdm_LvUp(void)
  {
  }
  void TMdm_LvUp::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_LvUp", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_lvup_pk));
      model_.intvecmap_["exp_for_next"].push_back(csv.GetInt(i, mdm_lvup_exp_for_next));
    }
    
  }
  s64 TMdm_LvUp::GetLineDt(s64 line, enmdm_LvUp dtNo)
  {
    return GenGetLineDt(line, M_LVUPCLM[dtNo].c_str());
  }
  BOOL TMdm_LvUp::GetLineDtBOOL(s64 line, enmdm_LvUp dtNo)
  {
    return GenGetLineDtBOOL(line, M_LVUPCLM[dtNo].c_str());
  }
  s64 TMdm_LvUp::GetPKDt(s64 pk, enmdm_LvUp dtNo)
  {
    return GenGetPKDt(pk, M_LVUPCLM[dtNo].c_str());
  }
  BOOL TMdm_LvUp::GetPKDtBOOL(s64 pk, enmdm_LvUp dtNo)
  {
    return GenGetPKDtBOOL(pk, M_LVUPCLM[dtNo].c_str());
  }
}
