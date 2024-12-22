#pragma once
#include "dbConst_Tuto.h"
#include "dbLyShiaiTutoGen.h"
#include "dbUGTutoMsgAuto.h"
#include "dbUGFinger.h"

namespace db
{
	class TSnShiai;
	class TLyShiaiTutoAuto :public TLyShiaiTutoGen
	{
	public:
		//コンストラクタ
		explicit TLyShiaiTutoAuto(TSnShiai* pScene, TUGShiai* ugshiai);
		virtual ~TLyShiaiTutoAuto(void);

	protected:
		void MvPage_GameAct();
		//void MvPage_Fin();

		//オーバーロード
		void DoLoadSub();
		void DoReadySub();
		void DoUpdateSub();
		void DoChangePage(s32 pageno);
		void DoUpdateSub(BOOL draw_f);
	};
}
