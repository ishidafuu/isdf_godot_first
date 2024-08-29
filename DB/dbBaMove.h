#pragma once
#include "dbBaCommon.h"
#include "dbBaShot.h"
namespace db
{
  //移動////////////////////////////////
  class TBaMove : public TBaHaveCommon
  {
  protected:
    void TimerCrtPosition();
    void WallAbs(BOOL ZWall_f);
    BOOL IsCheckNoneTag();
    TShotDt GetTagDist(enGetTagType tagtype, BOOL Return_f);
    void SAngle(TShotDt& ShDt, s32 spd);
    void SAngle(TShotDt& ShDt, s32 spd, BOOL acuteOK_f);
    BOOL IsTgOKMotion(s32 tgTNo, s32 tgNo);
    void SetShootBound();
    
    
    //キー入力水平軸
    BOOL CheckCKeyX(TCurveDt& tCvDt);
    //キー入力垂直軸
    BOOL CheckCKeyY(TCurveDt& tCvDt);
    //水平逆
    void C2SetX(TCurveDt& tCvDt);
    //垂直逆
    void C2SetY(TCurveDt& tCvDt);
    //変化球
    void COMCurving(TCurveDt& CvDt, s32 tagPNo);
    void Curving();

    //座標移動
    void Moving();
    void CrtPosition();
    void DecTimer();
    void DecTimer3();
    void DecTimerPrev();
    void DecTimerTuto();
  public:
    TBaShot* pbaShot_;

    //コンストラクタ
    explicit TBaMove(TGame* pGame, TBaCommon* pCommon);
    virtual ~TBaMove(void);
    //更新
    void UpDate();
  };
  
}
