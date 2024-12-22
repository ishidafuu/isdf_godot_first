#include "dbScene.h"


namespace db
{
  TScene::TScene(TGame* pGame, TMgUI* pMgUI, s32 myScene)
  : TSnBase(pGame->pmgEO_, myScene),
  pmgEO_(pGame->pmgEO_),
  pGame_(pGame),
  pmgUI_(pMgUI)
  {
  }
  
  
  TScene::~TScene()
  {
    
  }

  void TScene::SceneNSF(s32 nsfno)
  {
    if (stSn_.bgm_f_ == FALSE)
    {
      pmgEO_->mgNSF_.PlayID(nsfno);
      stSn_.bgm_f_ = TRUE;
    }
  }



}
