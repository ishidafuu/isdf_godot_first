#include "dbUGHeadder.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGHeadder::TUGHeadder(TUIGenLayer* player) :TUGGen(player)
    , ugbtn_prof_(player)
    , ugbtn_ggz_(player)
    , ugbtn_credit_(player)
    , ugr_star_(player)
    , ugr_todd_(player)
    , ugr_huku_(player)
    , ugr_gacha_(player)
  {
    stHeadder_.Init();
  }
  TUGHeadder::~TUGHeadder()
  {
  }

  void TUGHeadder::MakeGroup(TUIObj* parent)
  {
    const s32 BARW = 512;
    const s32 HEADH = 44;
    SetParent(parent);

    //帯
    field_->o_SetObjFilterGrp(BARW, HEADH, 0xFF, 0, 0, 0);
    //field_->z_SetPos_Full(0, 0);
    field_->z_SetPos_Full(0, 0);
    field_->g_SetDepth(UIDP_04HEADER);

    //影
    shadow_ = MakeObj();
    shadow_->o_SetParent(field_);
    shadow_->o_SetObjFilterGrp(BARW, 5, 0x80, 0, 0, 0);
    shadow_->z_SetPos_Full(0, HEADH);

    //ヘッダボタン
    ugbtn_prof_.MakeGroupProf(field_);
    //クレジットボタン
    ugbtn_credit_.MakeGroupCredit(field_);
    //ギンギンＺボタン
    ugbtn_ggz_.MakeGroupSupply_GGZ(field_);

    ugr_todd_.MakeGroup_Todd(field_);
    ugr_star_.MakeGroup_Star(field_);
    ugr_huku_.MakeGroup_Huku(field_);
    ugr_gacha_.MakeGroup_Gacha(field_);
    Refresh(TRUE);
  }

  void TUGHeadder::SetEnable()
  {
    Refresh(TRUE);
    ugbtn_credit_.SetDraw(TRUE);
    ugbtn_prof_.SetDraw(TRUE);
    ugbtn_ggz_.SetDraw(TRUE);
    ugr_todd_.SetDraw(FALSE);
    ugr_star_.SetDraw(FALSE);
    ugr_huku_.SetDraw(FALSE);
    ugr_gacha_.SetDraw(FALSE);
  }
  void TUGHeadder::SetDisable()
  {
    ugbtn_credit_.SetDraw(FALSE);
    ugbtn_prof_.SetDraw(FALSE);
    ugbtn_ggz_.SetDraw(FALSE);
    ugr_todd_.SetDraw(FALSE);
    ugr_star_.SetDraw(FALSE);
    ugr_huku_.SetDraw(FALSE);
    ugr_gacha_.SetDraw(FALSE);
  }
  void TUGHeadder::SetModal(BOOL modal_f)
  {
    ugbtn_credit_.SetModal(modal_f);
    ugbtn_prof_.SetModal(modal_f);
    ugbtn_ggz_.SetModal(modal_f);
  }
  void TUGHeadder::UndrawCredit()
  {
    ugr_todd_.SetDraw(FALSE);
    ugr_star_.SetDraw(FALSE);
    ugr_huku_.SetDraw(FALSE);
    ugr_gacha_.SetDraw(FALSE);
  }
  void TUGHeadder::RefreshCredit()
  {
    ugr_todd_.Refresh();
    ugr_star_.Refresh();
    ugr_huku_.Refresh();
    ugr_gacha_.Refresh();
  }

  void TUGHeadder::Refresh(BOOL rename_f)
  {
    ugbtn_prof_.Refresh(rename_f);
    ugbtn_credit_.Refresh();
    ugbtn_ggz_.Refresh();
  }

  void TUGHeadder::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    shadow_->Update(draw_f);
    
    ugbtn_credit_.Update(draw_f);
    ugbtn_prof_.Update(draw_f);
    ugbtn_ggz_.Update(draw_f);
    ugr_todd_.Update(draw_f);
    ugr_star_.Update(draw_f);
    ugr_huku_.Update(draw_f);
    ugr_gacha_.Update(draw_f);
  }

  void TUGHeadder::DoAction()
  {
    if (ugbtn_prof_.IsAct())
    {
      SetAct(enUGHeadderAct_profile);
    }
    else if (ugbtn_credit_.IsAct())
    {
      SetAct(enUGHeadderAct_techno);
    }
    else if (ugbtn_ggz_.IsAct())
    {
      SetAct(enUGHeadderAct_ggz);
    }
  }

  BOOL TUGHeadder::IsActRes(enUGHeadderAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }
}
