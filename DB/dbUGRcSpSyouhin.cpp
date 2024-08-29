#include "dbUGRcSpSyouhin.h"

namespace db
{
  TUGRcSpSyouhin::TUGRcSpSyouhin(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcSpSyouhin::~TUGRcSpSyouhin(void)
  {

  }
  void TUGRcSpSyouhin::DoMakeGroupSub2()
  {
    //ショップ用レコード
    basegrp_ = pmgUI_->GetUGGrp(ug_list_recordshop);
    field_->o_SetObjGrp(basegrp_);
  }
  void TUGRcSpSyouhin::DoSetLimY2(s32 limu, s32 hhh)
  {
  }
  void TUGRcSpSyouhin::DoUpdateSub2(BOOL draw_f)
  {
  }
  //商品データ
  void TUGRcSpSyouhin::SetSyouhinDt(s64 mid_spsyouhin)
  {
    //商品ＭＩＤ保存
    SetCommonID(mid_spsyouhin);
    s32 itemtype = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemType);
    s32 itemid = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemID);
    s32 itemnum = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemNum);

    s32 price = 0;
    BOOL star_f = TRUE;
    switch (itemtype)
    {
    //case enItemType_Sozai:
    //  price = pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt((itemid % SOZAIMID), mdm_rarity_priceBuy);
    //  star_f = FALSE;
    //  break;
    case enItemType_Supply: price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(itemid, mdm_supply_price); break;
    case enItemType_Pack: price = pmgEO_->mgCoM_.mdm_ItemPack_.GetPKDt(itemid, mdm_itempack_price); break;
    }
    ugname_.SetNameSpSyouhin(mid_spsyouhin);

#ifdef __K_MAKE_HIDE_SUPPLY_PRICE_DOWN__

	s32 salePrice = 0;
	s32 salePercent = 0;

#if TRUE
	// パーセント指定
	salePercent = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(itemid, mdm_supply_gift_f);
	salePrice = price - (price * (salePercent * 0.01f));
#else
	// 割引価格指定
	salePrice = 100;//price - price * 0.1; // ここをどこかのテーブルから取得するようにする
	salePercent = 100 - (f32)(salePrice) / (f32)(price)* 100;
#endif
	// 価格が違うときは割引表記
	if(price != salePrice)
	{
		std::string saleStr = "ＬＩＳＴ＿ＰＲＩＣＥ＿";
		saleStr += lib_str::IntToStr(price);
		saleStr += "＿ＳＡＬＥ＿";
		saleStr += lib_str::IntToStr(salePercent);
		saleStr += "％＿ＯＦＦ";
		SetSubName(saleStr.c_str());

		price = salePrice;
	}
#endif

    if (star_f)
    {
      SetValue_Star(price);
    }
    else
    {
      SetValue_Todd(price);
    }

  }
  //ばばあ素材販売データ
  void TUGRcSpSyouhin::SetBuySozaiDt(s64 mid_spsyouhin, s64 mid_bbabuy)
  {
    //商品ＭＩＤ保存
    SetCommonID(mid_spsyouhin);
    s32 itemtype = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemType);
    s32 itemid = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemID);
    s32 itemnum = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemNum);

    s32 price = pmgEO_->mgCoM_.mdm_BBABuy_.GetPKDt(mid_bbabuy, mdm_bbabuy_price);
    BOOL star_f = (pmgEO_->mgCoM_.mdm_BBABuy_.GetPKDt(mid_bbabuy, mdm_bbabuy_priceType) == enPriceType_Star);

    ugname_.SetNameSpSyouhin(mid_spsyouhin);
    if (star_f)
    {
      SetValue_Star(price);
    }
    else
    {
      SetValue_Todd(price);
    }

  }
  //ガチャデータ
  void TUGRcSpSyouhin::SetGachaDt(s64 mid_gacha)
  {
    SetCommonID(mid_gacha);
    s64 mid_supply_ken = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_id_m_supply_ken);
    s32 kennum = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_kenNum);
    s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply_ken, mdm_supply_price);
    BOOL star_f = (pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply_ken, mdm_supply_priceType) == enPriceType_Star);
    s32 havekennum = pmgEO_->mgCoU_.GetSupplyNum(mid_supply_ken);

    ugname_.SetNameGacha(mid_gacha);

    if (havekennum >= kennum)
    {
      SetValue_Gacha(kennum);
    }
    else if (star_f)
    {
      SetValue_Star(price);
    }
    else
    {
      SetValue_Todd(price);
    }
  }

}
