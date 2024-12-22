#pragma once
#include "dbChCommon.h"

namespace db
{
  //判定矩形作成////////////////////////////////
  class TChSetRect : public TChHaveCommon
  {
  protected:
    void SetOS();
  public:
    //コンストラクタ
    explicit TChSetRect(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo);
    virtual ~TChSetRect(void);
    
    //更新
    void UpDate();
    
    
  };
}

