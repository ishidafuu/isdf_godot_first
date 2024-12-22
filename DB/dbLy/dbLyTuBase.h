//utf8
#pragma once
#include "dbLyTuGen.h"
#include "dbUGButton.h"
#include "dbUGSerifu.h"
#include "dbUGFilter.h"
#include "dbUGFinger.h"
//=========================================================================
// 
// チュートリアル：ベースクラス
//
//=========================================================================
namespace db
{
	class TUGChGirl;
	class TUGFilter;

	class TLyTuBase :public TLyTuGen
	{

	public:
		//コンストラクタ
		explicit TLyTuBase(TSnTutorial* pScene);
		virtual ~TLyTuBase(void);
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
		void ConvCaptainName(std::string* s);
	};

}
