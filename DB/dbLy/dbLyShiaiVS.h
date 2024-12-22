#pragma once
#include "dbLyShiaiGen.h"

namespace db
{
  class TSnShiai;
  class TLyShiaiVS :public TLyShiaiGen
  {
  public:
    //コンストラクタ
    explicit TLyShiaiVS(TSnShiai* pScene, TUGShiai* ugshiai);
    virtual ~TLyShiaiVS(void);
  protected:
    void MvPage_Play();
    void MvPage_Idle();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoIdle();
    void DoRestart();
    void DoUpdate(BOOL draw_f);
    BOOL disconnect_f_;
  };
 
}
