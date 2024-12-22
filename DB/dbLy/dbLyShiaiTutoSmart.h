#pragma once
#include "dbConst_Tuto.h"
#include "dbLyShiaiTutoGen.h"
#include "dbUGTutoMsgSmart.h"
#include "dbUGFinger.h"

namespace db
{
  class TSnShiai;


  class TLyShiaiTutoSmart :public TLyShiaiTutoGen
  {
  protected:
    void MvPage_GameAct();
    //void MvPage_Fin();

    //オーバーロード
    void DoLoadSub();
    void DoReadySub();
    void DoUpdateSub();
    void DoChangePage(s32 pageno);
    void DoUpdateSub(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyShiaiTutoSmart(TSnShiai* pScene, TUGShiai* ugshiai);
    virtual ~TLyShiaiTutoSmart(void);
  };
 
}
