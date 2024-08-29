#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{

  struct TUGBtnShTimeSt
  {
    s32 time_;
    BOOL timeover_f_;
    BOOL skip_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnShTime : public TUGButton
  {
  protected:
    //TUIObj* gb_ggz_;
    void DoUpdateSub(BOOL draw_f);
  public:   
    TUGBtnShTimeSt stBtnShTime_;
    //コンストラクタ
    explicit TUGBtnShTime(TUIGenLayer* player);
    virtual ~TUGBtnShTime(void);
    void MakeGroupShTime(TUIObj* parent);
    void Refresh();
    void RefreshTimeOver(BOOL overtime_f);
    void RefreshRetry();
    void SwitchPause(BOOL pause_f);
    void SetDisable(BOOL disable_f);
    void SetSkipTuto();
  };

 
}
