#include "dbMdm_Help.h"
namespace db
{
  TMdm_Help::TMdm_Help():TModelGen("m_Help")
  {
  }
  TMdm_Help::~TMdm_Help(void)
  {
  }
  void TMdm_Help::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Help", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_help_pk));
      model_.intvecmap_["helpType"].push_back(csv.GetInt(i, mdm_help_helpType));
      model_.intvecmap_["category"].push_back(csv.GetInt(i, mdm_help_category));
      model_.strvecmap_["title_c64"].push_back(csv.GetStr(i, mdm_help_title_c64));
      model_.strvecmap_["message_text"].push_back(csv.GetStr(i, mdm_help_message_text));
    }
    
  }
  s64 TMdm_Help::GetLineDt(s64 line, enmdm_Help dtNo)
  {
    return GenGetLineDt(line, M_HELPCLM[dtNo].c_str());
  }
  BOOL TMdm_Help::GetLineDtBOOL(s64 line, enmdm_Help dtNo)
  {
    return GenGetLineDtBOOL(line, M_HELPCLM[dtNo].c_str());
  }
  std::string TMdm_Help::GetLineDtStr(s64 line, enmdm_HelpStr dtNo)
  {
    return GenGetLineDtStr(line, M_HELPCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Help::GetPKDt(s64 pk, enmdm_Help dtNo)
  {
    return GenGetPKDt(pk, M_HELPCLM[dtNo].c_str());
  }
  BOOL TMdm_Help::GetPKDtBOOL(s64 pk, enmdm_Help dtNo)
  {
    return GenGetPKDtBOOL(pk, M_HELPCLM[dtNo].c_str());
  }
  std::string TMdm_Help::GetPKDtStr(s64 pk, enmdm_HelpStr dtNo)
  {
    return GenGetPKDtStr(pk, M_HELPCLMSTR[dtNo].c_str());
  }
}
