#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Help
  {
    mdm_help_pk,//ヘルプＩＤ
    mdm_help_helpType,//ヘルプの種類
    mdm_help_category,//報酬などのカテゴリ
  };
  
  enum enmdm_HelpStr
  {
    mdm_help_title_c64,//タイトル
    mdm_help_message_text,//本文
  };
  

  const std::string M_HELPCLM[] = 
  {
    "pk",//ヘルプＩＤ
    "helpType",//ヘルプの種類
    "category",//報酬などのカテゴリ
  };
  
  const std::string M_HELPCLMSTR[] = 
  {
    "title_c64",//タイトル
    "message_text",//本文
  };
  
  class TMdm_Help: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Help();
    virtual ~TMdm_Help();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Help dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Help dtNo);
    std::string GetLineDtStr(s64 line, enmdm_HelpStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Help dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Help dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_HelpStr dtNo);
  };
}
