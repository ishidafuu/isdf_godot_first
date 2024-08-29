#include "dbUGTeam.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGTeam::TUGTeam(TUIGenLayer* player) :TUGGen(player)
    , ugname_(player)
    , ugtimer_(player)
    , ugtimer2_(player)
    , uggetstate_(player)
    , ugtokkou_(player)
    , ugshset_(player)
	, mid_prev_team_(NGNUM)
  {
    stTeam_.Init();
  }

  TUGTeam::~TUGTeam()
  {

  }
  void TUGTeam::MakeGroup(TUIObj* parent, s32 sideNo)
  {
    //SetParentなかったので位置ずれるようなら確認
    SetParent(parent);

    stTeam_.sideNo_ = sideNo;
    //チームカード地
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_team_card));

	const s32 OFFSETX = 20;
	const s32 OFFSETY = 12;
	const s32 OFFSETY2 = 8;

    //キャプテン
    cap_ = MakeObj();
    cap_->o_SetParent(field_);
    cap_->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconBody_[stTeam_.sideNo_]);
    cap_->z_SetPos(13, 21);

	capLv_ = MakeObj();
	capLv_->o_SetParent(field_);
	capLv_->z_SetPos(13 + 4 + OFFSETX, 21 + OFFSETY2);
	//capLv_->f_MakeFont_Mini("＋９９", FC2_mini);

    //メンバー
    for (s32 i = 0; i < DBMEMBER_ALL; ++i)
    {
      mem_[i] = MakeObj();
      mem_[i]->o_SetParent(field_);
      mem_[i]->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconChest_[stTeam_.sideNo_]);
      mem_[i]->g_SetPartNo(i);
      if (i > 3) mem_[i]->g_SetMirror(TRUE);

	  memLv_[i] = MakeObj();
	  memLv_[i]->o_SetParent(field_);
	  //memLv_[i]->f_MakeFont_Mini("＋９９", FC2_mini);
    }

    const s32 MPX = 47;
    const s32 MPY = 46;

    mem_[0]->g_SetDraw(FALSE);
    mem_[1]->z_SetPos(  MPX, MPY);
    mem_[2]->z_SetPos(  MPX + 4, MPY - 18);
    mem_[3]->z_SetPos(  MPX + 22, MPY - 10);
    mem_[4]->z_SetPos(  MPX + 48, MPY);
    mem_[5]->z_SetPos(  MPX + 48 - 4, MPY - 18);
    mem_[6]->z_SetPos(  MPX + 48 + 16, MPY - 10);
	memLv_[0]->g_SetDraw(FALSE);
	memLv_[1]->z_SetPos(MPX + OFFSETX, MPY + OFFSETY2 + 4);
	memLv_[2]->z_SetPos(MPX + 4 + OFFSETX, MPY - 18 + OFFSETY2 - 4);
	memLv_[3]->z_SetPos(MPX + 22 + OFFSETX, MPY - 10 + OFFSETY - 2);
	memLv_[4]->z_SetPos(MPX + 48 + OFFSETX, MPY + OFFSETY2 + 4);
	memLv_[5]->z_SetPos(MPX + 48 - 4 + OFFSETX, MPY - 18 + OFFSETY2 - 4);
	memLv_[6]->z_SetPos(MPX + 48 + 16 + OFFSETX + 4, MPY - 10 + OFFSETY - 2);

    //チーム名
    ugname_.MakeGroup(field_);
    ugname_.SetPos(54, 8);

    ugtimer_.MakeGroup(field_);
    ugtimer_.SetPos(46, 74);

    ugtimer2_.MakeGroup(field_);
    ugtimer2_.SetPos(13, 8);

    ugtimer_.SetDraw(FALSE);
    ugtimer2_.SetDraw(FALSE);

    uggetstate_.MakeGroup(field_);
    uggetstate_.SetPos(54, 3);
    uggetstate_.SetDraw(FALSE);

    ugtokkou_.MakeGroup(field_);
    ugtokkou_.SetPos(1 + 12, 5 + 12);

	tokkouTxt_ = MakeObj();
	tokkouTxt_->o_SetParent(field_);
	tokkouTxt_->z_SetPos(34, 7);
	tokkouTxt_->g_SetDraw(FALSE);

	/*
	uiStrST_ = MakeObj();
	uiStrST_->o_SetParent(field_);
	uiStrST_->z_SetPos(22, 7);
	uiStrST_->f_MakeFont_Mini("ＳＴ", FC2_mini);
	*/

	uiTotalTmSt_ = MakeObj();
	uiTotalTmSt_->o_SetParent(field_);
	uiTotalTmSt_->z_SetPos(22, 16);

    ugshset_.MakeGroup(field_);
    ugshset_.SetPos(68, 67 - 4);
    //ugshset_.SetPos(120,0);

    ////チームのID
    //teamid_ = MakeObj();
    //teamid_->o_SetParent(field_);
    //teamid_->z_SetPos(24, 8);

    //スタミナフィールド
    staminafield_ = MakeObj();
    staminafield_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_bar_stamina));
    staminafield_->o_SetParent(field_);
    staminafield_->z_SetPos(8, 82);
    //staminafield_->z_SetPos(30, 80);

    //スタミナバー
    stamina_ = MakeObj();
    stamina_->o_SetObjFilterGrp(50, 3, 0xFF, 0xFF, 179, 15);//50の値が変動
    stamina_->z_SetPos(1, 1);
    stamina_->o_SetParent(staminafield_);
  }
  void TUGTeam::SetItemGet(s64 mid_team, s32 lv, BOOL leftpos_f)
  {

    s64 id_team = 0;
    for (s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_m_team) == mid_team)
      {
        id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk);
        if (lv == NGNUM) lv = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_overlap);
      }
    }

    SetTeamDt_Get(NGNUM, mid_team, lv);
	s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
	SetParam(id_kantoku, mid_team, id_team, FALSE, lv);
	SetCharaLv(id_kantoku, mid_team, id_team);

    MakeIconTexture();
    SetTipsDraw_f(TRUE);
    SetDraw(TRUE);
	SetLRPosByGet(leftpos_f);
    if (lv == 1)
    {
      uggetstate_.SetMessage_mini("ＮＥＷ！", FC2_num_red, TRUE);
    }
    else
    {
      uggetstate_.SetMessage_mini("ＬＥＶＥＬ＿ＵＰ！", FC2_mini, FALSE);
    }
    uggetstate_.SetDraw(TRUE);

    ugtokkou_.SetTokkouNow(id_team, mid_team);
	
	tokkouTxt_->g_SetDraw(FALSE);
	if (ugtokkou_.GetTokkouMag() != NGNUM)
	{
		tokkouTxt_->g_SetDraw(TRUE);
		tokkouTxt_->f_MakeFont_Mini(ugtokkou_.GetTokkouMagStr().c_str(), FC2_mini);
	}
  }

  // チーム表示(チームレベルやキャラレベル、シュート装備のパラメータも表示)
  void TUGTeam::SetTeamView(s64 id_kantoku, s64 mid_team, BOOL leftpos_f)
  {
	  s64 id_team = 0;
	  s32 overlap = 0;
	  for (s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); ++i)
	  {
		  if (pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_m_team) == mid_team)
		  {
			  id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk);
			  overlap = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_overlap);
			  break;
		  }
	  }

	  SetTeamDt2(id_kantoku, id_team, mid_team, NGNUM, TRUE, NGNUM);
	  SetParam(id_kantoku, mid_team, id_team, FALSE, overlap);
	  SetCharaLv(id_kantoku, mid_team, id_team);
	  RefreshShoot();
	  MakeIconTexture();
	  SetTipsDraw_f(TRUE);
	  SetDraw(TRUE);
	  SetLRPos(leftpos_f);
	  uggetstate_.SetDraw(FALSE);
  }

  void TUGTeam::SetItemCnt(s64 mid_team, BOOL leftpos_f)
  {
    SetTeamDt_Cnt(NGNUM, mid_team, NGNUM);
    MakeIconTexture();
    SetTipsDraw_f(TRUE);
    SetDraw(TRUE);
    SetLRPos(leftpos_f);
    uggetstate_.SetDraw(FALSE);
  }

  void TUGTeam::SetLRPos(BOOL leftpos_f)
  {
    const s32 POSLX = -160;
    const s32 POSRX = 8;
    //const s32 POSY = -64;
    const s32 POSY = -55 + 15;

    s32 posx = (leftpos_f)
      ? POSLX
      : POSRX;

    field_->z_SetPos(posx, POSY);
  }
  void TUGTeam::SetLRPosByGet(BOOL leftpos_f)
  {
	  const s32 POSLX = -160;
	  const s32 POSRX = 8;
	  //const s32 POSY = -64;
	  const s32 POSY = -60;

	  s32 posx = (leftpos_f)
		  ? POSLX
		  : POSRX;

	  field_->z_SetPos(posx, POSY);
  }
  void TUGTeam::StaminaNone(BOOL none_f)
  {
    stTeam_.nonedraw_f_ = none_f;
    RefreshStamina();
  }
  void TUGTeam::SetParam(s64 id_kantoku, s64 mid_team, s64 id_team, BOOL isLvNone, s32 tmLv)
  {
	  s64 totalSt = 0;
	  for (s32 i = 0; i < DBMEMBER_ALL; i++)
	  {
		  // 実際に計算する
		  s64 Lv = 1;
		  if (!isLvNone && id_kantoku != NGNUM)
		  {
			  Lv = pmgEO_->mgCoU_.GetTmCharaLvByKantokuID(id_kantoku, mid_team, i);
		  }
		s64 charaid = pmgEO_->mgCoM_.GetCharID(mid_team, i);
		s64 stPow = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Pow, Lv, tmLv);
		s64 stGuts = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Guts, Lv, tmLv);
		s64 stKire = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Kire, Lv, tmLv);
		s64 stTech = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Tech, Lv, tmLv);
		totalSt += (stPow + stGuts + stKire + stTech);
	  }
	  // チームの総ステータス
	  uiTotalTmSt_->f_MakeFont_Mini(lib_str::IntToStr(totalSt).c_str(), FC2_mini);
  }
  // 計算済みテーブルを使用
  void TUGTeam::SetParamTBL(s64 id_kantoku, s64 mid_team, s64 id_team, BOOL isLvNone, s32 tmLv)
  {
	  s64 totalSt = 0;
	  for (s32 i = 0; i < DBMEMBER_ALL; i++)
	  {
		  s64 stPow = player_->pmgEO_->GetSt(id_team, i, clv_Pow);
		  s64 stGuts = player_->pmgEO_->GetSt(id_team, i, clv_Guts);
		  s64 stKire = player_->pmgEO_->GetSt(id_team, i, clv_Kire);
		  s64 stTech = player_->pmgEO_->GetSt(id_team, i, clv_Tech);
		  totalSt += (stPow + stGuts + stKire + stTech);
	  }
	  // チームの総ステータス
	  uiTotalTmSt_->f_MakeFont_Mini(lib_str::IntToStr(totalSt).c_str(), FC2_mini);
  }
  void TUGTeam::SetCharaLv(s64 id_kantoku, s64 mid_team, s64 id_team)
  {
	  for (s32 i = 0; i < DBMEMBER_ALL; i++)
	  {
		  //s64 charaid = pmgEO_->mgCoM_.GetCharID(mid_team, i);
		  
		  // レベルを取得
		  s64 lv = 0;
		  if (id_kantoku != NGNUM)
		  {
			  lv = pmgEO_->mgCoU_.GetTmCharaLvByKantokuID(id_kantoku, mid_team, i);
		  }

		  // キャプテン
		  std::string numStr = "";
		  if (i == 0)
		  {
			if(lv == 0)
			{
				capLv_->g_SetDraw(FALSE);
			}
			else
			{
				numStr = "ＬＶ" + lib_str::IntToStr(lv);
				capLv_->f_MakeFont_Mini(numStr.c_str(), FC2_mini);
				capLv_->g_SetDraw(TRUE);
			}
		  }
		  // メンバー
		  else
		  {
			  if(lv == 0)
			  {
				  memLv_[i]->g_SetDraw(FALSE);
			  }
			  else
			  {
				numStr = "ＬＶ" + lib_str::IntToStr(lv);
				memLv_[i]->f_MakeFont_Mini(numStr.c_str(), FC2_mini);
				memLv_[i]->g_SetDraw(TRUE);
			  }
		  }
	  }
  }

  void TUGTeam::GenSetTeamDt(s64 id_kantoku, s64 id_team, s64 mid_team, BOOL ready_f)
  {
    if ((ready_f == FALSE) && (id_team != NGNUM) && (stTeam_.id_team_ == id_team)) return;
	stTeam_.id_kantoku_ = id_kantoku;
    stTeam_.id_team_ = id_team;
    stTeam_.mid_team_ = mid_team;

    //チームID
    //teamid_->f_MakeFont_Mini(lib_str::IntToStr(mid_team).c_str(), FC2_mini);

    s32 rarity = player_->pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);

    //カードカラー
    field_->g_SetPartNo(rarity);

    //シュートセット
    //ugshset_.Refresh(mid_team, id_team, rarity);
  }

  void TUGTeam::SetTeamByCOM(s64 id_kantoku, s64 id_team, s64 mid_team, s64 mid_event, s32 tmLv)
  {
	  GenSetTeamDt(id_kantoku, id_team, mid_team, TRUE);
	  //SetCharaLv(mid_team, id_team);
	  //SetParam(id_team, id_team, FALSE);

	  ugname_.SetNameTeam_Get(mid_team, tmLv);
	  ugname_.SetIcon(FALSE);
	  ugtokkou_.SetTokkou(id_team, mid_team, mid_event);

	  tokkouTxt_->g_SetDraw(FALSE);
	  if (ugtokkou_.GetTokkouMag() != NGNUM)
	  {
		  tokkouTxt_->g_SetDraw(TRUE);
		  tokkouTxt_->f_MakeFont_Mini(ugtokkou_.GetTokkouMagStr().c_str(), FC2_mini);
	  }
	  SetParam(id_kantoku, mid_team, id_team, FALSE, tmLv);
  }

  void TUGTeam::SetTeamDt3(s64 id_kantoku, s64 id_team, s64 mid_team, s64 mid_event, BOOL ready_f)
  {
	SetTeamDt2(id_kantoku, id_team, mid_team, mid_event, ready_f, NGNUM);
	SetParam(id_kantoku, mid_team, id_team, FALSE, 1);
  }
  void TUGTeam::SetTeamDt2(s64 id_kantoku, s64 id_team, s64 mid_team, s64 mid_event, BOOL ready_f, s32 id_team_end)
  {
    GenSetTeamDt(id_kantoku, id_team, mid_team, ready_f);
	//SetCharaLv(mid_team, id_team);
	//SetParam(id_team, id_team, FALSE);

	ugname_.SetNameTeam(mid_team);
    ugname_.SetIcon(FALSE);
    ugtokkou_.SetTokkou(id_team, mid_team, mid_event);

	tokkouTxt_->g_SetDraw(FALSE);
	if (ugtokkou_.GetTokkouMag() != NGNUM)
	{
		tokkouTxt_->g_SetDraw(TRUE);
		tokkouTxt_->f_MakeFont_Mini(ugtokkou_.GetTokkouMagStr().c_str(), FC2_mini);
	}

    if (id_team_end > 0)
    {
      s64 mid_team_end = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team_end, mdteam_id_m_team);
      SetDrEnd((mid_team == mid_team_end));
    }
  }
  void TUGTeam::SetTeamDt_Get(s64 id_team, s64 mid_team, s32 lv)
  {
	s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
    GenSetTeamDt(id_kantoku, id_team, mid_team, TRUE);
    ugname_.SetNameTeam_Get(mid_team, lv);
    ugname_.SetIcon(FALSE);
  }
  void TUGTeam::SetTeamDt_Cnt(s64 id_team, s32 mid_team, s32 mid_team_end)
  {
	s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
    GenSetTeamDt(id_kantoku, id_team, mid_team, TRUE);
	SetParam(id_kantoku, mid_team, id_team, TRUE, NGNUM);

    ugname_.SetNameTeam_Cnt(mid_team);
    ugname_.SetIcon(FALSE);
    SetDrEnd((mid_team == mid_team_end));
  }

  //テクスチャ読み込み
  void TUGTeam::MakeIconTexture()
  {
    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(stTeam_.mid_team_, mdm_team_gb_f);
    for (s32 c = 0; c < DBMEMBER_ALL; ++c)
    {
      s32 charid = pmgEO_->mgCoM_.GetCharID(stTeam_.mid_team_, c);
      pmgEO_->mgGrp_.pmgChGrp_[stTeam_.sideNo_][c]->SetCharDt(charid, stTeam_.mid_team_, gb_f);
    }
    pmgEO_->mgGrp_.MakeIconTexture_Chest(stTeam_.sideNo_, FALSE, FALSE);
    pmgEO_->mgGrp_.MakeIconTexture_Body(stTeam_.sideNo_, 0, 0, FALSE, FALSE);
  }
  void TUGTeam::SetDrEnd(BOOL end_f)
  {
    if (end_f)
    {
      cap_->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconBodySub_[stTeam_.sideNo_]);
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
      {
        mem_[i]->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconChestSub_[stTeam_.sideNo_]);
      }
    }
    else
    {
      cap_->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconBody_[stTeam_.sideNo_]);
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
      {
        mem_[i]->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconChest_[stTeam_.sideNo_]);
      }
    }
    mem_[0]->g_SetDraw(FALSE);
	memLv_[0]->g_SetDraw(FALSE);
  }

  void TUGTeam::SetTipsDraw_f(BOOL tipsdraw_f)
  {
    stTeam_.tipsdraw_f_ = tipsdraw_f;

    if (stTeam_.tipsdraw_f_)
    {
      //スタミナリフレッシュ
      RefreshStamina();
    }
  }

  void TUGTeam::RefreshStamina()
  {
    if (stTeam_.nonedraw_f_)
    {
      const s32 BARW = 50;
      s32 fw = BARW;//満タン
      ugtimer_.SetDraw(FALSE);
      ugtimer2_.SetDraw(FALSE);
      stamina_->o_SetObjFilterGrp(fw, 3, 0xFF, 0xFF, 179, 15);
      std::string hp = "ＲＥＡＤＹ！";
      staminafield_->f_MakeFont_Mini(hp.c_str(), FC2_hd_normal);
      staminafield_->f_FontPos(0, -8);
      ugshset_.SetDraw(FALSE);
      return;
    }

    const s32 MINTOSEC = 60;
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(stTeam_.mid_team_, mdm_team_rarity);
    s32 waittime = pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt(rarity, mdm_rarity_staminaLim);

    //シュートセット
	//ugshset_.Refresh(stTeam_.mid_team_, stTeam_.id_team_, rarity);

    s64 nowwait = waittime;
    if (stTeam_.id_team_ != NGNUM)//敵以外
    {
      s64 nowtime = mid::midNowTime();
      s64 endtime = player_->pmgEO_->mgCoU_.mdTeam_.GetPKDt(stTeam_.id_team_, mdteam_useSec);
      nowwait = (nowtime - endtime) / MINTOSEC;
      if (nowwait > waittime) nowwait = waittime;
    }

    const s32 BARW = 50;
    s32 fw = BARW;//満タン
    BOOL wait_f = FALSE;
    if (nowwait != waittime)
    {
      wait_f = TRUE;
      fw = (nowwait * BARW) / waittime;
      ugtimer_.SetTimer(waittime - nowwait);
    }
    ugtimer_.SetDraw(wait_f);
    ugtimer2_.SetDraw(wait_f);
    stamina_->o_SetObjFilterGrp(fw, 3, 0xFF, 0xFF, 179, 15);

    std::string hp = (wait_f)
      ? "ＯＹＡＳＵＭＩ"
      : "ＲＥＡＤＹ！";
    staminafield_->f_MakeFont_Mini(hp.c_str(), FC2_hd_normal);
    staminafield_->f_FontPos(0, -8);

  }

  void TUGTeam::RefreshShoot()
  {
	s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(stTeam_.mid_team_, mdm_team_rarity);
	//シュートセット
	ugshset_.Refresh(stTeam_.mid_team_, stTeam_.id_team_, rarity);
  }

  void TUGTeam::RefreshParam(void)
  {
	  // 同じ
	  //if (stTeam_.mid_team_ == mid_prev_team_){ return; }

	  SetParam(stTeam_.id_kantoku_, stTeam_.mid_team_, stTeam_.id_team_, FALSE, NGNUM);
	  mid_prev_team_ = stTeam_.mid_team_;
  }

  void TUGTeam::RefreshCharaLv(void)
  {
	  SetCharaLv(stTeam_.id_kantoku_, stTeam_.mid_team_, stTeam_.id_team_);
  }

  void TUGTeam::SetDrawParam(BOOL isDraw)
  {
	  uiTotalTmSt_->g_SetDraw(isDraw);

  }

  void TUGTeam::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugname_.Update(draw_f);
    ugtimer_.Update(draw_f);
    ugtimer2_.Update(draw_f);
    uggetstate_.Update(draw_f);
    ugtokkou_.Update(draw_f);
	tokkouTxt_->Update(draw_f);
	//uiStrST_->Update(draw_f);
	uiTotalTmSt_->Update(draw_f);
    ugshset_.Update(draw_f);

    if (stTeam_.tipsdraw_f_)
    {
      cap_->Update(draw_f);
	  capLv_->Update(draw_f);
	  for (s32 i = 0; i < DBMEMBER_ALL; ++i)
	  {
		  mem_[i]->Update(draw_f);
		  memLv_[i]->Update(draw_f);
	  }

      //if (stTeam_.nonedraw_f_ == FALSE)
      {
        stamina_->Update(draw_f);
        staminafield_->Update(draw_f);
      }

    }

  }

}
