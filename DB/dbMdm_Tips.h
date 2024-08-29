#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Tips
  {
    mdm_tips_pk,//ＴｉｐｓＩＤ
    mdm_tips_id_m_timer,//タイム管理ＩＤ
    mdm_tips_osType,//ＯＳ種類
    mdm_tips_avtHair,//髪番号
    mdm_tips_avtFace,//顔番号
    mdm_tips_avtHairCol,//髪色
    mdm_tips_avtSkinCol,//肌色
    mdm_tips_avtEyeCol,//目色
    mdm_tips_avtAccCol,//アクセ色
    mdm_tips_avtClotheCol,//服色
    mdm_tips_avtShirCol,//シャツ色
    mdm_tips_avtPantCol,//パンツ色
    mdm_tips_avtSideLineCol,//サイドライン色
    mdm_tips_avtUnderLineCol,//ライン色
    mdm_tips_avtShoeCol,//シューズ色
    mdm_tips_avtItem,//所持アイテム
    mdm_tips_gb_f,//GBフラグ
    mdm_tips_personType,//キャラタイプ
    mdm_tips_moveType,//移動タイプ
    mdm_tips_speedType,//移動速度
    mdm_tips_selectPercent,//出現率
  };
  
  enum enmdm_TipsStr
  {
    mdm_tips_name_c32,//チーム名
    mdm_tips_text0_c128,//テキスト０
    mdm_tips_text1_c128,//テキスト１
    mdm_tips_text2_c128,//テキスト２
    mdm_tips_text3_c128,//テキスト３
    mdm_tips_text4_c128,//テキスト４
    mdm_tips_text5_c128,//テキスト５
  };
  

  const std::string M_TIPSCLM[] = 
  {
    "pk",//ＴｉｐｓＩＤ
    "id_m_timer",//タイム管理ＩＤ
    "osType",//ＯＳ種類
    "avtHair",//髪番号
    "avtFace",//顔番号
    "avtHairCol",//髪色
    "avtSkinCol",//肌色
    "avtEyeCol",//目色
    "avtAccCol",//アクセ色
    "avtClotheCol",//服色
    "avtShirCol",//シャツ色
    "avtPantCol",//パンツ色
    "avtSideLineCol",//サイドライン色
    "avtUnderLineCol",//ライン色
    "avtShoeCol",//シューズ色
    "avtItem",//所持アイテム
    "gb_f",//GBフラグ
    "personType",//キャラタイプ
    "moveType",//移動タイプ
    "speedType",//移動速度
    "selectPercent",//出現率
  };
  
  const std::string M_TIPSCLMSTR[] = 
  {
    "name_c32",//チーム名
    "text0_c128",//テキスト０
    "text1_c128",//テキスト１
    "text2_c128",//テキスト２
    "text3_c128",//テキスト３
    "text4_c128",//テキスト４
    "text5_c128",//テキスト５
  };
  
  class TMdm_Tips: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Tips();
    virtual ~TMdm_Tips();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Tips dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Tips dtNo);
    std::string GetLineDtStr(s64 line, enmdm_TipsStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Tips dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Tips dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_TipsStr dtNo);
  };
}
