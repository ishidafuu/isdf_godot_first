#pragma once
#include "dbInclude.h"
#include "dbGame.h"
#include "dbMgUI.h"
namespace db
{
  class TScene : public base::TSnBase
  {
  protected:
    void SceneNSF(s32 nsfno);
  public:
    //ゲームクラスポインタ
    TGame* pGame_;
    //UI管理クラスポインタ
    TMgUI* pmgUI_;
    //エンジンオブジェクトポインタ
    TMgEngineObj* pmgEO_;

    explicit TScene(TGame* pGame, TMgUI* pMgUI, s32 myScene);
    virtual ~TScene(void);

  };
}

