#include "dbUGChClerk.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGChClerk::TUGChClerk(TUIGenLayer* player) :TUGChGen(player)
  {
  }

  TUGChClerk::~TUGChClerk()
  {

  }
  void TUGChClerk::MakeGroup(TUIObj* parent, enClerkType ctype)
  {
    const s32 CLERKBMPNUM = 10;
    GenMakeGroup(parent, player_->pmgEO_->mgGrp_.pmgcg_clerk_, &player_->pmgEO_->mgDt_.dtBM_clerk_, (ctype * CLERKBMPNUM));
  }

  void TUGChClerk::SetMType(enClerkMotionType mtype)
  {
    SetGenMType(mtype);
  }
  void TUGChClerk::z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki)
  {
	  field_->z_MoveSetspd(tagx, tagy, spd);
	  stChar_.moveNo_ = moveno;
	  stChar_.muki_ = muki;
	  if (field_->stSO_.stZahyou_.eat_ > 1)
	  {
		  SetMType(cmt_Walk);
		  SetMirror((field_->stSO_.stZahyou_.dx_ < 0));
	  }
  }

  void TUGChClerk::DoActionSub()
  {
    if (IsAct())
    {
      switch (GetActRes())
      {
      case enUGChGenAct_MotihonEnd:
        SetMType(cmt_Stand);
        break;
      case enUGChGenAct_MoveStop:
        SetMType(cmt_Stand);
        break;
      }
    }

  }

}
