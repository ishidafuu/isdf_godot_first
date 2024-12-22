#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{

  struct TUGBtnSoubiSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnSoubi : public TUGButton
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
  public:   
    TUIObj* step_;
    TUIObj* ball_;
    TUIObj* slot_;
    TUGBtnSoubiSt stBtnSoubi_;
    //コンストラクタ
    explicit TUGBtnSoubi(TUIGenLayer* player);
    virtual ~TUGBtnSoubi(void);
    void MakeGroupSoubi(TUIObj* parent, BOOL shset_f);
    void SetLimY(s32 limu, s32 hhh);

    void SetFont(const char* msg, s32 fontcol);
    void SetFont(const char* msg, s32 fontcol, s32 fx, s32 fy);
    void SetFont_Mini(const char* msg, s32 fontcol, s32 fx, s32 fy);

    void SetStepIcon(s32 step);
    void SetBallIcon(s32 rarity);
    void SetBallNo(s32 ballno);
    void SetCharNo(s32 mid_team, s32 slotno);

  };

 
}
