#pragma once
#include "dbChCommon.h"

namespace db
{
  class TChDraw : public TChHaveCommon
  {
  protected:
    void Drawing();
  public:
    explicit TChDraw(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo);
    
    virtual ~TChDraw(void);
    
    void Update();
  };
}
