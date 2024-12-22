#include "dbMdm_Rarity.h"
namespace db
{
  TMdm_Rarity::TMdm_Rarity():TModelGen("m_Rarity")
  {
  }
  TMdm_Rarity::~TMdm_Rarity(void)
  {
  }
  void TMdm_Rarity::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Rarity", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_rarity_pk));
      model_.intvecmap_["priceSell"].push_back(csv.GetInt(i, mdm_rarity_priceSell));
      model_.intvecmap_["priceBuy"].push_back(csv.GetInt(i, mdm_rarity_priceBuy));
      model_.intvecmap_["gouseiTime"].push_back(csv.GetInt(i, mdm_rarity_gouseiTime));
      model_.intvecmap_["staminaLim"].push_back(csv.GetInt(i, mdm_rarity_staminaLim));
      model_.intvecmap_["sozaiQuality"].push_back(csv.GetInt(i, mdm_rarity_sozaiQuality));
      model_.intvecmap_["vshumWin"].push_back(csv.GetInt(i, mdm_rarity_vshumWin));
      model_.intvecmap_["vshumTimeupWin"].push_back(csv.GetInt(i, mdm_rarity_vshumTimeupWin));
      model_.intvecmap_["vshumTimeupLose"].push_back(csv.GetInt(i, mdm_rarity_vshumTimeupLose));
      model_.intvecmap_["vshumLose"].push_back(csv.GetInt(i, mdm_rarity_vshumLose));
      model_.intvecmap_["vshumWin_DC"].push_back(csv.GetInt(i, mdm_rarity_vshumWin_DC));
      model_.intvecmap_["vshumTimeupWin_DC"].push_back(csv.GetInt(i, mdm_rarity_vshumTimeupWin_DC));
      model_.intvecmap_["vshumTimeupLose_DC"].push_back(csv.GetInt(i, mdm_rarity_vshumTimeupLose_DC));
      model_.intvecmap_["vshumLose_DC"].push_back(csv.GetInt(i, mdm_rarity_vshumLose_DC));
      model_.intvecmap_["priceTeamSell"].push_back(csv.GetInt(i, mdm_rarity_priceTeamSell));
      model_.intvecmap_["charaLvCap"].push_back(csv.GetInt(i, mdm_rarity_charaLvCap));
    }
    
  }
  s64 TMdm_Rarity::GetLineDt(s64 line, enmdm_Rarity dtNo)
  {
    return GenGetLineDt(line, M_RARITYCLM[dtNo].c_str());
  }
  BOOL TMdm_Rarity::GetLineDtBOOL(s64 line, enmdm_Rarity dtNo)
  {
    return GenGetLineDtBOOL(line, M_RARITYCLM[dtNo].c_str());
  }
  s64 TMdm_Rarity::GetPKDt(s64 pk, enmdm_Rarity dtNo)
  {
    return GenGetPKDt(pk, M_RARITYCLM[dtNo].c_str());
  }
  BOOL TMdm_Rarity::GetPKDtBOOL(s64 pk, enmdm_Rarity dtNo)
  {
    return GenGetPKDtBOOL(pk, M_RARITYCLM[dtNo].c_str());
  }
}
