#include "dbMdm_Campaign.h"
namespace db
{
  TMdm_Campaign::TMdm_Campaign():TModelGen("m_Campaign")
  {
  }
  TMdm_Campaign::~TMdm_Campaign(void)
  {
  }
  void TMdm_Campaign::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Campaign", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_campaign_pk));
      model_.intvecmap_["id_m_timer"].push_back(csv.GetInt(i, mdm_campaign_id_m_timer));
      model_.intvecmap_["itemType"].push_back(csv.GetInt(i, mdm_campaign_itemType));
      model_.intvecmap_["itemID"].push_back(csv.GetInt(i, mdm_campaign_itemID));
      model_.intvecmap_["itemNum"].push_back(csv.GetInt(i, mdm_campaign_itemNum));
      model_.intvecmap_["inputType"].push_back(csv.GetInt(i, mdm_campaign_inputType));
      model_.intvecmap_["inputLen"].push_back(csv.GetInt(i, mdm_campaign_inputLen));
      model_.intvecmap_["commonCode_f"].push_back(csv.GetInt(i, mdm_campaign_commonCode_f));
      model_.intvecmap_["multiGet_f"].push_back(csv.GetInt(i, mdm_campaign_multiGet_f));
      model_.strvecmap_["name_c64"].push_back(csv.GetStr(i, mdm_campaign_name_c64));
    }
    
  }
  s64 TMdm_Campaign::GetLineDt(s64 line, enmdm_Campaign dtNo)
  {
    return GenGetLineDt(line, M_CAMPAIGNCLM[dtNo].c_str());
  }
  BOOL TMdm_Campaign::GetLineDtBOOL(s64 line, enmdm_Campaign dtNo)
  {
    return GenGetLineDtBOOL(line, M_CAMPAIGNCLM[dtNo].c_str());
  }
  std::string TMdm_Campaign::GetLineDtStr(s64 line, enmdm_CampaignStr dtNo)
  {
    return GenGetLineDtStr(line, M_CAMPAIGNCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Campaign::GetPKDt(s64 pk, enmdm_Campaign dtNo)
  {
    return GenGetPKDt(pk, M_CAMPAIGNCLM[dtNo].c_str());
  }
  BOOL TMdm_Campaign::GetPKDtBOOL(s64 pk, enmdm_Campaign dtNo)
  {
    return GenGetPKDtBOOL(pk, M_CAMPAIGNCLM[dtNo].c_str());
  }
  std::string TMdm_Campaign::GetPKDtStr(s64 pk, enmdm_CampaignStr dtNo)
  {
    return GenGetPKDtStr(pk, M_CAMPAIGNCLMSTR[dtNo].c_str());
  }
}
