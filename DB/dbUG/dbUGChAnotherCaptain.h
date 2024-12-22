#pragma once
#include "dbUGChBaseCaptain.h"
//=============================================================================================
//
//	別プレイヤーキャプテンクラス
//
//=============================================================================================
namespace db
{
	//リスト
	class TUGChAnotherCaptain : public TUGChBaseCaptain
	{
		typedef TUGChBaseCaptain BASE;

	public:

		//コンストラクタ
		explicit TUGChAnotherCaptain(TUIGenLayer* player, s32 no);
		virtual ~TUGChAnotherCaptain(void);

		void SetChDt(s32 teamid, s32 charano);
		void SetChDtRnd(void);	// デバッグ機能
		void MakeGroup(TUIObj* parent);
	protected:
		void DoActionSub();

		s32 no_;
	};

}
