#pragma once
#include "dbBaCommon.h"

namespace db
{
  class TBaDraw : public TBaHaveCommon
  {
  protected:
    void DrawBall();//ボール描画
    void DrawTime();//時間描画
  public:
    explicit TBaDraw(TGame* pGame, TBaCommon* pCommon);
    virtual ~TBaDraw(void);
    void Update();
  };
  
}
