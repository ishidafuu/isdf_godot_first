//utf8
#pragma once
#include "dbLyTuGen.h"
#include "dbUGButton.h"
//=========================================================================
// 
// テストクラス
//
//=========================================================================
namespace db
{
	class TUGChGirl;
	class TUGSerifu;
	class TUGFilter;

	class TLyTuTest :public TLyTuGen
	{
		struct HasebeDt
		{
			BOOL isReady_;
			s32 muki_;
			s32 motionNo_;
			void Init(void)
			{
				isReady_	= FALSE;
				muki_		= 0;
				motionNo_	= 0;
			}
		};


	public:
		//コンストラクタ
		explicit TLyTuTest(TSnTutorial* pScene);
		virtual ~TLyTuTest(void);

		BOOL isEndTutorial(void){ return isTutorialEnd_; }

	protected:
		TUIObj* pUIObj_;

		TUGButton ugbtn_;

		s32 haikeiNo_;
		HasebeDt hasebeDt_;

		TUGChGirl* ugHasebe_;
		TUGSerifu* ugserifu_;	//セリフ
		STRVECTOR serifuvec_;

		TUGFilter* ugfilter_;

		BOOL isTutorialEnd_;

		//オーバーロード
		void DoLoad();
		void DoReady();
		void DoChangePage(s32 pageno);
		void DoUpdate(BOOL draw_f);

		void InitTalk1(void);
		void Talk1(void);
		void InitTeamSelect(void);
		void TeamSelect(void);
		void InitTalk2(void);
		void Talk2(void);
		void InitFade(void);
		void Fade(void);
		void InitShiai(void);
		void Shiai(void);

		void SetMsg(const char* text0, const char* text1);
	};

}
