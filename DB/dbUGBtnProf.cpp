#include "dbUGBtnProf.h"
#include "dbUILayer.h"
#include "dbMgUI.h"

namespace db
{
  //コンストラクタ
  TUGBtnProf::TUGBtnProf(TUIGenLayer* player) :TUGButton(player)
    , ugkantoku_(player)
  {
    stBtnProf_.Init();
  }
  TUGBtnProf::~TUGBtnProf(void)
  {
  }
  void TUGBtnProf::MakeGroupProf(TUIObj* parent)
  {
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(ug_btn_profile);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(ug_btn_profile2);
    GenMakeGroup(parent, FALSE);//センタリングしない
    SetPos(152, 21);
    SetDepth(UIDP_06BUTTONHI);
    field_->b_SetModal(TRUE);

    //監督顔アイコン
    ugkantoku_.MakeGroup(field_, FALSE);//ここは常に自分
    ugkantoku_.SetMType(kmt_Icon);
    ugkantoku_.SetPos(26, 20);
    ugkantoku_.SetDraw(TRUE);

    //スタミナフィールド
    hb_staminafield_ = MakeObj();
    hb_staminafield_->o_SetParent(field_);
    hb_staminafield_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_bar_stamina));
    hb_staminafield_->z_SetPos(39, 15);

    //スタミナバー
    hb_stamina_ = MakeObj();
    hb_stamina_->o_SetParent(hb_staminafield_);
    hb_stamina_->o_SetObjFilterGrp(50, 3, 0xFF, 0xFF, 179, 15);//50の値が変動
    hb_stamina_->z_SetPos(1, 1);
    
  }

  //文字列セット
  void TUGBtnProf::Refresh(BOOL rename_f)
  {
    const s32 MINTOSEC = 60;
    const s32 MAXSTA = 100;

    s64 nowtime = mid::midNowTime();
    s64 endtime = player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_login_date);
    s64 spendtime = (nowtime - endtime);
    if (spendtime < 0) spendtime = 0;

    s64 nowstamina = player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_stamina) - spendtime;
    if (nowstamina < 0) nowstamina = 0;

    s32 stamina = (mid::midIsDXL())
      ? MAXSTA - 50
      : MAXSTA - ((nowstamina + (MINTOSEC - 1)) / MINTOSEC);

#ifdef __K_DEBUG_USER_DATA__
	stamina = kdebug::DebugUDManager::GetInstance()->GetHP();
#endif	// #ifdef __K_DEBUG_USER_DATA__

    hb_stamina_->o_SetObjFilterGrp((stamina / 2), 3, 0xFF, 0xFF, 179, 15);//50の値が変動
    std::string hp = "ＨＰ：" + lib_str::IntToStr(stamina);
    hb_staminafield_->f_MakeFont_Mini(hp.c_str(), FC2_hd_normal);
    hb_staminafield_->f_FontPos(54, 1);

    if (rename_f)
    {
      std::string name = player_->pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_name_c32);
      SetFont(name.c_str(), FC2_hd_normal, 40, 2);//かんとくくんが変動
    }
  }

  //更新
  void TUGBtnProf::DoUpdateSub(BOOL draw_f)
  {
    ugkantoku_.Update(draw_f);
    hb_staminafield_->Update(draw_f);
    hb_stamina_->Update(draw_f);


#ifdef __K_DEBUG_USER_DATA__
	bool isUpdate = false;
	if( isUpdate)
	{
		Refresh(false);
	}
#endif	// #ifdef __K_DEBUG_USER_DATA__
  }

}