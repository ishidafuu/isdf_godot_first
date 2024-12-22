#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{
 
  struct TUGMessageSt
  {
    BOOL title_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGRMessage :public TUGGen
  {
  protected:

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGMessageSt stMsg_;
    TUIObj* title_;
    std::vector<TUIObj*> text_;

    //コンストラクタ
    explicit TUGRMessage(TUIGenLayer* player);
    virtual ~TUGRMessage(void);

    void MakeGroup(TUIObj* parent, BOOL titlebar_f);
	void MakeGroup_Mini(TUIObj* parent, BOOL titlebar_f);
    void SetData(STRVECTOR* text);
    void SetEvent(s32 mid_event);
  };
 
}
