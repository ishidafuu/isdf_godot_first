#include "dbUGLiMember.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiMember::TUGLiMember(TUIGenLayer* player) :TUGLiGen(player)
  {
    stLiMember_.Init();
  }

  TUGLiMember::~TUGLiMember()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  void TUGLiMember::DoMakeGroupSub()
  {

  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiMember::DoClearRecord()
  {
    stLiMember_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  TUGRcMember* TUGLiMember::MakeObj_Record()
  {
    TUGRcMember* newrecord = new TUGRcMember(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }
  void TUGLiMember::Refresh(s64 id_team, s64 mid_team, BOOL soubi_f)
  {
    //各レコードクリアして入れる
    ClearRecord();
    SetDraw(TRUE);
    stLiMember_.id_team_ = id_team;
    stLiMember_.mid_team_ = mid_team;
    stLiMember_.selectedNo_ = NGNUM;
    stLiMember_.soubi_f_ = soubi_f;
    for (s32 i = 0; i < DBMEMBER_ALL; ++i)
    {
      TUGRcMember* newrecord = (TUGRcMember*)MakeObj_Record();
      newrecord->SetDt(mid_team, i);
    }

    if (soubi_f)
    {
      SetSelect(0);//キャプテン
    }
    else
    {
      SetSelect(NGNUM);//キャプテン
    }
    //SetZeroPos();
  }

  //void TUGLiMember::RefreshShSet(s32 shsetNo)
  //{
  //  ugradio_sub_.SetSelect(shsetNo);
  //}

  void TUGLiMember::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiMember::DoActionSub()
  {
    if (stLiMember_.soubi_f_)
    {
      SetSelect();
    }
    else
    {
      if (GetActIndex() != NGNUM)
      {
        if (stLiMember_.selectedNo_ == GetActIndex())
        {
          stLiMember_.selectedNo_ = NGNUM;
          SetSelect(NGNUM);
        }
        else
        {
          stLiMember_.selectedNo_ = GetActIndex();
          SetSelect();
        }
      }
    }
  }

  BOOL TUGLiMember::IsSelected()
  {
    return (stLiMember_.selectedNo_ != NGNUM);
  }

}
