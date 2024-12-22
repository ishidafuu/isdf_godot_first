#include "dbUGChKantoku.h"
#include "dbUILayer.h"

namespace db
{
	//コンストラクタ
	TUGChKantoku::TUGChKantoku(TUIGenLayer* player) :TUGChBaseKantoku(player)
	{
	}

	TUGChKantoku::~TUGChKantoku()
	{
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChKantoku::MakeGroup(TUIObj* parent, BOOL enemy_f)
	{
		TMgCharGrp* pmgcg = (enemy_f)
			? player_->pmgEO_->mgGrp_.pmgcg_enemy_
			: player_->pmgEO_->mgGrp_.pmgcg_kantoku_;

		BASE::MakeGroup(parent, pmgcg);
	}
}
