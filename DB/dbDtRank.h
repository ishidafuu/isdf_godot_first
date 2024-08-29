#pragma once
#include "dbInclude.h"

namespace db
{

  //ランクデータ///////////////////////////
  enum enRankLevel//レベル
  {
    rkBallSpd = 0,//球速
    rkShStMotion,//振りかぶりフレ
    rkShEdMotion,//投げ終わりフレ
    rkPaSpd,//パス速度
    rkPaBwSpd,//パスバウンド速度
    rkGrpSpin,//グラフィック回転速度
    rkLandRefAg,//着地時反射角度
    rkBwSpCourt,//バウンド速度コート
    rkBwSpWall,//バウンド速度壁
    rkDodgeTime,//よけ開始時間範囲
    rkBallSpdInc,//必殺シュート速度＋
    rkBallSpdDec,//必殺シュート速度－
    rkLv_END,
  };
  enum enRankSpeed//すばやさ
  {
    rkDashAcc = rkLv_END,//ダッシュ加速度
    rkDashMAXAcc,//ダッシュ限界加速量
    rkWkX,//歩き速度Ｘ
    rkLDSJPX,//前押しダッシュジャンプ飛距離dX
    rkWkJPX,//歩Jp飛距離dX
    rkPaWaitPos,//パス待ち位置
    rkIkiItv,//息継ぎ間隔
    rkspd_END,
  };
  enum enRankHP//のこりＨＰ
  {
    rkHPtoRank = rkspd_END,//のこりＨＰ→ランク
    rkRevFrm,//起き上がりまでの時間
    rkRollNum,//ごろごろ回数
    rkBreath,//息切れ回数
    rkBreathRev,//息切れコート戻り時間
    rkhp_END,
  };
  enum enRankTech//技術比
  {
    rkTechtoRank = (s32)rkhp_END,//技量比→ランク
    rkCatchTime,//キャッチ時間
    rkNiceCatchTime,//ナイスキャッチ時間
    rkSemiCatchTime,//キャッチ時間
    rkSemiNiceCatchTime,//ナイスキャッチ時間
    rkRefSp,//反射速度
    rkRefAg,//反射角度
    rktech_END
  };
  enum enRankPow//パワー比
  {
    rkPowtoRank = (s32)rktech_END,//パワー比→ランク
    rkSlipSp,//スリップ量
    rkSlipEff,//スリップ量
    rkFrSkip,//ふっとびフレームスキップ
    rkFryNlSpd,//普通シュート吹っ飛び速度　Lv16
    rkFryNlAgl,//普通シュート吹っ飛び角度　Lv16
    rkFrySpSpd,//必殺シュート汎用吹っ飛び速度　Lv16
    rkFrySpAgl,//必殺シュート汎用吹っ飛び角度　Lv16
    rkFryLsYSp,//緩い吹っ飛び速度　Lv08
    rkFryLsAgl,//緩い吹っ飛び角度　Lv08
    rkFryLsXSp,//緩い吹っ飛Ｘ速度　Lv08
    rkFryLsGrv,//緩い吹っ飛重力　Lv08
    rkFryBrYSp,//脳天吹っ飛び速度　Lv08
    rkFryBrAgl,//脳天吹っ飛び角度　Lv08
    rkFryBrXSp,//脳天吹っ飛Ｘ速度　Lv08
    rkFryVrYSp,//真上吹っ飛び速度　Lv08
    rkFryVrAgl,//真上吹っ飛び角度　Lv08
    rkFryVrXSp,//真上吹っ飛Ｘ速度　Lv08
    rkFryHrYSp,//真横吹っ飛び速度　Lv08
    rkFryHrAgl,//真横吹っ飛び角度　Lv08
    rkFryHrXSp,//真横吹っ飛Ｘ速度　Lv08
    rkFryRvYSp,//逆吹っ飛び速度　Lv08
    rkFryRvAgl,//逆吹っ飛び角度　Lv08
    rkFryRvXSp,//逆吹っ飛Ｘ速度　Lv08
    rkFryGrYSp,//周回吹っ飛び速度　Lv08
    rkFryGrAgl,//周回吹っ飛び角度　Lv08
    rkFryGrXSp,//周回吹っ飛Ｘ速度　Lv08
    rkFryGrGrv,//周回吹っ飛重力　Lv08
    rkFryDnYSp,//その場ダウン速度　Lv08
    rkFryDnAgl,//その場ダウン角度　Lv08
    rkFryDnXSp,//その場ダウンＸ速度　Lv08
    rkFryVRYSp,//縦跳吹っ飛び速度　Lv08
    rkFryVRAgl,//縦跳吹っ飛び角度　Lv08
    rkFryVRXSp,//縦跳吹っ飛Ｘ速度　Lv08
    rkFryVRGrv,//縦跳吹っ飛重力　Lv08
    rkFryHRYSp,//横跳吹っ飛び速度　Lv08
    rkFryHRAgl,//横跳吹っ飛び角度　Lv08
    rkFryHRXSp,//横跳吹っ飛Ｘ速度　Lv08
    rkFryHRGrv,//横跳吹っ飛重力　Lv08
    rkFryEx1YSp,//特１吹っ飛び速度
    rkFryEx1Agl,//特１吹っ飛び角度
    rkFryEx1XSp,//特１吹っ飛Ｘ速度
    rkFryEx2YSp,//特２吹っ飛び速度
    rkFryEx2Agl,//特２吹っ飛び角度
    rkFryEx2XSp,//特２吹っ飛Ｘ速度
    rkFryEx3YSp,//特３吹っ飛び速度
    rkFryEx3Agl,//特３吹っ飛び角度
    rkFryEx3XSp,//特３吹っ飛Ｘ速度
    rkFryEx4YSp,//特４吹っ飛び速度
    rkFryEx4Agl,//特４吹っ飛び角度
    rkFryEx4XSp,//特４吹っ飛Ｘ速度
    rkFryEx5YSp,//特５吹っ飛び速度
    rkFryEx5Agl,//特５吹っ飛び角度
    rkFryEx5XSp,//特５吹っ飛Ｘ速度
    rkFryEx6YSp,//特６吹っ飛び速度
    rkFryEx6Agl,//特６吹っ飛び角度
    rkFryEx6XSp,//特６吹っ飛Ｘ速度
    rkFryEx7YSp,//特７吹っ飛び速度
    rkFryEx7Agl,//特７吹っ飛び角度
    rkFryEx7XSp,//特７吹っ飛Ｘ速度
    rkFryEx8YSp,//特８吹っ飛び速度
    rkFryEx8Agl,//特８吹っ飛び角度
    rkFryEx8XSp,//特８吹っ飛Ｘ速度
    rkpow_END,
  };


  const s32 enbtRankDataEND = rkpow_END;

  class TDtRank
  {
  protected:
    s32 dt_[enbtRankDataEND][RANKNUM];
  public:
    explicit TDtRank(void);
    virtual ~TDtRank(void);

    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);
    //取得
    s32 GenGetRankDt(s32 dtNo, s32 rank);
    s32 GetRankDtLevel(enRankLevel dtNo, s32 rank);
    s32 GetRankDtSpeed(enRankSpeed dtNo, s32 rank);
    s32 GetRankDtHP(enRankHP dtNo, s32 rank);
    s32 GetRankDtTech(enRankTech dtNo, s32 rank);
    s32 GetRankDtPow(enRankPow dtNo, s32 rank);

    s32 GetRankTech(s32 shtech, s32 hitmantech);
    s32 GetRankPow(s32 shpow, s32 hitmanpow);
    s32 GetRankHP(s32 hp, s32 maxhp);
  };
}
