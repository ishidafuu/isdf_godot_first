#include "dbUGLiSpSyouhin.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiSpSyouhin::TUGLiSpSyouhin(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiSpSyouhin_.Init();
  }

  TUGLiSpSyouhin::~TUGLiSpSyouhin()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiSpSyouhin::DoClearRecord()
  {
    stLiSpSyouhin_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiSpSyouhin::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));
    uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
  }

  void TUGLiSpSyouhin::Refresh(enShopType shoptype)
  {
    ClearRecord();

    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineNum(); ++i)
    {
      //このショップの種類
      if (pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_shopType) == shoptype)
      {
        s64 mid_syouhin = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_pk);
        TUGRcSpSyouhin* newrecord = new TUGRcSpSyouhin(player_);
        s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
        newrecord->SetSyouhinDt(mid_syouhin);
        records_.push_back(newrecord);
        GenMakeObj_Record(newrecord, rh);
      }
    }
  }
  void TUGLiSpSyouhin::RefreshBuySozai(s32 typest, s32 typeed, BOOL vip_f)
  {
    ClearRecord();

    enShopType shoptype = (vip_f)
      ? enShopType_BuySozaiVip
      : enShopType_BuySozai;

    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineNum(); ++i)
    {
      //このショップの種類
      if (pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_shopType) == shoptype)
      {
        s32 mid_sozai = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_itemID) / 10;//タイプはレア度を省いたものでチェック

        if (mid_sozai< typest) continue;
        if ((typeed != 0) && (mid_sozai >= typeed)) continue;

        s64 mid_syouhin = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_pk);
        
        TUGRcSpSyouhin* newrecord = new TUGRcSpSyouhin(player_);
        s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
        newrecord->SetSyouhinDt(mid_syouhin);
        records_.push_back(newrecord);
        GenMakeObj_Record(newrecord, rh);
      }
    }

    SetZeroPos();
  }
  void TUGLiSpSyouhin::DoActionSub()
  {
    //選ばれたレコード色づけ
    //SetSelect();
  }
  void TUGLiSpSyouhin::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiSpSyouhin::ChangeListEnd(BOOL listend_f)
  {
    uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  }
  void TUGLiSpSyouhin::SetListEnd(s32 recordidx)
  {
    //レコードから商品MIDを取得
    uglistend_.SetDataSpSyouhin(GetCommonID());
  }
  s64 TUGLiSpSyouhin::GetCommonID()
{
    //レコードから商品MIDを取得
    return records_[GetActIndex()]->GetCommonID();
  }

}
