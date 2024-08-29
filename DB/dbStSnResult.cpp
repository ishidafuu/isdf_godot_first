#include "dbStSnResult.h"

namespace db
{


  //Resultステート******************************************************
  TStSnResult::TStSnResult()
  {
    Init();
  }

  TStSnResult::~TStSnResult()
  {
  }

  void TStSnResult::SetResult(s32 myside, s32 vsresult, s32 deathnum, s32 cleattime, s32 maxdam, s32 totaldam)
  {
    vsresult_ = vsresult;
    killnum_ = tmpkillnum_[myside];
    deathnum_ = deathnum;
    cleattime_ = cleattime;
    maxdam_ = maxdam;
    totaldam_ = totaldam;
  }
  void TStSnResult::SetResultVSHum(s32 myside, s32 vsresult, s32 deathnum, s32 cleattime, s32 maxdam, s32 totaldam, s32 errcode)
  {
    SetResult(myside, vsresult, deathnum, cleattime, maxdam, totaldam);
    errcode_ = errcode;
  }

}
