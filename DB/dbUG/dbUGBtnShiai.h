#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{


  struct TUGBtnShiaiSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnShiai : public TUGButton
  {
  protected:
    TUIObj* banner_;//バナー
    void DoUpdateSub(BOOL draw_f);
  public:   
    TUGBtnShiaiSt stBtnShiai_;
    //コンストラクタ
    explicit TUGBtnShiai(TUIGenLayer* player);
    virtual ~TUGBtnShiai(void);
    void MakeGroupShiai(TUIObj* parent, base::TGrPart* pgrpart, s32 partno, const char* msg);
    void SetBanner(base::TGrPart* pgrpart, s32 partno, const char* msg);
    void SetBannerMsg(const char* msg);
    void SetEvent(s64 mid_event);
  };

 
}
