#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_NaBox
  {
    mdm_nabox_pk,//なつかしＢＯＸＩＤ
    mdm_nabox_id_m_timer,//タイム管理ＩＤ
    mdm_nabox_id_m_supply_ken,//ガチャ券アイテムID
    mdm_nabox_kenNum,//ガチャ券必要枚数
    mdm_nabox_boxSize,//ボックスの数
    mdm_nabox_iconNo,//アイコン番号
  };
  
  enum enmdm_NaBoxStr
  {
    mdm_nabox_name_c64,//なつかしＢＯＸの名前
    mdm_nabox_text0_c64,//テキスト０
    mdm_nabox_text1_c64,//テキスト１
    mdm_nabox_text2_c64,//テキスト２
    mdm_nabox_text3_c64,//テキスト３
  };
  

  const std::string M_NABOXCLM[] = 
  {
    "pk",//なつかしＢＯＸＩＤ
    "id_m_timer",//タイム管理ＩＤ
    "id_m_supply_ken",//ガチャ券アイテムID
    "kenNum",//ガチャ券必要枚数
    "boxSize",//ボックスの数
    "iconNo",//アイコン番号
  };
  
  const std::string M_NABOXCLMSTR[] = 
  {
    "name_c64",//なつかしＢＯＸの名前
    "text0_c64",//テキスト０
    "text1_c64",//テキスト１
    "text2_c64",//テキスト２
    "text3_c64",//テキスト３
  };
  
  class TMdm_NaBox: public TModelGen
  {
  protected:

  public:
    explicit TMdm_NaBox();
    virtual ~TMdm_NaBox();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_NaBox dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_NaBox dtNo);
    std::string GetLineDtStr(s64 line, enmdm_NaBoxStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_NaBox dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_NaBox dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_NaBoxStr dtNo);
  };
}
