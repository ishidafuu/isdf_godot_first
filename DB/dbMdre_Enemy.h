#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdre_Enemy
  {
    mdre_enemy_pk,//敵ID
    mdre_enemy_id_kantoku,//監督ＩＤ
    mdre_enemy_mbgID,//モバゲーＩＤ
    mdre_enemy_avtHair,//髪番号
    mdre_enemy_avtFace,//顔番号
    mdre_enemy_avtHairCol,//髪色
    mdre_enemy_avtSkinCol,//肌色
    mdre_enemy_avtEyeCol,//目色
    mdre_enemy_avtAccCol,//アクセ色
    mdre_enemy_avtClotheCol,//服色
    mdre_enemy_id_Team,//疑似対人用チームＩＤ
    mdre_enemy_id_m_bgm,//ホームグラウンド背景
    mdre_enemy_id_m_haikei,//ホームグラウンドＢＧＭ
    mdre_enemy_create_date,//作成日時
    mdre_enemy_updated_date,//更新日時
    mdre_enemy_del_f,//削除フラグ
  };
  
  enum enmdre_EnemyStr
  {
    mdre_enemy_name_c32,//監督名
    mdre_enemy_prof0_c128,//自己紹介０
    mdre_enemy_prof1_c128,//自己紹介１
    mdre_enemy_prof2_c128,//自己紹介２
  };
  

  const std::string RE_ENEMYCLM[] = 
  {
    "pk",//敵ID
    "id_kantoku",//監督ＩＤ
    "mbgID",//モバゲーＩＤ
    "avtHair",//髪番号
    "avtFace",//顔番号
    "avtHairCol",//髪色
    "avtSkinCol",//肌色
    "avtEyeCol",//目色
    "avtAccCol",//アクセ色
    "avtClotheCol",//服色
    "id_Team",//疑似対人用チームＩＤ
    "id_m_bgm",//ホームグラウンド背景
    "id_m_haikei",//ホームグラウンドＢＧＭ
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  const std::string RE_ENEMYCLMSTR[] = 
  {
    "name_c32",//監督名
    "prof0_c128",//自己紹介０
    "prof1_c128",//自己紹介１
    "prof2_c128",//自己紹介２
  };
  
  class TMdre_Enemy: public TModelGen
  {
  protected:

  public:
    explicit TMdre_Enemy();
    virtual ~TMdre_Enemy();
    s64 GetLineDt(s64 line, enmdre_Enemy dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdre_Enemy dtNo);
    std::string GetLineDtStr(s64 line, enmdre_EnemyStr dtNo);
    s64 GetPKDt(s64 pk, enmdre_Enemy dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdre_Enemy dtNo);
    std::string GetPKDtStr(s64 pk, enmdre_EnemyStr dtNo);
  };
}
