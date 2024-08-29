#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_GachaCnt
  {
    mdm_gachacnt_pk,//ガチャ中身詳細ＩＤ
    mdm_gachacnt_cntGrpNo,//中身グループ番号
    mdm_gachacnt_itemType,//アイテムタイプ
    mdm_gachacnt_itemID,//アイテムID
    mdm_gachacnt_itemNum,//アイテム個数
    mdm_gachacnt_prob,//確率（重み）
  };
  
  const std::string M_GACHACNTCLM[] = 
  {
    "pk",//ガチャ中身詳細ＩＤ
    "cntGrpNo",//中身グループ番号
    "itemType",//アイテムタイプ
    "itemID",//アイテムID
    "itemNum",//アイテム個数
    "prob",//確率（重み）
  };
  
  class TMdm_GachaCnt: public TModelGen
  {
  protected:

  public:
    explicit TMdm_GachaCnt();
    virtual ~TMdm_GachaCnt();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_GachaCnt dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_GachaCnt dtNo);
    s64 GetPKDt(s64 pk, enmdm_GachaCnt dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_GachaCnt dtNo);
  };
}
