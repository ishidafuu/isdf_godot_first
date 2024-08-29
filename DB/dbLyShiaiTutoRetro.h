#pragma once
#include "dbConst_Tuto.h"
#include "dbLyShiaiTutoGen.h"
#include "dbUGTutoMsgRetro.h"
#include "dbUGFinger.h"

namespace db
{
  class TSnShiai;

  class TLyShiaiTutoRetro :public TLyShiaiTutoGen
  {
  protected:
    void MvPage_GameAct();
    void MvPage_GameAct_Normal();

    void MvPage_Fin();

    //オーバーロード
    void DoLoadSub();
    void DoReadySub();
    void DoUpdateSub();
    void DoChangePage(s32 pageno);
    void DoUpdateSub(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyShiaiTutoRetro(TSnShiai* pScene, TUGShiai* ugshiai);
    virtual ~TLyShiaiTutoRetro(void);
  };

}
