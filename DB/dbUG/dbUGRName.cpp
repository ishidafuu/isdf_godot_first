#include "dbUGRName.h"
#include "dbUILayer.h"

namespace db
{
  const s32 ICONX = -12;
  const s32 ICONY = 4;
  const s32 ICONXVAL = -8;
  const s32 ICONYVAL = 7;
  const s32 ICONXFACE = -12;
  const s32 ICONYFACE = 3;
  const s32 RARITYX_TEAM = 66 - 6;
  const s32 RARITYX_SHOT = RARITYX_TEAM + 16;
  const s32 RARITYY = - 6;
  //コンストラクタ
  TUGRName::TUGRName(TUIGenLayer* player) : TUGGen(player)//アクションなし
    , ugicon_(player)
    , ugrarity_(player)
  {
    stRName_.Init();
	nameStr_.clear();
  }

  TUGRName::~TUGRName()
  {

  }
  void TUGRName::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    //チームレアリティ
    ugrarity_.MakeGroup(field_);
    ugrarity_.SetDraw(FALSE);

    ugicon_.MakeGroup(field_);
    ugicon_.SetPos(ICONX, ICONY);
    ugicon_.SetDraw(FALSE);//デフォはアイコン無し

  }
  void TUGRName::SetRarityPosShot(s32 fontsize, s32 rarity)
  {
	//GRIDSIZEは文字サイズ（８どっと）が入ってますね/3は１文字３ばいとなのと
	//+2は文字サイズは影やふちがあってすこしひろいので

    //s32 www = (fontsize* GRIDSIZE / 3) + 2;// +(GRIDSIZE * 2);
    s32 posx = (stRName_.label_f_)
      ? (fontsize* GRIDSIZE / 3) + 2 -(GRIDSIZE)
      : RARITYX_SHOT;
    ugrarity_.SetPos(posx, RARITYY);
    ugrarity_.Refresh(rarity);

    stRName_.rare_f_ = TRUE;
  }
  void TUGRName::SetRarityPosTeam(s32 fontsize, s32 rarity, s32 lv)
  {
    //s32 www = (fontsize* GRIDSIZE / 3) + 2;// +(GRIDSIZE * 2);
    s32 posx = (stRName_.label_f_)
      ? (fontsize* GRIDSIZE / 3) + 2 - (GRIDSIZE)
      : RARITYX_TEAM;
    ugrarity_.SetPos(posx, RARITYY);
    ugrarity_.Refresh(rarity, lv, FALSE);

    stRName_.rare_f_ = TRUE;
  }
  void TUGRName::SetIcon(BOOL icon_f)
  {
    ugicon_.SetDraw(icon_f);
  }

  void TUGRName::SetNameTeam(s64 mid_team)
  {
	  SetNameTeam(mid_team, pmgEO_->mgCoU_.GetTeamLv(mid_team));
  }
  void TUGRName::SetNameTeam(s64 mid_team, s32 lv)
  {
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    std::string name = pmgEO_->mgCoM_.mdm_Team_.GetLineDtStr(mid_team, mdm_team_name_c32);
    field_->f_MakeFont(name.c_str(), (FC2_rareD + rarity));
    SetRarityPosTeam(name.size(), rarity, lv);
    ugicon_.SetIconTeam(mid_team);

    stRName_.namesize_ = name.size();
  }

  void TUGRName::SetNameTeam_Get(s64 mid_team, s32 lv)
  {
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    std::string name = pmgEO_->mgCoM_.mdm_Team_.GetLineDtStr(mid_team, mdm_team_name_c32);
    field_->f_MakeFont(name.c_str(), (FC2_rareD + rarity));
    SetRarityPosTeam(name.size(), rarity, lv);
    ugicon_.SetIconTeam(mid_team);

    stRName_.namesize_ = name.size();
  }
  void TUGRName::SetNameTeam_Cnt(s64 mid_team)
  {
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    std::string name = pmgEO_->mgCoM_.mdm_Team_.GetLineDtStr(mid_team, mdm_team_name_c32);
    field_->f_MakeFont(name.c_str(), (FC2_rareD + rarity));
    SetRarityPosTeam(name.size(), rarity, 0);
    ugicon_.SetIconTeam(mid_team);

    stRName_.namesize_ = name.size();
  }

  void TUGRName::SetNameSozai(s64 mid_sozai, s32 rarity, BOOL shop_f)
  {
	  std::string name;
	  if (mid_sozai >= 0)
	  {
		  name = pmgEO_->mgDt_.dtShElm_.GetDtStr(mid_sozai, szs_name);
	  }
	  else
	  {
		  name = "？？？？？？？？";
	  }
    field_->f_MakeFont(name.c_str(), (FC2_rareD + rarity));
    if (shop_f == FALSE)
      SetRarityPosShot(name.size(), rarity);
    ugicon_.SetIconSozai(rarity, shop_f);
    stRName_.namesize_ = name.size();
  }
  void TUGRName::SetNameSozai2(s64 mid_sozairarity, BOOL shop_f)
  {
    SetNameSozai((mid_sozairarity / SOZAIMID), (mid_sozairarity % SOZAIMID), shop_f);
  }
  void TUGRName::SetNameSozaiSub(s64 mid_sozai, s32 rarity)
  {
	  std::string name;
	  if (mid_sozai >= 0)
	  {
		  name = pmgEO_->mgDt_.dtShElm_.GetDtStr(mid_sozai, szs_name2);
	  }
	  else
	  {
		  name = "負負負負負負負負";
	  }
    field_->f_MakeFont(name.c_str(), (FC2_rareD + rarity));
    stRName_.namesize_ = name.size();
  }


  void TUGRName::SetNameShot(s64 id_shot, BOOL shop_f)
  {
    if (id_shot != NGID)
    {
      TSozaiData sozaidt = pmgEO_->mgCoU_.GetSozaiData(id_shot, 0);
      std::string name = pmgEO_->mgCoU_.GetShotName(id_shot);
      //std::string name = pmgEO_->mgDt_.dtShElm_.GetName(sozaidt.mid_sozai_);
      field_->f_MakeFont(name.c_str(), (FC2_rareD + sozaidt.rarity_));
      if (shop_f == FALSE)  SetRarityPosShot(name.size(), sozaidt.rarity_);
      ugicon_.SetIconSozai(sozaidt.rarity_, shop_f);
      stRName_.namesize_ = name.size();
    }
    else
    {
      SetDraw(FALSE);
      ugrarity_.SetDraw(FALSE);
      stRName_.namesize_ = 0;
    }
  }
  void TUGRName::SetNameShotSub(s64 id_shot)
  {
    if (id_shot != NGID)
    {
      TSozaiData sozaidt = pmgEO_->mgCoU_.GetSozaiData(id_shot, 0);
      SetNameSozaiSub(sozaidt.mid_sozai_, sozaidt.rarity_);
    }
    else
    {
      SetDraw(FALSE);
      stRName_.namesize_ = 0;
    }
  }
  void TUGRName::SetNameShotSub(TSozaiData dt)
  {
	  SetNameSozaiSub(dt.mid_sozai_, dt.rarity_);
  }
  void TUGRName::SetNameDefShot(s64 defshot)
  {
    //固定ショット
    std::string name = pmgEO_->mgDt_.dtShSyn_.GetName(defshot);
    s32 rarity = pmgEO_->mgDt_.dtShSyn_.GetDt(defshot, gs_base_rare);
    field_->f_MakeFont(name.c_str(), (FC2_rareD + rarity));
    SetRarityPosShot(name.size(), rarity);
    ugicon_.SetIconShot(rarity);
    stRName_.namesize_ = name.size();
  }
  void TUGRName::SetNameAlbum(s64 mid_title)
  {
    std::string name = pmgEO_->mgDt_.dtTitle_.GetStrPK(mid_title, enDtTitleStr_shortname);
    SetName(name);
    ugicon_.SetIconAlbum(mid_title);
  }
  void TUGRName::SetNameTape(s64 mid_bgm, BOOL home_f)
  {
    std::string name = pmgEO_->mgDt_.dtBGM_.GetStrPK(mid_bgm, enDtBGMStr_name);
    SetName(name);
    ugicon_.SetIconTape(mid_bgm, home_f);
  }
  void TUGRName::SetNameTapeTitle(s64 mid_title)
  {
    std::string name = pmgEO_->mgDt_.dtTitle_.GetStrPK(mid_title, enDtTitleStr_shortname);
    SetName(name);
    ugicon_.SetIconTapeTitle(mid_title);
  }
  void TUGRName::SetNameHaikeiTitle(s64 mid_title)
  {
    std::string name = pmgEO_->mgDt_.dtTitle_.GetStrPK(mid_title, enDtTitleStr_shortname);
    SetName(name);
    ugicon_.SetIconHaikeiTitle(mid_title);
  }
  void TUGRName::SetNameHaikei(s64 mid_haikei)
  {
    std::string name = pmgEO_->mgDt_.dtCourt_.GetStrPK(mid_haikei, enDtCourtStr_name);
    SetName(name);
    ugicon_.SetIconHaikei(mid_haikei);
  }
  void TUGRName::SetNameHaikeiPicture(s64 mid_haikei)
  {
    std::string name = pmgEO_->mgDt_.dtCourt_.GetStrPK(mid_haikei, enDtCourtStr_name);
    const s32 MAXLEN = 18;
    field_->f_MakeFont_Len(name.c_str(), FC2_mini, MAXLEN);
    ugrarity_.SetDraw(FALSE);
    stRName_.namesize_ = name.size();
  }
  void TUGRName::SetNameSupply(s64 mid_supply, s32 num)
  {
    std::string name = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_name_c32);
    if (num >= 2) name += "＿" + lib_str::IntToStr(num) + "こ";
    SetName(name);
    ugicon_.SetIconSupply(mid_supply);
  }
  void TUGRName::SetNameItemPack(s64 mid_ItemPack, s32 num)
  {
    std::string name = pmgEO_->mgCoM_.mdm_ItemPack_.GetPKDtStr(mid_ItemPack, mdm_itempack_name_c64);
    if (num >= 2) name += "＿" + lib_str::IntToStr(num) + "こ";
    SetName(name);
    ugicon_.SetIconItemPack(mid_ItemPack);
  }
  void TUGRName::SetNameTodd(s64 num)
  {
    std::string name = lib_str::IntToStr(num);
    name += "トッド";
    SetName(name);
    ugicon_.SetIcon(ugi_Todd);
  }
  void TUGRName::SetNameStar(s64 num)
  {
    std::string name = lib_str::IntToStr(num);
    name += "スター";
    SetName(name);
    ugicon_.SetIcon(ugi_Star);
  }
  void TUGRName::SetNameHukuKen(s64 num)
  {
    std::string name = "ふくびきけん＿";
    name += lib_str::IntToStr(num);
    name += "まい";
    SetName(name);
    ugicon_.SetIcon(ugi_Huku);
  }
  void TUGRName::SetNameGacha(s64 mid_gacha)
  {
    s32 iconno = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_iconNo);
    std::string name = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDtStr(mid_gacha, mdm_gacha_name_c64);
    SetName(name);
    ugicon_.SetIconGacha(mid_gacha);
  }
  void TUGRName::SetNameNaBox(s64 mid_nabox)
  {
    s32 iconno = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_iconNo);
    std::string name = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(mid_nabox, mdm_nabox_name_c64);
    SetName(name);
    ugicon_.SetIconNaBox(mid_nabox);
  }
  void TUGRName::SetNameHuku(s64 mid_huku)
  {
    s32 iconno = pmgEO_->mgCoM_.mdm_Huku_.GetPKDt(mid_huku, mdm_huku_iconNo);
    std::string name = pmgEO_->mgCoM_.mdm_Huku_.GetPKDtStr(mid_huku, mdm_huku_name_c64);
    SetName(name);
    ugicon_.SetIconHuku(mid_huku);
  }
  void TUGRName::SetNameSpSyouhin(s64 mid_spsyouhin)
  {
    s64 itemtype = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemType);
    s64 itemid = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemID);
    s64 itemnum = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemNum);
    SetName(itemtype, itemid, itemnum, TRUE);
    ugicon_.SetIcon(itemtype, itemid, TRUE);
  }
  void TUGRName::SetNameGiftBox(s64 id_giftbox)
  {
    s64 itemtype = pmgEO_->mgCoU_.mdGiftbox_.GetPKDt(id_giftbox, mdgiftbox_itemType);
    s64 itemid = pmgEO_->mgCoU_.mdGiftbox_.GetPKDt(id_giftbox, mdgiftbox_itemID);
    s64 itemnum = pmgEO_->mgCoU_.mdGiftbox_.GetPKDt(id_giftbox, mdgiftbox_itemNum);
    SetName(itemtype, itemid, itemnum);
    ugicon_.SetIcon(itemtype, itemid, FALSE);
  }
  void TUGRName::SetNameCol(s32 hue)
  {
    SetName("いろばんごう" + lib_str::IntToStr(hue, 2));
    //    ugicon_.SetIconCol(hue, 1);
    ugicon_.SetIconChest((hue / DBMEMBER_ALL), (hue % DBMEMBER_ALL));
  }
  void TUGRName::SetNameCol2(s32 hue, s32 lum)
  {
    std::string lumstr = "";
    switch (lum)
    {
    case 0:lumstr = "あかるい"; break;
    case 1:lumstr = "ふつう"; break;
    case 2:lumstr = "くらい"; break;
    case 3:lumstr = "ちょくら"; break;
    }
    SetName("いろ" + lib_str::IntToStr(hue, 2) + lumstr);
    //    ugicon_.SetIconCol(hue, lum);
    ugicon_.SetIconChest((lum / DBMEMBER_ALL), (lum % DBMEMBER_ALL));
  }
  void TUGRName::SetNameSkinCol(s32 hue)
  {
    SetName("はだばんごう" + lib_str::IntToStr(hue, 2));
    //    ugicon_.SetIconSkin(hue, 1);
    ugicon_.SetIconChest((hue / DBMEMBER_ALL), (hue % DBMEMBER_ALL));
  }
  void TUGRName::SetNameSkinCol2(s32 hue, s32 lum)
  {
    std::string lumstr = "の" + lib_str::IntToStr(lum + 1);
    SetName("はだ" + lib_str::IntToStr(hue, 2) + lumstr);
    ugicon_.SetIconChest((lum / DBMEMBER_ALL), (lum % DBMEMBER_ALL));
  }
  void TUGRName::SetNameFace(s32 charno, s32 grpno)
  {
    SetName(pmgEO_->mgDt_.dtChar_.GetName(charno) + "ふう");
    ugicon_.SetIconChest((grpno / DBMEMBER_ALL), (grpno % DBMEMBER_ALL));
    ugicon_.SetPos(ICONXFACE, ICONYFACE);
  }

  void TUGRName::SetNameRanking(s64 mid_ranking)
  {
    std::string name = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDtStr(mid_ranking, mdm_ranking_name_c64);
    SetName(name);
    ugicon_.SetIcon(ugi_Ranking);
    //s32 iconno = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDt(mid_ranking, mdm_ranking_iconNo);
    //ugicon_.SetIconEvent(iconno);
  }
  void TUGRName::SetNameRankingItem(s64 mid_ranking)
  {
    //std::string name = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDtStr(mid_ranking, mdm_ranking_name_c64);
    std::string strscore = "かくとくすう";
    SetName(strscore);
    //ugicon_.SetIcon(ugi_Ranking);
    //s32 iconno = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDt(mid_ranking, mdm_ranking_iconNo);
    s32 mid_supply = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDt(mid_ranking, mdm_ranking_id_m_supply);
    ugicon_.SetIconSupply(mid_supply);
  }

  void TUGRName::SetNameRanker(s64 id_ranker, s32 lineno)
  {
    std::string name = pmgEO_->mgCoU_.mdre_Ranker_.GetPKDtStr(id_ranker, mdre_ranker_name_c32);
    SetName(name);

    ugicon_.SetIconChest((lineno / DBMEMBER_ALL), (lineno % DBMEMBER_ALL));
    ugicon_.SetPos(ICONXFACE, ICONYFACE);
  }
  void TUGRName::SetNameRankerByData(s64 id_ranker)
  {
	  std::string name = pmgEO_->mgCoU_.rankerData_.GetPKDtStr(id_ranker, mdre_ranker_name_c32);
	  SetName(name);

	  s32 no = pmgEO_->mgCoU_.rankerData_.GetLineNo(id_ranker, enmdre_Ranker::mdre_ranker_pk);	  
	  ugicon_.SetIconRankerChest(no);
	  ugicon_.SetPos(ICONXFACE, ICONYFACE);
  }
  void TUGRName::SetNameTousen(s32 lineno)
  {
    std::string name = pmgEO_->mgCoU_.mdre_Enemy_.GetLineDtStr(lineno, mdre_enemy_name_c32);
    SetName(name);

    ugicon_.SetIconChest((lineno / DBMEMBER_ALL), (lineno % DBMEMBER_ALL));
    ugicon_.SetPos(ICONXFACE, ICONYFACE);
  }

  void TUGRName::SetName(std::string name)
  {
    SetName(name, FALSE, FC2_normal);
  }
  void TUGRName::SetName(std::string name, s32 maxlen)
  {
	  SetName(name, FALSE, FC2_normal, maxlen);
  }
  void TUGRName::SetName(std::string name, BOOL nolim_f, s32 col, s32 maxlen)
  {
    if (nolim_f)
    {
      field_->f_MakeFont(name.c_str(), col);
    }
    else
    {
      //const s32 MAXLEN = 13;
		field_->f_MakeFont_Len(name.c_str(), col, maxlen);
    }
    ugrarity_.SetDraw(FALSE);
    stRName_.namesize_ = name.size();
	nameStr_ = name;
  }
  void TUGRName::SetName(s64 itemtype, s64 itemid, s64 itemnum)
  {
    SetName(itemtype, itemid, itemnum, FALSE);
  }
  void TUGRName::SetName(s64 itemtype, s64 itemid, s64 itemnum, BOOL shop_f)
  {
    switch (itemtype)
    {
    case enItemType_Team: SetNameTeam_Cnt(itemid); break;
    case enItemType_Sozai:SetNameSozai2(itemid, shop_f); break;
    case enItemType_Shot: break;
    case enItemType_BGM:SetNameTape(itemid, FALSE); break;
    case enItemType_Haikei:SetNameHaikei(itemid); break;
    case enItemType_Supply:SetNameSupply(itemid, itemnum); break;
    case enItemType_Todd:SetNameTodd(itemnum); break;
    case enItemType_Star:SetNameStar(itemnum); break;
    case enItemType_Pack:SetNameItemPack(itemid, itemnum); break;
    case enItemType_Huku:SetNameHukuKen(itemnum); break;
    }
    ugicon_.SetIcon(itemtype, itemid, shop_f);
  }
  void TUGRName::SetNameHukuCnt(s32 mid_Hukucnt)
  {
    s64 itemtype = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_itemType);
    s64 itemid = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_itemID);
    s64 itemnum = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_itemNum);
    s32 grade = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_grade);
    s32 num = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_num);
    s32 restnum = pmgEO_->mgCoU_.mdw_HukuCnt_.GetPKDt(mid_Hukucnt, mdw_hukucnt_restNum);

    SetName(itemtype, itemid, itemnum, FALSE);
    ugicon_.SetIconCupHuku(grade);
  }
  void TUGRName::SetNameIcon(std::string name, enUGPIcon iconno, s32 maxlen)
  {
	SetName(name, maxlen);
    ugicon_.SetIcon(iconno);
  }
  void TUGRName::GenSetValue(s64 num)
  {
	std::string numstr;
	if (num == NGNUM)
	{
		numstr = "もってない";
	}
	else
	{
		numstr = lib_str::IntToStr(num);
	}
    SetName(numstr);
    s32 posx = ICONXVAL - (numstr.size() * GRIDSIZE / 3);
    ugicon_.SetPos(posx, ICONYVAL);
  }
  void TUGRName::SetValueTodd(s64 num)
  {
    GenSetValue(num);
    ugicon_.SetIcon(ugi_Todd);
  }
  void TUGRName::SetValueStar(s64 num)
  {
    GenSetValue(num);
    ugicon_.SetIcon(ugi_Star);
  }
  void TUGRName::SetValueHukuKen(s64 num)
  {
    GenSetValue(num);
    ugicon_.SetIcon(ugi_HukuMini);
  }
  void TUGRName::SetValueGachaKen(s64 num)
  {
    GenSetValue(num);
    ugicon_.SetIcon(ugi_GachaKen);
  }
  void TUGRName::SetValueCoachKen(s64 mid_supply, s64 num)
  {
	  GenSetValue(num);
	  ugicon_.SetIconSupply(mid_supply);
  }
  void TUGRName::SetUseStar(s64 num)
  {
    std::string numstr = "＿"+ lib_str::IntToStr(num) + "スター＿しようします";
    SetName(numstr, TRUE, FC2_normal);
    field_->f_SetCenter(TRUE);
    s32 posx =  -((((s32)numstr.size() * GRIDSIZE / 3) + 2) / 2);
    ugicon_.SetPos(posx, 0);
    ugicon_.SetIcon(ugi_Star);
  }

  void TUGRName::SetUseTodd(s64 num, BOOL get_f)
  {
    std::string numstr = "＿" + lib_str::IntToStr(num) + "トッド＿";
    if (get_f)
    {
      numstr += "かくとくします";
    }
    else
    {
      numstr += "しようします";
    }
    SetName(numstr, TRUE, FC2_normal);
    field_->f_SetCenter(TRUE);
    s32 posx = -((((s32)numstr.size() * GRIDSIZE / 3) + 2) / 2);
    ugicon_.SetPos(posx, 0);
    ugicon_.SetIcon(ugi_Todd);
  }

  void TUGRName::SetPage(s32 nowpage, s32 maxpage)
  {
    std::string pagestr = "ページ" + lib_str::IntToStr(nowpage + 1) + "／" + lib_str::IntToStr(maxpage + 1);
    SetName(pagestr);
    //s32 posx = ICONXVAL - (numstr.size() * GRIDSIZE / 3);
    //ugicon_.SetPos(posx, ICONYVAL);
  }
  void TUGRName::SetRightShift()
  {
    field_->f_SetRightShift();
  }

  void TUGRName::SetLimY(s32 limu, s32 hhh)
  {
    field_->g_SetLimY(limu, hhh);
    ugrarity_.SetLimY(limu, hhh);
    //rare_->g_SetLimY(limu, hhh);
    //lv_->g_SetLimY(limu, hhh);
    //lvPlus_->g_SetLimY(limu, hhh);
    ugicon_.SetLimY(limu, hhh);
  }
  void TUGRName::SetRecordDefPos()
  {
    field_->z_SetPos(43, 6);
  }
  s32 TUGRName::GetNameLen()
  {
    if (IsRare())
    {
      return stRName_.namesize_ + 3;
    }
    else
    {
      return stRName_.namesize_;
    }

  }
  std::string TUGRName::GetName(void)
  {
	  return nameStr_;
  }
  BOOL TUGRName::IsRare()
  {
    return stRName_.rare_f_;
  }

  void TUGRName::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    //rare_->Update(draw_f);
    //lv_->Update(draw_f);
    //lvPlus_->Update(draw_f);
    ugicon_.Update(draw_f);
    ugrarity_.Update(draw_f);
  }

  void TUGRName::SetLabel(BOOL label_f)
  {
    stRName_.label_f_ = label_f;
  }



}
