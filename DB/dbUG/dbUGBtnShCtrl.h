#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{

  struct TUGBtnShCtrlSt
  {
    s32 ctrltype_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnShCtrl : public TUGButton
  {
  protected:
    //TUIObj* gb_ggz_;
    void DoUpdateSub(BOOL draw_f);
  public:   
    TUGBtnShCtrlSt stBtnShCtrl_;
    //コンストラクタ
    explicit TUGBtnShCtrl(TUIGenLayer* player);
    virtual ~TUGBtnShCtrl(void);
    void MakeGroupShCtrl(TUIObj* parent);
    void SetDisable(BOOL disable_f);
    void NextCtrl();
    BOOL IsSmart();
    BOOL IsRetro();
    BOOL IsAuto();
    BOOL IsTech();
	BOOL IsDebug();
    s32 GetCtrl();

    void SetSmart();
    void SetRetro();
	void SetDebug();
  };

 
}
