#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
namespace db
{
  //シュートセットメニュー
  class TUGLiShSetMenu :public TUGLiGen
  {
  protected:
    void DoClearRecord();
  public:
    std::vector<TUGRcNormal*> records_;

    //コンストラクタ
    explicit TUGLiShSetMenu(TUIGenLayer* player);
    virtual ~TUGLiShSetMenu(void);
    TUGRcNormal* MakeObj_Record();
    void Ready(BOOL omakase_f);
    s64 GetCommonID();
  };
 
}
