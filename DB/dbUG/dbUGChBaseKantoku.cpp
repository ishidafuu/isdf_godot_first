#include "dbUGChBaseKantoku.h"
#include "dbUILayer.h"
//=============================================================================================
//
//	かんとくベースクラス
//
//=============================================================================================
namespace db
{

	//-----------------------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------------------
	TUGChBaseKantoku::TUGChBaseKantoku(TUIGenLayer* player) :TUGChGen(player)
	{
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	TUGChBaseKantoku::~TUGChBaseKantoku()
	{
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBaseKantoku::MakeGroup(TUIObj* parent, TMgCharGrp* pmgcg)
	{
		GenMakeGroup(parent, pmgcg, &player_->pmgEO_->mgDt_.dtBM_kantoku_, 0);
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBaseKantoku::SetMType(enKantokuMotionType mtype)
	{
		SetGenMType(mtype);
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBaseKantoku::z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd)
	{
		z_MoveWork(moveno, tagx, tagy, spd, cmk_None);
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBaseKantoku::z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki)
	{
		field_->z_MoveSetspd(tagx, tagy, spd);
		stChar_.moveNo_ = moveno;
		stChar_.muki_ = muki;
		if (field_->stSO_.stZahyou_.eat_ > 1)
		{
			SetMType(kmt_Wk);
			SetMirror((field_->stSO_.stZahyou_.dx_ < 0));
		}
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBaseKantoku::z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd)
	{
		z_MoveWork(moveno, tagx - mid::midFullDotL_Hf(), tagy - mid::midFullDotS_Hf(), spd, cmk_None);
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBaseKantoku::z_MoveWork_FullMuki(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki)
	{
		z_MoveWork(moveno, tagx - mid::midFullDotL_Hf(), tagy - mid::midFullDotS_Hf(), spd, muki);
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBaseKantoku::z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd, f32 lookx)
	{
		enUGChGenMuki muki = (tagx < lookx) ? cmk_Right : cmk_Left;
		z_MoveWork_FullMuki(moveno, tagx, tagy, spd, muki);
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBaseKantoku::DoActionSub()
	{
		if (IsAct())
		{
			switch (GetActRes())
			{
			case enUGChGenAct_MotihonEnd:
			case enUGChGenAct_MoveStop:
				SetMType(kmt_St);
				break;
			}
		}
	}
}
