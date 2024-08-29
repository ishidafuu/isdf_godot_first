#include "dbUGRIcon.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGRIcon::TUGRIcon(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stRIcon_.Init();

  }

  TUGRIcon::~TUGRIcon()
  {

  }
  void TUGRIcon::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    field_->g_SetCenter(TRUE);
    field_->g_SetDraw(FALSE);//デフォはアイコン無し
  }

  void TUGRIcon::SetDrawIcon(enUGGrPart gptype, s32 iconno)
  {
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(gptype));
    field_->g_SetPartNo(iconno);
    field_->g_SetDraw(TRUE);
  }

  void TUGRIcon::SetIconTeam(s64 mid_team)
  {
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    SetDrawIcon(ugp_icon_rarity, rarity);
  }
  void TUGRIcon::SetIconSozai(s64 mid_sozairarity, BOOL shop_f)
  {
    SetIconSozai((mid_sozairarity / SOZAIMID), (mid_sozairarity % SOZAIMID), shop_f);
  }
  void TUGRIcon::SetIconSozai(s64 mid_sozai, s32 rarity, BOOL shop_f)
  {
    if (shop_f)
    {
      SetIcon((enUGPIcon)(ugi_RarityD + rarity));
    }
    else
    {
      SetDrawIcon(ugp_icon_rarity,ugr_Ball + rarity);
    }
  }
  void TUGRIcon::SetIconSozaiGenre(s64 mid_sozai)
  {
	  if (mid_sozai >= 0)
	  {
		  SetDrawIcon(ugp_shot_genre, pmgEO_->mgDt_.dtShElm_.GetGenre8(mid_sozai));//8type
	  }
	  else
	  {
		  SetIcon(enUGPIcon::ugi_Nazo);
	  }
  }
  void TUGRIcon::SetIconSozaiGenreSub(s64 mid_sozai, s32 genreno)
  {
    SetDrawIcon(ugp_shot_genre, genreno);
    BOOL draw_f = pmgEO_->mgDt_.dtShElm_.IsLightFlag(mid_sozai, genreno);
    SetDraw(draw_f);//8type
    //pmgEO_->mgDt_.dtShElm_.GetGenre(mid_sozai);
  }
  void TUGRIcon::SetIconShot(s64 id_shot)
  {
    TSozaiData sozaidt = pmgEO_->mgCoU_.GetSozaiData(id_shot, 0);
    SetDrawIcon(ugp_icon_rarity,ugr_Ball + sozaidt.rarity_);
  }
  void TUGRIcon::SetIconShotQuality(s64 quality)
  {
    if (quality != NGNUM)
    {
      SetDrawIcon(ugp_souko_num, quality);
    }
    else
    {
      SetIcon(enUGPIcon::ugi_Nazo);
    }
  }
  void TUGRIcon::SetIconAlbum(s64 mid_title)
  {
    s32 iconno = pmgEO_->mgDt_.dtTitle_.GetDtPK(mid_title, enDtTitle_iconno);
    SetDrawIcon(ugp_icon_album, iconno);
  }
  void TUGRIcon::SetIconTape(s64 mid_bgm, BOOL home_f)
  {
    if (home_f)
    {
      SetIcon(ugi_HomeBGM);
    }
    else
    {
      s64 mid_title = pmgEO_->mgDt_.dtBGM_.GetDtPK(mid_bgm, enDtBGM_titleid);
      s32 iconno = pmgEO_->mgDt_.dtTitle_.GetDtPK(mid_title, enDtTitle_iconno);
      SetDrawIcon(ugp_icon_tapes, iconno);
    }
  }
  void TUGRIcon::SetIconTapeTitle(s64 mid_title)
  {
    s32 iconno = pmgEO_->mgDt_.dtTitle_.GetDtPK(mid_title, enDtTitle_iconno);
    SetDrawIcon(ugp_icon_tapes, iconno);
  }
  void TUGRIcon::SetIconHaikeiTitle(s64 mid_title)
  {
    s32 iconno = pmgEO_->mgDt_.dtTitle_.GetDtPK(mid_title, enDtTitle_iconno);
    SetDrawIcon(ugp_icon_films, iconno);
  }
  void TUGRIcon::SetIconHaikei(s64 mid_haikei)
  {
    s64 mid_title = pmgEO_->mgDt_.dtCourt_.GetDtPK(mid_haikei, enDtCourt_titleid);
    s32 iconno = pmgEO_->mgDt_.dtTitle_.GetDtPK(mid_title, enDtTitle_iconno);
    SetDrawIcon(ugp_icon_films, iconno);
  }
  void TUGRIcon::SetIconSupply(s64 mid_supply)
  {
    //イベントアイテム
    if (mid_supply >= 20000)
    {
      s32 iconno = ((mid_supply % 10) % 4);
      field_->o_SetObjGrp(pmgUI_->GetEvIcon(iconno));
      field_->g_SetDraw(TRUE);
    }
    else
    {
      s32 iconno = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply, mdm_supply_iconNo);
      SetDrawIcon(ugp_icon_supply, iconno);
    }

  }
  void TUGRIcon::SetIconItemPack(s64 mid_ItemPack)
  {
    s32 iconno = pmgEO_->mgCoM_.mdm_ItemPack_.GetPKDt(mid_ItemPack, mdm_itempack_iconNo);
    SetDrawIcon(ugp_icon_supply, iconno);
  }
  void TUGRIcon::SetIconGacha(s64 mid_gacha)
  {
    s32 iconno = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_iconNo);
    SetDrawIcon(ugp_icon_supply, iconno);
  }
  void TUGRIcon::SetIconNaBox(s64 mid_nabox)
  {
    s32 iconno = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_iconNo);
    SetDrawIcon(ugp_icon_nabox, iconno);
  }
  void TUGRIcon::SetIconHuku(s64 mid_huku)
  {
    s32 iconno = pmgEO_->mgCoM_.mdm_Huku_.GetPKDt(mid_huku, mdm_huku_iconNo);
    SetIcon(ugi_HukuGara);
  }
  void TUGRIcon::SetIconSpSyouhin(s64 mid_spsyouhin)
  {
    s64 itemtype = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemType);
    s64 itemid = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemID);
    SetIcon(itemtype, itemid, TRUE);
  }
  void TUGRIcon::SetIconGiftBox(s64 id_giftbox)
  {
    s64 itemtype = pmgEO_->mgCoU_.mdGiftbox_.GetPKDt(id_giftbox, mdgiftbox_itemType);
    s64 itemid = pmgEO_->mgCoU_.mdGiftbox_.GetPKDt(id_giftbox, mdgiftbox_itemID);
    SetIcon(itemtype, itemid, FALSE);
  }
  void TUGRIcon::SetIconChest(s32 sideno, s32 posno)
  {
    field_->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconChest_[sideno]);
    field_->g_SetPartNo(posno);
    field_->g_SetDraw(TRUE);
  }
  void TUGRIcon::SetIconRankerChest(s32 no)
  {
	  field_->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconRankerChest_[no]);
	  field_->g_SetPartNo(0);
	  field_->g_SetDraw(TRUE);
  }
  void TUGRIcon::SetIconCup(s32 grade)
  {
    SetDrawIcon(ugp_icon_rarity, ugr_Cup + grade);
  }
  void TUGRIcon::SetIconCupHuku(s32 grade)
  {
    SetDrawIcon(ugp_icon_rarity, ugr_Huku + grade);
  }
  //void TUGRIcon::SetIconEvent(base::TGrp* pgr)
  //{
  //  pmgUI_->GetEvIcon(iconno)
  //  field_->o_SetObjGrp(pgr);
  //  field_->g_SetDraw(TRUE);
  //}
  void TUGRIcon::SetIcon(s64 itemtype, s64 itemid, BOOL shop_f)
  {
    switch (itemtype)
    {
    case enItemType_Team: SetIconTeam(itemid); break;
    case enItemType_Sozai:SetIconSozai(itemid, shop_f); break;
    case enItemType_Shot: break;
    case enItemType_BGM:SetIconTape(itemid, FALSE); break;
    case enItemType_Haikei:SetIconHaikei(itemid); break;
    case enItemType_Supply:SetIconSupply(itemid); break;
    case enItemType_Todd:SetIcon(ugi_Todd); break;
    case enItemType_Star:SetIcon(ugi_Star); break;
    case enItemType_Pack:SetIconItemPack(itemid); break;
    case enItemType_Huku:SetIcon(ugi_HukuMini); break;
    }
  }
  void TUGRIcon::SetIcon(enUGPIcon iconno)
  {
    SetDrawIcon(ugp_icon_item, iconno);
  }

  void TUGRIcon::SetLimY(s32 limu, s32 hhh)
  {
    field_->g_SetLimY(limu, hhh);
  }

  void TUGRIcon::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
  }

}
