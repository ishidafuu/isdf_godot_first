#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Rarity
  {
    mdm_rarity_pk,//素材価格ＩＤ
    mdm_rarity_priceSell,//売るときの値段
    mdm_rarity_priceBuy,//買うときの値段
    mdm_rarity_gouseiTime,//合成時間
    mdm_rarity_staminaLim,//チームスタミナ限界
    mdm_rarity_sozaiQuality,//素材質確率
    mdm_rarity_vshumWin,//対戦ポイント（勝利）
    mdm_rarity_vshumTimeupWin,//対戦ポイント（時間切れ勝利）
    mdm_rarity_vshumTimeupLose,//対戦ポイント（時間切れ敗北）
    mdm_rarity_vshumLose,//対戦ポイント（敗北）
    mdm_rarity_vshumWin_DC,//対戦ポイント（切断勝利）
    mdm_rarity_vshumTimeupWin_DC,//対戦ポイント（切断時間切れ勝利）
    mdm_rarity_vshumTimeupLose_DC,//対戦ポイント（切断時間切れ敗北）
    mdm_rarity_vshumLose_DC,//対戦ポイント（切断敗北）
    mdm_rarity_priceTeamSell,//チームを売るときの値段
    mdm_rarity_charaLvCap,//チームキャラのレベルキャップ
  };
  
  const std::string M_RARITYCLM[] = 
  {
    "pk",//素材価格ＩＤ
    "priceSell",//売るときの値段
    "priceBuy",//買うときの値段
    "gouseiTime",//合成時間
    "staminaLim",//チームスタミナ限界
    "sozaiQuality",//素材質確率
    "vshumWin",//対戦ポイント（勝利）
    "vshumTimeupWin",//対戦ポイント（時間切れ勝利）
    "vshumTimeupLose",//対戦ポイント（時間切れ敗北）
    "vshumLose",//対戦ポイント（敗北）
    "vshumWin_DC",//対戦ポイント（切断勝利）
    "vshumTimeupWin_DC",//対戦ポイント（切断時間切れ勝利）
    "vshumTimeupLose_DC",//対戦ポイント（切断時間切れ敗北）
    "vshumLose_DC",//対戦ポイント（切断敗北）
    "priceTeamSell",//チームを売るときの値段
    "charaLvCap",//チームキャラのレベルキャップ
  };
  
  class TMdm_Rarity: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Rarity();
    virtual ~TMdm_Rarity();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Rarity dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Rarity dtNo);
    s64 GetPKDt(s64 pk, enmdm_Rarity dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Rarity dtNo);
  };
}
