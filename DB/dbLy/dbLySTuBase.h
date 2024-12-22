//utf8
#pragma once
#include "dbLyHmGen.h"
#include "dbLyTuGen.h"
#include "dbUGButton.h"
#include "dbUGSerifu.h"
#include "dbUGFilter.h"
#include "dbUGFinger.h"
//=========================================================================
// 
// サブチュートリアル：ベースクラス
//
//=========================================================================
namespace db
{
	class TUGChGirl;
	class TUGFilter;

	class TLySTuBase :public /*TLyTuGen*/ TLyHmGen
	{

	public:
		//コンストラクタ
		//explicit TLySTuBase(TSnTutorial* pScene);
		explicit TLySTuBase(TSnHome* pScene);
		virtual ~TLySTuBase(void);
		BOOL IsTutoEnd(void){ return isTutoEnd_; }

	protected:
		TUGSerifu ugserifu_;	//セリフ
		TUGFilter ugfilter_;
		TUGFinger ugfinger_;

		STRVECTOR serifuvec_;

		s32 haikeiNo_;
		BOOL isTutoEnd_;

		//オーバーロード
		void DoLoad();
		void DoReady();
		void DoChangePage(s32 pageno);
		void DoUpdate(BOOL draw_f);

		void SetMsg(const char* text0, const char* text1 = NULL, const char* text2 = NULL);
	};

}
