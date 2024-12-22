#include "dbUGLiTousen.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiTousen::TUGLiTousen(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiTousen_.Init();
  }

  TUGLiTousen::~TUGLiTousen()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiTousen::DoClearRecord()
  {
    stLiTousen_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiTousen::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
    uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
  }

  //マスターガチャIDを入れてレコードをすべて作成
  void TUGLiTousen::Refresh_Gacha()
  {
    ClearRecord();
    s32 no = 0;
    for (s32 i = 0; i < pmgEO_->mgCoU_.mdw_log_TousenGacha_.GetLineNum(); ++i)
    {
      TUGRcTousen* newrecord = new TUGRcTousen(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetDt_Gacha(i);
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
      ++no;
    }
  }
  //マスターガチャIDを入れてレコードをすべて作成
  void TUGLiTousen::Refresh_Huku()
  {
    ClearRecord();
    s32 no = 0;
    for (s32 i = 0; i < pmgEO_->mgCoU_.mdw_log_TousenHuku_.GetLineNum(); ++i)
    {
      TUGRcTousen* newrecord = new TUGRcTousen(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetDt_Huku(i);
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
      ++no;
    }
  }
  void TUGLiTousen::ChangeListEnd(BOOL listend_f)
  {
    uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  }
  void TUGLiTousen::SetListEnd(s32 recordidx)
  {
    //uglistend_.SetDataNaBox(records_[recordidx]->GetCommonID());
  }

  void TUGLiTousen::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiTousen::DoActionSub()
  {
  }
  s64 TUGLiTousen::GetCommonID()
  {
    return records_[GetActIndex()]->GetCommonID();
  }

}
