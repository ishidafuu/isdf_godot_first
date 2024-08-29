#include "dbUGLiSyn.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{

  //コンストラクタ
  TUGLiSyn::TUGLiSyn(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiSyn_.Init();
  }

  TUGLiSyn::~TUGLiSyn()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiSyn::DoClearRecord()
  {
    stLiSyn_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcSyn* TUGLiSyn::MakeObj_Record()
  {
    TUGRcSyn* newrecord = new TUGRcSyn(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }

  void TUGLiSyn::Refresh(s64 mid_team)
  {
    SetDraw(TRUE);
    ClearRecord();//レコードクリアstLiSyn_もクリアされてる
    //s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    for (s32 i = 0; i < DBMEMBER_ALL; ++i)
    {
      s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, i);
      TUGRcSyn* newrecord = (TUGRcSyn*)MakeObj_Record();
      newrecord->SetShotDt(i, charid);//デフォシュートＩＤ
    }
    records_[0]->SetSelect(TRUE);

  }


  void TUGLiSyn::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiSyn::DoActionSub()
  {
    //選ばれたレコード色づけ
    SetSelect();
  }
  s64 TUGLiSyn::GetCommonID()
  {
    return records_[GetActIndex()]->GetCommonID();
  }

  s64 TUGLiSyn::GetCommonID_index(s32 index)
  {
    return records_[index]->GetCommonID();
  }
}
