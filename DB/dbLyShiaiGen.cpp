#include "dbLyShiaiGen.h"
#include "dbSnShiai.h"
namespace db
{
  //コンストラクタ
  TLyShiaiGen::TLyShiaiGen(TSnShiai* pScene, TUGShiai* ugshiai) :TLyShGen(pScene)
  {
    ugshiai_ = ugshiai;
  }
  
  TLyShiaiGen::~TLyShiaiGen()
  {
  }
  
  void TLyShiaiGen::Restart()
  {
    DoRestart();
  }
  void TLyShiaiGen::Idle()
  {
    DoIdle();
  }
}
