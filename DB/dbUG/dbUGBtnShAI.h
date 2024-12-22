#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRadio.h"

namespace db
{

  struct TUGShAISt
  {
    s32 AI_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGShAI : public TUGGen
  {
  protected:
    TUGRadio ugairadio_;
    std::vector<TUGButton*> ugaibtn_;

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:   
    TUGShAISt stBtnShAI_;
    
    //コンストラクタ
    explicit TUGShAI(TUIGenLayer* player);
    virtual ~TUGShAI(void);
    void MakeGroup(TUIObj* parent);
    void Refresh();
    void SwitchPause(BOOL pause_f);
    void SetDisable(BOOL disable_f);
  };

 
}
