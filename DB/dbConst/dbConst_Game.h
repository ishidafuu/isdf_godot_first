#pragma once
#include "dbInclude.h"

namespace db
{
  struct TShotDt
  {
    s32 tdx;
    s32 tdz;
    s32 tdy;
    s32 tdxs;
    s32 tdzs;
    s32 tdys;
  };
  struct TCurveDt
  {
    BOOL ZShoot_f;
    f32 C1X;
    f32 C1Z;
    s32 C1Y;
    BOOL C2_f;
    BOOL C2Y_f;
    s32 h;
    base::enCrsType C1XKey;
    base::enCrsType C1ZKey;
    base::enCrsType C1YKey;
    BOOL UKey_f;
    BOOL DKey_f;
    BOOL LKey_f;
    BOOL RKey_f;

    BOOL OvTag_f;

    BOOL CvLeft_f;
    BOOL CvRight_f;
    BOOL CvRear_f;
    BOOL CvFront_f;
    BOOL CvHop_f;

    void Init()
    {
      Zeromem(this);
    }
  };


  enum enCrtLine { crtLLine, crtRLine, crtFLine, crtBLine };
  enum enDBPOS { dbpoI0, dbpoI1, dbpoI2, dbpoI3, dbpoO2, dbpoO3, dbpoO4 };//ポジション
  enum enATType { ATA, ATD, ATF };//動作
  enum enCKeyType { CKTX, CKTZ };//変化方向
  enum enJudgeType { JdNoHit, JdCatch, JdNiceCatch, JdHit };
  enum enGetTagType { gtLevel, gtTgY, gtTg16, gtBase };//変化方向

  //外見
  enum enCharLooks
  {
    clk_id,//キャラＩＤ
    clk_FaceNo,//かお番号
    clk_ZuraNo,//ずら番号
    clk_SkinCol,//はだいろ
    clk_HairCol,//かみいろ
    clk_AcceCol,//あくせいろ
    clk_EyesCol,//めいろ
    clk_SearchFlag,//検索ひっかかるフラグ
    clk_LooksFlag,//みためフラグ
    clk_END,
  };

  //レベル
  enum enCharLv
  {
    clv_Pow = clk_END,//ぱわー
    clv_Guts,//がっつ
    clv_Kire,//きれ
    clv_Tech,//てく
    clv_Speed,//すばやさ（必殺ステップ）
    clv_Rev,//レベル補正
    clv_DefShotID,//シュートＩＤ
    clv_Kakusi,//上田スイッチなど
    clv_Formation,//フォーメーション
    clv_COM,//COM思考タイプ
    clv_AtcLine,//攻撃ライン
    clv_DsmnNoJp,//ダッシュマンがジャンプしない
	clv_RarityLv,//レアリティレベル
	clv_BodyType,//体技
	clv_CharacterType,//心
	clv_RandomVal,//ブレ値（乱数値）
    clv_END,
  };
  enum enSearchFlag
  {
    sf_None = 0x00,
    sf_Face = 0x01,//顔検索する
    sf_Hair = 0x02,//づらけんさくする
  };
  enum enLooksFlag
  {
    lf_None = 0x00,
    lf_Hage = 0x01,// スキンヘッド
    lf_BarCode = 0x02,//バーコード
    lf_Pants = 0x04,//パンツ
    lf_Mask = 0x08,//マスク
    lf_ZuraKaibutsu = 0x10,//怪物髪
    lf_KaoKaibutsu = 0x20,//怪物ズラ
  };

  //星座
  enum enCharStar
  {
    cdst_00_ARIES,//おひつじ3月21日?4月19日生まれ
    cdst_01_TAURUS,//おうし4月20日?5月20日生まれ
    cdst_02_GEMINI,//ふたご5月21日?6月21日生まれ
    cdst_03_CANCER,//かに6月22日?7月22日生まれ
    cdst_04_LEO,//しし7月23日?8月22日生まれ
    cdst_05_VIRGO,//おとめ8月23日?9月22日生まれ
    cdst_06_LIBRA,//てんびん9月23日?10月23日生まれ
    cdst_07_SCORPIO,//さそり10月24日?11月22日生まれ
    cdst_08_SAGITTARIUS,//いて11月23日?12月21日生まれ
    cdst_09_CAPRICORN,//やぎ12月22日?1月19日生まれ
    cdst_10_AQUARIUS,//みずがめ1月20日?2月18日生まれ
    cdst_11_PISCES,//うお2月19日?3月20日生まれ
    cdst_UNKNOWN,//ひみつ
    cdst_END,
  };

  // キャラデータフラグ
  enum enCharSp_fs
  {
    csNone = 0x00, // 通常
    csUeda = 0x01,//うえだ
    cdRandom = 0x02,//ランダム
  };

  const s32 PACAJPTIME = 13;//ジャンプパスキャッチタイミング
  const s32 JPINVALIDHEIGHT = 16; // 操作無効高さ
  const s32 JPGAIYAPER = 70;//外野斜めジャンプのXZ

}
