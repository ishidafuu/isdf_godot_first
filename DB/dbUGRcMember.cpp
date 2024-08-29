#include "dbUGRcMember.h"
#include "dbUILayer.h"

namespace db
{
  const s32 RECORDH = 48;

  TUGRcMember::TUGRcMember(TUIGenLayer* player) : TUGRcGen(player)
  {
    
  }
  TUGRcMember::~TUGRcMember(void)
  {
  
  }
  s32 TUGRcMember::DoMakeGroupSub()
  {
    //TUGRcGen::MakeGroupから呼ばれる
    basegrp_ = pmgUI_->GetUGGrp(ug_list_recordmem);
    basegrp2_ = pmgUI_->GetUGGrp(ug_list_recordmem2);
    field_->o_SetObjGrp(basegrp_);

    s32 bx = 36;
    s32 by = 4;
    s32 dx = 56;
    s32 dy = 12;

    s32 sideno = 0;
    body_ = MakeObj();
    body_->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconBody_[sideno]);
    //body_->z_SetPos(13, 21);
    body_->z_SetPos(0, 0);

    name_ = MakeObj();
    name_->z_SetPos(bx, by);
	lv_ = MakeObj();
	lv_->z_SetPos(bx + dx, by);
    //hp_ = MakeObj();
    //hp_->z_SetPos(bx + dx, by);
    pow_ = MakeObj();
    pow_->z_SetPos(bx, by + dy);
    guts_ = MakeObj();
    guts_->z_SetPos(bx + dx, by + dy);
    spd_ = MakeObj();
    spd_->z_SetPos(bx, by + dy + dy);
    tec_ = MakeObj();
    tec_->z_SetPos(bx + dx, by + dy + dy);

    //items登録
    items_.push_back(body_);
    items_.push_back(name_);
	items_.push_back(lv_);
    //items_.push_back(hp_);
    items_.push_back(pow_);
    items_.push_back(guts_);
    items_.push_back(spd_);
    items_.push_back(tec_);

    for (s32 i = 0; i < items_.size(); ++i) items_[i]->o_SetParent(field_);

    //レコード高さを返す
    return RECORDH;
  }

  //チームマスターから情報を引いてくる
  void TUGRcMember::SetDt(s32 mid_team, s32 posno)
  {
    //選手の画像はリストを呼び出す段階で作っておく
    //s32 rarity = player_->pmgEO_->mgDt_.dtTeam_.GetDt(teamid, dbtm_Rarity);
	  s32 memid = pmgEO_->mgCoM_.GetCharID(mid_team, posno);
    //name_->f_MakeFont(player_->pmgEO_->mgDt_.dtTeam_.GetTeamName(teamid).c_str(), (FC2_rareD + rarity));
    //選手名
	std::string strName = player_->pmgEO_->mgDt_.dtChar_.GetName(memid);
	name_->f_MakeFont(strName.c_str(), FC2_normal);


	// チームのマスターIDから手持ちのチームのIDを検索(ついでに限界突破値も取得)
	s64 tmLv = NGNUM;
	s64 id_team = 0;
	for (s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); ++i)
	{
		if (pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_m_team) == mid_team)
		{
			id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk);
			tmLv = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_overlap);
			break;
		}
	}

	// 実際に計算する
	s64 Lv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team, posno);

	std::string stlv = "ＬＶ" + lib_str::IntToStr(Lv);
	lv_->f_MakeFont_Mini(stlv.c_str(), FC2_mini);
	f32 bx = 0;
	f32 by = 0;
	name_->z_GetPos(&bx, &by);

	//GRIDSIZEは文字サイズ（８どっと）が入ってますね/3は１文字３ばいとなのと
	//+2は文字サイズは影やふちがあってすこしひろいので
	const s32 BYTE = 3;
	lv_->z_SetPos(bx + (strName.size() * GRIDSIZE / BYTE) + 2, by + 4);
	lv_->g_SetDraw(TRUE);

	s64 stPow = player_->pmgEO_->CalcSt(mid_team, id_team, memid, clv_Pow, Lv, tmLv);
	s64 stGuts = player_->pmgEO_->CalcSt(mid_team, id_team, memid, clv_Guts, Lv, tmLv);
	s64 stKire = player_->pmgEO_->CalcSt(mid_team, id_team, memid, clv_Kire, Lv, tmLv);
	s64 stTech = player_->pmgEO_->CalcSt(mid_team, id_team, memid, clv_Tech, Lv, tmLv);
	std::string stpow = "ぱわー" + lib_str::IntToStr(stPow);
	std::string stguts = "きあい" + lib_str::IntToStr(stGuts);
	std::string stspd = "すぴん" + lib_str::IntToStr(stKire);
	std::string sttec = "まもり" + lib_str::IntToStr(stTech);

    
    //std::string sthp = "ＨＰ" + lib_str::IntToStr(player_->pmgEO_->mgDt_.dtChar_.GetLv(clv_Pow, memid));
    //std::string stpow = "ぱわー" + lib_str::IntToStr(player_->pmgEO_->mgDt_.dtChar_.GetLv(clv_Pow, memid));
    //std::string stguts = "きあい" + lib_str::IntToStr(player_->pmgEO_->mgDt_.dtChar_.GetLv(clv_Guts, memid));
    //std::string stspd = "すぴん" + lib_str::IntToStr(player_->pmgEO_->mgDt_.dtChar_.GetLv(clv_Kire, memid));
    //std::string sttec = "まもり" + lib_str::IntToStr(player_->pmgEO_->mgDt_.dtChar_.GetLv(clv_Tech, memid));
    //hp_->f_MakeFont(sthp.c_str(), FC2_normal);
    pow_->f_MakeFont(stpow.c_str(), FC2_normal);
    guts_->f_MakeFont(stguts.c_str(), FC2_normal);
    spd_->f_MakeFont(stspd.c_str(), FC2_normal);
    tec_->f_MakeFont(sttec.c_str(), FC2_normal);

    body_->g_SetPartNo(posno);

    //if (btn_f == FALSE) field_->b_SetBtn(btn_f);
    
  }
}
