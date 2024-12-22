#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
//=====================================================================================================
//
//	バナー用パラメータクラス
//
//=====================================================================================================
namespace db
{

	class TUGBannerParam
	{
	public:
		TUGBannerParam(void);
		virtual ~TUGBannerParam(void);

#ifdef __K_DEBUG_ROOT__
		void SetupDebugParam1(void);
		void SetupDebugParam2(void);
		void SetupDebugParam3(void);
#endif

		//------------------------------------------------------------
		// セッター
		void SetBannerID(s32 id){ bannerID_ = id; }
		void SetText(STRVECTOR v) { textVec_ = v; }
		void SetTextDispType(INTVECTOR v) { textDrawTypeVec_ = v; }
		void SetDispChangeType(INTVECTOR v){ dispChangeTypeVec_ = v; }
    void SetDate(s64 st_date, s64 ed_date) { startDate_ = st_date; endDate_ = ed_date; }
		//------------------------------------------------------------
		// ゲッター
		s32 GetBannerID(void){ return bannerID_; }
		STRVECTOR GetText(void){ return textVec_; }
		std::string GetText(s32 no)
		{
			if (no >= textVec_.size()){ return ""; }
			return textVec_[no];
		}
		INTVECTOR GetTextDrawType(void){ return textDrawTypeVec_; }
		s32 GetTextDrawType(s32 no)
		{
			if (no >= textDrawTypeVec_.size()){ return NGNUM; }
			return textDrawTypeVec_[no];
		}
		INTVECTOR GetDispChangeType(void){ return dispChangeTypeVec_; }
		s32 GetDispChangeType(s32 no)
		{
			if (no >= dispChangeTypeVec_.size()){ return NGNUM; }
			return dispChangeTypeVec_[no];
		}

		// 今バナーを表示する時間かどうか
		BOOL IsDrawDate(void);

	protected:
		s32 bannerID_;					// バナー画像ID
		STRVECTOR textVec_;				// テキストリスト
		INTVECTOR textDrawTypeVec_;		// テキストの表示方法(テキストリストと数は必ず一致)
		INTVECTOR dispChangeTypeVec_;	// ディスプレイの切替方法(3つ)
		s64 startDate_;		// 表示開始時間
		s64 endDate_;		// 表示終了時間
	};
}
