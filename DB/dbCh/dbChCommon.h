#pragma once
#include "dbChGen.h"

namespace db
{
  
  //シュートタイプ
  enum enShotType
  {
    shtS, shtJS, 
    shtDS, shtDJS, 
    shtJNS, shtDNS, shtDJNS, 
    shtHS, shtJHS,
  };
  
  enum enPassTag
  {
    patgOut2,//２番外野
    patgOut3,//３番外野
    patgOut4,//４番外野
    patgUFDsmn,//上部前方ダッシュマン
    patgURDsmn,//上部後方ダッシュマン
    patgDFDsmn,//下部前方ダッシュマン
    patgDRDsmn,//下部後方ダッシュマン
    patgUDsmn,//上部ダッシュマン
    patgDDsmn,//下部ダッシュマン
    patgFDsmn,//最前列ダッシュマン
    patgRDsmn,//最後尾ダッシュマン
    patgNDsmn,//最寄りダッシュマン
  };
  
  enum enTAGR { tgrU, tgrUR, tgrR, tgrDR, tgrD, tgrDL, tgrL, tgrUL };//角度
  enum enNaiyaTag { TGNG, TGNOAGL, TGOK };//タゲ
  enum enDMTag { DMTG_NG, DMTG_OK, DMTG_WAIT  };//タゲ
  
  struct TShotInfo
  {
    enShotType sType;//シュートタイプ
    s32 tShSp;//シュートスピード
    s32 tShRank;//シュートランク
    BOOL HS_f;
    s32 HSTiming;
    BOOL tNoTagHS_f;//タゲナシ
    s32 tHSAni;//アニメーション
    s32 tShPow;//シュートパワー（投げ手ぱわー＋必殺補正）
    s32 tShTech;//シュートテク（投げ手すぴん＋必殺補正）
    BOOL tNice_f;//ナイスシュート
    BOOL tJSh_f;//ジャンプシュート
    BOOL tDSh_f;//ダッシュシュート
    BOOL tNoTag_f;//タゲ無し必殺（効果音のみ）
    s32 tdx, tdz, tdy;
    s32 tdxs, tdzs, tdys;
    s32 resttime;
    s32 orishno;//折りシュート名
    //s32 crvtag;
    void Init()
    {
      Zeromem(this);
    }
  };
  
  struct TPassInfo
  {
    s32 tdx, tdz, tdy;//距離
    s32 tdxs, tdzs, tdys;//符号
    enPassLine tPaLine;//パスライン
    
    s32 tvx, tvz, tvy,tgrv;//タゲ速度
    s32 tvz_time;//ダッシュマンZ移動してる時間
    s32 tdmz;//ダッシュマンZ移動の行き先（目標ではなくジャンプラインまでに移動できる分）
    BOOL tdm_f;//タゲがダッシュマン
    BOOL Quick_f;//長押し：アリウープ　短押し：クイック
    BOOL Late_f;//間に合わない
    BOOL stopDm_f;//偏差射撃を行わない
    s32 ArriveTime;//予定到着時間
    s32 alleyoop;
    s32 tPaGrv;//重力
    s32 tPaSpd;//速度
    s32 tHoldNo;
    s32 PaETA_c;
    void Init()
    {
      Zeromem(this);
    }
  };
  
  
  //共通処理////////////////////////////////
  class TChCommon : public TChGen
  {
  protected:
    
    void SetShotPrev();
    void SetShotShiai();
    void SetShotTest();

    //フレーム情報ＳＥ
    void SettingSE();
    //タゲ角度
    //s32 GetTagAgl(s32 tX, s32 tZ);
    s32 GetTagAgl2(s32 tX, s32 tZ);
    
    ////ねらえる状況か
    //BOOL IsTgOKMotion(s32 tgNo);
    //角度に入っているか
    BOOL IsCheckNoAgl(s32 tgX, s32 tgZ);
    
    //敵陣地に着地するか
    BOOL IsCheckLandEnPos(s32 tPNo);
    
    //モーション切り替え
    void GenSetMtype(enDBMotionType tMtype);
    //Ｚ向きごとのアニメーションオフセット
    //void SetFrameMukiZ(s32 tSpan);
    enDBMotionNo RevMNoZ(enDBMotionNo mno);
    
    void SetNoneTagPos(TShotInfo* ShDt);
    
    s32 GetArTagY(TStChar* tagPSt, s32 t);
    
    //シュートタゲまでの距離
    void GetShotTagDist(TShotInfo* ShDt);
    //シュート角度
    void SAngle(TShotInfo* ShDt);

    //空中シュートタイミング判定
    s32 GetAirShootTiming();
    //ダッシュシュートタイミング判定
    s32 GetDashShootTiming();

    //シュート種類
    void GetShotType(TShotInfo* ShDt);
    
    ////シュートパワーからシュートレベルを求める
    //s32 GetShLv(s32 shpow);
    //シュートスピード
    void GetShSpeed(TShotInfo* ShDt);
    //シュート
    void SetShoot(BOOL tJSh_f);
    

    //新シュートのソート
    void SortShot(s32 timing);
    //距離セット
    void SetTagDist(TPassInfo* PaDt, s32 ttx, s32 tty, s32 ttz);
    //パスタゲまでの距離
    void GetPassTagDist(TPassInfo* PaDt);
    //パス重力・速度
    void PGrvSpdSet(TPassInfo* PaDt);
    //★ダッシュマン移動先への角度
    void DashmanAngle(TPassInfo* PaDt);
    //パス角度
    void PAngle(TPassInfo* PaDt);
    //パス
    void SetPass();
    
