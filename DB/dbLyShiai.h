#pragma once
#include "dbLyShiaiGen.h"

namespace db
{
  class TSnShiai;
  class TLyShiai :public TLyShiaiGen
  {
  protected:
    void MvPage_Play();
    void MvPage_Idle();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoIdle();
    void DoRestart();
    void DoUpdate(BOOL draw_f);

 public:
    //コンストラクタ
    explicit TLyShiai(TSnShiai* pScene, TUGShiai* ugshiai);
    virtual ~TLyShiai(void);

  };
 
}
