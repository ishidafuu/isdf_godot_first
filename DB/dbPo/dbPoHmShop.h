#pragma once
#include "dbPoGen.h"


namespace db
{
  enum enPostHmShop
  {
    enPostHmShop_buy,
    enPostHmShop_gacha,
    enPostHmShop_tousenlog,
    enPostHmShop_nabox,
    enPostHmShop_nabox_otona,
    enPostHmShop_huku,
    enPostHmShop_hukucnt,
    enPostHmShopEND,
  };

  class TPostHmShop : public TPoGen
  {
  protected:
    enPostHmShop posttype_;
  public:
    explicit TPostHmShop(TMgEngineObj* pmgEO);
    virtual ~TPostHmShop(void);
    
    void Ready_buy(s64 mid_supply);
    void Ready_gacha(s64 mid_gacha);
    void Ready_tousengacha(s64 mid_gacha, s32 grade, s32 page);
    void Ready_tousenhuku(s64 mid_hukucnt, s32 grade, s32 page);
    void Ready_nabox(s64 mid_nabox);
    void Ready_nabox_otona(s64 mid_nabox);
    void Ready_huku(s64 mid_huku);
    void Ready_hukucnt(s64 mid_huku);
  };
}
