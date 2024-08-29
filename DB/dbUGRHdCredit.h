#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGRIcon.h"

namespace db
{
  class TUILayer;

  struct TUGRHdCreditSt
  {
        s32 credittype_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGRHdCredit : public TUGGen
  {
  protected:
    void GenMakeGroup(TUIObj* parent);
    s32 GenRefresh();
    void SetName(std::string name);
    void DoUpdate(BOOL draw_f);
  public:
    TUGRHdCreditSt stHdCredit_;
    TUGRIcon ugicon_;
    //コンストラクタ
    explicit TUGRHdCredit(TUIGenLayer* player);
    virtual ~TUGRHdCredit(void);

    void MakeGroup_Todd(TUIObj* parent);
    void MakeGroup_Star(TUIObj* parent);
    void MakeGroup_Huku(TUIObj* parent);
    void MakeGroup_Gacha(TUIObj* parent);
    void Refresh();
    void RefreshMid();
    void RefreshGacha(s32 mid_gacha);
  };
 
}
