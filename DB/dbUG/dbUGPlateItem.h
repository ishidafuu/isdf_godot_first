#pragma once
#include "dbInclude.h"
#include "dbUGButton.h"
namespace db
{

  //リスト
  class TUGPlateItem :public TUGGen
  {
  protected:
    void GenSetSupplyText(s64 mid_supply);
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGRName ugname_;
    TUIObj* text0_;
    TUIObj* text1_;
    TUIObj* text2_;
    TUIObj* text3_;
    //TUIObj* textEv0_;
    //TUIObj* textEv1_;

    //コンストラクタ
    explicit TUGPlateItem(TUIGenLayer* player);
    virtual ~TUGPlateItem(void);

    void MakeGroup(TUIObj* parent);

    void SetDataSupply(s64 mid_supply, s32 num);
    void SetDataSupplyEvent(s64 mid_supply, s32 num, s32 tokkouNum, s32 tokkouMag);
    void SetDataPack(s64 mid_itempack, s32 num);
    void SetDataTodd(s32 num);
    void SetDataStar(s32 num);
    void SetDataHuku(s32 num);
    void SetDataBGM(s64 mid_bgm);
    void SetLRPos(BOOL leftpos_f);
  };
 
}
