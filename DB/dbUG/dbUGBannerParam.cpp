#include "dbUGBannerParam.h"
#include "dbUILayer.h"
#include "dbUGBanner.h"
//=====================================================================================================
//
//	バナー用パラメータクラス
//
//=====================================================================================================
namespace db
{
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGBannerParam::TUGBannerParam(void)
	{



	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGBannerParam::~TUGBannerParam()
	{

	}
#ifdef __K_DEBUG_ROOT__
	//--------------------------------------------------------------------------------------------
	// デバッグパラメータセット
	//--------------------------------------------------------------------------------------------
	void TUGBannerParam::SetupDebugParam1(void)
	{
		bannerID_ = 1;
		textVec_ = { "だみーの＿てきすとを＿すくろーる＿させています心", "ふらっしゅ！" };
		textDrawTypeVec_ = { etdt_Scroll, etdt_Flash };
		dispChangeTypeVec_ = { edct_SnowNoise, edct_SnowNoise, edct_SnowNoise };
		startDate_ = mid::midNowTime() - 100;
		endDate_ = mid::midNowTime() + 1000;
	}
	void TUGBannerParam::SetupDebugParam2(void)
	{
		bannerID_ = 2;
		textVec_ = { "ふたつめの＿ばなーを＿ひょうじちゅう", "のんびり＿すくろーる＿なう", "かってね！" };
		textDrawTypeVec_ = { etdt_Scroll, etdt_Scroll, etdt_Flash };
		dispChangeTypeVec_ = { edct_SnowNoise, edct_SnowNoise, edct_SnowNoise };
		startDate_ = mid::midNowTime() - 100;
		endDate_ = mid::midNowTime() + 1000;
	}
	void TUGBannerParam::SetupDebugParam3(void)
	{
		bannerID_ = 3;
		textVec_ = { "みっつめ！", "ふらっしゅで！", "ひょうじ！" };
		textDrawTypeVec_ = { etdt_Flash, etdt_Flash, etdt_Flash };
		dispChangeTypeVec_ = { edct_SnowNoise, edct_SnowNoise, edct_SnowNoise };
		startDate_ = mid::midNowTime() - 100;
		endDate_ = mid::midNowTime() + 1000;
	}
#endif

	//--------------------------------------------------------------------------------------------
	// 今バナーを表示する時間かどうか
	//--------------------------------------------------------------------------------------------
	BOOL TUGBannerParam::IsDrawDate(void)
	{
#ifdef __K_DEBUG_ROOT__
		return TRUE;
#endif

		// 今の時間
		s64 nowDate = mid::midNowTime();

		// 表示開始時間過ぎていない
		if (nowDate <= startDate_) { return FALSE; }
		
		// 表示終了時間を過ぎている
		if (nowDate >= endDate_) { return FALSE; }

		// 今表示して大丈夫
		return TRUE;
	}
}
