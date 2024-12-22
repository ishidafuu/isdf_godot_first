#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGRIcon.h"

namespace db
{
  class TUILayer;

  struct TUGRTimerSt
  {
    s32 Timersize_;
    BOOL rare_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGRTimer : public TUGGen
  {
  protected:
    TUIObj* time_;
    void DoUpdate(BOOL draw_f);
  public:
    TUGRTimerSt stRTimer_;
    //コンストラクタ
    explicit TUGRTimer(TUIGenLayer* player);
    virtual ~TUGRTimer(void);
    void MakeGroup(TUIObj* parent);
    void SetIcon(BOOL icon_f);
    void SetTimer(std::string numstr);
    void SetTimer(s64 num);
    void SetRightShift();

    void SetLimY(s32 limu, s32 hhh);
  };
 
}
