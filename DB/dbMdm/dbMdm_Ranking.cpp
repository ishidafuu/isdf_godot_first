#include "dbMdm_Ranking.h"
namespace db
{
  TMdm_Ranking::TMdm_Ranking():TModelGen("m_Ranking")
  {
  }
  TMdm_Ranking::~TMdm_Ranking(void)
  {
  }
  void TMdm_Ranking::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Ranking", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_ranking_pk));
      model_.intvecmap_["id_m_event"].push_back(csv.GetInt(i, mdm_ranking_id_m_event));
      model_.intvecmap_["scoreType"].push_back(csv.GetInt(i, mdm_ranking_scoreType));
      model_.intvecmap_["id_m_supply"].push_back(csv.GetInt(i, mdm_ranking_id_m_supply));
      model_.intvecmap_["iconNo"].push_back(csv.GetInt(i, mdm_ranking_iconNo));
      model_.strvecmap_["name_c64"].push_back(csv.GetStr(i, mdm_ranking_name_c64));
    }
    
  }
  s64 TMdm_Ranking::GetLineDt(s64 line, enmdm_Ranking dtNo)
  {
    return GenGetLineDt(line, M_RANKINGCLM[dtNo].c_str());
  }
  BOOL TMdm_Ranking::GetLineDtBOOL(s64 line, enmdm_Ranking dtNo)
  {
    return GenGetLineDtBOOL(line, M_RANKINGCLM[dtNo].c_str());
  }
  std::string TMdm_Ranking::GetLineDtStr(s64 line, enmdm_RankingStr dtNo)
  {
    return GenGetLineDtStr(line, M_RANKINGCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Ranking::GetPKDt(s64 pk, enmdm_Ranking dtNo)
  {
    return GenGetPKDt(pk, M_RANKINGCLM[dtNo].c_str());
  }
  BOOL TMdm_Ranking::GetPKDtBOOL(s64 pk, enmdm_Ranking dtNo)
  {
    return GenGetPKDtBOOL(pk, M_RANKINGCLM[dtNo].c_str());
  }
  std::string TMdm_Ranking::GetPKDtStr(s64 pk, enmdm_RankingStr dtNo)
  {
    return GenGetPKDtStr(pk, M_RANKINGCLMSTR[dtNo].c_str());
  }
}
