#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGRIcon.h"
#include "dbUGRRarity.h"

namespace db
{
  class TUILayer;

  struct TUGRNameSt
  {
    s32 namesize_;
    BOOL rare_f_;
    BOOL label_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGRName : public TUGGen
  {
  protected:
    TUGRRarity ugrarity_;
    void SetRarityPosShot(s32 fontsize, s32 rarity);
    void SetRarityPosTeam(s32 fontsize, s32 rarity, s32 lv);
    void DoUpdate(BOOL draw_f);
	std::string nameStr_;
  public:
    TUGRNameSt stRName_;
    TUGRIcon ugicon_;
    //コンストラクタ
    explicit TUGRName(TUIGenLayer* player);
    virtual ~TUGRName(void);
    void MakeGroup(TUIObj* parent);

    void SetIcon(BOOL icon_f);

    void SetNameTeam(s64 mid_team);
	void SetNameTeam(s64 mid_team, s32 lv);
    void SetNameTeam_Get(s64 mid_team, s32 lv);
    void SetNameTeam_Cnt(s64 mid_team);
    void SetNameSozai(s64 mid_sozai, s32 rarity, BOOL shop_f);
    void SetNameSozai2(s64 mid_sozairarity, BOOL shop_f);
    void SetNameSozaiSub(s64 mid_sozai, s32 rarity);

    void SetNameShot(s64 id_shot, BOOL shop_f);
    void SetNameShotSub(s64 id_shot);
	void SetNameShotSub(TSozaiData dt);

    void SetNameDefShot(s64 defshot);
    void SetNameAlbum(s64 mid_title);
    void SetNameTape(s64 mid_bgm, BOOL home_f);
    void SetNameTapeTitle(s64 mid_title);
    void SetNameHaikeiTitle(s64 mid_title);
    void SetNameHaikei(s64 mid_haikei);
    void SetNameHaikeiPicture(s64 mid_haikei);
    void SetNameSupply(s64 mid_supply, s32 num);
    void SetNameItemPack(s64 mid_ItemPack, s32 num);
    void SetNameTodd(s64 num);
    void SetNameStar(s64 num);
    void SetNameHukuKen(s64 num);

    void SetNameGacha(s64 mid_gacha);
    void SetNameNaBox(s64 mid_nabox);
    void SetNameHuku(s64 mid_huku);
    void SetNameSpSyouhin(s64 mid_spsyouhin);
    void SetNameGiftBox(s64 id_giftbox);
    void SetNameCol(s32 hue);
    void SetNameCol2(s32 hue, s32 lum);
    void SetNameSkinCol(s32 hue);
    void SetNameSkinCol2(s32 hue, s32 lum);
    void SetNameFace(s32 charno, s32 grpno);
    void SetNameRanking(s64 mid_ranking);
    void SetNameRankingItem(s64 mid_ranking);
    void SetNameRanker(s64 id_ranker, s32 lineno);
	void SetNameRankerByData(s64 id_ranker);
    void SetNameTousen(s32 lineno);
    void SetName(std::string name);
	void SetName(std::string name, s32 maxlen);
	void SetName(std::string name, BOOL nolim_f, s32 col, s32 maxlen = 13);
    void SetName(s64 itemtype, s64 itemid, s64 itemnum);
    void SetName(s64 itemtype, s64 itemid, s64 itemnum, BOOL shop_f);
    void SetNameIcon(std::string name, enUGPIcon iconno, s32 maxlen = 13);
    void SetNameHukuCnt(s32 mid_Hukucnt);

    void GenSetValue(s64 num);
    void SetValueTodd(s64 num);
    void SetValueStar(s64 num);
    void SetValueHukuKen(s64 num);
    void SetValueGachaKen(s64 num);
	void SetValueCoachKen(s64 mid_supply, s64 num);
    void SetUseStar(s64 num);
    void SetUseTodd(s64 num, BOOL get_f);

    void SetPage(s32 nowpage, s32 maxpage);
    void SetRightShift();

    void SetLimY(s32 limu, s32 hhh);
    void SetRecordDefPos();
    s32 GetNameLen();
	std::string GetName(void);
    BOOL IsRare();
    void SetLabel(BOOL label_f);
  };

}
