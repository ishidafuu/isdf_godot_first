#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
#include "dbUGChKantoku.h"
namespace db
{

  struct TUGBtnProfSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGBtnProf : public TUGButton
  {
  protected:
    TUGChKantoku ugkantoku_;
    TUIObj* hb_staminafield_;
    TUIObj* hb_stamina_;
    void DoUpdateSub(BOOL draw_f);
  public:   
    TUGBtnProfSt stBtnProf_;
    //コンストラクタ
    explicit TUGBtnProf(TUIGenLayer* player);
    virtual ~TUGBtnProf(void);
    void MakeGroupProf(TUIObj* parent);
    void Refresh(BOOL rename_f);
  };

 
}
