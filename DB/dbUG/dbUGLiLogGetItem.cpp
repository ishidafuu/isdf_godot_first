#include "dbUGLiLogGetItem.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiLogGetItem::TUGLiLogGetItem(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiLogGetItem_.Init();
  }

  TUGLiLogGetItem::~TUGLiLogGetItem()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiLogGetItem::DoClearRecord()
  {
    stLiLogGetItem_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiLogGetItem::DoMakeGroupSub()
  {
    //field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
    uglistend_.MakeGroup_ListEnd(slider_, sliderx_, slidery_);
  }

  //マスターガチャIDを入れてレコードをすべて作成
  void TUGLiLogGetItem::Refresh()
  {
    ClearRecord();

    for (s32 i = 0; i < pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineNum(); ++i)
    {
      TUGRcLogItem* newrecord = new TUGRcLogItem(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetLogGetItemDt(pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_pk));
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
    }

    ChangeListEnd(FALSE);
    SetZeroPos();
  }
  void TUGLiLogGetItem::ChangeListEnd(BOOL listend_f)
  {
    uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  }
  void TUGLiLogGetItem::SetListEnd(s32 recordidx)
  {
    uglistend_.SetDataGetLog(records_[recordidx]->GetCommonID());
  }

  void TUGLiLogGetItem::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiLogGetItem::DoActionSub()
  {
  }

}
