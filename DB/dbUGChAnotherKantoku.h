#pragma once
#include "dbUGChBaseKantoku.h"
//=============================================================================================
//
//	別プレイヤーかんとくクラス
//
//=============================================================================================
namespace db
{
	class TUGChAnotherKantoku : public TUGChBaseKantoku
	{
		typedef TUGChBaseKantoku BASE;

	public:
		//コンストラクタ
		explicit TUGChAnotherKantoku(TUIGenLayer* player, s32 no);
		virtual ~TUGChAnotherKantoku(void);
		void SetChDt(s32 teamid, s32 charano);
		void SetChDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 clothecol);
		void SetChDtRnd(void);	// デバッグ機能
		void MakeGroup(TUIObj* parent);

		void SetScreenOut(s32 side, s32 sp);	// 画面外へ
		void SetScreenIn(s32 side, s32 tx, s32 ty, s32 sp);
	protected:

		s32 no_;	// 自身の番号
		BOOL isAutoWalk_;
	};
}
