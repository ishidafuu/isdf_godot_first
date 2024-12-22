#include "dbUGLiFace.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiFace::TUGLiFace(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiFace_.Init();
  }

  TUGLiFace::~TUGLiFace()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiFace::DoClearRecord()
  {
    stLiFace_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }


  TUGRcNormal* TUGLiFace::MakeObj_Record()
  {
    TUGRcNormal* newrecord = new TUGRcNormal(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }
  void TUGLiFace::RefreshFace(INTVECTOR dtvec)
  {
    SetDraw(TRUE);

    //各レコードクリアして入れる
    ClearRecord();
    for (s32 i = 0; i < dtvec.size(); ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)MakeObj_Record();
      newrecord->SetCommonID(dtvec[i]);
      newrecord->ugname_.SetNameFace(dtvec[i], i);
    }
    SetZeroPos();

  }
  void TUGLiFace::RefreshCol(INTVECTOR dtvec, BOOL skin_f)
  {
    SetDraw(TRUE);
    //各レコードクリアして入れる
    ClearRecord();
    for (s32 i = 0; i < dtvec.size(); ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)MakeObj_Record();
      newrecord->SetCommonID(dtvec[i]);
      
      s32 colno = (dtvec[i] / 10);
      if (skin_f)
      {
        newrecord->ugname_.SetNameSkinCol(colno);
      }
      else
      {
        newrecord->ugname_.SetNameCol(colno);
      }
    }
    SetZeroPos();
  }
  void TUGLiFace::RefreshCol2(INTVECTOR dtvec, BOOL skin_f)
  {
    SetDraw(TRUE);
    //各レコードクリアして入れる
    ClearRecord();
    for (s32 i = 0; i < dtvec.size(); ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)MakeObj_Record();
      newrecord->SetCommonID(dtvec[i]);
      
      s32 colno = (dtvec[i] / 10);
      s32 colno2 = (dtvec[i] % 10);
      
      if (skin_f)
      {
        newrecord->ugname_.SetNameSkinCol2(colno, colno2);
      }
      else
      {
        newrecord->ugname_.SetNameCol2(colno, colno2);
      }
    }
    SetZeroPos();
  }

  
  void TUGLiFace::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiFace::DoActionSub()
  {
    //選ばれたレコード色づけ
    SetSelect();
  }

  s64 TUGLiFace::GetCommonID()
{
    //レコードからMIDを取得
    return records_[GetActIndex()]->GetCommonID();
  }


}
