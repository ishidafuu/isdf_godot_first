#include "dbUGChGirl.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGChGirl::TUGChGirl(TUIGenLayer* player) :TUGChGen(player)
  {
    stGirl_.Init();
  }

  TUGChGirl::~TUGChGirl()
  {

  }
  void TUGChGirl::MakeGroup(TUIObj* parent, s32 girlNo, BOOL wonder_f)
  {
    const s32 GIRLBMPNUM = 6;
    GenMakeGroup(parent, player_->pmgEO_->mgGrp_.pmgcg_girl_, &player_->pmgEO_->mgDt_.dtBM_girl_, (girlNo * GIRLBMPNUM));
    stGirl_.wonder_f_ = wonder_f;
  }

  void TUGChGirl::SetMType(enGirlMotionType mtype)
  {
    SetGenMType(mtype);
  }
  void TUGChGirl::SetWander(s32 stx, s32 edx, s32 liney)
  {
    SetGenMType(gmt_Wk);
    stx -= mid::midFullDotL_Hf();
    edx -= mid::midFullDotL_Hf();
    liney -= mid::midFullDotS_Hf();
    stGirl_.stx_ = stx;
    stGirl_.edx_ = edx;
    stGirl_.liney_ = liney;
    field_->z_SetPos(stx, liney);
    field_->z_MoveSetspd(edx, liney, 1);
    SetMirror((field_->stSO_.stZahyou_.dx_ < 0));
  }
  void TUGChGirl::z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki)
  {
    field_->z_MoveSetspd(tagx, tagy, spd);
    stChar_.moveNo_ = moveno;
    stChar_.muki_ = muki;
    if (field_->stSO_.stZahyou_.eat_ > 1)
    {
      SetMType(gmt_Wk);
      SetMirror((field_->stSO_.stZahyou_.dx_ < 0));
    }
  }

  void TUGChGirl::NextWander()
  {
    SetGenMType(gmt_Wk);
    field_->z_SetPos(stGirl_.stx_, stGirl_.liney_);
    field_->z_MoveSetspd(stGirl_.edx_, stGirl_.liney_, 1);
    SetMirror((field_->stSO_.stZahyou_.dx_ < 0));
  }

  void TUGChGirl::DoActionSub()
  {
    if (IsAct())
    {
      switch (GetActRes())
      {
        //case enUGChGenAct_MotihonEnd:
      case enUGChGenAct_MoveStop:
        if (stGirl_.isFrameOut_)
        {
          SetMType(gmt_St);
        }
        else
        {
          if (stGirl_.wonder_f_)
          {
            NextWander();
          }
          else
          {
            SetMType(gmt_St);
          }
        }
        break;
      }
    }

  }

}
