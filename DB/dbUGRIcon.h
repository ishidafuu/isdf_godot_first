#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
  class TUILayer;

  struct TUGRIconSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGRIcon : public TUGGen
  {
  protected:
    void DoUpdate(BOOL draw_f);
    void SetDrawIcon(enUGGrPart gptype, s32 iconno);
  public:
    TUGRIconSt stRIcon_;
    void MakeGroup(TUIObj* parent);

    void SetIconTeam(s64 mid_team);
    void SetIconSozai(s64 mid_sozairarity, BOOL shop_f);
    void SetIconSozai(s64 mid_sozai, s32 rarity, BOOL shop_f);
    void SetIconSozaiGenre(s64 mid_sozai);
    void SetIconSozaiGenreSub(s64 mid_sozai, s32 genreno);
    void SetIconShot(s64 id_shot);
    void SetIconShotQuality(s64 quality);
    void SetIconAlbum(s64 mid_title);
    void SetIconTape(s64 mid_bgm, BOOL home_f);
    void SetIconTapeTitle(s64 mid_title);
    void SetIconHaikeiTitle(s64 mid_title);
    void SetIconHaikei(s64 mid_haikei);
    void SetIconSupply(s64 mid_supply);
    void SetIconItemPack(s64 mid_ItemPack);
    void SetIconGacha(s64 mid_gacha);
    void SetIconNaBox(s64 mid_nabox);
    void SetIconHuku(s64 mid_huku);
    void SetIconSpSyouhin(s64 mid_spsyouhin);
    void SetIconGiftBox(s64 id_giftbox);
    void SetIconChest(s32 sideno, s32 posno);
	void SetIconRankerChest(s32 no);
    void SetIconCup(s32 grade);
    void SetIconCupHuku(s32 grade);
    //void SetIconEvent(s32 iconno);
    void SetIcon(s64 itemtype, s64 itemid, BOOL shop_f);
    void SetIcon(enUGPIcon iconno);
    void SetLimY(s32 limu, s32 hhh);
    //コンストラクタ
    explicit TUGRIcon(TUIGenLayer* player);
    virtual ~TUGRIcon(void);
  };
 
}
