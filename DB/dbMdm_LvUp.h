#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_LvUp
  {
    mdm_lvup_pk,//現在Ｌｖ（ＰＫ）
    mdm_lvup_exp_for_next,//次Ｌｖ必要経験値
  };
  
  const std::string M_LVUPCLM[] = 
  {
    "pk",//現在Ｌｖ（ＰＫ）
    "exp_for_next",//次Ｌｖ必要経験値
  };
  
  class TMdm_LvUp: public TModelGen
  {
  protected:

  public:
    explicit TMdm_LvUp();
    virtual ~TMdm_LvUp();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_LvUp dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_LvUp dtNo);
    s64 GetPKDt(s64 pk, enmdm_LvUp dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_LvUp dtNo);
  };
}
