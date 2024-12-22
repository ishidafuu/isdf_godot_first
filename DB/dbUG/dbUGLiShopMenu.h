#pragma once
#include "dbInclude.h"
#include "dbUGLiMenu.h"
#include "dbUGRcNormal.h"
namespace db
{
  //ショップ全般メニュー（値段表示が出ない段階）
  class TUGLiShopMenu :public TUGLiMenu
  {
  protected:
    void DoMakeGroupSub();
  public:
    //コンストラクタ
    explicit TUGLiShopMenu(TUIGenLayer* player);
    virtual ~TUGLiShopMenu(void);
    TUGRcNormal* MakeObj_Record();
  };
 
}
