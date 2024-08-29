#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcRanking.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiRankingSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiRanking :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcRanking*> records_;
    std::vector<TUGRcNormal*> records2_;
	LONGVECTOR helpCate_;

    //TUGListEnd uglistend_;
    TUGLiRankingSt stLiRanking_;

    //コンストラクタ
    explicit TUGLiRanking(TUIGenLayer* player);
    virtual ~TUGLiRanking(void);

    void RefreshAllEvent(BOOL result_f);
    void RefreshEvent(s32 mid_event);
    void ChangeListEnd(BOOL listend_f);
    s64 GetCommonID();
	s64 GetHelpCategory();

  };
 
}
