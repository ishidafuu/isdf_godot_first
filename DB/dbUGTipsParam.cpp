#include "dbUGTipsParam.h"
#include "dbUILayer.h"
#include "dbUGTipsManager.h"
#include "dbUGChBasePerson.h"
//=====================================================================================================
//
//	Tipsキャラ用パラメータクラス
//
//=====================================================================================================
namespace db
{
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGTipsParam::TUGTipsParam(void)
	{
		isEndTips_ = FALSE;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGTipsParam::~TUGTipsParam()
	{

	}
#ifdef __K_DEBUG_ROOT__
	//--------------------------------------------------------------------------------------------
	// デバッグパラメータセット
	//--------------------------------------------------------------------------------------------
	void TUGTipsParam::SetupDebugParam(void)
	{
		// ランダムでチョイス
		while (1)
		{
			hairNo_ = mid::midGetRandMn(pPlayer_->pmgEO_->mgDt_.dtChar_.GetLineNum());
			// 出して良いキャラか判定
			s32 hairBitFlg = pPlayer_->pmgEO_->mgDt_.dtChar_.GetLooks(clk_SearchFlag, hairNo_);
			if (hairBitFlg & sf_Hair){ break; }
		}

		while (1)
		{
			faceNo_ = mid::midGetRandMn(pPlayer_->pmgEO_->mgDt_.dtChar_.GetLineNum());
			// 出して良いキャラか判定
			s32 faceBitFlg = pPlayer_->pmgEO_->mgDt_.dtChar_.GetLooks(clk_SearchFlag, faceNo_);
			if (faceBitFlg & sf_Face){ break; }
		}

		// カラーテーブル1(肌の色)
		const s32 SKIN_COL_TBL_MAX = 9;
		const s32 SkinColTbl[SKIN_COL_TBL_MAX] =
		{
			// 明るい		// 暗い		
			0, 1, 2,		// 白人	
			10, 11, 12,		// 黄色人
			20, 21, 22,		// 黒人
		};

		// カラーテーブル2(肌以外の色)
		const s32 ETC_COL_TBL_MAX = 52;
		const s32 EtcColTbl[ETC_COL_TBL_MAX] =
		{
			// 明るい				// 暗い
			0, 1, 2, 3,		// 白～黒
			10, 11, 12, 13,		// 
			20, 21, 22, 23,
			30, 31, 32, 33,
			40, 41, 42, 43,
			50, 51, 52, 53,
			60, 61, 62, 63,
			70, 71, 72, 73,
			80, 81, 82, 83,
			90, 91, 92, 93,
			100, 101, 102, 103,
			110, 111, 112, 113,
			120, 121, 122, 123
		};

		colHair_ = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];

		colSkin_ = SkinColTbl[mid::midGetRandMn(SKIN_COL_TBL_MAX)];

		colEye_ = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];
		
		colAcc_ = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];

		colWear_ = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];

		colPants_ = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];

		colSide_ = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];

		colUnder_ = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];

		colShoe_ = EtcColTbl[mid::midGetRandMn(ETC_COL_TBL_MAX)];


		// 1/5の確立でGBカラー
		isGB_ = (mid::midGetRandMn(5) == 0) ? TRUE : FALSE;

		speedType_ = mid::midGetRandMn(6);

		personType_ = (enPersonType)mid::midGetRandMn(pt_Max);
		name_ = pPlayer_->pmgEO_->mgDt_.dtChar_.GetName(faceNo_);

		isEndTips_ = FALSE;
	}
	void TUGTipsParam::SetupDebugParam1(void)
	{
		id_ = 1;
		SetupDebugParam();
		STRVECTOR2 msg =
		{
			{ "$PERSON_NAME$「てすと１", "ひとりめ" },
		};
		msg_ = msg;
		startDate_ = mid::midNowTime() - 100;
		endDate_ = mid::midNowTime() + 1000;
	}
	void TUGTipsParam::SetupDebugParam2(void)
	{
		id_ = 2;
		SetupDebugParam();
		STRVECTOR2 msg =
		{
			{ "$PERSON_NAME$「てすと２", "ふたりめ" },
			{ "$PERSON_NAME$「ふひひ", },
		};
		msg_ = msg;
		startDate_ = mid::midNowTime() - 100;
		endDate_ = mid::midNowTime() + 1000;
	}
	void TUGTipsParam::SetupDebugParam3(void)
	{
		id_ = 3;
		SetupDebugParam();
		STRVECTOR2 msg =
		{
			{ "$PERSON_NAME$「てすと３", "さんにんめ" },
			{ "$PERSON_NAME$「なめんなよ", },
			{ "$PERSON_NAME$「ふひひ", },
		};
		msg_ = msg;
		startDate_ = mid::midNowTime() - 100;
		endDate_ = mid::midNowTime() + 1000;
	}
#endif

	//--------------------------------------------------------------------------------------------
	// 今バナーを表示する時間かどうか
	//--------------------------------------------------------------------------------------------
	BOOL TUGTipsParam::IsDrawDate(void)
	{
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
