#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcLogStar.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiLogBuyStarSt
  {
    //s64 mid_gacha_;
    void Init()
    {
      Zeromem(this);
      //mid_gacha_ = NGNUM;
    }
  };


  //リスト
  class TUGLiLogBuyStar :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcLogStar*> records_;
    //TUGListEnd uglistend_;
    TUGLiLogBuyStarSt stLiLogBuyStar_;
    //コンストラクタ
    explicit TUGLiLogBuyStar(TUIGenLayer* player);
    virtual ~TUGLiLogBuyStar(void);
    void Refresh();
    //void ChangeListEnd(BOOL listend_f);
    //void SetListEnd(s32 recordidx);
  };
 
}
