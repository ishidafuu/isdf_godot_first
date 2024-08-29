#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
namespace db
{
  struct TUGLiHaikeiSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiHaikei :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNormal*> records_;
    INTVECTOR allnumvec_;
    TUGLiHaikeiSt stLiHaikei_;


    //コンストラクタ
    explicit TUGLiHaikei(TUIGenLayer* player);
    virtual ~TUGLiHaikei(void);
    TUGRcNormal* MakeObj_Record();
    void Refresh();
	void Refresh(s64 mid_event);
  };
 
}
