#pragma once
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
#include "dbUGButton.h"
#include "dbUGTitle.h"
//======================================================================================================
//
//	ホーム画面に配置される所持チーム中の最強チーム表示掲示板
//
//======================================================================================================
//#define __TM_STATE_PLATE_DEBUG__
namespace db
{
	class TUGTmBoard : public TUGGen
	{

	public:
		//コンストラクタ
		explicit TUGTmBoard(TUIGenLayer* player);
		virtual ~TUGTmBoard(void);

		void MakeGroup(TUIObj* parent);
		void SetTopTeamDt(void);
		void SetTeamDt(s64 id_team);

	protected:
		void DoUpdate(BOOL draw_f);

		// 枠
		TUIObj* uiBg_;

		TUGRIcon uiTeamIcon_;
		TUIObj* uiTeamName_;
		TUIObj* uiTeamState_;

		TUGRIcon uiTeamShotIcon_;
		TUIObj* uiTeamShotName_;
		TUIObj* uiTeamShotState_;

		TUIGenLayer* pPlayer;
	};
}
