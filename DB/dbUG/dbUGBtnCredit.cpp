#include "dbUGBtnCredit.h"
#include "dbUILayer.h"
#include "dbMgUI.h"

namespace db
{
  //コンストラクタ
  TUGBtnCredit::TUGBtnCredit(TUIGenLayer* player) :TUGButton(player)
  {
    stBtnCredit_.Init();
  }
  TUGBtnCredit::~TUGBtnCredit(void)
  {
  }
  void TUGBtnCredit::MakeGroupCredit(TUIObj* parent)
  {
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(ug_btn_footer);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(ug_btn_footer2);
    GenMakeGroup(parent, FALSE);//センタリングしない
    //SetPos(352 - (TABDX * 4) - 30, 21);
    SetPos(352 - 30, 21);
    SetDepth(UIDP_06BUTTONHI);
    field_->b_SetModal(TRUE);

    //スター
    cb_star_ = MakeObj();
    cb_star_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_item));
    cb_star_->g_SetPartNo(ugi_Star);
    cb_star_->o_SetParent(field_); 
    cb_star_->z_SetPos(0, 0-4);
    AddUI(cb_star_);

    //トッド
    cb_todd_ = MakeObj();
    cb_todd_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_item));
    cb_todd_->g_SetPartNo(ugi_Todd);
    cb_todd_->o_SetParent(field_);
    cb_todd_->z_SetPos(0, 9 - 4);
    AddUI(cb_todd_);

  }

  //文字列セット
  void TUGBtnCredit::Refresh()
  {
    s32 allstar = player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveFStar)
      + player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveBStar);
    std::string star = lib_str::IntToStr(allstar);
    std::string todd = lib_str::IntToStr(player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveTodd));
    cb_star_->f_MakeFont_Ex(star.c_str(), base::fd_Mini | base::fd_Pro, FC2_hd_normal, FC2_hd_normal, 0);
    cb_star_->f_FontPos(51 - 32, 11);
    cb_todd_->f_MakeFont_Ex(todd.c_str(), base::fd_Mini | base::fd_Pro, FC2_hd_normal, FC2_hd_normal, 0);
    cb_todd_->f_FontPos(51 - 32, 11);
  }


  //更新
  void TUGBtnCredit::DoUpdateSub(BOOL draw_f)
  {
    //gb_ggz_->Update(draw_f);
  }

}