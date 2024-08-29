#include "dbMdm_Info.h"
namespace db
{
  TMdm_Info::TMdm_Info():TModelGen("m_Info")
  {
  }
  TMdm_Info::~TMdm_Info(void)
  {
  }
  void TMdm_Info::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Info", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_info_pk));
      model_.intvecmap_["osType"].push_back(csv.GetInt(i, mdm_info_osType));
      model_.intvecmap_["important_f"].push_back(csv.GetInt(i, mdm_info_important_f));
      model_.intvecmap_["iconNo"].push_back(csv.GetInt(i, mdm_info_iconNo));
      model_.intvecmap_["infoSt_date"].push_back(csv.GetInt(i, mdm_info_infoSt_date));
      model_.intvecmap_["infoEd_date"].push_back(csv.GetInt(i, mdm_info_infoEd_date));
      model_.intvecmap_["newEd_date"].push_back(csv.GetInt(i, mdm_info_newEd_date));
      model_.strvecmap_["title_c64"].push_back(csv.GetStr(i, mdm_info_title_c64));
      model_.strvecmap_["message_text"].push_back(csv.GetStr(i, mdm_info_message_text));
    }
    
  }
  s64 TMdm_Info::GetLineDt(s64 line, enmdm_Info dtNo)
  {
    return GenGetLineDt(line, M_INFOCLM[dtNo].c_str());
  }
  BOOL TMdm_Info::GetLineDtBOOL(s64 line, enmdm_Info dtNo)
  {
    return GenGetLineDtBOOL(line, M_INFOCLM[dtNo].c_str());
  }
  std::string TMdm_Info::GetLineDtStr(s64 line, enmdm_InfoStr dtNo)
  {
    return GenGetLineDtStr(line, M_INFOCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Info::GetPKDt(s64 pk, enmdm_Info dtNo)
  {
    return GenGetPKDt(pk, M_INFOCLM[dtNo].c_str());
  }
  BOOL TMdm_Info::GetPKDtBOOL(s64 pk, enmdm_Info dtNo)
  {
    return GenGetPKDtBOOL(pk, M_INFOCLM[dtNo].c_str());
  }
  std::string TMdm_Info::GetPKDtStr(s64 pk, enmdm_InfoStr dtNo)
  {
    return GenGetPKDtStr(pk, M_INFOCLMSTR[dtNo].c_str());
  }
}
