#pragma once
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
#include "dbUGButton.h"
#include "dbUGTitle.h"
//======================================================================================================
//
//	チームステータス表示クラス
//
//======================================================================================================
//#define __TM_STATE_PLATE_DEBUG__
namespace db
{
	class TUGTmStatePlate : public TUGGen
	{
		enum
		{
			TM_PARAM_LV = 0,		// Lv
			TM_PARAM_POW,			// ぱわー
			TM_PARAM_GUTS,			// きあい
			TM_PARAM_SHOTNUM,		// そうび
			TM_PARAM_KIRE,			// すぴん
			TM_PARAM_TECH,			// まもり
			TM_PARAM_MAX,
		};

		struct TmState
		{
			s64 lv_;
			s64 shotNum_;
			s64 stPow_;
			s64 stGuts_;
			s64 stKire_;
			s64 stTech_;
			
			void Init(void)
			{
				lv_ = 0;
				shotNum_ = 0;
				stPow_ = 0;
				stGuts_ = 0;
				stKire_ = 0;
				stTech_ = 0;
			}
		};

	public:
		//コンストラクタ
		explicit TUGTmStatePlate(TUIGenLayer* player);
		virtual ~TUGTmStatePlate(void);

		void MakeGroup(TUIObj* parent);
		void SetLRPos(BOOL leftpos_f);
		void SetSt(s64 mid_team, s32 overlap, BOOL leftpos_f);

	protected:
		void SetText(s32 no);
		void DoUpdate(BOOL draw_f);

		// 枠
		TUIObj* uiBg_;

		// テキスト系
		TUIObj* paramNameTxt_[TM_PARAM_MAX];
		TUIObj* paramTxt_[TM_PARAM_MAX];
		TUIObj* paramUpTxt_[TM_PARAM_MAX];

		TUIGenLayer* pPlayer;

		TmState nowSt_;
		TmState prevSt_;

#ifdef __TM_STATE_PLATE_DEBUG__
		void DebugUpdate(void);
#endif
	};
}
