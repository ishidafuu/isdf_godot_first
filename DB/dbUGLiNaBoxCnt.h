#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNaBoxCnt.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiNaBoxCntSt
  {
    s64 mid_nabox_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiNaBoxCnt :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNaBoxCnt*> records_;

    TUGLiNaBoxCntSt stLiBoxCnt_;
    //コンストラクタ
    explicit TUGLiNaBoxCnt(TUIGenLayer* player);
    virtual ~TUGLiNaBoxCnt(void);
    void Refresh_NaBox(s64 mid_nabox);
    s64 GetMIDNaBoxCnt(s32 recordidx);
  };
 
}
