#include "dbUGHaikei.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGHaikei::TUGHaikei(TUIGenLayer* player) :TUGGen(player)
  ,ugname_(player)
//    ugbtn_back_(player),
//    uglookbtn_(player)
  {
    stHaikei_.Init();
  }

  TUGHaikei::~TUGHaikei()
  {

  }
  void TUGHaikei::MakeGroup(TUIObj* parent)//, base::TGrPart* grpicture)
  {
    //SetParentなかったので位置ずれるようなら確認
    SetParent(parent);
    //下地
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_hk_frame));
    
    //写真
    picture_ = MakeObj();
    picture_->o_SetParent(field_);
    picture_->z_SetPos(4, 4);
    picture_->f_FontPos(100,88);

    //なまえ
    ugname_.MakeGroup(field_);
    ugname_.SetPos(4, 102);

	// フェード用
	ugFade_ = MakeObj();
	ugFade_->o_SetParent(picture_);
	ugFade_->o_SetObjFilterGrp(144, 96, 0xFF, 0, 0, 0);
	ugFade_->g_SetAlp(FALSE, 0xD0);
	//ugFade_->g_SetDepth(UIDP_08MSGFILTER);
	ugFade_->g_SetDraw(TRUE);

	ugNo_ = MakeObj();
	ugNo_->o_SetParent(picture_);
	ugNo_->z_SetPos(3, 5);

	// アイコン
	ugIcon_ = MakeObj();
	ugIcon_->o_SetParent(ugFade_);
	ugIcon_->z_SetPos(72-8, 48-8);
	ugIcon_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_item));
	ugIcon_->g_SetPartNo(enUGPIcon::ugi_Search2);

	// 発売日表記
	ugDate_ = MakeObj();
	ugDate_->o_SetParent(picture_);
	ugDate_->z_SetPos(100, 90);
	SetHide(FALSE);
  }

  void TUGHaikei::SetHaikeiDt(base::TGrp* pgrp, s32 mid_haikei)
  {
    picture_->o_SetObjGrp(pgrp);
    ugname_.SetNameHaikeiPicture(mid_haikei);

	std::string strNo = pmgEO_->mgDt_.dtCourt_.GetStrPK(mid_haikei, enDtCourtStr_no);
	ugNo_->f_MakeFont_Mini(strNo.c_str(), FC2_mini);

	s32 titleID = pmgEO_->mgDt_.dtCourt_.GetDtPK(mid_haikei, enDtCourt_titleid);
	std::string strDate = pmgEO_->mgDt_.dtTitle_.GetStrPK(titleID, enDtTitleStr_day);
	ugDate_->f_MakeFont_Mini(strDate.c_str(), FC2_mini);
  }

  void TUGHaikei::SetHaikeiDt_ItemView(s64 mid_haikei)
  {
    SetDraw(TRUE);

    //レイヤの画像をクリアしてるので、使い所によってはバッティングする
    player_->ClearGrp();//pgrvec_をクリア
    std::string thumbname = "t_" + pmgEO_->mgDt_.dtCourt_.GetStrPK(mid_haikei, enDtCourtStr_filename);
    base::TGrp* court = player_->LoadGrp("thumb", thumbname.c_str());//pgrvec_の中に入る
    SetHaikeiDt(court, mid_haikei);
    field_->z_SetPos(8, -56);//背景単体の時は指定位置

	std::string strNo = pmgEO_->mgDt_.dtCourt_.GetStrPK(mid_haikei, enDtCourtStr_no);
	ugNo_->f_MakeFont_Mini(strNo.c_str(), FC2_mini);

	s32 titleID = pmgEO_->mgDt_.dtCourt_.GetDtPK(mid_haikei, enDtCourt_titleid);
	std::string strDate = pmgEO_->mgDt_.dtTitle_.GetStrPK(titleID, enDtTitleStr_day);
	ugDate_->f_MakeFont_Mini(strDate.c_str(), FC2_mini);
  }

  void TUGHaikei::SetLRPos(BOOL leftpos_f)
  {
    const s32 POSLX = -160;
    const s32 POSRX = 8;
    const s32 POSY = -56;

    s32 posx = (leftpos_f)
      ? POSLX
      : POSRX;

    field_->z_SetPos(posx, POSY);
  }

  void TUGHaikei::SetHide(BOOL isHide)
  {
	  ugFade_->g_SetDraw(isHide);
	  ugIcon_->g_SetDraw(isHide);
	  ugDate_->g_SetDraw(!isHide);
  }


  void TUGHaikei::SetTipsDraw_f(BOOL tipsdraw_f)
  {
    stHaikei_.tipsdraw_f_ = tipsdraw_f;
  }
  BOOL TUGHaikei::IsSameGrp(base::TGrp* pgrp)
  {
    return picture_->o_IsSameGrp(pgrp);
  }

  void TUGHaikei::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    picture_->Update(draw_f);
    ugname_.Update(draw_f);
	ugFade_->Update(draw_f);
	ugIcon_->Update(draw_f);
	ugDate_->Update(draw_f);
	ugNo_->Update(draw_f);
  }

}
