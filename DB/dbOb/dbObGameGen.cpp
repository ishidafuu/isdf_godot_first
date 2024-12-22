#include "dbObGameGen.h"
#include "dbGame.h"

namespace db
{
  
  //コンストラクタ
  TGameObjGen::TGameObjGen(TGame* pGame)
  : pGame_(pGame), pmgEO_(pGame->pmgEO_), pmgGO_(&pGame->mgGO_), pmgSG_(&pGame->mgSG_)
  {
    
  }
  
  TGameObjGen::~TGameObjGen(void)
  {
    
  }
  
}
