#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Banner
  {
    mdm_banner_pk,//マスターバナーＩＤ
    mdm_banner_st_date,//表示開始時間
    mdm_banner_ed_date,//表示終了時間
    mdm_banner_jumpNo0,//飛び先番号0
    mdm_banner_jumpNo1,//飛び先番号1
    mdm_banner_jumpNo2,//飛び先番号2
    mdm_banner_jumpNo3,//飛び先番号3
    mdm_banner_jumpNo4,//飛び先番号4
    mdm_banner_switchA,//TVＡローテ切替時画像ID
    mdm_banner_switchB,//TVＢローテ切替時画像ID
    mdm_banner_switchC,//TVＣローテ切替時画像ID
    mdm_banner_textAni0,//テキスト表示方法0
    mdm_banner_textAni1,//テキスト表示方法1
    mdm_banner_textAni2,//テキスト表示方法2
  };
  
  enum enmdm_BannerStr
  {
    mdm_banner_text0_c128,//テキスト0
    mdm_banner_text1_c128,//テキスト1
    mdm_banner_text2_c128,//テキスト2
  };
  

  const std::string M_BANNERCLM[] = 
  {
    "pk",//マスターバナーＩＤ
    "st_date",//表示開始時間
    "ed_date",//表示終了時間
    "jumpNo0",//飛び先番号0
    "jumpNo1",//飛び先番号1
    "jumpNo2",//飛び先番号2
    "jumpNo3",//飛び先番号3
    "jumpNo4",//飛び先番号4
    "switchA",//TVＡローテ切替時画像ID
    "switchB",//TVＢローテ切替時画像ID
    "switchC",//TVＣローテ切替時画像ID
    "textAni0",//テキスト表示方法0
    "textAni1",//テキスト表示方法1
    "textAni2",//テキスト表示方法2
  };
  
  const std::string M_BANNERCLMSTR[] = 
  {
    "text0_c128",//テキスト0
    "text1_c128",//テキスト1
    "text2_c128",//テキスト2
  };
  
  class TMdm_Banner: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Banner();
    virtual ~TMdm_Banner();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Banner dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Banner dtNo);
    std::string GetLineDtStr(s64 line, enmdm_BannerStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Banner dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Banner dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_BannerStr dtNo);
  };
}
