#pragma once
#include "dbBaCommon.h"

namespace db
{
  //移動////////////////////////////////
  class TBaCount : public TBaHaveCommon
  {
  protected:
    //回転カウンタ
    void Spining();
    //カメラ移動
    void CameraPosMove();
    //void CameraPosMove2();
  public:
    //コンストラクタ
    explicit TBaCount(TGame* pGame, TBaCommon* pCommon);
    virtual ~TBaCount(void);
    //更新
    void UpDate();
    
  };
  
}
