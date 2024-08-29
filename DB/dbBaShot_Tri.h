#pragma once
#include "dbBaCommon.h"
#include "dbStShot.h"
namespace db
{

 // //三角関数用データ
 // struct TShotTriData{

 //   //ボール位置は仮想位置と表示位置の２つがあり、
 //   //今までは仮想位置を計算してきたが
 //   //ココでは仮想位置から表示位置に換算する。
 //   f32 shotPosX;//ボール位置x（仮想位置）
 //   f32 shotPosY;//ボール位置y（仮想位置）
 //   f32 shotPosZ;//ボール位置z（仮想位置）
 //   f32 ballMasterPosX;//振動によりシフトしたボール位置x（表示位置）
 //   f32 ballMasterPosY;//振動によりシフトしたボール位置y（表示位置）
 //   f32 ballMasterPosZ;//振動によりシフトしたボール位置z（表示位置）

 //   s32 dispPosX;//実際の表示位置x
 //   s32 dispPosY;//実際の表示位置y
 //   s32 dispPosZ;//実際の表示位置z

 //   //データから取得
 //   //波フラグ
 //   BOOL waveX_f;//波前後フラグ
 //   BOOL waveY_f;//波縦フラグ
 //   BOOL waveZ_f;//波サイドフラグ


	////各種イニシャルフラグ
 //   BOOL waveXInit_f;//波xイニシャルフラグ
 //   BOOL waveYInit_f;//波yイニシャルフラグ
 //   BOOL waveZInit_f;//波zイニシャルフラグ

 //   //波前後
 //   s32 waveXType;//タイプ：sin・cos、ギザ・曲線
 //   s32 waveXLenLv;//波長Lv　５段階
 //   s32 waveXWidLv;//振幅Lv　５段階
 //   s32 waveXWChangeType;//振幅変化タイプ　減衰・減増減…・増減増…・増幅
 //   s32 waveXWChangeSpeed;//振幅変化速度　５段階
 //   s32 waveXPeakStop;//頂点停止
 //   BOOL waveXPeakStop_f;//頂点停止フラグ
 //   s32 waveXPeakStop_c;//頂点停止C
 //   f32 waveXAngle;//掛振動中の現角度
 //   s32 waveXWChangeCount;//掛振幅変化時間カウント
 //   s32 waveXDiv;//波分割
 //   f32 waveXZigAdd;//掛ジグザグ時の移動量
 //   BOOL waveXDiv_f;//波分割中フラグ
 //   s32 waveXDiv_c;//分割中時間カウント
 //   s32 waveXDivTime;//分割中時間
 //   s32 waveXAnglePhase;//象限数：第１象限～第４象限
 //   s32 waveXAngleRtTms;//何周したか

 //   //波縦
 //   s32 waveYType;//タイプ：sin・cos、ギザ・曲線
 //   s32 waveYLenLv;//波長Lv　５段階
 //   s32 waveYWidLv;//振幅Lv　５段階
 //   s32 waveYWChangeType;//振幅変化タイプ　減衰・減増減…・増減増…・増幅
 //   s32 waveYWChangeSpeed;//振幅変化速度　５段階
 //   s32 waveYPeakStop;//頂点停止
 //   BOOL waveYPeakStop_f;//頂点停止フラグ
 //   s32 waveYPeakStop_c;//頂点停止C
 //   f32 waveYAngle;//掛振動中の現角度
 //   s32 waveYWChangeCount;//掛振幅変化加算値
 //   s32 waveYDiv;//波分割
 //   f32 waveYZigAdd;//掛ジグザグ時の移動量
 //   BOOL waveYDiv_f;//波分割中フラグ
 //   s32 waveYDivTime;//分割中時間
 //   s32 waveYDiv_c;//分割中時間カウント
 //   s32 waveYAnglePhase;//象限数：第１象限～第４象限
 //   s32 waveYAngleRtTms;//何周したか

