#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_GachaGrp
  {
    mdm_gachagrp_pk,//ガチャ中身グループＩＤ
    mdm_gachagrp_id_m_timer,//タイム管理ＩＤ
    mdm_gachagrp_lotGrpNo,//抽選グループ番号
    mdm_gachagrp_cntGrpNo,//中身グループ番号
    mdm_gachagrp_grade,//等級
    mdm_gachagrp_prob,//確率（重み）
    mdm_gachagrp_boxNum,//BOX時個数
    mdm_gachagrp_boxReset_f,//BOXリセット対象flg
  };
  
  enum enmdm_GachaGrpStr
  {
    mdm_gachagrp_name_c64,//グループの名前
  };
  

  const std::string M_GACHAGRPCLM[] = 
  {
    "pk",//ガチャ中身グループＩＤ
    "id_m_timer",//タイム管理ＩＤ
    "lotGrpNo",//抽選グループ番号
    "cntGrpNo",//中身グループ番号
    "grade",//等級
    "prob",//確率（重み）
    "boxNum",//BOX時個数
    "boxReset_f",//BOXリセット対象flg
  };
  
  const std::string M_GACHAGRPCLMSTR[] = 
  {
    "name_c64",//グループの名前
  };
  
  class TMdm_GachaGrp: public TModelGen
  {
  protected:

  public:
    explicit TMdm_GachaGrp();
    virtual ~TMdm_GachaGrp();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_GachaGrp dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_GachaGrp dtNo);
    std::string GetLineDtStr(s64 line, enmdm_GachaGrpStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_GachaGrp dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_GachaGrp dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_GachaGrpStr dtNo);
  };
}
