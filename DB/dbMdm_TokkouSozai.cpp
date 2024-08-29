#include "dbMdm_TokkouSozai.h"
namespace db
{
  TMdm_TokkouSozai::TMdm_TokkouSozai():TModelGen("m_TokkouSozai")
  {
  }
  TMdm_TokkouSozai::~TMdm_TokkouSozai(void)
  {
  }
  void TMdm_TokkouSozai::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_TokkouSozai", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_tokkousozai_pk));
      model_.intvecmap_["tokkouNo"].push_back(csv.GetInt(i, mdm_tokkousozai_tokkouNo));
      model_.intvecmap_["id_m_sozai"].push_back(csv.GetInt(i, mdm_tokkousozai_id_m_sozai));
      model_.intvecmap_["magD"].push_back(csv.GetInt(i, mdm_tokkousozai_magD));
      model_.intvecmap_["magC"].push_back(csv.GetInt(i, mdm_tokkousozai_magC));
      model_.intvecmap_["magB"].push_back(csv.GetInt(i, mdm_tokkousozai_magB));
      model_.intvecmap_["magA"].push_back(csv.GetInt(i, mdm_tokkousozai_magA));
      model_.intvecmap_["magS"].push_back(csv.GetInt(i, mdm_tokkousozai_magS));
      model_.intvecmap_["magSS"].push_back(csv.GetInt(i, mdm_tokkousozai_magSS));
      model_.intvecmap_["magSSS"].push_back(csv.GetInt(i, mdm_tokkousozai_magSSS));
    }
    
  }
  s64 TMdm_TokkouSozai::GetLineDt(s64 line, enmdm_TokkouSozai dtNo)
  {
    return GenGetLineDt(line, M_TOKKOUSOZAICLM[dtNo].c_str());
  }
  BOOL TMdm_TokkouSozai::GetLineDtBOOL(s64 line, enmdm_TokkouSozai dtNo)
  {
    return GenGetLineDtBOOL(line, M_TOKKOUSOZAICLM[dtNo].c_str());
  }
  s64 TMdm_TokkouSozai::GetPKDt(s64 pk, enmdm_TokkouSozai dtNo)
  {
    return GenGetPKDt(pk, M_TOKKOUSOZAICLM[dtNo].c_str());
  }
  BOOL TMdm_TokkouSozai::GetPKDtBOOL(s64 pk, enmdm_TokkouSozai dtNo)
  {
    return GenGetPKDtBOOL(pk, M_TOKKOUSOZAICLM[dtNo].c_str());
  }
}
