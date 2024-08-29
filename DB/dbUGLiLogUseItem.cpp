#include "dbUGLiLogUseItem.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiLogUseItem::TUGLiLogUseItem(TUIGenLayer* player) :TUGLiGen(player)
    //, uglistend_(player)
  {
    stLiLogUseItem_.Init();
  }

  TUGLiLogUseItem::~TUGLiLogUseItem()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiLogUseItem::DoClearRecord()
  {
    stLiLogUseItem_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiLogUseItem::DoMakeGroupSub()
  {
    //field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
    //uglistend_.MakeGroup_ListEnd(slider_, sliderx_, slidery_);
  }

  void TUGLiLogUseItem::Refresh()
  {
    ClearRecord();

    for (s32 i = 0; i < pmgEO_->mgCoU_.mdlog_SupplyUse_.GetLineNum(); ++i)
    {
      TUGRcLogItem* newrecord = new TUGRcLogItem(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetLogUseItemDt(pmgEO_->mgCoU_.mdlog_SupplyUse_.GetLineDt(i, mdlog_supplyuse_pk));//行番号とＰＫ一致
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
    }
    SetZeroPos();
  }
  //void TUGLiLogUseItem::ChangeListEnd(BOOL listend_f)
  //{
  //  uglistend_.SetDraw(listend_f);
  //  for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  //}
  //void TUGLiLogUseItem::SetListEnd(s32 recordidx)
  //{
  //  //uglistend_.SetDataLogUseItem(records_[recordidx]->GetCommonID());
  //}

  void TUGLiLogUseItem::DoUpdateSub(BOOL draw_f)
  {
    //uglistend_.Update(draw_f);
  }

  void TUGLiLogUseItem::DoActionSub()
  {
  }

}
