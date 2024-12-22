#pragma once
#include "dbUGChBaseKantoku.h"
//=============================================================================================
//
//	自プレイヤーかんとくクラス
//
//=============================================================================================
namespace db
{
	class TUGChKantoku : public TUGChBaseKantoku
	{
		typedef TUGChBaseKantoku BASE;

	public:
		explicit TUGChKantoku(TUIGenLayer* player);
		virtual ~TUGChKantoku(void);
		void MakeGroup(TUIObj* parent, BOOL enemy_f);
	};
}
