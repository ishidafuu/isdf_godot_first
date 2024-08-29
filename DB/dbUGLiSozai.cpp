#include "dbUGLiSozai.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiSozai::TUGLiSozai(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiItemGet_.Init();
  }

  TUGLiSozai::~TUGLiSozai()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiSozai::DoClearRecord()
  {
    stLiItemGet_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiSozai::DoMakeGroupSub()
  {
    slider_->b_SetModal(TRUE);
  }

  void TUGLiSozai::SetData(SOZAIDTVECTOR sozai)
  {
    //レコードクリア
    ClearRecord();

    //獲得ログからレコード作成
    s32 num = sozai.size();

    for (s32 i = 0; i < num; ++i)
    {
      TUGRcNormal* newrecord = new TUGRcNormal(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->GetField()->b_SetModal(TRUE);
      newrecord->SetCommonID(i);

      newrecord->ugname_.SetNameSozai2(sozai[i].GetSozaiR(), FALSE);
      newrecord->ugname_.SetIcon(TRUE);

      GenMakeObj_Record(newrecord, rh);
      records_.push_back(newrecord);
    }
    SetZeroPos();
  }

  void TUGLiSozai::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiSozai::DoActionSub()
  {

  }

  s64 TUGLiSozai::GetCommonID()
  {
    return records_[GetActIndex()]->GetCommonID();
  }
  s64 TUGLiSozai::GetEtcNum()
  {
    return records_[GetActIndex()]->GetEtcNum();
  }
}
