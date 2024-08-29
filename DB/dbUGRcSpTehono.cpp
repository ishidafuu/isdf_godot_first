#include "dbUGRcSpTechno.h"
namespace db
{
  TUGRcSpTechno::TUGRcSpTechno(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcSpTechno::~TUGRcSpTechno(void)
  {
  
  }
  void TUGRcSpTechno::DoMakeGroupSub2()
  {
    //ショップ用レコード
    basegrp_ = pmgUI_->GetUGGrp(ug_list_recordshop);
    field_->o_SetObjGrp(basegrp_);
  }
  void TUGRcSpTechno::DoSetLimY2(s32 limu, s32 hhh)
  {
  }
  void TUGRcSpTechno::DoUpdateSub2(BOOL draw_f)
  {
  }
  //商品データ
  void TUGRcSpTechno::SetTechnoDt(s64 mid_spsyouhin)
  {
    //商品ＭＩＤ保存
    SetCommonID(mid_spsyouhin);
    //s32 itemtype = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemType);
    s32 itemid = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemID);
    //s32 itemnum = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemNum);
    //s32 prise = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(itemid, mdm_supply_price); 
    ugname_.SetNameSpSyouhin(mid_spsyouhin);
    std::string text = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(itemid, mdm_supply_text1_c64);
    SetSubName(text.c_str());
    ////SetSubValue("ＰＲＩＣＥ");
    //SetValue(lib_str::IntToStr(prise).c_str());
  }
  //ガチャデータ
  void TUGRcSpTechno::SetGachaDt(s64 mid_gacha)
  {
    SetCommonID(mid_gacha);
    s64 mid_supply_ken = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_id_m_supply_ken);
  
    ugname_.SetNameGacha(mid_gacha);
    //SetSubName(strno.c_str());
    //SetSubValue("ＰＲＩＣＥ");
    s32 prise = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply_ken, mdm_supply_price);
    SetValue(lib_str::IntToStr(prise).c_str());
  }

}
