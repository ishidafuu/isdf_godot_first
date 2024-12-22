#include "dbMdm_SpSyouhin.h"
namespace db
{
  TMdm_SpSyouhin::TMdm_SpSyouhin():TModelGen("m_SpSyouhin")
  {
  }
  TMdm_SpSyouhin::~TMdm_SpSyouhin(void)
  {
  }
  void TMdm_SpSyouhin::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_SpSyouhin", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_spsyouhin_pk));
      model_.intvecmap_["shopType"].push_back(csv.GetInt(i, mdm_spsyouhin_shopType));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_spsyouhin_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_spsyouhin_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_spsyouhin_itemNum));
    }
    
  }
  s64 TMdm_SpSyouhin::GetLineDt(s64 line, enmdm_SpSyouhin dtNo)
  {
    return GenGetLineDt(line, M_SPSYOUHINCLM[dtNo].c_str());
  }
  BOOL TMdm_SpSyouhin::GetLineDtBOOL(s64 line, enmdm_SpSyouhin dtNo)
  {
    return GenGetLineDtBOOL(line, M_SPSYOUHINCLM[dtNo].c_str());
  }
  s64 TMdm_SpSyouhin::GetPKDt(s64 pk, enmdm_SpSyouhin dtNo)
  {
    return GenGetPKDt(pk, M_SPSYOUHINCLM[dtNo].c_str());
  }
  BOOL TMdm_SpSyouhin::GetPKDtBOOL(s64 pk, enmdm_SpSyouhin dtNo)
  {
    return GenGetPKDtBOOL(pk, M_SPSYOUHINCLM[dtNo].c_str());
  }
}
