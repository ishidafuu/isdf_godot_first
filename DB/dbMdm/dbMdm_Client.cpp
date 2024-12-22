#include "dbMdm_Client.h"
namespace db
{
  TMdm_Client::TMdm_Client():TModelGen("m_Client")
  {
  }
  TMdm_Client::~TMdm_Client(void)
  {
  }
  void TMdm_Client::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Client", FALSE, TRUE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_client_pk));
      model_.intvecmap_["mbgCircleID"].push_back(csv.GetInt(i, mdm_client_mbgCircleID));
      model_.intvecmap_["vsbuf"].push_back(csv.GetInt(i, mdm_client_vsbuf));
      model_.intvecmap_["caper"].push_back(csv.GetInt(i, mdm_client_caper));
      model_.intvecmap_["ncaper"].push_back(csv.GetInt(i, mdm_client_ncaper));
    }
    
  }
  s64 TMdm_Client::GetLineDt(s64 line, enmdm_Client dtNo)
  {
    return GenGetLineDt(line, M_CLIENTCLM[dtNo].c_str());
  }
  BOOL TMdm_Client::GetLineDtBOOL(s64 line, enmdm_Client dtNo)
  {
    return GenGetLineDtBOOL(line, M_CLIENTCLM[dtNo].c_str());
  }
  s64 TMdm_Client::GetPKDt(s64 pk, enmdm_Client dtNo)
  {
    return GenGetPKDt(pk, M_CLIENTCLM[dtNo].c_str());
  }
  BOOL TMdm_Client::GetPKDtBOOL(s64 pk, enmdm_Client dtNo)
  {
    return GenGetPKDtBOOL(pk, M_CLIENTCLM[dtNo].c_str());
  }
}
