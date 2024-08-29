#pragma once
#include "dbStGen.h"

namespace db
{
  class TMgPad;

  //DBモーション
  enum enDBMotionType
  {
    dbmtSt,
    dbmtBreath,
    dbmtWk,
    dbmtDs,
    dbmtJCr,
    dbmtCJCr,
    dbmtJUp,
    dbmtJDn,
    dbmtARv,
    dbmtCr,
    dbmtFlF,
    dbmtFlB,
    dbmtPHF,
    dbmtPHB,
    dbmtDnHF,
    dbmtDnHB,
    dbmtKG,
    dbmtDnF,
    dbmtDnB,
    dbmtDRv,
    dbmtCM,
    dbmtJCM,
    dbmtFB,
    dbmtJFB,
    dbmtPW,
    dbmtPWWk,
    dbmtPWDs,
    dbmtSl,
    dbmtSh,
    dbmtRtSh,
    dbmtPa,
    dbmtJSh,
    dbmtRtJSh,
    dbmtJPa,
    dbmtCa,
    dbmtJCa,
    dbmtDg,
    dbmtJDg,
    dbmtRoF,
    dbmtRoB,
    dbmtDRAW,
    dbmtWIN,
    dbmtLOSE,
    dbmtOvL,
    dbmtUSA,
    dbmtUSA2,
    dbmtIKI,
    dbmtLOOK,
    dbmtLOOK2,
    dbmtFALL,
    dbmtAGE2,
    dbmtAGE3,
    dbmtAGE4,
    dbmtAGE5,
    dbmtDO1,
    dbmtDO2,
    dbmtANG,
  };

  //モーションフラグ
  enum enDBMotionFlag
  {
    dbmfNone = 0x00000,
    dbmfAr = 0x00001,
    dbmfDs = 0x00002,
    dbmfAct = 0x00004,
    dbmfDam = 0x00008,
    dbmfMTK = 0x00010,
    dbmfDmOK = 0x00020,
    dbmfJpOK = 0x00040,
    dbmfMuki = 0x00080,
    dbmfANG = 0x00100,
    dbmfRES = 0x00200,
    dbmfDn = 0x00400,
    dbmfPHit = 0x00800,
    dbmfPW = 0x01000,
    dbmfKG = 0x02000,
    dbmfSlip = 0x04000,
    dbmfAtCa = 0x08000,
    dbmfDg = 0x10000,
  };

  //モーション番号
  enum enDBMotionNo
  {
    dbmnSTF,//立ち（前）
    dbmnSTN,//立ち（横）
    dbmnSTR,//立ち（奥）
    dbmnWKF,//歩き（前）
    dbmnWKN,//歩き（横）
    dbmnWKR,//歩き（奥）
    dbmnDS,//ダッシュ

    dbmnJCF,//ジャンプ（前）
    dbmnJCN,//ジャンプ（横）
    dbmnJCR,//ジャンプ（奥）

    dbmnCRF,//着地（前）
    dbmnCRN,//着地（横）
    dbmnCRR,//着地（奥）

    dbmnFLF,//吹っ飛び（前から）
    dbmnFLB,//吹っ飛び（後ろから）

    dbmnKG,//息切れ
    dbmnKG2,//息切れ２

    dbmnDNF,//ダウン（前から）
    dbmnDNB,//ダウン（後ろから）
    dbmnCMF,//キャッチング（前）
    dbmnCMN,//キャッチング（横）
    dbmnCMR,//キャッチング（奥）

    dbmnJCMF,//ジャンプキャッチング（前）
    dbmnJCMN,//ジャンプキャッチング（横）
    dbmnJCMR,//ジャンプキャッチング（奥）

    dbmnFBF,//ファンブル（前）
    dbmnFBN,//ファンブル（横）
    dbmnFBR,//ファンブル（奥）

    dbmnJFBF,//ジャンプファンブル（前）
    dbmnJFBN,//ジャンプファンブル（横）
    dbmnJFBR,//ジャンプファンブル（奥）

    dbmnPWF,//パス要求（前）
    dbmnPWN,//パス要求（横）
    dbmnPWR,//パス要求（奥）

    dbmnSL,//スリップ

    dbmnSHF,//シュート（前）
    dbmnSHN,//シュート（横）
    dbmnSHR,//シュート（奥）

    dbmnPAF,//パス（前）
    dbmnPAN,//パス（横）
    dbmnPAR,//パス（奥）

    dbmnJSHF,//ジャンプシュート（前）
    dbmnJSHN,//ジャンプシュート（横）
    dbmnJSHR,//ジャンプシュート（奥）

