#include "dbMdm_Tournament.h"
namespace db
{
  TMdm_Tournament::TMdm_Tournament():TModelGen("m_Tournament")
  {
  }
  TMdm_Tournament::~TMdm_Tournament(void)
  {
  }
  void TMdm_Tournament::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Tournament", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_tournament_pk));
      model_.intvecmap_["round"].push_back(csv.GetInt(i, mdm_tournament_round));
      model_.intvecmap_["probD"].push_back(csv.GetInt(i, mdm_tournament_probD));
      model_.intvecmap_["probC"].push_back(csv.GetInt(i, mdm_tournament_probC));
      model_.intvecmap_["probB"].push_back(csv.GetInt(i, mdm_tournament_probB));
      model_.intvecmap_["probA"].push_back(csv.GetInt(i, mdm_tournament_probA));
      model_.intvecmap_["probS"].push_back(csv.GetInt(i, mdm_tournament_probS));
      model_.intvecmap_["probSS"].push_back(csv.GetInt(i, mdm_tournament_probSS));
      model_.intvecmap_["probSSS"].push_back(csv.GetInt(i, mdm_tournament_probSSS));
    }
    
  }
  s64 TMdm_Tournament::GetLineDt(s64 line, enmdm_Tournament dtNo)
  {
    return GenGetLineDt(line, M_TOURNAMENTCLM[dtNo].c_str());
  }
  BOOL TMdm_Tournament::GetLineDtBOOL(s64 line, enmdm_Tournament dtNo)
  {
    return GenGetLineDtBOOL(line, M_TOURNAMENTCLM[dtNo].c_str());
  }
  s64 TMdm_Tournament::GetPKDt(s64 pk, enmdm_Tournament dtNo)
  {
    return GenGetPKDt(pk, M_TOURNAMENTCLM[dtNo].c_str());
  }
  BOOL TMdm_Tournament::GetPKDtBOOL(s64 pk, enmdm_Tournament dtNo)
  {
    return GenGetPKDtBOOL(pk, M_TOURNAMENTCLM[dtNo].c_str());
  }
}
