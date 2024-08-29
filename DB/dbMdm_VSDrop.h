#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_VSDrop
  {
    mdm_vsdrop_pk,//新通信対戦ボーナスＩＤ
    mdm_vsdrop_prob_hw,//確率（重み）対人勝利
    mdm_vsdrop_prob_hl,//確率（重み）対人敗北
    mdm_vsdrop_prob_cw,//確率（重み）ＣＯＭ勝利
    mdm_vsdrop_prob_cl,//確率（重み）ＣＯＭ敗北
    mdm_vsdrop_itemType,//アイテムタイプ
    mdm_vsdrop_itemID,//アイテムＩＤ
    mdm_vsdrop_itemNum,//アイテム個数
  };
  
  const std::string M_VSDROPCLM[] = 
  {
    "pk",//新通信対戦ボーナスＩＤ
    "prob_hw",//確率（重み）対人勝利
    "prob_hl",//確率（重み）対人敗北
    "prob_cw",//確率（重み）ＣＯＭ勝利
    "prob_cl",//確率（重み）ＣＯＭ敗北
    "itemType",//アイテムタイプ
    "itemID",//アイテムＩＤ
    "itemNum",//アイテム個数
  };
  
  class TMdm_VSDrop: public TModelGen
  {
  protected:

  public:
    explicit TMdm_VSDrop();
    virtual ~TMdm_VSDrop();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_VSDrop dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_VSDrop dtNo);
    s64 GetPKDt(s64 pk, enmdm_VSDrop dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_VSDrop dtNo);
  };
}
