#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Kiyaku
  {
    mdm_kiyaku_pk,//規約ＩＤ
  };
  
  enum enmdm_KiyakuStr
  {
    mdm_kiyaku_title_c64,//タイトル
    mdm_kiyaku_message_text,//本文
  };
  

  const std::string M_KIYAKUCLM[] = 
  {
    "pk",//規約ＩＤ
  };
  
  const std::string M_KIYAKUCLMSTR[] = 
  {
    "title_c64",//タイトル
    "message_text",//本文
  };
  
  class TMdm_Kiyaku: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Kiyaku();
    virtual ~TMdm_Kiyaku();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Kiyaku dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Kiyaku dtNo);
    std::string GetLineDtStr(s64 line, enmdm_KiyakuStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Kiyaku dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Kiyaku dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_KiyakuStr dtNo);
  };
}
