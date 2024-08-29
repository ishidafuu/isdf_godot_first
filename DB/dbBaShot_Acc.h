#pragma once
#include "dbBaCommon.h"
#include "dbStShot.h"

namespace db
{
 //   //加速系処理名
 //   enum enAccDt{
 //       adt_Non,//処理なし
 //       adt_Acl,//加速処理
 //       adt_Dcl,//減速処理
 //       adt_YAc,//Y加速処理
 //       adt_YAA,//Y加速＋XZ加速減速処理
 //       adt_Plb,//放物線処理
 //       adt_PlA,//放物線＋XZ加速減速処理
 //       adt_END
 //   };

 // //二次関数用データ
 // struct TShotAccData{
 //   //使用変数等

 //   f32 shotPosX;//術者X（シュート開始位置）
 //   f32 shotPosY;//術者Y（シュート開始位置）
 //   f32 shotPosZ;//術者Z（シュート開始位置）
 //   f32 targPosX;//標的位置x
 //   f32 targPosY;//標的位置y
 //   f32 targPosZ;//標的位置z
 //   f32 paraGrav;//山なり素材に入っている重力データを式に使用できるよう変換したデータ
 //   f32 accelPow;//加速度関連データを使用できるよう変換したデータ
 //   f32 spd;//xyz未分割の速度…処理の簡略化の為、放物線に大きく関わる方向間での分割を優先する
	//f32 dclInitPosX;//減速開始位置x（減速効果の段階に突入した時の点）
 // f32 dclInitActPosX;//減速本番開始位置x（減速効果が実際に発動する点）
 // f32 COURT_LENGTH = 448;//コート長さ
	////基本的にはx進行方向y重力加速方向となるので、x方向y方向間での速度分割となる。
 //   //その際、z方向の速度は、z方向の必要な距離を計算し、x方向で割り出された速度などから
 //   //z方向速度を決める
 //   f32 accelSpeedX;//加速系軌道の速度x
 //   f32 accelSpeedY;//加速系軌道の速度y
 //   f32 accelSpeedZ;//加速系軌道の速度z

	//f32 mainSpeedX;//SAngleにて速度分割直後の速度X
	//f32 mainSpeedY;//SAngleにて速度分割直後の速度Y
	//f32 mainSpeedZ;//SAngleにて速度分割直後の速度Z

	//BOOL palabOrb_f;//山なり軌道素材
	//BOOL accelEff_f;//加速効果素材
	//BOOL decelEff_f;//減速効果素材
	//BOOL yAccelEff_f;//y加速効果素材
 //BOOL yxAccelEff_f;//y加速効果素材
 //BOOL plAccOrb_f;//山なり＋加速軌道素材

	//BOOL palabOrbInit_f;//山なり軌道素材
	//BOOL accelEffInit_f;//加速効果素材
	//BOOL decelEffInit_f;//減速効果素材
	//BOOL yAccelEffInit_f;//y加速効果素材
 //BOOL yxAccelEffInit_f;//y加速効果素材
 //BOOL plAccOrbInit_f;//山なり＋加速軌道素材


 // void Init() { Zeromem(this); }
 // };

  class TBaShot_Acc : public TBaHaveCommon
  {
  protected:
    //TShotAccData* accdt;

	//x等速放物線軌道初期化
    void InitParabolicOrbit();
    void InitParabolicOrbit2();

    f32 GetParabHeight();
    f32 GetParabTime();
    f32 GetParabAcTime(f32 acc);

    //加速シュート系初期化
    void InitXAccelOrbit();
	//減速シュート系初期化
	void InitXDecelOrbit();
	//直落下系初期化…もずの最後など
    void InitYAccelOrbit();
    void InitYXAccelOrbit();
    //y放物線x加減速
    void InitParabolicXAccelOrbit();

    ////毎Fr処理
    //加速処理
    void AddXAccelOrbit();
	//加速処理
	void AddXDecelOrbit();
	//重力処理
    void AddYAccelOrbit();
    //y放物線x加減速
    void AddParabolicXAccelOrbit();


    //山なり処理
    void ParabolicOrbit();
    //加速系処理
    void XAccelOrbit();
	//減速系処理
	void XDecelOrbit();
	//y加速系処理
    void YAccelOrbit();
    void YXAccelOrbit();

    //x加速放物線処理
    void ParabolicXAccelOrbit();

	//二次関数系素材の有無チェック⇒あればSet関数へ
	BOOL CheckAccFlag();

	void SetAccData();
	void SetUpdatedData();
	void GetAccData();
	void SetPosData();
	void SetAccelPow();
  public:
    //コンストラクタ
    explicit TBaShot_Acc(TGame* pGame, TBaCommon* pCommon);
    virtual ~TBaShot_Acc(void);
    //更新
    void UpDate();
    //初期化用関数
    void AccInit();
  };
  
}
