#pragma once
#include "dbStSnGen.h"

namespace db
{
  //Resultステート
  class TStSnResult : TStSnGen
  {
  public:
    //コンストラクタ
    explicit TStSnResult();
    virtual ~TStSnResult();
    void Init(){ Zeromem(this); };
    void IncTmpKillNum(s32 sideno){ ++tmpkillnum_[sideno]; };
    s32 GetVsResult(){ return vsresult_; };
    s32 GetKillnum(){ return killnum_; };
    s32 GetDeathnum(){ return deathnum_; };
    s32 GetCleattime(){ return cleattime_; };
    s32 GetMaxdam(){ return maxdam_; };
    s32 GetTotaldam(){ return totaldam_; };
    s32 GetErrorCode(){ return errcode_; };
    BOOL IsWin(){ return (vsresult_ == 1); };
    void SetResult(s32 myside, s32 vsresult, s32 deathnum, s32 cleattime, s32 maxdam, s32 totaldam);
    void SetResultVSHum(s32 myside, s32 vsresult, s32 deathnum, s32 cleattime, s32 maxdam, s32 totaldam, s32 errcode);
  protected:
    //試合結果
    s32 vsresult_;
    s32 killnum_;
    s32 deathnum_;
    s32 cleattime_;
    s32 maxdam_;
    s32 totaldam_;
    s32 tmpkillnum_[DBSIDE];
    s32 errcode_;
  };

}