    BOOL IsShTag();
    
    //ダッシュマンになれるか
    BOOL IsDMOK();
    
    //パスタゲにならない
    s32 GetDist(s32 pos);
    s32 ShSptoLv(s32 ShSp);
    
    
    //拾える位置関係か
    BOOL IsPickUpPos(BOOL ca_f);
    
    
    void SetBallMukiX();
    void SetBallMukiZ();
    
  public:
    //コンストラクタ
    explicit TChCommon(TGame* pGame, s32 sideNo, s32 charNo);
    virtual ~TChCommon(void);
    
    //内野間パスタゲセット
    s32 GetNaiyaPassTag();
    //外野間パスタゲセット
    s32 GetGaiyaPassTag();
    
    
    //ステータス初期化
    void StInit(BOOL iochange_f);


    //シュート変更
    s32 ChangeStShotDebug(s32 mv);

    //シュート変更プレビュー
    void ChangeStShotPrev(const SOZAIDTVECTOR& sozaivec, s32 totalpow, s32 totalspin);

    //復活
    void RespawnInit();
    //逆向き
    enMukiType RevMuki(enMukiType tMuki);
    //効果音
    void SESet(enSEID SENo);
    //効果音
    void SESetInt(s32 SENo);
    //効果音
    void CatchSE();
    //モーション切り替え
    void SetMtype(enDBMotionType tMtype);
    void SetMtypeReset(enDBMotionType tMtype);
    //フレーム情報のセット
    void SetFrameData(BOOL InLoop_f);
    //動き停止
    void ZeroXYZ();
    //成績
    void AddRec(enRecord dt);
    void AddRec(enRecord dt, s32 addnum);
    void MaxRec(enRecord dt, s32 num);
    //ターゲッティング用向き
    void SetMukiAgl(BOOL fL, BOOL fR, BOOL fU, BOOL fD);
    //AUTO向き変え初期化
    void AutoMukiInit();
    
    enMukiTypeA GetMoveMuki(BOOL Input_f);
    enMukiZTypeA GetMoveMukiZ(BOOL Input_f);
    enMukiZTypeA GetMoveMukiZMukiLock(BOOL Input_f);
    
    //向き変え
    BOOL MukiSet(BOOL PosMuki_f, BOOL PosMuki2_f, BOOL input_f);
    //向き変え(強制的にAUTOMUKIに向かせる)
    BOOL MukiSetAuto();
    //ダッシュ向き変え(強制的にAUTOMUKIに向かせる)
    BOOL MukiSetDash(enMukiType dsMuki);
    
    s32 GetWkX();
    s32 GetWkZ();
    s32 GetNWkX();
    s32 GetNWkZ();
    s32 GetWkJpX();
    s32 GetWkJpZ();
    s32 GetNWkJpX();
    s32 GetNWkJpZ();
    
    //スリップ量セット＆状態をスリップに
    void SetSlip();
    //ジャンプ
    void JumpSet(BOOL mfCheck_f, BOOL canselDs_f, BOOL vjp_f);
    //キャンセルジャンプ
    BOOL CanselJump(BOOL canselDs_f);
    //ボールとの距離
    void GetBallDist();
    //ボール持った処理
    void BallGet(BOOL NoSE_f, BOOL LookBall_f);
    //パスタゲセット
    s32 GetTag(BOOL NoRefMuki_f);
    //変化球タゲセット
    s32 GetCrvTag();
    
    //パスタゲにならない
    BOOL IsNGPassTag(s32 pos);
    //ボールマンダッシュが可能
    BOOL IsBMDashOK();
    
    //コートに入ってるかどうか
    BOOL IsInCourt();
    
    //パスカットタゲセット
    void PaCtTagSet();
    //NGパスタゲシフト
    void NGPaTagShift();
    //パスタゲセット
    void PaTagSet();
    
    //シュートタゲセット
    void ShTagSet(BOOL NoRefMuki_f);
    s32 GetSLX2(s32 tz, BOOL sideR_f);

    //ｎフレーム後の空中位置
    s32 GetAirPosY(s32 frame);
    
    //勝手に拾う処理
    void AutoPickUp();
    
    //タゲ方向を向く
    void LookTg(s32 TgPNo, BOOL Pa_f, BOOL AtLook_f);
    
    //パス
    void Passing(BOOL Jp_f);
    
    //自分で操作
    void SelfCtrl(enATType ATType);

    //危険球
    BOOL IsDangerBall();
    BOOL IsStrongBall();
    
    //デバッグ時成績表示用
    void RecMsg();//s32 num, char* msg
    void SetCOMCatchRec(s32 ShSp);
    //キャッチ向き
    void SetCatchMuki();
    BOOL IsHSTiming();
  };
  
  
  
  //共通処理TChCommonをもつ////////////////////////////////
  class TChHaveCommon : public TChGen
  {
  protected:
    TChCommon* pCommon_;//共通処理
    //コンストラクタ
  public:
    explicit TChHaveCommon(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo);
    virtual ~TChHaveCommon(void);
  };
  
}
