#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Logbo
  {
    mdm_logbo_pk,//ログボＩＤ
    mdm_logbo_itemType,//アイテムタイプ
    mdm_logbo_itemID,//アイテムＩＤ
    mdm_logbo_itemNum,//アイテム個数
  };
  
  const std::string M_LOGBOCLM[] = 
  {
    "pk",//ログボＩＤ
    "itemType",//アイテムタイプ
    "itemID",//アイテムＩＤ
    "itemNum",//アイテム個数
  };
  
  class TMdm_Logbo: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Logbo();
    virtual ~TMdm_Logbo();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Logbo dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Logbo dtNo);
    s64 GetPKDt(s64 pk, enmdm_Logbo dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Logbo dtNo);
  };
}
