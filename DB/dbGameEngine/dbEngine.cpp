#include "dbEngine.h"
#include "dbSceneList.h"

namespace db
{
  //コンストラクタ
  TEngine::TEngine() :TEngineBase(&mgEO_), mgUI_(&mgEO_), game_(&mgEO_)
  {
    mid::midLog("TEngine::TEngine\n");
    //各シーンクラス作成
    pSnList_[snTitle] = new TSnTitle(&game_, &mgUI_, snTitle);
    pSnList_[snHome] = new TSnHome(&game_, &mgUI_, snHome);
    pSnList_[snScenario] = new TSnScenario(&game_, &mgUI_, snScenario);
    pSnList_[snShiai] = new TSnShiai(&game_, &mgUI_, snShiai);
    pSnList_[snTutorial] = new TSnTutorial(&game_, &mgUI_, snTutorial);

    //シーンの追加
    for (s32 i = 0; i < snEND; ++i)
    {
      snSl_.AddScene(pSnList_[i]);
    }

  }

  //デストラクタ
  TEngine::~TEngine()
  {
    //シーン解放
    for (s32 i = 0; i < snEND; ++i)
    {
      Freemem(pSnList_[i]);
    }
  }
}
