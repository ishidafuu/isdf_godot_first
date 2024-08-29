#include "dbUGPlateItem.h"
#include "dbUILayer.h"
namespace db
{

  //コンストラクタ
  TUGPlateItem::TUGPlateItem(TUIGenLayer* player) :TUGGen(player)
    , ugname_(player)
  {

  }

  TUGPlateItem::~TUGPlateItem()
  {

  }

  void TUGPlateItem::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_itemget_panel));
    field_->g_SetDepth(UIDP_09MSGBOX);
    SetLRPos(FALSE);

    static const s32 NAMEX = 28;
    static const s32 NAMEY = 8;

    ugname_.MakeGroup(field_);
    ugname_.SetPos(NAMEX, NAMEY);

    const s32 TEXTY = 24;
    const s32 DISTY = 12;
    const s32 TEXTX = 12;

    text0_ = MakeObj();
    text0_->o_SetParent(field_);
    text0_->z_SetPos(TEXTX, TEXTY + (DISTY * 0));

    text1_ = MakeObj();
    text1_->o_SetParent(field_);
    text1_->z_SetPos(TEXTX, TEXTY + (DISTY * 1));

    text2_ = MakeObj();
    text2_->o_SetParent(field_);
    text2_->z_SetPos(TEXTX, TEXTY + (DISTY * 2));

    text3_ = MakeObj();
    text3_->o_SetParent(field_);
    text3_->z_SetPos(TEXTX, TEXTY + (DISTY * 3));

    //textEv0_ = MakeObj();
    //textEv0_->o_SetParent(field_);
    //textEv0_->z_SetPos(TEXTX, TEXTY + (DISTY * 4));

    //textEv1_ = MakeObj();
    //textEv1_->o_SetParent(field_);
    //textEv1_->z_SetPos(TEXTX, TEXTY + (DISTY * 5));

  }

  void TUGPlateItem::GenSetSupplyText(s64 mid_supply)
  {
    text0_->f_MakeFont(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_text0_c64).c_str(), FC2_normal);
    text1_->f_MakeFont(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_text1_c64).c_str(), FC2_normal);
    text2_->f_MakeFont(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_text2_c64).c_str(), FC2_normal);
    //text3_->f_MakeFont(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_text2_c64).c_str(), FC2_normal);
  }
  //消耗品
  void TUGPlateItem::SetDataSupply(s64 mid_supply, s32 num)
  {
    
    ugname_.SetNameSupply(mid_supply, num);
    GenSetSupplyText(mid_supply);
    SetDraw(TRUE);
  }
  //消耗品
  void TUGPlateItem::SetDataSupplyEvent(s64 mid_supply, s32 num, s32 tokkouNum, s32 tokkouMag)
  {
    if (tokkouMag == NGNUM)
    {
      SetDataSupply(mid_supply, num);
    }
    else
    {
      ugname_.SetNameSupply(mid_supply, num);
      //アイコンをイベントアイテムに
      //s32 iconno = ((mid_supply % 10) % 4, 1);
      //std::string ev_icon = "ev_icon" + (lib_str::IntToStr1b((mid_supply % 10) % 4, 1));
      //ugname_.ugicon_.SetIconEvent(mid_supply);//試合終了のときのみなので余り気にしない
      std::string strnum = "とっこうＢＧＭ＆はいけい＿" + lib_str::IntToStr(tokkouNum);
      s32 addnum = lib_num::Percent(num, tokkouMag);
      std::string strmag = "ボーナス＋" + lib_str::IntToStr(addnum) + "こ（＋" + lib_str::IntToStr(tokkouMag) + "％）";
      std::string strtotal = "ごうけいかくとく＿" + lib_str::IntToStr(num + addnum)+"こ";
      text0_->f_MakeFont(strnum.c_str(), FC2_normal);
      text1_->f_MakeFont(strmag.c_str(), FC2_normal);
      text3_->f_MakeFont(strtotal.c_str(), FC2_normal);

      SetDraw(TRUE);
    }
  }
  //アイテムパック
  void TUGPlateItem::SetDataPack(s64 mid_itempack, s32 num)
  {
    ugname_.SetNameItemPack(mid_itempack, num);
    text0_->f_MakeFont(pmgEO_->mgCoM_.mdm_ItemPack_.GetPKDtStr(mid_itempack, mdm_itempack_text0_c64).c_str(), FC2_normal);
    text1_->f_MakeFont(pmgEO_->mgCoM_.mdm_ItemPack_.GetPKDtStr(mid_itempack, mdm_itempack_text1_c64).c_str(), FC2_normal);
    text2_->f_MakeFont(pmgEO_->mgCoM_.mdm_ItemPack_.GetPKDtStr(mid_itempack, mdm_itempack_text2_c64).c_str(), FC2_normal);
    SetDraw(TRUE);
  }
  //BGM
  void TUGPlateItem::SetDataBGM(s64 mid_bgm)
  {
    s32 mid_title = pmgEO_->mgDt_.dtBGM_.GetDtPK(mid_bgm, enDtBGM_titleid);
    ugname_.SetNameTapeTitle(mid_title);

    std::string text0 = pmgEO_->mgDt_.dtBGM_.GetStrLine(mid_bgm, enDtBGMStr_name);
    std::string text1 = "";
    const s32 MAXLEN = (13 * 3);
    if (text0.size() > MAXLEN)
    {
      text0.erase(MAXLEN, text0.size() - MAXLEN);
      text1 = pmgEO_->mgDt_.dtBGM_.GetStrLine(mid_bgm, enDtBGMStr_name);
      text1.erase(0, MAXLEN);
    }
    text0_->f_MakeFont_Len(text0.c_str(), FC2_normal, 13);
    text1_->f_MakeFont_Len(text1.c_str(), FC2_normal, 13);
    text2_->f_MakeFont("", FC2_normal);
    SetDraw(TRUE);
  }
  //トッド
  void TUGPlateItem::SetDataTodd(s32 num)
  {
    ugname_.SetNameTodd(num);
    const s32 PKTODD = 2;
    GenSetSupplyText(PKTODD);
    SetDraw(TRUE);
  }
  //スター
  void TUGPlateItem::SetDataStar(s32 num)
  {
    ugname_.SetNameStar(num);
    const s32 PKSTAR = 1;
    GenSetSupplyText(PKSTAR);
    SetDraw(TRUE);
  }

  void TUGPlateItem::SetDataHuku(s32 num)
  {
    ugname_.SetNameHukuKen(num);
    const s32 PKHUKUKEN = 3;
    GenSetSupplyText(PKHUKUKEN);
    SetDraw(TRUE);
  }

  void TUGPlateItem::SetLRPos(BOOL leftpos_f)
  {
    const s32 POSLX = -156;
    const s32 POSRX = 12;
    const s32 POSY = -55;

    s32 posx = (leftpos_f)
      ? POSLX
      : POSRX;

    field_->z_SetPos(posx, POSY);
  }

  void TUGPlateItem::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugname_.Update(draw_f);
    text0_->Update(draw_f);
    text1_->Update(draw_f);
    text2_->Update(draw_f);
    text3_->Update(draw_f);
    //textEv0_->Update(draw_f);
    //textEv1_->Update(draw_f);
  }

  void TUGPlateItem::DoAction()
  {
  }
}
