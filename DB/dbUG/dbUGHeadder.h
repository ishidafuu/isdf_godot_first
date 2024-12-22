#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRadio.h"

#include "dbUGRProf.h"
#include "dbUGBtnSupply.h"
#include "dbUGBtnCredit.h"
#include "dbUGBtnProf.h"
#include "dbUGRHdCredit.h"

namespace db
{
  enum  enUGHeadderAct
  {
    enUGHeadderAct_profile,
    enUGHeadderAct_techno,
    enUGHeadderAct_ggz,
  };

  enum enHeadderState
  {
    head_Normal,
    head_NoInput,
    head_Invisible,
  };

  struct TUGHeadderSt
  {
    enHeadderState state_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGHeadder : public TUGGen
  {
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGHeadderSt stHeadder_;
    TUIObj* shadow_;
    TUGBtnCredit ugbtn_credit_;
    TUGBtnProf ugbtn_prof_;
    TUGBtnSupply ugbtn_ggz_;
    TUGRHdCredit ugr_star_;
    TUGRHdCredit ugr_todd_;
    TUGRHdCredit ugr_huku_;
    TUGRHdCredit ugr_gacha_;

    //コンストラクタ
    explicit TUGHeadder(TUIGenLayer* player);
    virtual ~TUGHeadder(void);

    void MakeGroup(TUIObj* parent);
    void SetEnable();
    void SetDisable();
    void SetModal(BOOL modal_f);
    void UndrawCredit();
    void Refresh(BOOL rename_f);
    void RefreshCredit();

    BOOL IsActRes(enUGHeadderAct res);
  };
 
}
