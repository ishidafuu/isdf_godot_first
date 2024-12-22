#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcProb.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiProbSt
  {
    s64 groupNo_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiProb :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcProb*> records_;
    TUGListEnd uglistend_;
    TUGLiProbSt stLiProb_;

    //コンストラクタ
    explicit TUGLiProb(TUIGenLayer* player);
    virtual ~TUGLiProb(void);
    void Refresh(s64 mid_gacha);
    s32 GetCntGrpNo(s32 recordidx);
  };
 
}
