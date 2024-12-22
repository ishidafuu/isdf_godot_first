#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
#include "dbUGListEnd.h"
namespace db
{
  struct TUGLiMenuSt
  {
    //s32 shop_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiMenu :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
    void DoMakeGroupSub();
  public:
    std::vector<TUGRcNormal*> records_;
    TUGListEnd uglistend_;
    TUGLiMenuSt stLiMenu_;

    //コンストラクタ
    explicit TUGLiMenu(TUIGenLayer* player);
    virtual ~TUGLiMenu(void);
    TUGRcNormal* MakeObj_Record();
    void ChangeListEnd(BOOL listend_f);

    //void InitShop();
    s64 GetCommonID();
  };
 
}
