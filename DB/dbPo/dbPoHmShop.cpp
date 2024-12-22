#include "dbPoHmShop.h"

namespace db
{
  TPostHmShop::TPostHmShop(TMgEngineObj* pmgEO):TPoGen(pmgEO)
  {
  }

  TPostHmShop::~TPostHmShop(void)
  {
  }

  //消耗品購入
  void TPostHmShop::Ready_buy(s64 mid_supply)
  { 
    ClearMap();
    posttype_ = enPostHmShop_buy;
    sendmap_.longmap_["mid_supply"] = mid_supply;
    StartConnect_NoneRetry("hmshop_buy");
  }
  //ガチャ（お金使用）
  void TPostHmShop::Ready_gacha(s64 mid_gacha)
  {
    ClearMap();
    posttype_ = enPostHmShop_gacha;
    sendmap_.longmap_["mid_gacha"] = mid_gacha;
    StartConnect_NoneRetry("hmshop_gacha");
  }
  //ガチャ当選者取得（ログ）
  void TPostHmShop::Ready_tousengacha(s64 mid_gacha, s32 grade, s32 page)
  {
    ClearMap();
    posttype_ = enPostHmShop_tousenlog;
    sendmap_.longmap_["mid_gacha"] = mid_gacha;
    sendmap_.longmap_["grade"] = grade;
    sendmap_.longmap_["page"] = page;
    StartConnect("hmshop_tousengacha");
  }
  //ふくびき当選者取得（ログ）
  void TPostHmShop::Ready_tousenhuku(s64 mid_hukugrp, s32 grade, s32 page)
  {
    ClearMap();
    posttype_ = enPostHmShop_tousenlog;
    sendmap_.longmap_["mid_hukugrp"] = mid_hukugrp;
    sendmap_.longmap_["grade"] = grade;
    sendmap_.longmap_["page"] = page;
    StartConnect("hmshop_tousenhuku");
  }
  //なつかしＢＯＸ
  void TPostHmShop::Ready_nabox(s64 mid_nabox)
  {
    ClearMap();
    posttype_ = enPostHmShop_nabox;
    sendmap_.longmap_["mid_nabox"] = mid_nabox;
    StartConnect_NoneRetry("hmshop_nabox");
  }
  //なつかしＢＯＸ大人買い
  void TPostHmShop::Ready_nabox_otona(s64 mid_nabox)
  {
    ClearMap();
    posttype_ = enPostHmShop_nabox_otona;
    sendmap_.longmap_["mid_nabox"] = mid_nabox;
    StartConnect_NoneRetry("hmshop_nabox_otona");
  }
  //ふくびき
  void TPostHmShop::Ready_huku(s64 mid_huku)
  {
    ClearMap();
    posttype_ = enPostHmShop_huku;
    sendmap_.longmap_["mid_huku"] = mid_huku;
    StartConnect_NoneRetry("hmshop_huku");
  }
  //ふくびきなかみ
  void TPostHmShop::Ready_hukucnt(s64 mid_huku)
  {
    ClearMap();
    posttype_ = enPostHmShop_hukucnt;
    sendmap_.longmap_["mid_huku"] = mid_huku;
    StartConnect("hmshop_hukucnt");
  }
}
