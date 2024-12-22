#pragma once
#include "dbInclude.h"
#include "dbObGameGen.h"

namespace db
{
  //十字種類
  enum enPadType
  {
    enPadType_DXL,//DXL
    enPadType_MT,//石田式マニュアル
    enPadType_FC,//ファミコン
    enPadType_SP,//スマホ
    enPadType_AT,//セミオート
  };

  struct TStPad
  {
    s32 dhtime_;
    enCrsType nowDHKey_;//連打
    enCrsType tmpDHKeyLR_;//連打
    enCrsType tmpDHKeyUD_;//連打
    enCrsType nowDHKeyLR_;//連打
    enCrsType nowDHKeyUD_;//連打
    BOOL dhndelay_f_;//斜め待ち
    s32 dhLR_c_;
  };

  //ゲームパッド
  class TMgPad : public TGameObjGen
  {
  protected:
    
    //変数一式
    TStPad stPad_;

    //ＵＩからの入力（セミオート）
    BOOL GenIsUIBit(enUIBitType uibit);
    BOOL GenIsUIBit2(enUIBitType uibit);
    //共通シフト入力(ポーズも含む)
    BOOL GenIsShift(enShiftBitType sft);
    BOOL GenIsShift2(enShiftBitType sft);
    enPadType GetPadType();//入力方式
    BOOL GenIsJump(enBtnType btnA, enBtnType btnB);//ジャンプ
    BOOL IsOtherDown(enCrsType oBtn, enCrsType crs, s32 delay);
    BOOL IsOtherCrs(enCrsType crs, s32 delay);
    void GenGetVec16Crs(BOOL btm_f, BOOL right_f, BOOL* crsL_f, BOOL* crsR_f, BOOL* crsU_f, BOOL* crsD_f);
    BOOL GenIsVecCrs(BOOL btm_f, BOOL right_f, enCrsType crs);
    BOOL GenIsFlicVecCrs(BOOL btm_f, BOOL right_f, enCrsType crs);
    void SetDHit();//連打セット
    void SetDHitUDLR();//連打セット
    void SettmpDHitUD();//連打セット
    void SettmpDHitLR();//連打セット
  public:
    base::TPad* ppad_;

    //コンストラクタ
    explicit TMgPad(TGame* pGame, base::TPad* ppad);
    virtual ~TMgPad(void);
    BOOL IsPadType(enPadType padtype);//入力方式

    //十字が押された（操作権委譲）
    BOOL IsPaTgCtrl();
    //ダッシュマン
    BOOL IsDMCall();
    //ダッシュマン方向キー
    void GetDMCallCrs(BOOL* crsL_f, BOOL* crsR_f, BOOL* crsU_f, BOOL* crsD_f);

    //ＤＸＬボタン入力（シュート切り替えデバッグ用）
    BOOL IsDXLBtn(enBtnType btn);

    //ＵＩからのセット
    void SetBitPause();
    void SetBitTouch();
    void SetBitOrder();
    void SetBitAuto();
    void SetBitSmart();
    void SetBitRetro();
    void SetBitTech();
    void SetBitCmd(s32 cmdNo);

    //ポーズボタン
    BOOL IsPause();

    //画面タッチ（セミオート）
    BOOL IsTouch();

    //次へ
    BOOL IsNext();

    //セミオートか
    BOOL IsSemiAuto();
    //オーダーか
    BOOL IsSemiAutoOrder();
    //スマート操作か
    BOOL IsSmart();

    //作戦番号
    s32 GetCmdNo();

    //ダッシュ方向（斜めダッシュも必要であればそういう）
    //BOOL IsDashUD();
    BOOL IsDash(BOOL ball_f);
    BOOL IsDashLR(enCrsType crs);
    BOOL IsDashAuto();
    //BOOL IsCaCounter();
    //ダッシュ加速入力
    BOOL IsDashAcc(enCrsType needcrs);
    //ダッシュストップ
    BOOL IsDashStop(enCrsType needcrs);

    //ジャンプ入力
    BOOL IsJump();//ジャンプ
    BOOL IsJumpLongTouch();//ジャンプ
    BOOL IsDsJumpLongTouch();//ジャンプ
    BOOL IsJump2();//オートジャンプ（ラインオーバー回避）

    //十字入力ジャンプ
    BOOL IsJumpMv(enCrsType crs);

    //十字入力おしっぱ（歩き）
    BOOL IsAnyWalk2();
    BOOL IsWalk2(enCrsType crs);
    //アナログ歩き
    BOOL IsWalkVec();
    u32 GetWalkVec();

    //パス

    //パス入力
    BOOL IsPass();
    //パス入力おしっぱ（ミラーパス）
    BOOL IsPass2();
    //パス方向
    BOOL IsPassCrs(enCrsType crs);

    //自動タゲ向き（ニュートラル）
    BOOL IsAutoLook();

    //シュート

    //シュート
    BOOL IsShot();
    //ジャンプシュート
    BOOL IsJumpShot();
    //シュートおしっぱ（ミラーパスからのシュート）
    BOOL IsShot2();
    //ダッシュ中シュート
    BOOL IsDsShot(enCrsType crs);

    //キャッチ

    //キャッチ入力
    BOOL IsCatch();
    //キャッチ入力（リリース）
    BOOL IsCatch3();
    //キャッチ後ダッシュ継続入力
    BOOL IsCatchDash(enCrsType needcrs);

    //よけ

    //よけ入力
    BOOL IsDodge();
    //よけ入力おしっぱ
    BOOL IsDodge2();
    //レバー画像番号
    s32 GetLeverNo();

    void Update();//更新

  };
}
