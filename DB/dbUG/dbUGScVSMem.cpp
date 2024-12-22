#include "dbUGScVSMem.h"


namespace db
{
  //コンストラクタ
  TUGScVSMem::TUGScVSMem(TUIGenLayer* player) : TUGGen(player)
  {
    stScVSMem_.Init();
  }
  TUGScVSMem::~TUGScVSMem()
  {

  }
  void TUGScVSMem::MakeGroup(TUIObj* parent, s32 sideno, s32 memno)
  {
    SetParent(parent);
    stScVSMem_.sideno_ = sideno;
    stScVSMem_.memno_ = memno;

    body_ = MakeObj();
    body_->o_SetObjGrPart(pmgEO_->mgGrp_.pgr_IconBody_[sideno]);
    body_->g_SetPartNo(memno);
    body_->o_SetParent(field_);
    body_->g_SetCenter(TRUE);
    body_->g_SetUnder();
    //body_->g_SetOvOvj(team_[sideno].field_);
    s32 posx = (memno * 4);
    s32 posy = (memno * 40) - 40;// +84;

    if (memno > 3)
    {
      posx -= 88;
      posy -= 136;
    }
    

    if (sideno == SIDE1)
    {
      posx = posx + 136;// -160;
      body_->g_SetMirror(TRUE);
    }
    else
    {
      posx = -(posx + 136);// -160);
    } 
    body_->z_SetPos(posx, posy);

    //名前
    s32 nameposx = (sideno == SIDE1)
      ? -10
      : +10;
    name_ = MakeObj();

    name_->o_SetParent(body_);
    name_->z_SetPos(nameposx, -16);
    if (sideno == SIDE1) name_->f_SetRightShift();

    //番号
    s32 noposx = (sideno == SIDE1)
      ? -14
      : +14;
    no_ = MakeObj();
    std::string no = lib_str::IntToStr(memno+1, 3);
    no_->f_MakeFont(no.c_str(), FC2_normal);
    no_->o_SetParent(body_);
    no_->z_SetPos(noposx, -28);
    if (sideno == SIDE1) no_->f_SetRightShift();

    ////ＨＰゲージ
    //s32 hpposx = (sideno == SIDE1)
    //  ? +8
    //  : -8;
    //hp_ = MakeObj();
    //hp_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_vs_hp));
    //hp_->o_SetParent(body_);
    //hp_->z_SetPos(hpposx, 1);
    //if (sideno == SIDE1) hp_->g_SetRightShift();

	//Lv
	//番号
	s32 lvposx = (sideno == SIDE1)
		? -14
		: +14;
	lv_ = MakeObj();
	lv_->o_SetParent(body_);
	lv_->z_SetPos(lvposx, -32);
	if (sideno == SIDE1) lv_->f_SetRightShift();
  }

  void TUGScVSMem::Refresh()
  {
	  s64 id_kantoku = pmgEO_->stShiai_.GetKantokuID(stScVSMem_.sideno_);
    s32 mid_team = pmgEO_->stShiai_.GetMidTeam(stScVSMem_.sideno_);
	s32 id_team = pmgEO_->stShiai_.GetIDTeam(stScVSMem_.sideno_);
    s32 memid = pmgEO_->mgCoM_.GetCharID(mid_team, stScVSMem_.memno_);
	BOOL isCOM = pmgEO_->stShiai_.IsCOM(stScVSMem_.sideno_);
    std::string memname = pmgEO_->mgDt_.dtChar_.GetName(memid);
    name_->f_MakeFont(memname.c_str(), FC2_normal);
    //hp_->g_SetSrcW(pmgEO_->mgDt_.dtChar_.GetLv(clv_Pow, memid) * 4);

	s32 lv = 1;
	// 通信対戦時
	if (pmgEO_->stShiai_.IsVSHum())
	{
		s32 myside = mid::midGetMyIdx();

		// 自分
		if (myside == stScVSMem_.sideno_)
		{
			lv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team, stScVSMem_.memno_);
		}
		// 相手
		else
		{			
			lv = pmgEO_->mgCoU_.GetTmCharaLvByKantokuID(id_kantoku, mid_team, stScVSMem_.memno_);
		}
	}
	else
	{
		// 自分
		if (SIDE0 == stScVSMem_.sideno_)
		{
			lv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team, stScVSMem_.memno_);
		}
		// 勝ち抜きイベント時の相手チーム
		else if (pmgEO_->stShiai_.IsTour() && SIDE1 == stScVSMem_.sideno_)
		{
			lv = pmgEO_->mgCoU_.GetTmCharaLvByKantokuID(id_kantoku, mid_team, stScVSMem_.memno_);
		}
		// COM
		else
		{
			lv = 1;
		}
	}

	// COMならレベル表示しない
	if(isCOM)
	{
		lv_->g_SetDraw(FALSE);
	}
	else
	{
		std::string memlv = "ＬＶ" + lib_str::IntToStr(lv);
		lv_->f_MakeFont_Mini(memlv.c_str(), FC2_mini);
		lv_->g_SetDraw(TRUE);
	}
  }

  void TUGScVSMem::DoUpdate(BOOL draw_f)
  {
    body_->Update(draw_f);
    no_->Update(draw_f);
    name_->Update(draw_f);
    //hp_->Update(draw_f);
	lv_->Update(draw_f);
  }

}