    dbmnCAF,//キャッチモーション（前）
    dbmnCAN,//キャッチモーション（横）
    dbmnCAR,//キャッチモーション（奥）

    dbmnJCAF,//ジャンプキャッチモーション（前）
    dbmnJCAN,//ジャンプキャッチモーション（横）
    dbmnJCAR,//ジャンプキャッチモーション（奥）

    dbmnDGF,//しゃがみよけ（前）
    dbmnDGN,//しゃがみよけ（横）
    dbmnDGR,//しゃがみよけ（奥）

    dbmnAIRUPF,//ジャンプ上昇中（前）
    dbmnAIRUPN,//ジャンプ上昇中（横）
    dbmnAIRUPR,//ジャンプ上昇中（奥）

    dbmnAIRDNF,//ジャンプ下降中（前）
    dbmnAIRDNN,//ジャンプ下降中（横）
    dbmnAIRDNR,//ジャンプ下降中（奥）

    dbmnJPAF,//ジャンプパス（前）
    dbmnJPAN,//ジャンプパス（横）
    dbmnJPAR,//ジャンプパス（奥）

    dbmnRTNSH,//振り返りシュート
    dbmnRTNJSH,//振り返りジャンプシュート
    dbmnPHF,//立ちパス喰らい（前から）
    dbmnPHB,//立ちパス喰らい（後ろから）

    dbmnDNHF,//ダウン喰らい（前から）
    dbmnDNHB,//ダウン喰らい（後ろから）

    dbmnROF,//転がり前転
    dbmnROB,//転がり後転

    dbmnOVLF,//オーバーライン（前）
    dbmnOVLN,//オーバーライン（横）
    dbmnOVLR,//オーバーライン（奥）
    dbmnJDGF,//空中しゃがみよけ（前）
    dbmnJDGN,//空中しゃがみよけ（横）
    dbmnJDGR,//空中しゃがみよけ（奥）

    dbmnDRVF,//ダウン起き上がり（前）
    dbmnDRVN,//ダウン起き上がり（横）
    dbmnDRVR,//ダウン起き上がり（奥）

    dbmnPWWKF,//パス要求歩き（前）
    dbmnPWWKN,//パス要求歩き（横）
    dbmnPWWKR,//パス要求歩き（奥）
    dbmnPWDS,//パス要求走り

    dbmnBRF,//息継ぎ（前）
    dbmnBRN,//息継ぎ（横）
    dbmnBRR,//息継ぎ（奥）

    dbmnANG,//天使
    dbmnDRAW,//引き分け
    dbmnWIN,//勝ち
    dbmnLOSE,//負け

    //dbmnUSA,//兎跳び
    //dbmnUSA2,//兎跳び２
    //dbmnIKI,//デモ息切れ
    //dbmnLOOK,//こっちみる
    //dbmnLOOK2,//にっこりする
    //dbmnAGEF,//胴上げ前
    //dbmnAGEN,//胴上げ横
    //dbmnAGER,//胴上げ奥
    //dbmnAGEC,//胴上げ真正面
    //dbmnDO1,//胴あげられ１
    //dbmnDO2,//胴あげられ２

  };
  //オート動作
  enum enDBAutoType
  {
    dbatFree,
    dbatAlleyoop,
    dbatReturn,
    dbatKgReturn,
    dbatGet,
    dbatPassCvr,
    dbatDfPos,
    dbatPassCach,
    dbatPassJp,
    dbatDg,
    dbatDgJp,
    dbatDgSw,
    dbatShCv,
    dbatCPUPaCut,
    dbatCPUHelp,
    dbatCPUCa,
    dbatCPUShPa,
    dbatCPUJumpBall,
    dbatGohome,
    dbatDashman
  };

  //オート向き
  enum enMukiTypeA { maL, maN, maR };
  //オート向き
  enum enMukiZTypeA { mzaF, mzaN, mzaB };
  //COMオート動作
  enum enDBCOMAtType
  {
    cmatDSh,
    cmatJSh,
    cmatPa,
    cmatJPa
  };

  //必殺アシスト
  enum enHSTiming
  {
    hstmNormal,
    hstmSpecial,
    hstmNice,
    hstmInori,
  };

  //モーション
  struct TStChMotion
  {
    enDBMotionType Mtype;//モーション
    //enDBMotionFlag MFlags;//モーションフラグ
    u32 MFlags;//モーションフラグ
    s32 M_c;//そのモーション全体のカウンタ
    enDBMotionNo MNo;//BMおよびTECEditorの技番号

    BOOL IsMFlags(enDBMotionFlag tCheckFlag)
    {
      return (lib_num::IsFlags(MFlags, (u32)tCheckFlag));
    }

