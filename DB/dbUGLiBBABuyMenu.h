#pragma once
#include "dbInclude.h"
#include "dbUGLiMenu.h"
#include "dbUGRcNormal.h"
namespace db
{
  //シュート販売メニュー（値段表示が出ない段階）
  class TUGLiBBABuyMenu :public TUGLiMenu
  {
  public:
    //コンストラクタ
    explicit TUGLiBBABuyMenu(TUIGenLayer* player);
    virtual ~TUGLiBBABuyMenu(void);
    TUGRcNormal* MakeObj_Record();
    void Refresh();
  protected:
    void DoMakeGroupSub();
  };
 
}
