#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcLogItem.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiLogUseItemSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiLogUseItem :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcLogItem*> records_;
    //TUGListEnd uglistend_;
    TUGLiLogUseItemSt stLiLogUseItem_;
    //コンストラクタ
    explicit TUGLiLogUseItem(TUIGenLayer* player);
    virtual ~TUGLiLogUseItem(void);
    void Refresh();
    //void ChangeListEnd(BOOL listend_f);
    //void SetListEnd(s32 recordidx);
  };
 
}