    void SubMFlags(enDBMotionFlag tSubFlag)
    {
      MFlags = (u32)((MFlags | tSubFlag) ^ tSubFlag);
    }
    void AddMFlags(enDBMotionFlag tAddFlag)
    {
      MFlags = (u32)(MFlags | tAddFlag);
    }
  };

  //描画情報
  struct TStChAnime
  {
    s32 FrameNo;//BMおよびTECEditorのアニメーション番号
    s32 Ani_c;//アニメーション内のカウンタ
    s32 Loop_c;//ループ回数
    s32 LoopStNo;//ループ始点
    BOOL ActP_f;//アクションポイントフラグ
  };

  //オート情報
  struct TStChAuto
  {
    enDBAutoType AutoType;//自動動作状態
    enMukiTypeA AMuki;
    enMukiZTypeA AMukiZ;
    BOOL ADash_f;
    BOOL ADashStoped_f;//一旦止まった
  };

  const s32 COMCATCHRECNUM = 5;

  //COMオート情報
  struct TStChCOM
  {
    s32 JpBallStep;
    s32 JpBallWait_c;
    s32 CatchRec[COMCATCHRECNUM];

    s32 COMMv_c;//一定時間その移動をするカウンタ兼フラグ
    s32 KeepMuki_c;
    enMukiTypeA COMMvAMuki;
    enMukiZTypeA COMMvAMukiZ;
  };

  //めんばーぱっど情報
  struct TStChManPad
  {
    BOOL MANChar_f;//キャラ操作
    s32 MANCharPadNo;
    TMgPad* pCharPad_;//パッド
  };

  struct TStChShotElems
  {
    s32 shotId;
    s32 elem_[HSELEMMAX];
    s32 rare_[HSELEMMAX];
    s32 pow_;
    s32 spin_;
    s32 oriNameNo_;
  };

  //合成シュート
  struct TStChGousei
  {
    TStChShotElems shot_[STEPNUM];
    BOOL set_f_[STEPNUM];
    BOOL syn_f_[STEPNUM];

    TStChShotElems vjshot_;
    BOOL vjset_f_;
    BOOL vjsyn_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //成績
  enum enRecord
  {
    //シュート・パス処理部分
    recShot_c,//シュート数合計
    recJSPShot_c,//ＤＪ必殺シュート数
    recDSPShot_c,//Ｄ必殺シュート数
    recDNiceShot_c,//Ｄナイスシュート数
    recJNiceShot_c,//Ｊナイスシュート数
    recDJNiceShot_c,//ＤＪナイスシュート数
    recGaiyaShot_c,//外野シュート数合計
    recDashManShot_c,//ダッシュマンシュート
    recPassTry_c,//パスした数
    recTag_c,//タゲになった回数
    recEnShot_c,//シュート数合計
    //天使処理部分
    recKill_c,//アウトにした数
    recDeath_c,//アウトにされた数
    //ヒット処理部分
    recTagDam_c,//ボールに当たった数
    recNoTagDam_c,//ボールに当たった数
    recHit_c,//シュートを当てた数
    recJSPHit_c,//必殺シュートを当てた数
    recDSPHit_c,//必殺シュートを当てた数
    recNiceHit_c,//ナイスシュートを当てた数
    recMoreHit_c,//追い打ちシュートを当てた数
    recNoTagHit_c,//ターゲット以外にあてた数
    recGaiyaHit_c,//外野にあてた数
    //キャッチ処理部分
    recPassOK_c,//パスした数
    recPassCut_c,//パスカットした数
    recCatchOK_c,//キャッチ成功した数
    recCatchTry_c,//キャッチした数(守備機会数合計ヒット＋キャッチ)
    recNiceCatch_c,//ナイスキャッチ成功
    //オーバーライン処理部分
    recOverLine_c,//オーバーラインした数
    recRebound_c,//リバウンドキャッチ
    recMaxdam,
    recTotaldam,
    recEND,
  };


  //成績
  struct TStChRecord
  {
    s32 Game_c;//試合数合計
    //各種データ
    s32 dt[recEND];

    void Init()
    {
      Zeromem(this);
    }
    s32& Dt(enRecord dtNo)
    {
      return dt[dtNo];
    }
    //すべて加算
    TStChRecord Add(TStChRecord BaseDt, TStChRecord AddDt)
    {
      BaseDt.Game_c++;
      for (s32 i = 0; i < recEND; ++i) BaseDt.dt[i] += AddDt.dt[i];

      return BaseDt;
    }

    //すべて加算
    void AddtoMe(TStChRecord AddDt)
    {
      Game_c++;
      for (s32 i = 0; i < recEND; ++i) dt[i] += AddDt.dt[i];
    }

  };

