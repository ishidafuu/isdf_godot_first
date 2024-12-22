#include "dbUGChAnotherKantoku.h"
#include "dbUILayer.h"

namespace db
{
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	TUGChAnotherKantoku::TUGChAnotherKantoku(TUIGenLayer* player, s32 no)
		: TUGChBaseKantoku(player)
		, no_(no)
		, isAutoWalk_(FALSE)
	{
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	TUGChAnotherKantoku::~TUGChAnotherKantoku()
	{
	}

	//-----------------------------------------------------------------
	// キャラデータセット
	//-----------------------------------------------------------------
	void TUGChAnotherKantoku::SetChDt(s32 teamid, s32 charano)
	{
		BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(teamid, mdm_team_gb_f);
		s64 charID = pmgEO_->mgCoM_.GetCharID(teamid, charano);
		pmgEO_->mgGrp_.pmgcg_enemies_[no_]->SetCharDt(charID, teamid, gb_f);
		pmgEO_->mgGrp_.MakeEnemiesTexture(no_);
	}

	//-----------------------------------------------------------------
	// キャラデータセット
	//-----------------------------------------------------------------
	void TUGChAnotherKantoku::SetChDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 clothecol)
	{
		pmgEO_->mgGrp_.pmgcg_enemies_[no_]->SetCharDt(hair, face, haircol, skincol, eyecol, acccol, clothecol, false);
		pmgEO_->mgGrp_.MakeEnemiesTexture(no_);
	}
	// デバッグ機能
	void TUGChAnotherKantoku::SetChDtRnd(void)
	{
		// ランダムでチョイス
		s32 hair = 0;
		while (1)
		{
			hair = mid::midGetRandMn(pmgEO_->mgDt_.dtChar_.GetLineNum());
			// 出して良いキャラか判定
			s32 hairBitFlg = pmgEO_->mgDt_.dtChar_.GetLooks(clk_SearchFlag, hair);
			if (hairBitFlg & sf_Hair){ break; }
		}
		s32 face = 0;
		while(1)
		{
			face = mid::midGetRandMn(pmgEO_->mgDt_.dtChar_.GetLineNum());
			// 出して良いキャラか判定
			s32 faceBitFlg = pmgEO_->mgDt_.dtChar_.GetLooks(clk_SearchFlag, face);
			if (faceBitFlg & sf_Face){ break; }
		}

		// カラーテーブル1(肌の色)
		const s32 SKIN_COL_TBL_MAX = 9;
		const s32 SkinColTbl[SKIN_COL_TBL_MAX] =
		{
			// 明るい		// 暗い		
			0, 1, 2,		// 白人	
			10, 11, 12,		// 黄色人
			20, 21, 22,		// 黒人
		};

		// カラーテーブル2(肌以外の色)
		const s32 ETC_COL_TBL_MAX = 52;
		const s32 EtcColTbl[ETC_COL_TBL_MAX] =
		{
			// 明るい				// 暗い
			0, 1, 2, 3,		// 白～黒
			10, 11, 12, 13,		// 
			20, 21, 22, 23,
			30, 31, 32, 33,
			40, 41, 42, 43,
			50, 51, 52, 53,
			60, 61, 62, 63,
			70, 71, 72, 73,
			80, 81, 82, 83,
			90, 91, 92, 93,
			100, 101, 102, 103,
			110, 111, 112, 113,
			120, 121, 122, 123
		};

		s32 haircol = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];
		s32 skincol = SkinColTbl[mid::midGetRandMn(SKIN_COL_TBL_MAX)];
		s32 eyecol = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];
		s32 acccol = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];
		s32 clothecol = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];

		pmgEO_->mgGrp_.pmgcg_enemies_[no_]->SetCharDt(hair, face, haircol, skincol, eyecol, acccol, clothecol, false);
		pmgEO_->mgGrp_.MakeEnemiesTexture(no_);
	}

	//-----------------------------------------------------------------
	// 現在のデータから自身を生成
	//-----------------------------------------------------------------
	void TUGChAnotherKantoku::MakeGroup(TUIObj* parent)
	{
		BASE::MakeGroup(parent, player_->pmgEO_->mgGrp_.pmgcg_enemies_[no_]);
	}


	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChAnotherKantoku::SetScreenOut(s32 side, s32 sp)
	{
		f32 x = 0;
		f32 y = 0;
		GetField()->z_GetPos(&x, &y);

		// 左
		if (side == 0)
		{
			// キャプテンを連れて画面外なので結構画面外へ
			z_MoveWork(0, -mid::midGetDotL_Hf() - 60, y, sp);
		}
		// 右
		else
		{
			z_MoveWork(0, mid::midGetDotL_Hf() + 60, y, sp);
		}
		isAutoWalk_ = TRUE;
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChAnotherKantoku::SetScreenIn(s32 side, s32 tx, s32 ty, s32 sp)
	{
		f32 x = 0;
		f32 y = 0;

		// 左
		if (side == 0)
		{
			SetPos_Full(0 - 60, ty);
			z_MoveWork_Full(0, tx, ty, sp);
		}
		// 右
		else
		{
			SetPos_Full(mid::midGetDotL() + 60, ty);
			z_MoveWork_Full(0, tx, ty, sp);
		}
		isAutoWalk_ = TRUE;
	}
}
