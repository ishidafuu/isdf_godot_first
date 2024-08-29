#include "dbUGLiLogBuyStar.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiLogBuyStar::TUGLiLogBuyStar(TUIGenLayer* player) :TUGLiGen(player)
    //, uglistend_(player)
  {
    stLiLogBuyStar_.Init();
  }

  TUGLiLogBuyStar::~TUGLiLogBuyStar()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiLogBuyStar::DoClearRecord()
  {
    stLiLogBuyStar_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiLogBuyStar::DoMakeGroupSub()
  {
    //uglistend_.MakeGroup_ListEnd(slider_, sliderx_, slidery_);
  }

  //マスターガチャIDを入れてレコードをすべて作成
  void TUGLiLogBuyStar::Refresh()
  {
    ClearRecord();

    for (s32 i = 0; i < pmgEO_->mgCoU_.mdlog_BStar_.GetLineNum(); ++i)
    {
      TUGRcLogStar* newrecord = new TUGRcLogStar(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetLogBuyStarDt(pmgEO_->mgCoU_.mdlog_BStar_.GetLineDt(i, mdlog_bstar_pk));
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
    }

    SetZeroPos();
    //ChangeListEnd(FALSE);
  }
  //void TUGLiLogBuyStar::ChangeListEnd(BOOL listend_f)
  //{
  //  uglistend_.SetDraw(listend_f);
  //  for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  //}
  //void TUGLiLogBuyStar::SetListEnd(s32 recordidx)
  //{
  //  uglistend_.SetDataLogBuyStar(records_[recordidx]->GetCommonID());
  //}

  void TUGLiLogBuyStar::DoUpdateSub(BOOL draw_f)
  {
    //uglistend_.Update(draw_f);
  }

  void TUGLiLogBuyStar::DoActionSub()
  {
  }

}
