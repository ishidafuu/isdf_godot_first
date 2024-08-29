#pragma once
#include "dbLyShGen.h"
#include "dbUGShiai.h"

namespace db
{
  class TSnShiai;
  class TLyShiaiGen :public TLyShGen
  {
  protected:
    virtual void DoIdle(){}
    virtual void DoRestart(){}
  public:
    TUGShiai* ugshiai_;

    //コンストラクタ
    explicit TLyShiaiGen(TSnShiai* pScene, TUGShiai* ugshiai);
    virtual ~TLyShiaiGen(void);

    void Idle();
    void Restart();
  };
 
}
