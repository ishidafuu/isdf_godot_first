//utf8
#pragma once
#include "dbLySTuBase.h"
#include "dbUGButton.h"
//=========================================================================
// 
// サブチュートリアル：試合タブ選択時の説明
//
//=========================================================================
namespace db
{
	class TUGChGirl;
	class TUGFilter;

	class TLySTuShiai :public TLySTuBase
	{
		typedef TLySTuBase Base;


	public:
		//コンストラクタ
		//explicit TLySTuShiai(TSnTutorial* pScene);
		explicit TLySTuShiai(TSnHome* pScene);
		virtual ~TLySTuShiai(void);

		void Post_STuShiai(void);
		void Connect_STuShiai(void);

	protected:

		//オーバーロード
		void DoLoad();
		void DoUpdate(BOOL draw_f);
    void DoConnect(BOOL rqok_f);

	private:
		void MvPage_Init(void);
		void MvPage_SubTuto1(void);
		void MvPage_SubTuto2(void);
		void MvPage_End(void);
		s32	serifuNo_;
	};

}
