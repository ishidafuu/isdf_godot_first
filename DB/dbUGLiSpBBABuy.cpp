#include "dbUGLiSpBBABuy.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiSpBBABuy::TUGLiSpBBABuy(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiSpBBABuy_.Init();
  }

  TUGLiSpBBABuy::~TUGLiSpBBABuy()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiSpBBABuy::DoClearRecord()
  {
    stLiSpBBABuy_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiSpBBABuy::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));
    uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
  }


  void TUGLiSpBBABuy::Refresh(s32 type_st, s32 type_ed, s32 mid_bbabuy)
  {
    ClearRecord();

    s32 shoptype = pmgEO_->mgCoM_.mdm_BBABuy_.GetPKDt(mid_bbabuy, mdm_bbabuy_shopType);

    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineNum(); ++i)
    {
      //このショップの種類
      if (pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_shopType) == shoptype)
      {
        s32 mid_sozai = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_itemID) / 10;//タイプはレア度を省いたものでチェック

        if (mid_sozai< type_st) continue;
        if ((type_ed != 0) && (mid_sozai >= type_ed)) continue;

        s64 mid_syouhin = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_pk);
        
        TUGRcSpSyouhin* newrecord = new TUGRcSpSyouhin(player_);
        s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
        newrecord->SetBuySozaiDt(mid_syouhin, mid_bbabuy);
        records_.push_back(newrecord);
        GenMakeObj_Record(newrecord, rh);
      }
    }

    SetZeroPos();
  }
  void TUGLiSpBBABuy::DoActionSub()
  {
    //選ばれたレコード色づけ
    //SetSelect();
  }
  void TUGLiSpBBABuy::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiSpBBABuy::ChangeListEnd(BOOL listend_f)
  {
    uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  }
  void TUGLiSpBBABuy::SetListEnd(s32 recordidx)
  {
    //レコードから商品MIDを取得
    uglistend_.SetDataSpSyouhin(GetCommonID());
  }
  s64 TUGLiSpBBABuy::GetCommonID()
{
    //レコードから商品MIDを取得
    return records_[GetActIndex()]->GetCommonID();
  }

}
