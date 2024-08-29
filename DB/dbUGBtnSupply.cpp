#include "dbUGBtnSupply.h"
#include "dbMgUI.h"

namespace db
{
  const s32 ST_GGZ = 4;
  const s32 ST_MGK = 5;
  const s32 ST_FKT = 3;

  //コンストラクタ
  TUGBtnSupply::TUGBtnSupply(TUIGenLayer* player) :TUGButton(player)
  {
    stBtnSupply_.Init();
  }
  TUGBtnSupply::~TUGBtnSupply(void)
  {
  }
  void TUGBtnSupply::GenMakeGroupSupply(TUIObj* parent, s32 stype, s32 mid_supply)
  {
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(ug_btn_footer);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(ug_btn_footer2);
    GenMakeGroup(parent, FALSE);//センタリングしない

    if (mid_supply == SPL_GGZ)
    {
      SetPos(352 - (TABDX * 4) - 30, 21);
    }
    //else if (mid_supply == SPL_MGK)
    //{
    //  //SetPos(352 - (TABDX * 4) - 30, 21);
    //  SetPos(352 - (TABDX * 1) - 30, 3);
    //}
    else
    {
      SetPos(352 - (TABDX * 4) - 30, 3);
    }   
    SetDepth(UIDP_06BUTTONHI);
    field_->b_SetModal(TRUE);

    ugricon_.SetIconSupply(mid_supply);
    ugricon_.SetPos(20, 12);
    stBtnSupply_.supplytype_ = stype;
  }
  void TUGBtnSupply::MakeGroupSupply_GGZ(TUIObj* parent)
  {
    GenMakeGroupSupply(parent, ST_GGZ, SPL_GGZ);
  }
  void TUGBtnSupply::MakeGroupSupply_MGK(TUIObj* parent)
  {
    GenMakeGroupSupply(parent, ST_MGK, SPL_MGK);
  }
  void TUGBtnSupply::MakeGroupSupply_FKT(TUIObj* parent)
  {
    GenMakeGroupSupply(parent, ST_FKT, SPL_FKT);
  }

  //文字列セット
  void TUGBtnSupply::Refresh()
  {
    std::string strnum = "";
    switch (stBtnSupply_.supplytype_)
    {
    case ST_GGZ: strnum = lib_str::IntToStr(pmgEO_->mgCoU_.GetSupplyNum_GGZ()); break;
    case ST_MGK: strnum = lib_str::IntToStr(pmgEO_->mgCoU_.GetSupplyNum_MGK()); break;
    case ST_FKT: strnum = lib_str::IntToStr(pmgEO_->mgCoU_.GetSupplyNum_FKT()); break;
    }
    ugricon_.GetField()->f_MakeFont_Ex(strnum.c_str(), base::fd_Mini | base::fd_Pro, FC2_hd_normal, FC2_hd_normal, 0);
    ugricon_.GetField()->f_FontPos(10, 3);
  }


  //更新
  void TUGBtnSupply::DoUpdateSub(BOOL draw_f)
  {
    //gb_ggz_->Update(draw_f);
  }

}