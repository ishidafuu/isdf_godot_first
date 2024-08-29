#pragma once
#include "dbChCommon.h"

namespace db
{
  
  const s32 DASHDIST = 32 * XYMAG;
  const s32 DASHDISTZ = 16 * XYMAG;
  enum enTDGSL//よけ種類
  {
    dgsNone,
    dgsJp,
    dgsCr,
    dgsSw,
    dgsCa
  };
  
  enum enCOMPassMuki//COMパス方向
  {
    cpmNone,
    cpmTaimen,
    cpm342,
    cpm243,
    cpmInf,
  };
  
  
  
  //移動////////////////////////////////
  class TChMove : public TChHaveCommon
  {
  protected:
    
    //スウェー
    void SwayDodge();
    //よけ矩形チェック
    BOOL IsDgBoxCheck(BOOL jpdg_f);
    //よけ種類
    enTDGSL CheckDS(BOOL tCOMCa_f);
    //カバージャンプするか
    BOOL IsCheckPaCvJp();
    //カバージャンプするか
    BOOL IsCheckShCvJp();
    //次オートモーション設定(dbatFreeにもどる条件)
    void InitAutoTypeMuki();
    //オート戻りラインチェック
    BOOL IsReturnLine(enCrtLine lineType, s32 tMax);
    //反転
    void SetAMuki(s32 RevNo);
    //オート戻り
    void CheckAutoReturn();
    //タイトルデモ用帰宅
    void CheckGohome();
    //パスカバー
    void CheckPassCover();
    //当たらなかったシュートのカバー
    void CheckShootCover();
    void ShCoverMove();
    //パスキャッチジャンプ
    void CheckPassCatchJp();
    //向かう先
    void SetTagPos(s32 tgX, s32 tgZ);
    //パスカット
    void CheckPassCut();
    //味方ヘルプガード
    void SetCOMMv();
    void GuardMove();
    void CheckHelp();
    void CheckGuard();
    void CheckCOMHelpGuard(BOOL ctrl_f);
    //アリウープダッシュ
    void CheckAlleyoopMove();
    //パスキャッチモーション
    void CheckPassCatchMotion();
    //よけモーション
    void CheckDodge();
    BOOL IsCheckPosX(s32 tPX, s32 dist);
    BOOL IsCheckPosZ(s32 tPZ, s32 dist);
    //ポジショニング種類
    void CheckMovePosType();
    s32 GetSidePosX(BOOL L_f);
    s32 GetSidePosZ(BOOL F_f);
    s32 GetRearPosX(BOOL L_f);
    s32 GetRearPosZ(BOOL F_f);
    s32 GetMidPos(BOOL IsO2O3_f, BOOL LF_f);
    //ポジショニング向き
    void MoveDfPos();
    void CheckMovePosMuki_InfDfPos();
    void SetNextAuto_InfPosOf(s32 tPX, s32 tPZ);
    //内野オフェンスポジショニング向き
    void CheckMovePosMuki_InfOfPosOf();
    //その他ポジショニング向き
    void CheckMovePosMuki_GaiyaPos();
    //ポジショニング
    void CheckMovePos();
    //★ダッシュマン
    void CheckDashman();
    //自動取りに行く
    void CheckAutoGetter();
    
    //狙いに近いパスタゲポジション取得
    s32 GetCPUNearPaTag(s32 enPos);
    //指定向きパスタゲポジション取得
    s32 GetCPUMukiPaTag(enCOMPassMuki pamuki);
    
    BOOL CPUChance();//チャンスチェック
    BOOL CPUNCChance();//ナイスキャッチチャンスチェック
    BOOL CPUCCChance();//カウンターキャッチチャンス
    
    BOOL CPUDashman();
    BOOL CPUChanceMake();//チャンスメイク
    BOOL CPUOrderPass();//オーダーパス
    BOOL CPUSinglePass();//シングルパス
    BOOL CPUSetterPass();//セッターパス
    
    
    //ＣＯＭ思考★★
    void CheckDMAtcCPU();
    void CheckStdAtcCPU();
    void AMvSet_DMCPU();
    void AMvSet_dspos();
    void AMvSet_StdCPU();
    void CheckAtcCPU();
    
    //ＣＯＭよけ
    BOOL IsOKDashCatch();//ダッシュキャッチねらえる
    void CheckDodgeCPU();
    //ＣＯＭジャンプボール★
    void CheckJumpBallCPU();
    //ダッシュ
    void DashSet_SetAutoMove();
    //ボールの方向を向く
    void SetBallMukiX();
    void SetBallMukiX(s32 distx);
    void SetBallMukiZ();
    
    void GenAMv_Wk(BOOL &MukiSet_f);
    void GenAMv_WkLkBl(BOOL &MukiSet_f);
    void GenAMv_Ds(BOOL &MukiSet_f);
    BOOL AMv_Free();
    BOOL AMv_Alleyoop();
    BOOL AMv_Pos();
    BOOL AMv_GoToBall();
    BOOL AMv_Return();
    BOOL AMv_GoHome();
    BOOL AMv_Get();
    BOOL AMv_ShCvr();
    BOOL AMv_CPUPaCut();
    BOOL AMv_CPUHelp();
    void AMv_PaCatch();
    BOOL AMv_CPUShPaDM();
    BOOL AMv_CPUShPaStd();
    BOOL AMv_CPUShPa();
    BOOL AMv_CPUCa();
    BOOL AMv_DgSw();
    BOOL AMv_CPUJumpBall();
    BOOL AMv_Dashman();
    void AMv_MukiSet();
    //設定された動作を反映
    void SetAutoMove();
    //ダッシュ入力
    void DashInput();
    //直前のキー
    void SetLastKeyLR();
    //キー入力
    void MCKeyInput();
    //操作権キャラ操作
    void ManualMove();
    //ＣＯＭ操作権キャラ操作
    void COMMove();
    //オート操作
    void AutoMove();
    //動かさないとき
    void NoMove();
    //デモ帰宅
    void GohomeMove();
    //必殺アシスト★
    void GetHSTiming();
    
  public:
    //コンストラクタ
    explicit TChMove(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo);
    virtual ~TChMove(void);
    
    //更新
    void UpDate();
  };
}