  //プレーヤーステータス
  struct TStChar
  {

    TZahyouRec Zahyou;
    TStChMotion Motion;
    TStChAnime Anime;
    TStChAuto Auto;
    TStChAuto NextAuto;
    TStChCOM stCOM;
    TStChManPad stPad;
    TStChGousei stGousei;

    BOOL Live_f;//生存
    BOOL Dead_f;//天使の間 GenSaiObjのLive_f=Falseは完全消滅
    BOOL ANGEL_f;//外野天使
    s32 CaDamAfter_c;//キャッチダメージ顔

    TRect Kurai;
    s64 id_char;//キャラのＩＤ
    s32 level;//レベル（チームレア＋補正）

	s64 stPow;	// ぱわー
	s64 stGuts;	// きあい
	s64 stKire;	// きれ
	s64 stTech;	// てく

    s32 comCaPer_;//敵キャッチ確率
    s32 comNCaPer_;//敵ナイスキャッチ確率

    s32 BallDist;//ボールとの距離
    s32 BallLandDist;//ボール着地地点との距離
    s32 BallLandLineDist;

    s32 JpCr_c;
    s32 Down_c;
    s32 Catch_c;//ボールをキャッチできるカウンタ
    s32 CatchW_c;//次のキャッチまでのラグ
    s32 Block_c;//ブロックカウンタ
    s32 ShStWait_c;
    s32 ShEdWait_c;
    s32 ShTg_c;
    s32 PaStWait_c;
    s32 Breath_c;
    //必殺タイミング
    s32 Step_c;//歩数
    s32 MadStep_c;//歩数

    s32 Air_c;//滞空時間
    s32 ShAir_c;//シュートタイミング用滞空時間
    enHSTiming HSTiming;//必殺アシスト状態
    s32 TopTiming;//頂点の時間
    BOOL AirCatch_f;//空中キャッチ
    BOOL AirAct_f;//空中アクションしました（キャッチしたらまたオフ）
    BOOL AirRev_f;//空中アクションしました（キャッチしたらまたオフ）
    BOOL Round_f;//世界一周します
    s32 Roll_c;//回転します
    s32 HP;//体力
    BOOL MHP_f;//体力全快
    s32 Holy_c;//天使カウンタ
    s32 MukiAgl12;//ターゲッティング角度（１２分割、一番左側の番号、そこから４つ）
    s32 AShCv_c;//がたつき防止
    s32 FricX;//キャッチ滑り時
    s32 FricZ;//キャッチ滑り時
    s32 Kagami_c;//回数とフラグを兼ねる
    s32 KagamiRev_c;//よりコート中心に戻れるカウンタ
    s32 KagamiRevKgNo;//屈み戻り予定位置
    s32 ECDdg_c;//敵コート避け時間
    s32 DsAcc_c;
    BOOL DsAccIOS_f;

    BOOL enshCa_f;//敵シュートキャッチ

    //ボールから受け取る吹っ飛び絡みのデータ
    BOOL Stick_f;//アッパーシュートくっつきフラグ
    BOOL Sticking_f;//くっつき"中"フラグ　くっつき中はヒット状態なので非ヒット状態になったらオフ
    BOOL StickingPr_f;//１Fr前くっつき"中"フラグ　くっつき中はヒット状態なので非ヒット状態になったらオフ
    BOOL Sticked_f;//くっつき"後"フラグ　ノーマル吹っ飛びに移行するけど無敵はダウンしてから
    s32  StickTime_c; //くっつき時間カウント　０でリリース＆↑０でもリリースorくっつき非発動
    BOOL suction_f;  //12:吸引効果
    BOOL suction_c;  //12:吸引効果発生時からのカウント
    s32 suctionLv;  //12:吸引効果Lv
    f32 suctionFlySpdBase;//100%のときのXZ合計の速度･･･最初に読み込んでおく
    f32 suctionFlySpdRatio;//↑を０から倍々にしていく
    BOOL suctionLockX_f;//吸引効果でボールとキャラとの間が４以下になったらロックするためのフラグ
    f32 suctionLockDistX;//吸引効果ロック中のボール～キャラ間
    BOOL suctionLockZ_f;//吸引効果でボールとキャラとの間が４以下になったらロックするためのフラグ
    f32 suctionLockDistZ;//吸引効果ロック中のボール～キャラ間
    BOOL rndWorld_f;  //12:世界一周効果
    s32 rndWorldLv;  //12:世界一周効果Lv
    s32 flying_c;  //12:吹っ飛び中カウント
    BOOL flyLoose_f;//緩跳ね吹っ飛び
    BOOL noMvDwn_f;//その場吹っ飛び
    BOOL noMvDwnLv;//その場吹っ飛び
    BOOL refVer_f;//縦跳ね吹っ飛び
    s32 refVerLv;//縦跳ね吹っ飛びLv
    BOOL refHor_f;//横跳ね吹っ飛び
    s32 refHorLv;//横跳ね吹っ飛びLv
    BOOL combo_f;//コンボフラグ
    s32  combo_c;//コンボ回数　０でコンボしない（他キャラによる貫通力減少が起こるので貫通力と別で持つ）
    s32  comboMax;//コンボ回数　０でコンボしない（他キャラによる貫通力減少が起こるので貫通力と別で持つ）
    s32  comboNoHitTime_c;//コンボ中ヒットフラグ発生までの時間→０より大きい場合は無敵＆０でヒットOK
    BOOL flySpGrav_f;//特殊吹っ飛び重力処理フラグ
    s32 hitMTime_cd;//食らったあとの無敵時間カウント…減算
    s32 hitMltDam;//連続ヒット時のダメージ
    BOOL noDmg_f;//ダメナシフラグ
    s32 hitSound;
    BOOL rndWorldCh_f;  //12:世界一周効果シュートのキャッチ
    BOOL refHorCh_f;//横跳ね吹っ飛び効果シュートのキャッチ
    BOOL symCtrl_f;//マリオネット効果フラグ
    enSEID seChiu;//ちうちう用予約SE
    enDBMotionType ctMotionPast;//ChCount用１フレ前のモーションタイプ