 //   //波サイド
 //   s32 waveZType;//タイプ：sin・cos、ギザ・曲線
 //   s32 waveZLenLv;//波長Lv　５段階
 //   s32 waveZWidLv;//振幅Lv　５段階
 //   s32 waveZWChangeType;//振幅変化タイプ　減衰・減増減…・増減増…・増幅
 //   s32 waveZWChangeSpeed;//振幅変化速度　５段階
 //   s32 waveZPeakStop;//頂点停止
 //   BOOL waveZPeakStop_f;//頂点停止フラグ
 //   s32 waveZPeakStop_c;//頂点停止C
 //   f32 waveZAngle;//掛振動中の現角度
 //   s32 waveZWChangeCount;//掛振幅変化加算値
 //   s32 waveZDiv;//波分割
 //   f32 waveZZigAdd;//掛ジグザグ時の移動量
 //   BOOL waveZDiv_f;//波分割中フラグ
 //   s32 waveZDivTime;//分割中時間
 //   s32 waveZDiv_c;//分割中時間カウント
 //   s32 waveZAnglePhase;//象限数：第１象限～第４象限
 //   s32 waveZAngleRtTms;//何周したか

 //   f32 addVal;//主軌道等に追加される値
 //   f32 triFuncVal;//三角関数等で得られる、振動-1～+1の値。⇒アニメ等で使用
 //   f32 triFuncValPr;//三角関数等で得られる、振動-1～+1の値。⇒アニメ等で使用
 //   f32 triFuncValFt;
 //   s32 triFuncPeakTime;
 //   void Init() { Zeromem(this); }
 // };

  class TBaShot_Tri : public TBaHaveCommon
  {
  protected:
    //TShotTriData* tridt;

    void WaveData(s32 direction, f32 shotPos, s32 waveType, s32 waveLenLv, s32 waveWidLv,
        s32 waveWChangeType, s32 waveWChngeSpeed, s32 waveWChageCount,
      f32 waveAngle, s32 waveDiv, f32 waveZigAdd);//波データ初期化
    void WaveSinFunction(s32 waveLenLv, s32 waveWidLv, f32 waveAngle, BOOL dir, f32 waveRatio);//sin関数
    void WaveCosFunction(s32 waveLenLv, s32 waveWidLv, f32 waveAngle, BOOL dir, f32 waveRatio);//cos関数
    void WaveSinZigFunction(s32 waveLenLv, s32 waveWidLv, f32 waveAngle, BOOL dir, f32 waveRatio);//sinギザ関数
    void WaveCosZigFunction(s32 waveLenLv, s32 waveWidLv, f32 waveAngle, BOOL dir, f32 waveRatio);//cosギザ関数
    f32  GetZigAdd(BOOL cosSwitch, f32 zigVal);
    s32 GetWaveDirection(s32 dir);
    f32 GetWaveWidthMax(s32 waveWidLv);
    f32 GetAngleSpdLv(s32 waveLenLv);
    f32 GetAngleSpd(f32 waveWidth);
    f32 GetWaveWidthRatio(s32 waveWChangeType, s32 waveWChngeSpeed, s32 waveWChageCount);

    //波分割処理&頂点停止処理
    BOOL CheckAddWaveAngle(s32 div, BOOL& div_f, s32& div_c, s32& divTime, f32 nowVal, f32 pstVal, s32 waveAngle,
        s32& wavePhase, s32& waveRt, s32 waveType , s32 peakStop, BOOL& peakStop_f, s32& peakStop_c);
    void CheckQdrtDt(s32 nowQdrt, s32 pstQdrt, s32& wavePhase, s32& waveRt, BOOL& ph_f);
    s32 GetQdrtDt(f32 angleDt, s32 waveType);
    BOOL GetPeakStop(s32 peakStopType, s32 nowQdrt, s32 pstQdrt);

    //波３方向
    void WaveXOrbit();
    void WaveYOrbit();
    void WaveZOrbit();

    //波３方向
    void waveXOrbit();
    void waveYOrbit();
    void waveZOrbit();

    void ReSAngle();//SAngle再計算


    //三角関数が関係ない場合の処理
    void DispPosForNoTriFunction();
	void CheckTriFlag();
    void SetTriData();
	void SetBasicData();
	void SetWaveXData();
	void SetWaveYData();
	void SetWaveZData();
	void SetSepFormData();
	void SetWFormData();

    void GetTriData();
  public:
    //コンストラクタ
    explicit TBaShot_Tri(TGame* pGame, TBaCommon* pCommon);
    virtual ~TBaShot_Tri(void);
    //更新
    void TriInit();
    void UpDate();
  };
  
}
