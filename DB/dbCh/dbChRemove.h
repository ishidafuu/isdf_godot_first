#pragma once
#include "dbChCommon.h"


namespace db
{
  //位置補正////////////////////////////////
  class TChRemove : public TChHaveCommon
  {
  protected:
    void Remove();
  public:
    //コンストラクタ
    explicit TChRemove(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo);
    virtual ~TChRemove(void);
    
    //更新
    void UpDate();
  };
}
