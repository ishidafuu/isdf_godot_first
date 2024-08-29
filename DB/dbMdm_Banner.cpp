#include "dbMdm_Banner.h"
namespace db
{
  TMdm_Banner::TMdm_Banner():TModelGen("m_Banner")
  {
  }
  TMdm_Banner::~TMdm_Banner(void)
  {
  }
  void TMdm_Banner::DoLoadDt(const char *CSVDir)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, "m_Banner", FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      if (i == 0) continue;
      model_.intvecmap_["pk"].push_back(csv.GetInt(i, mdm_banner_pk));
      model_.intvecmap_["st_date"].push_back(csv.GetInt(i, mdm_banner_st_date));
      model_.intvecmap_["ed_date"].push_back(csv.GetInt(i, mdm_banner_ed_date));
      model_.intvecmap_["jumpNo0"].push_back(csv.GetInt(i, mdm_banner_jumpNo0));
      model_.intvecmap_["jumpNo1"].push_back(csv.GetInt(i, mdm_banner_jumpNo1));
      model_.intvecmap_["jumpNo2"].push_back(csv.GetInt(i, mdm_banner_jumpNo2));
      model_.intvecmap_["jumpNo3"].push_back(csv.GetInt(i, mdm_banner_jumpNo3));
      model_.intvecmap_["jumpNo4"].push_back(csv.GetInt(i, mdm_banner_jumpNo4));
      model_.intvecmap_["switchA"].push_back(csv.GetInt(i, mdm_banner_switchA));
      model_.intvecmap_["switchB"].push_back(csv.GetInt(i, mdm_banner_switchB));
      model_.intvecmap_["switchC"].push_back(csv.GetInt(i, mdm_banner_switchC));
      model_.intvecmap_["textAni0"].push_back(csv.GetInt(i, mdm_banner_textAni0));
      model_.intvecmap_["textAni1"].push_back(csv.GetInt(i, mdm_banner_textAni1));
      model_.intvecmap_["textAni2"].push_back(csv.GetInt(i, mdm_banner_textAni2));
      model_.strvecmap_["text0_c128"].push_back(csv.GetStr(i, mdm_banner_text0_c128));
      model_.strvecmap_["text1_c128"].push_back(csv.GetStr(i, mdm_banner_text1_c128));
      model_.strvecmap_["text2_c128"].push_back(csv.GetStr(i, mdm_banner_text2_c128));
    }
    
  }
  s64 TMdm_Banner::GetLineDt(s64 line, enmdm_Banner dtNo)
  {
    return GenGetLineDt(line, M_BANNERCLM[dtNo].c_str());
  }
  BOOL TMdm_Banner::GetLineDtBOOL(s64 line, enmdm_Banner dtNo)
  {
    return GenGetLineDtBOOL(line, M_BANNERCLM[dtNo].c_str());
  }
  std::string TMdm_Banner::GetLineDtStr(s64 line, enmdm_BannerStr dtNo)
  {
    return GenGetLineDtStr(line, M_BANNERCLMSTR[dtNo].c_str());
  }
  s64 TMdm_Banner::GetPKDt(s64 pk, enmdm_Banner dtNo)
  {
    return GenGetPKDt(pk, M_BANNERCLM[dtNo].c_str());
  }
  BOOL TMdm_Banner::GetPKDtBOOL(s64 pk, enmdm_Banner dtNo)
  {
    return GenGetPKDtBOOL(pk, M_BANNERCLM[dtNo].c_str());
  }
  std::string TMdm_Banner::GetPKDtStr(s64 pk, enmdm_BannerStr dtNo)
  {
    return GenGetPKDtStr(pk, M_BANNERCLMSTR[dtNo].c_str());
  }
}
