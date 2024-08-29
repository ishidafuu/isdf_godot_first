#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdKantokuSub
  {
    mdkantokusub_pk,//監督ＩＤ
    mdkantokusub_mbgID,//モバゲーＩＤ
    mdkantokusub_id_guild,//ギルドＩＤ
    mdkantokusub_logbo_date,//ログインボーナス取得日時
    mdkantokusub_loginNum,//ログイン回数
    mdkantokusub_vsbonus_date,//ＶＳボーナス取得日時
    mdkantokusub_vsbonusNum,//ＶＳボーナス回数
    mdkantokusub_osType,//osの種類
    mdkantokusub_monthKakin,//月額課金額
    mdkantokusub_errorNum,//不正値検出数
    mdkantokusub_vsHum,//人対戦数
    mdkantokusub_vsHumWin,//人対戦勝利数
    mdkantokusub_vsHumWins,//人対戦連勝数
    mdkantokusub_vsError,//対戦不正終了回数
    mdkantokusub_vsReport,//被通報回数
    mdkantokusub_id_team,//最終使用チームＩＤお気に入り
    mdkantokusub_id_teamD,//最終使用チームＩＤD
    mdkantokusub_id_teamC,//最終使用チームＩＤC
    mdkantokusub_id_teamB,//最終使用チームＩＤB
    mdkantokusub_id_teamA,//最終使用チームＩＤA
    mdkantokusub_id_teamS,//最終使用チームＩＤS
    mdkantokusub_id_teamSS,//最終使用チームＩＤSS
    mdkantokusub_id_teamSSS,//最終使用チームＩＤSSS
    mdkantokusub_id_m_bgm,//ホームグラウンド背景
    mdkantokusub_id_m_haikei,//ホームグラウンドＢＧＭ
    mdkantokusub_id_m_giftbox,//最終受け取り運営贈り物ＩＤ
    mdkantokusub_id_kantoku_inv,//招待者監督ＩＤ
    mdkantokusub_id_shset,//シュートセットＩＤ
    mdkantokusub_avtHair,//髪番号
    mdkantokusub_avtFace,//顔番号
    mdkantokusub_avtHairCol,//髪色
    mdkantokusub_avtSkinCol,//肌色
    mdkantokusub_avtEyeCol,//目色
    mdkantokusub_avtAccCol,//アクセ色
    mdkantokusub_avtClotheCol,//服色
    mdkantokusub_tutoBit,//チュートリアルビット
    mdkantokusub_optionBit,//オプションビット
    mdkantokusub_create_date,//作成日時
    mdkantokusub_updated_date,//更新日時
    mdkantokusub_del_f,//削除フラグ
  };
  
  enum enmdKantokuSubStr
  {
    mdkantokusub_name_c32,//監督名
    mdkantokusub_prof0_c128,//自己紹介０
    mdkantokusub_prof1_c128,//自己紹介１
    mdkantokusub_prof2_c128,//自己紹介２
  };
  

  const std::string KANTOKUSUBCLM[] = 
  {
    "pk",//監督ＩＤ
    "mbgID",//モバゲーＩＤ
    "id_guild",//ギルドＩＤ
    "logbo_date",//ログインボーナス取得日時
    "loginNum",//ログイン回数
    "vsbonus_date",//ＶＳボーナス取得日時
    "vsbonusNum",//ＶＳボーナス回数
    "osType",//osの種類
    "monthKakin",//月額課金額
    "errorNum",//不正値検出数
    "vsHum",//人対戦数
    "vsHumWin",//人対戦勝利数
    "vsHumWins",//人対戦連勝数
    "vsError",//対戦不正終了回数
    "vsReport",//被通報回数
    "id_team",//最終使用チームＩＤお気に入り
    "id_teamD",//最終使用チームＩＤD
    "id_teamC",//最終使用チームＩＤC
    "id_teamB",//最終使用チームＩＤB
    "id_teamA",//最終使用チームＩＤA
    "id_teamS",//最終使用チームＩＤS
    "id_teamSS",//最終使用チームＩＤSS
    "id_teamSSS",//最終使用チームＩＤSSS
    "id_m_bgm",//ホームグラウンド背景
    "id_m_haikei",//ホームグラウンドＢＧＭ
    "id_m_giftbox",//最終受け取り運営贈り物ＩＤ
    "id_kantoku_inv",//招待者監督ＩＤ
    "id_shset",//シュートセットＩＤ
    "avtHair",//髪番号
    "avtFace",//顔番号
    "avtHairCol",//髪色
    "avtSkinCol",//肌色
    "avtEyeCol",//目色
    "avtAccCol",//アクセ色
    "avtClotheCol",//服色
    "tutoBit",//チュートリアルビット
    "optionBit",//オプションビット
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  const std::string KANTOKUSUBCLMSTR[] = 
  {
    "name_c32",//監督名
    "prof0_c128",//自己紹介０
    "prof1_c128",//自己紹介１
    "prof2_c128",//自己紹介２
  };
  
  class TMdKantokuSub: public TModelGen
  {
  protected:

  public:
    explicit TMdKantokuSub();
    virtual ~TMdKantokuSub();
    s64 GetLineDt(s64 line, enmdKantokuSub dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdKantokuSub dtNo);
    std::string GetLineDtStr(s64 line, enmdKantokuSubStr dtNo);
    s64 GetPKDt(s64 pk, enmdKantokuSub dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdKantokuSub dtNo);
    std::string GetPKDtStr(s64 pk, enmdKantokuSubStr dtNo);
    s64 GetMyDt(enmdKantokuSub dtNo);
    BOOL GetMyDtBOOL(enmdKantokuSub dtNo);
    std::string GetMyDtStr(enmdKantokuSubStr dtNo);
  };
}
