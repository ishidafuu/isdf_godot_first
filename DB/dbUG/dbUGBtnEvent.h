#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{

  struct TUGBtnEventSt
  {
    s32 mid_event_;
    s32 wid_event_;
    s32 resttime_;
    s32 mv_c_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnEvent : public TUGButton
  {
  protected:
    TUIObj* banner_;//バナー
    void MvTimer();
    void DoUpdateSub(BOOL draw_f);
  public:
    TUGBtnEventSt stBtnEvent_;
    //コンストラクタ
    explicit TUGBtnEvent(TUIGenLayer* player);
    virtual ~TUGBtnEvent(void);
    void MakeGroupEvent(TUIObj* parent, base::TGrPart* pgrpart);
    void SetEvent(s64 mid_event);
    void Refresh();
  };


}
