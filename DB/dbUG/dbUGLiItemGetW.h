#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGLiGenW.h"
#include "dbUGRcNormal.h"
namespace db
{

  //struct TUGListWSt
  //{
  //  void Init()
  //  {
  //    Zeromem(this);
  //  }
  //};


  //リスト
  class TUGLiItemGetW :public TUGLiGenW
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
    void DoMakeGroupSub();
  public:
    std::vector<TUGRcNormal*> records_;
    //TUGListWSt stLiItemGet_;
    void Refresh();

    //コンストラクタ
    explicit TUGLiItemGetW(TUIGenLayer* player);
    virtual ~TUGLiItemGetW(void);
    s64 GetCommonID();
    s64 GetEtcNum();
  };
 
}
