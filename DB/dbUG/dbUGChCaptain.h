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
	class TUGChCaptain : public TUGChBaseCaptain
	{
		typedef TUGChBaseCaptain BASE;

	public:
		explicit TUGChCaptain(TUIGenLayer* player);
		virtual ~TUGChCaptain(void);

		// 今のチームでキャプテン情報セットアップ
		void SetupMgCharGrp(void);
		void MakeGroup(TUIObj* parent);

	protected:
		void DoActionSub();

		s32 no_;
	};

}
