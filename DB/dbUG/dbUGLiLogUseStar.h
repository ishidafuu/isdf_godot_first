#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcLogStar.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiLogUseStarSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiLogUseStar :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcLogStar*> records_;
    //TUGListEnd uglistend_;
    TUGLiLogUseStarSt stLiLogUseStar_;
    //コンストラクタ
    explicit TUGLiLogUseStar(TUIGenLayer* player);
    virtual ~TUGLiLogUseStar(void);
    void Refresh(BOOL bstar_f, BOOL fstar_f);
    //void ChangeListEnd(BOOL listend_f);
    //void SetListEnd(s32 recordidx);
  };
 
}
