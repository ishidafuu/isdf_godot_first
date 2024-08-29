//utf8
#pragma once
#include "dbLySTuBase.h"
#include "dbUGButton.h"
//=========================================================================
// 
// サブチュートリアル：チームタブ初選択時の説明
//
//=========================================================================
namespace db
{
	class TUGChGirl;
	class TUGFilter;

	class TLySTuTeam :public TLySTuBase
	{
		typedef TLySTuBase Base;


	public:
		//コンストラクタ
		//explicit TLySTuTeam(TSnTutorial* pScene);
		explicit TLySTuTeam(TSnHome* pScene);
		virtual ~TLySTuTeam(void);

		void Post_STuTeam(void);
		void Connect_STuTeam(void);

	protected:

		//オーバーロード
		void DoLoad();
		void DoReady();
		void DoChangePage(s32 pageno);
		void DoUpdate(BOOL draw_f);
    void DoConnect(BOOL rqok_f);
	private:
		void MvPage_Default(void);
		void MvPage_End(void);

		s32			serifuNo_;
	};

}
