#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Huku
  {
    mdm_huku_pk,//マスターふくびきＩＤ
    mdm_huku_id_w_hukugrp,//ふくびきグループ番号
    mdm_huku_id_m_timer,//タイム管理ＩＤ
    mdm_huku_kenNum,//ふくびき券必要枚数
    mdm_huku_priority,//ソート優先度
    mdm_huku_lotNumA,//抽選回数A
    mdm_huku_lotGradeA,//抽選等級A
    mdm_huku_lotNumB,//抽選回数B
    mdm_huku_lotGradeB,//抽選等級B
    mdm_huku_iconNo,//アイコン番号
  };
  
  enum enmdm_HukuStr
  {
    mdm_huku_name_c64,//ふくびきの名前
    mdm_huku_text0_c64,//テキスト０
    mdm_huku_text1_c64,//テキスト１
    mdm_huku_text2_c64,//テキスト２
    mdm_huku_text3_c64,//テキスト３
  };
  

  const std::string M_HUKUCLM[] = 
  {
    "pk",//マスターふくびきＩＤ
    "id_w_hukugrp",//ふくびきグループ番号
    "id_m_timer",//タイム管理ＩＤ
    "kenNum",//ふくびき券必要枚数
    "priority",//ソート優先度
    "lotNumA",//抽選回数A
    "lotGradeA",//抽選等級A
    "lotNumB",//抽選回数B
    "lotGradeB",//抽選等級B
    "iconNo",//アイコン番号
  };
  
  const std::string M_HUKUCLMSTR[] = 
  {
    "name_c64",//ふくびきの名前
    "text0_c64",//テキスト０
    "text1_c64",//テキスト１
    "text2_c64",//テキスト２
    "text3_c64",//テキスト３
  };
  
  class TMdm_Huku: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Huku();
    virtual ~TMdm_Huku();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Huku dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Huku dtNo);
    std::string GetLineDtStr(s64 line, enmdm_HukuStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Huku dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Huku dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_HukuStr dtNo);
  };
}
