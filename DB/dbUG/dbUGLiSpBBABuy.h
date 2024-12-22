#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcSpSyouhin.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiSpBBABuySt
  {
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiSpBBABuy :public TUGLiGen
  {
  protected:
    void DoActionSub();
    void DoUpdateSub(BOOL draw_f);
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcSpSyouhin*> records_;
    TUGListEnd uglistend_;
    TUGLiSpBBABuySt stLiSpBBABuy_;

    //コンストラクタ
    explicit TUGLiSpBBABuy(TUIGenLayer* player);
    virtual ~TUGLiSpBBABuy(void);

    void Refresh(s32 type_st, s32 type_ed, s32 mid_bbabuy);
    void ChangeListEnd(BOOL listend_f);
    void SetListEnd(s32 recordidx);
    s64 GetCommonID();

  };
 
}
