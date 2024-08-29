#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Info
  {
    mdm_info_pk,//おしらせＩＤ
    mdm_info_osType,//ios/android
    mdm_info_important_f,//重要フラグ
    mdm_info_iconNo,//おしらせ用アイコン番号
    mdm_info_infoSt_date,//開始日時
    mdm_info_infoEd_date,//終了日時
    mdm_info_newEd_date,//new終了日時
  };
  
  enum enmdm_InfoStr
  {
    mdm_info_title_c64,//タイトル
    mdm_info_message_text,//本文
  };
  

  const std::string M_INFOCLM[] = 
  {
    "pk",//おしらせＩＤ
    "osType",//ios/android
    "important_f",//重要フラグ
    "iconNo",//おしらせ用アイコン番号
    "infoSt_date",//開始日時
    "infoEd_date",//終了日時
    "newEd_date",//new終了日時
  };
  
  const std::string M_INFOCLMSTR[] = 
  {
    "title_c64",//タイトル
    "message_text",//本文
  };
  
  class TMdm_Info: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Info();
    virtual ~TMdm_Info();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Info dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Info dtNo);
    std::string GetLineDtStr(s64 line, enmdm_InfoStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Info dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Info dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_InfoStr dtNo);
  };
}
