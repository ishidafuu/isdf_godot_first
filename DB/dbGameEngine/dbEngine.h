#pragma once
#include "dbInclude.h"
#include "dbSceneList.h"
#include "dbMgEngineObj.h"
#include "dbGame.h"
#include "dbMgUI.h"

namespace db
{
  class TEngine :public base::TEngineBase
  {
  protected:
    
    //全シーン
    base::TSnBase* pSnList_[snEND];

    //エンジンオブジェクト
    TMgEngineObj mgEO_;

    //ゲームクラス
    TGame game_;



  public:
    //コンストラクタ
    explicit TEngine();
    virtual ~TEngine();

    //UIクラス
    TMgUI mgUI_;
  };
}
