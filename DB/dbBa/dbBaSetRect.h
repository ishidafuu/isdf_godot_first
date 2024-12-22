#pragma once
#include "dbBaCommon.h"

namespace db
{
  //判定矩形作成////////////////////////////////
  class TBaSetRect : public TBaHaveCommon
  {
  protected:
    void SetOS();
  public:
    //コンストラクタ
    explicit TBaSetRect(TGame* pGame, TBaCommon* pCommon);
    virtual ~TBaSetRect(void);
    //更新
    void UpDate();
  };


}
