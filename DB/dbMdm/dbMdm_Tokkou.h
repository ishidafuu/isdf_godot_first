#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Tokkou
  {
    mdm_tokkou_pk,//マスター特効ＩＤ
    mdm_tokkou_tokkouNo,//特効グループ番号
    mdm_tokkou_id_m_team,//マスターチームＩＤ
    mdm_tokkou_magNo,//マスター特効チーム倍率番号
  };
  
  const std::string M_TOKKOUCLM[] = 
  {
    "pk",//マスター特効ＩＤ
    "tokkouNo",//特効グループ番号
    "id_m_team",//マスターチームＩＤ
    "magNo",//マスター特効チーム倍率番号
  };
  
  class TMdm_Tokkou: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Tokkou();
    virtual ~TMdm_Tokkou();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Tokkou dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Tokkou dtNo);
    s64 GetPKDt(s64 pk, enmdm_Tokkou dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Tokkou dtNo);
  };
}
