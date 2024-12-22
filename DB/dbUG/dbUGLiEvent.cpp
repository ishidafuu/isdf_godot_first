#include "dbUGLiEvent.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiEvent::TUGLiEvent(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiEvent_.Init();
  }

  TUGLiEvent::~TUGLiEvent()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiEvent::DoClearRecord()
  {
    stLiEvent_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }


  TUGRcEvent* TUGLiEvent::MakeObj_Record()
  {
    TUGRcEvent* newrecord = new TUGRcEvent(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }
  void TUGLiEvent::Refresh()
  {
    SetDraw(TRUE);

    //レイヤーの画像をクリア（共通のところなのでバッティングあるかも）
    player_->ClearGrp();

    //各レコードクリアして入れる
    ClearRecord();

    //現在のイベント
    LONGVECTOR eventvec = pmgEO_->mgCoU_.GetNowEvent();//マップTRUE

    for (s32 i = 0; i < eventvec.size(); ++i)
    {
      TUGRcEvent* newrecord = (TUGRcEvent*)MakeObj_Record();
      newrecord->SetDt(eventvec[i]);
    }

    SetZeroPos();
  }
  void TUGLiEvent::Refresh(s64 mid_event)
  {
	  SetDraw(TRUE);

	  //レイヤーの画像をクリア（共通のところなのでバッティングあるかも）
	  player_->ClearGrp();

	  //各レコードクリアして入れる
	  ClearRecord();

	  //現在のイベント
	  LONGVECTOR eventvec = pmgEO_->mgCoU_.GetNowEvent();//マップTRUE

	  for (s32 i = 0; i < eventvec.size(); ++i)
	  {
		  if (eventvec[i] == mid_event){ continue; }

		  TUGRcEvent* newrecord = (TUGRcEvent*)MakeObj_Record();
		  newrecord->SetDt(eventvec[i]);
	  }

	  SetZeroPos();
  }

  void TUGLiEvent::DoUpdateSub(BOOL draw_f)
  {
    
  }

  void TUGLiEvent::DoActionSub()
  {
    //選ばれたレコード色づけ
    SetSelect();
  }

  s64 TUGLiEvent::GetMIDEvent()
  {
    return records_[GetActIndex()]->GetMIDEvent();
  }

}
