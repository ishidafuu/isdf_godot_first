#include "dbMdm_GachaGrp.h"
namespace db
{
  TMdm_GachaGrp::TMdm_GachaGrp():TModelGen("m_GachaGrp")
  {
  }
  TMdm_GachaGrp::~TMdm_GachaGrp(void)
  {
  }
  void TMdm_GachaGrp::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_GachaGrp", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_gachagrp_pk));
      model_.intvecmap_["id_m_timer"].push_back(csv.GetInt(i, mdm_gachagrp_id_m_timer));
      model_.intvecmap_["lotGrpNo"].push_back(csv.GetInt(i, mdm_gachagrp_lotGrpNo));
      model_.intvecmap_["cntGrpNo"].push_back(csv.GetInt(i, mdm_gachagrp_cntGrpNo));
      model_.intvecmap_["grade"].push_back(csv.GetInt(i, mdm_gachagrp_grade));
      model_.intvecmap_["prob"].push_back(csv.GetInt(i, mdm_gachagrp_prob));
      model_.intvecmap_["boxNum"].push_back(csv.GetInt(i, mdm_gachagrp_boxNum));
      model_.intvecmap_["boxReset_f"].push_back(csv.GetInt(i, mdm_gachagrp_boxReset_f));
      model_.strvecmap_["name_c64"].push_back(csv.GetStr(i, mdm_gachagrp_name_c64));
    }
    
  }
  s64 TMdm_GachaGrp::GetLineDt(s64 line, enmdm_GachaGrp dtNo)
  {
    return GenGetLineDt(line, M_GACHAGRPCLM[dtNo].c_str());
  }
  BOOL TMdm_GachaGrp::GetLineDtBOOL(s64 line, enmdm_GachaGrp dtNo)
  {
    return GenGetLineDtBOOL(line, M_GACHAGRPCLM[dtNo].c_str());
  }
  std::string TMdm_GachaGrp::GetLineDtStr(s64 line, enmdm_GachaGrpStr dtNo)
  {
    return GenGetLineDtStr(line, M_GACHAGRPCLMSTR[dtNo].c_str());
  }
  s64 TMdm_GachaGrp::GetPKDt(s64 pk, enmdm_GachaGrp dtNo)
  {
    return GenGetPKDt(pk, M_GACHAGRPCLM[dtNo].c_str());
  }
  BOOL TMdm_GachaGrp::GetPKDtBOOL(s64 pk, enmdm_GachaGrp dtNo)
  {
    return GenGetPKDtBOOL(pk, M_GACHAGRPCLM[dtNo].c_str());
  }
  std::string TMdm_GachaGrp::GetPKDtStr(s64 pk, enmdm_GachaGrpStr dtNo)
  {
    return GenGetPKDtStr(pk, M_GACHAGRPCLMSTR[dtNo].c_str());
  }
}
