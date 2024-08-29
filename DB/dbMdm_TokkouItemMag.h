#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_TokkouItemMag
  {
    mdm_tokkouitemmag_pk,//マスター特効アイテム倍率ＩＤ
    mdm_tokkouitemmag_tokkouNo,//特効グループ番号
    mdm_tokkouitemmag_effectType,//効果タイプ
    mdm_tokkouitemmag_haveNum,//所持数
    mdm_tokkouitemmag_effect,//効果値
  };
  
  const std::string M_TOKKOUITEMMAGCLM[] = 
  {
    "pk",//マスター特効アイテム倍率ＩＤ
    "tokkouNo",//特効グループ番号
    "effectType",//効果タイプ
    "haveNum",//所持数
    "effect",//効果値
  };
  
  class TMdm_TokkouItemMag: public TModelGen
  {
  protected:

  public:
    explicit TMdm_TokkouItemMag();
    virtual ~TMdm_TokkouItemMag();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_TokkouItemMag dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_TokkouItemMag dtNo);
    s64 GetPKDt(s64 pk, enmdm_TokkouItemMag dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_TokkouItemMag dtNo);
  };
}
