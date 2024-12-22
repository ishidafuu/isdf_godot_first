#include "dbMdm_Kiyaku.h"
namespace db
{
  TMdm_Kiyaku::TMdm_Kiyaku():TModelGen("m_Kiyaku")
  {
  }
  TMdm_Kiyaku::~TMdm_Kiyaku(void)
  {
  }
  void TMdm_Kiyaku::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Kiyaku", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_kiyaku_pk));
      model_.strvecmap_["title_c64"].push_back(csv.GetStr(i, mdm_kiyaku_title_c64));
      model_.strvecmap_["message_text"].push_back(csv.GetStr(i, mdm_kiyaku_message_text));
    }
    
  }
  s64 TMdm_Kiyaku::GetLineDt(s64 line, enmdm_Kiyaku dtNo)
  {
    return GenGetLineDt(line, M_KIYAKUCLM[dtNo].c_str());
  }
  BOOL TMdm_Kiyaku::GetLineDtBOOL(s64 line, enmdm_Kiyaku dtNo)
  {
    return GenGetLineDtBOOL(line, M_KIYAKUCLM[dtNo].c_str());
  }
  std::string TMdm_Kiyaku::GetLineDtStr(s64 line, enmdm_KiyakuStr dtNo)
  {
    return GenGetLineDtStr(line, M_KIYAKUCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Kiyaku::GetPKDt(s64 pk, enmdm_Kiyaku dtNo)
  {
    return GenGetPKDt(pk, M_KIYAKUCLM[dtNo].c_str());
  }
  BOOL TMdm_Kiyaku::GetPKDtBOOL(s64 pk, enmdm_Kiyaku dtNo)
  {
    return GenGetPKDtBOOL(pk, M_KIYAKUCLM[dtNo].c_str());
  }
  std::string TMdm_Kiyaku::GetPKDtStr(s64 pk, enmdm_KiyakuStr dtNo)
  {
    return GenGetPKDtStr(pk, M_KIYAKUCLMSTR[dtNo].c_str());
  }
}
