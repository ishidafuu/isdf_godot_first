#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_LogboSozai
  {
    mdm_logbosozai_pk,//ログボ素材ＩＤ
    mdm_logbosozai_logboType,//ログボタイプ
    mdm_logbosozai_itemID,//素材ＩＤ
  };
  
  const std::string M_LOGBOSOZAICLM[] = 
  {
    "pk",//ログボ素材ＩＤ
    "logboType",//ログボタイプ
    "itemID",//素材ＩＤ
  };
  
  class TMdm_LogboSozai: public TModelGen
  {
  protected:

  public:
    explicit TMdm_LogboSozai();
    virtual ~TMdm_LogboSozai();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_LogboSozai dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_LogboSozai dtNo);
    s64 GetPKDt(s64 pk, enmdm_LogboSozai dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_LogboSozai dtNo);
  };
}
