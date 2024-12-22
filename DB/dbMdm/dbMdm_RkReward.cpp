#include "dbMdm_RkReward.h"
namespace db
{
  TMdm_RkReward::TMdm_RkReward():TModelGen("m_RkReward")
  {
  }
  TMdm_RkReward::~TMdm_RkReward(void)
  {
  }
  void TMdm_RkReward::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_RkReward", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_rkreward_pk));
      model_.intvecmap_["id_m_ranking"].push_back(csv.GetInt(i, mdm_rkreward_id_m_ranking));
      model_.intvecmap_["grade"].push_back(csv.GetInt(i, mdm_rkreward_grade));
      model_.intvecmap_["top"].push_back(csv.GetInt(i, mdm_rkreward_top));
      model_.intvecmap_["bottom"].push_back(csv.GetInt(i, mdm_rkreward_bottom));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_rkreward_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_rkreward_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_rkreward_itemNum));
      model_.strvecmap_["comment_c512"].push_back(csv.GetStr(i, mdm_rkreward_comment_c512));
    }
    
  }
  s64 TMdm_RkReward::GetLineDt(s64 line, enmdm_RkReward dtNo)
  {
    return GenGetLineDt(line, M_RKREWARDCLM[dtNo].c_str());
  }
  BOOL TMdm_RkReward::GetLineDtBOOL(s64 line, enmdm_RkReward dtNo)
  {
    return GenGetLineDtBOOL(line, M_RKREWARDCLM[dtNo].c_str());
  }
  std::string TMdm_RkReward::GetLineDtStr(s64 line, enmdm_RkRewardStr dtNo)
  {
    return GenGetLineDtStr(line, M_RKREWARDCLMSTR[dtNo].c_str());
  }
  s64 TMdm_RkReward::GetPKDt(s64 pk, enmdm_RkReward dtNo)
  {
    return GenGetPKDt(pk, M_RKREWARDCLM[dtNo].c_str());
  }
  BOOL TMdm_RkReward::GetPKDtBOOL(s64 pk, enmdm_RkReward dtNo)
  {
    return GenGetPKDtBOOL(pk, M_RKREWARDCLM[dtNo].c_str());
  }
  std::string TMdm_RkReward::GetPKDtStr(s64 pk, enmdm_RkRewardStr dtNo)
  {
    return GenGetPKDtStr(pk, M_RKREWARDCLMSTR[dtNo].c_str());
  }
}
