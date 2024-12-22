#include "dbUGLiCol.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"


namespace db
{


  //コンストラクタ
  TUGLiCol::TUGLiCol(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiCol_.Init();
  }

  TUGLiCol::~TUGLiCol()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }


  void TUGLiCol::DoMakeGroupSub()
  {
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiCol::DoClearRecord()
  {
    stLiCol_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcNormal* TUGLiCol::MakeObj_Record()
  {
    TUGRcNormal* newrecord = new TUGRcNormal(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }

  void TUGLiCol::Refresh_Hue()
  {
    ClearRecord();
    SetDraw(TRUE);
    for (s32 i = 0; i < pmgEO_->mgDt_.dtBGM_.GetLineNum(); ++i)
    {
      TUGRcNormal* newrecord = MakeObj_Record();
      newrecord->SetCommonID(i);
      newrecord->ugname_.SetNameCol(i);
    }
    SetZeroPos();//レコードが出来てから戻す
  }
  void TUGLiCol::Refresh_Lum(s32 hue)
  {
    ClearRecord();
    SetDraw(TRUE);
    for (s32 i = 0; i < pmgEO_->mgDt_.dtBGM_.GetLineNum(); ++i)
    {
      TUGRcNormal* newrecord = MakeObj_Record();
      newrecord->SetCommonID(i);
      newrecord->ugname_.SetNameCol2(hue, i);
    }
    SetZeroPos();//レコードが出来てから戻す
  }
  void TUGLiCol::Refresh_SkinHue()
  {
    ClearRecord();
    SetDraw(TRUE);
    for (s32 i = 0; i < pmgEO_->mgDt_.dtBGM_.GetLineNum(); ++i)
    {
      TUGRcNormal* newrecord = MakeObj_Record();
      newrecord->SetCommonID(i);
      newrecord->ugname_.SetNameSkinCol(i);
    }
    SetZeroPos();//レコードが出来てから戻す
  }
  void TUGLiCol::Refresh_SkinLum(s32 hue)
  {
    ClearRecord();
    SetDraw(TRUE);
    for (s32 i = 0; i < pmgEO_->mgDt_.dtBGM_.GetLineNum(); ++i)
    {
      TUGRcNormal* newrecord = MakeObj_Record();
      newrecord->SetCommonID(i);
      newrecord->ugname_.SetNameSkinCol2(hue, i);
    }
    SetZeroPos();//レコードが出来てから戻す
  }

  void TUGLiCol::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiCol::DoActionSub()
  {
    //選ばれたレコード色づけ
    SetSelect();
  }

  s64 TUGLiCol::GetCommonID()
{
    return records_[GetActIndex()]->GetCommonID();
  }

}
