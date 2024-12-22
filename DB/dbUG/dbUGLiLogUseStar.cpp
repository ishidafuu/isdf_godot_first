#include "dbUGLiLogUseStar.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiLogUseStar::TUGLiLogUseStar(TUIGenLayer* player) :TUGLiGen(player)
    //, uglistend_(player)
  {
    stLiLogUseStar_.Init();
  }

  TUGLiLogUseStar::~TUGLiLogUseStar()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiLogUseStar::DoClearRecord()
  {
    stLiLogUseStar_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiLogUseStar::DoMakeGroupSub()
  {
    //field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
    //uglistend_.MakeGroup_ListEnd(slider_, sliderx_, slidery_);
  }

  void TUGLiLogUseStar::Refresh(BOOL bstar_f, BOOL fstar_f)
  {
    ClearRecord();

    for (s32 i = 0; i < pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineNum(); ++i)
    {
      TUGRcLogStar* newrecord = new TUGRcLogStar(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetLogUseStarDt(pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_pk), bstar_f, fstar_f);//行番号とＰＫ一致
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
    }
    
    SetZeroPos();
  }
  //void TUGLiLogUseStar::ChangeListEnd(BOOL listend_f)
  //{
  //  uglistend_.SetDraw(listend_f);
  //  for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  //}
  //void TUGLiLogUseStar::SetListEnd(s32 recordidx)
  //{
  //  //uglistend_.SetDataLogUseStar(records_[recordidx]->GetCommonID());
  //}

  void TUGLiLogUseStar::DoUpdateSub(BOOL draw_f)
  {
    //uglistend_.Update(draw_f);
  }

  void TUGLiLogUseStar::DoActionSub()
  {
  }

}
