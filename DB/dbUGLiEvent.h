#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcEvent.h"
namespace db
{
  struct TUGLiEventSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiEvent :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcEvent*> records_;

    TUGLiEventSt stLiEvent_;

    //コンストラクタ
    explicit TUGLiEvent(TUIGenLayer* player);
    virtual ~TUGLiEvent(void);
    TUGRcEvent* MakeObj_Record();
    void Refresh();
	void Refresh(s64 mid_event);
    s64 GetMIDEvent();
  };
 
}
