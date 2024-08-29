#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_TokkouSozai
  {
    mdm_tokkousozai_pk,//マスター特効ＩＤ
    mdm_tokkousozai_tokkouNo,//特効グループ番号
    mdm_tokkousozai_id_m_sozai,//マスターチームＩＤ
    mdm_tokkousozai_magD,//Dランク倍率
    mdm_tokkousozai_magC,//Cランク倍率
    mdm_tokkousozai_magB,//Bランク倍率
    mdm_tokkousozai_magA,//Aランク倍率
    mdm_tokkousozai_magS,//Sランク倍率
    mdm_tokkousozai_magSS,//SSランク倍率
    mdm_tokkousozai_magSSS,//SSSランク倍率
  };
  
  const std::string M_TOKKOUSOZAICLM[] = 
  {
    "pk",//マスター特効ＩＤ
    "tokkouNo",//特効グループ番号
    "id_m_sozai",//マスターチームＩＤ
    "magD",//Dランク倍率
    "magC",//Cランク倍率
    "magB",//Bランク倍率
    "magA",//Aランク倍率
    "magS",//Sランク倍率
    "magSS",//SSランク倍率
    "magSSS",//SSSランク倍率
  };
  
  class TMdm_TokkouSozai: public TModelGen
  {
  protected:

  public:
    explicit TMdm_TokkouSozai();
    virtual ~TMdm_TokkouSozai();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_TokkouSozai dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_TokkouSozai dtNo);
    s64 GetPKDt(s64 pk, enmdm_TokkouSozai dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_TokkouSozai dtNo);
  };
}
