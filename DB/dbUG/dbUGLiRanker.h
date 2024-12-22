#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcRanker.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiRankerSt
  {
    BOOL myscore_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiRanker :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoMakeGroupSub();
    void DoClearRecord();
    TUGRcNormal* MakeScoreRecord();
	
  public:
    std::vector<TUGRcRanker*> records_;
    std::vector<TUGRcNormal*> records2_;
    //TUGListEnd uglistend_;
    TUGLiRankerSt stLiRanker_;

    //コンストラクタ
    explicit TUGLiRanker(TUIGenLayer* player);
    virtual ~TUGLiRanker(void);

    void Refresh(BOOL myscore_f, s64 id_ranking);
    void ChangeListEnd(BOOL listend_f);
    //void SetListEnd(s32 recordidx);
    s64 GetCommonID();
  };
 
}
