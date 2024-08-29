#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcLogItem.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiLogGetItemSt
  {
    s64 mid_gacha_;
    void Init()
    {
      Zeromem(this);
      mid_gacha_ = NGNUM;
    }
  };


  //リスト
  class TUGLiLogGetItem :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcLogItem*> records_;
    TUGListEnd uglistend_;
    TUGLiLogGetItemSt stLiLogGetItem_;
    //コンストラクタ
    explicit TUGLiLogGetItem(TUIGenLayer* player);
    virtual ~TUGLiLogGetItem(void);
    void Refresh();
    void ChangeListEnd(BOOL listend_f);
    void SetListEnd(s32 recordidx);
  };
 
}
