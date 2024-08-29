#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_HukuCnt
  {
    mdm_hukucnt_pk,//ふくびき中身詳細ＩＤ
    mdm_hukucnt_id_w_hukugrp,//ふくびきグループ番号
    mdm_hukucnt_grade,//等級
    mdm_hukucnt_num,//本数
    mdm_hukucnt_itemType,//アイテムタイプ
    mdm_hukucnt_itemID,//アイテムID
    mdm_hukucnt_itemNum,//アイテム個数
  };
  
  const std::string M_HUKUCNTCLM[] = 
  {
    "pk",//ふくびき中身詳細ＩＤ
    "id_w_hukugrp",//ふくびきグループ番号
    "grade",//等級
    "num",//本数
    "itemType",//アイテムタイプ
    "itemID",//アイテムID
    "itemNum",//アイテム個数
  };
  
  class TMdm_HukuCnt: public TModelGen
  {
  protected:

  public:
    explicit TMdm_HukuCnt();
    virtual ~TMdm_HukuCnt();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_HukuCnt dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_HukuCnt dtNo);
    s64 GetPKDt(s64 pk, enmdm_HukuCnt dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_HukuCnt dtNo);
  };
}