    BOOL stk1StNow_f;//現Fr時、判定１ジャッジでくっつき状態か･･･くっつき効果とは無関係
    BOOL stk1StPst_f;//１Fr前、判定１ジャッジでくっつき状態か･･･くっつき効果とは無関係
    BOOL stk2StNow_f;//現Fr時、判定２ジャッジでくっつき状態か･･･くっつき効果とは無関係
    BOOL stk2StPst_f;//１Fr前、判定２ジャッジでくっつき状態か･･･くっつき効果とは無関係
    s32 FlySkip;//吹っ飛びスキップ量
    s32 FlySkip_c;//吹っ飛びスキップ量
    BOOL ECDdg_f;//敵コート避け
    BOOL ECDjp_f;//敵コート避け
    BOOL VJump_f;//垂直ジャンプ
    BOOL LJump_f;//ロングジャンプ

    enMukiTypeA LastLRKey;//最後に向いた向き（２，３番外野パス用）
    BOOL GaiyaOvL_f;//外野でもオーバーラインになるフラグ
    BOOL LandSet_f;
    BOOL Nomove_f;
    BOOL O4BackSh_f;//４番外野後ろ狙う

    s32 LandX;
    s32 LandZ;//着地予想地点
    s32 LandRest;//着地予想までの時間
    s32 NowGrv;//現在の重力
    BOOL LandEnCourt_f;//敵コートに着地予定
    s32 Ueda_c;//復活回数

    s32 DashmanNo;//ダッシュマン番号
    s32 DashmanTgZ;//ダッシュマン向かうＺ軸

    s32 DashmanPaOK_c;//ダッシュマンにパスを出しても大丈夫な時間
    s32 DOEtc_c;//汎用カウンタ
    s32 DOLoop_c;//何週目か

    s32 LongKeep_c;//長持ちセリフ用
    s32 PassWait_c;//パス待ちセリフ用

    enMukiType LastMuki;//オート向き変えが入る前の向き
    enMukiZType LastMukiZ;//オート向き変えが入る前の奥行き向き

    BOOL COMCounter_f;//キャッチカウンター状態
    BOOL COMTossPassGet_f;//内野からのトスパスを受けた状態
    //BOOL HitStop_f;//ヒットストップ
    BOOL Utrun_f;//振り返りシュート
    BOOL FreeAct_f;//フリー状態からのアクション

    //BOOL QuickPass_f;//トスパス
    BOOL TossPass_f;//トスパス
    s32 MirPass_c;//ミラーパス
    s32 MirShot_c;//ミラーパス的シュート
    s32 MirShotLim_c;//ミラーパス的シュート受付時間

    s32 Fmbl_c;//ファンブルした後オートシュートしないためのカウンタ

    s32 Biorhythm_c;

    BOOL jumpok_f_;//キャッチ後長押し維持でジャンプしてしまうのを防ぐため
    BOOL shotok_f_;//キャッチ後リリースでシュートしてしまうのを防ぐため
    void Init()
    {
      Zeromem(this);
    }
  };

}
