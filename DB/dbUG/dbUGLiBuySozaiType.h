#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
//#include "dbUGRcBuySozaiType.h"
namespace db
{



  struct TUGLiBuySozaiTypeSt
  {
    //s64 id_team_;
    //s64 mid_team_;
    //s32 posno_;
    //s32 maxpage_;
    //s32 pageno_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiBuySozaiType :public TUGLiGen
  {
  protected:
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNormal*> records_;
    TUGLiBuySozaiTypeSt stLiBuySozaiType_;
    //コンストラクタ
    explicit TUGLiBuySozaiType(TUIGenLayer* player);
    virtual ~TUGLiBuySozaiType(void);
    void Refresh();
	void Refresh(s32 mid_bbabuy);
    s32 GetType();
    s32 GetNextType();
  };

}
