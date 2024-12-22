#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Gacha
  {
    mdm_gacha_pk,//ガチャＩＤ
    mdm_gacha_id_m_timer,//タイム管理ＩＤ
    mdm_gacha_id_m_supply_ken,//ガチャ券アイテムID
    mdm_gacha_kenNum,//ガチャ券必要枚数
    mdm_gacha_lotNumA,//抽選回数A
    mdm_gacha_lotGrpNoA,//抽選グループA
    mdm_gacha_lotGradeA,//抽選等級A
    mdm_gacha_lotNumB,//抽選回数B
    mdm_gacha_lotGrpNoB,//抽選グループB
    mdm_gacha_lotGradeB,//抽選等級B
    mdm_gacha_box_f,//BOXフラグ
    mdm_gacha_bonus_f,//ボーナスフラグ
    mdm_gacha_bonusNumA,//ボーナス抽選回数A
    mdm_gacha_bonusGrpA,//ボーナス抽選グループA
    mdm_gacha_bonusGradeA,//ボーナス抽選等級A
    mdm_gacha_bonusNumB,//ボーナス抽選回数B
    mdm_gacha_bonusGrpB,//ボーナス抽選グループB
    mdm_gacha_bonusGradeB,//ボーナス抽選等級B
    mdm_gacha_step_f,//ステップアップフラグ
    mdm_gacha_id_m_Gacha_stepNext,//ステップアップ用NEXTガチャID（NGNUMで頭打ち）
    mdm_gacha_id_m_Gacha_stepReset,//ステップアップリセット時ガチャID（NGNUMでリセットなし
    mdm_gacha_stepLvUp_f,//ステップ連動レベルアップフラグ
    mdm_gacha_id_m_gacha_stepLvUp,//ステップ連動レベルアップ参照ガチャID
    mdm_gacha_point_f,//ポイントフラグ
    mdm_gacha_id_m_gacha_point,//ポイントガチャＩＤ
    mdm_gacha_omake_f,//おまけBOXフラグ
    mdm_gacha_id_m_gacha_omake,//おまけBOXID
    mdm_gacha_retry_f,//リトライフラグ
    mdm_gacha_iconNo,//アイコン番号
  };
  
  enum enmdm_GachaStr
  {
    mdm_gacha_name_c64,//ガチャの名前
    mdm_gacha_text0_c64,//テキスト０
    mdm_gacha_text1_c64,//テキスト１
    mdm_gacha_text2_c64,//テキスト２
    mdm_gacha_text3_c64,//テキスト３
  };
  

  const std::string M_GACHACLM[] = 
  {
    "pk",//ガチャＩＤ
    "id_m_timer",//タイム管理ＩＤ
    "id_m_supply_ken",//ガチャ券アイテムID
    "kenNum",//ガチャ券必要枚数
    "lotNumA",//抽選回数A
    "lotGrpNoA",//抽選グループA
    "lotGradeA",//抽選等級A
    "lotNumB",//抽選回数B
    "lotGrpNoB",//抽選グループB
    "lotGradeB",//抽選等級B
    "box_f",//BOXフラグ
    "bonus_f",//ボーナスフラグ
    "bonusNumA",//ボーナス抽選回数A
    "bonusGrpA",//ボーナス抽選グループA
    "bonusGradeA",//ボーナス抽選等級A
    "bonusNumB",//ボーナス抽選回数B
    "bonusGrpB",//ボーナス抽選グループB
    "bonusGradeB",//ボーナス抽選等級B
    "step_f",//ステップアップフラグ
    "id_m_Gacha_stepNext",//ステップアップ用NEXTガチャID（NGNUMで頭打ち）
    "id_m_Gacha_stepReset",//ステップアップリセット時ガチャID（NGNUMでリセットなし
    "stepLvUp_f",//ステップ連動レベルアップフラグ
    "id_m_gacha_stepLvUp",//ステップ連動レベルアップ参照ガチャID
    "point_f",//ポイントフラグ
    "id_m_gacha_point",//ポイントガチャＩＤ
    "omake_f",//おまけBOXフラグ
    "id_m_gacha_omake",//おまけBOXID
    "retry_f",//リトライフラグ
    "iconNo",//アイコン番号
  };
  
  const std::string M_GACHACLMSTR[] = 
  {
    "name_c64",//ガチャの名前
    "text0_c64",//テキスト０
    "text1_c64",//テキスト１
    "text2_c64",//テキスト２
    "text3_c64",//テキスト３
  };
  
  class TMdm_Gacha: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Gacha();
    virtual ~TMdm_Gacha();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Gacha dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Gacha dtNo);
    std::string GetLineDtStr(s64 line, enmdm_GachaStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Gacha dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Gacha dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_GachaStr dtNo);
  };
}
