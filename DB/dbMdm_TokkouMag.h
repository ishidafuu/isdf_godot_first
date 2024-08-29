#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_TokkouMag
  {
    mdm_tokkoumag_pk,//マスター特効チーム倍率ＩＤ
    mdm_tokkoumag_magNo,//倍率パーセント
    mdm_tokkoumag_overlap,//ダブり枚数
    mdm_tokkoumag_mag,//倍率パーセント
  };
  
  const std::string M_TOKKOUMAGCLM[] = 
  {
    "pk",//マスター特効チーム倍率ＩＤ
    "magNo",//倍率パーセント
    "overlap",//ダブり枚数
    "mag",//倍率パーセント
  };
  
  class TMdm_TokkouMag: public TModelGen
  {
  protected:

  public:
    explicit TMdm_TokkouMag();
    virtual ~TMdm_TokkouMag();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_TokkouMag dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_TokkouMag dtNo);
    s64 GetPKDt(s64 pk, enmdm_TokkouMag dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_TokkouMag dtNo);
  };
}
