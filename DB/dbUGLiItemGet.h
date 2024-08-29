#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
#include "dbUGListEnd.h"
namespace db
{
  struct TUGLiItemGetSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiItemGet :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
    void DoMakeGroupSub();
  public:
    std::vector<TUGRcNormal*> records_;
    TUGLiItemGetSt stLiItemGet_;
    void Refresh();

    //コンストラクタ
    explicit TUGLiItemGet(TUIGenLayer* player);
    virtual ~TUGLiItemGet(void);
    s64 GetCommonID();
    s64 GetEtcNum();
    
  };
 
}
