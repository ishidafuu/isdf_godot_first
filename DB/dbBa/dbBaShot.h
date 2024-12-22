#pragma once
#include "dbBaCommon.h"
#include "dbBaShot_Tri.h"
#include "dbBaShot_Acc.h"

namespace db
{
  //移動////////////////////////////////
  class TBaShot : public TBaHaveCommon
  {
  protected:
    //現在の座標に速度を追加
      void NormalMove(f32 flag, f32 valdX, f32 valdY, f32 valdZ);
	void GlobalMove();
 BOOL CheckBeyondTarget();
 BOOL CheckBeyondTargetXZ(f32 spd,f32 pos,f32 tagpos);
 f32 CheckWaveAbs(f32 wvAbs_f, s32 wvType, f32 addVal);
	//変化素材による段階変化チェック
	void CheckStepChange();//段階変化チェック

	void ChangeStepNumAtTime();//変化素材処理：時間
	void ChangeStepNumAtHeight();//変化素材処理：高さ
	void ChangeStepNumAtPosDir();//変化素材処理：位置 固定位置
	void ChangeStepNumAtPosObj();//変化素材処理：位置 指定物位置

	BOOL CheckBeyondObjPos(f32 dir, f32 pos, f32 objN, f32 objP);//指定物座標超えチェック

	void ChangeStepNumAtDist();//変化素材処理：距離
	void ChangeStepNumAtLoop();//変化素材処理：ループ
	void ChangeStepNumAtWave(s32 waveTimes_c, s32 waveTimes_Max);//変化素材処理：波回数

	void ChangeStepNumAtHit();//変化素材処理：ヒット
	void ChangeStepNumAtGround();//変化素材処理：地上
	void ChangeStepNumAtMultiAnglr();//変化素材処理：多角終
	void ChangeStepNumAtPassOrbit();//変化素材処理：パス挙動終

	void AddPhaseNum();//次段階へ進める

  void RemoveElmToCrt();//Elm ⇒ Crt
  void RemoveChangeElmToCrt();//変化素材Elm ⇒ Crt
  void RemoveOrbitElmToCrt();//軌道素材Elm ⇒ Crt
  void RemoveTargetElmToCrt();//標的素材Elm ⇒ Crt

  void RemoveWaveElmToCrt(s32 elmWvType, s32 elmWvWd, s32 elmWvFq,
    s32 elmWvWCMt, s32 elmWvWCSp, BOOL elmWvPS, s32 elmWvDir,
    s32 crtWvType, s32 crtWvWd, s32 crtWvFq,
    s32 crtWvWCMt, s32 crtWvWCSp, BOOL crtWvPS, s32 crtWvDir);//波Elm ⇒ Crt
  void RemoveWaveXElmToCrt();//波XElm ⇒ Crt
  void RemoveWaveYElmToCrt();//波YElm ⇒ Crt
  void RemoveWaveZElmToCrt();//波ZElm ⇒ Crt

  void InitAccTriData();
  void CheckAccFlag();

  void CheckDelaySE();


  void SetPhaseData();
  void SetMomentData();
  void SetAddDtToMainPos();
  void SetTargetByWEdgeEff();
  void SetTargetByGenocideEff();

  void SetAddDir(f32 valdX , f32 valdY , f32 valdZ);
  f32 GetAddDir(f32 pos, f32 tagpos, f32 vald);
  void SetEffSpd();

  public:
    TBaShot_Tri* pbaShot_Tri_;
    TBaShot_Acc* pbaShot_Acc_;

    //コンストラクタ
    explicit TBaShot(TGame* pGame, TBaCommon* pCommon);
    virtual ~TBaShot(void);
    //更新
    void UpDate();
  };
  
}
