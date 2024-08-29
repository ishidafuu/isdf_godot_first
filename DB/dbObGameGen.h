#pragma once
#include "dbInclude.h"
#include "dbMgStGame.h"

namespace db
{
  class TGame;
  class TMgGameObj;
  //struct TMgStGame;
  class TMgEngineObj;
  
  
  //オブジェクト基礎////////////////////////////////
  class TGameObjGen
  {
  protected:
    //クラスポインタ
    TGame* pGame_;//ゲームクラス
    TMgGameObj* pmgGO_;//ゲーム内オブジェ
    TMgStGame* pmgSG_;//ゲーム内ステータス
    TMgEngineObj* pmgEO_;//エンジンオブジェ
  public:
    //コンストラクタ
    explicit TGameObjGen(TGame* pGame);
    virtual ~TGameObjGen(void);
  };
}
