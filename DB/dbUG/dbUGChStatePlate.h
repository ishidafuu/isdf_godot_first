#pragma once
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
#include "dbUGButton.h"
#include "dbUGTitle.h"
#include "dbUGTexNum.h"
//======================================================================================================
//
//	キャラステータス表示クラス
//
//======================================================================================================
//#define __TM_STATE_PLATE_DEBUG__
namespace db
{
	class TUGChStatePlate : public TUGGen
	{
		enum
		{
			TM_PARAM_LV = 0,		// Lv
			TM_PARAM_POW,			// ぱわー
			TM_PARAM_GUTS,			// きあい
			TM_PARAM_KIRE,			// すぴん
			TM_PARAM_TECH,			// まもり
			TM_PARAM_EXP,			// 経験値
			TM_PARAM_MAX,
		};

		struct ChState
		{
			s64 lv_;
			s64 exp_;
			s64 stPow_;
			s64 stGuts_;
			s64 stKire_;
			s64 stTech_;
			
			void Init(void)
			{
				lv_ = 0;
				exp_ = 0;
				stPow_ = 0;
				stGuts_ = 0;
				stKire_ = 0;
				stTech_ = 0;
			}
		};

	public:
		//コンストラクタ
		explicit TUGChStatePlate(TUIGenLayer* player);
		virtual ~TUGChStatePlate(void);

		void MakeGroup(TUIObj* parent);
		void SetSt(s64 mid_team, s32 posNo, s32 overlap);	// 現在のデータをセット(即反映)
		void SetAnim(f64 addExp);	// キャラ育成時の経験値増加アニメ
		void FinishAnim(void);		// アニメ強制終了
		BOOL IsAnim(void){ return isExpAnim_; }

	protected:
		void SetText(s32 no);
		void SetExpBar(s64 now, s64 next);
		void DoUpdate(BOOL draw_f);
		
		void UpdateExpAnim(void);

		// 枠
		TUIObj* uiBg_;
		// 体
		TUIObj* uiBody_;
		// 名前
		TUIObj* uiName_;

		// 経験値バー(枠部分)
		TUIObj* uiExpBarBody_;
		// 経験値バー(ゲージ部分)
		TUIObj* uiExpBar_;
		
		BOOL isExpAnim_;	// アニメーション中フラグ
		s64 addExp_;
		f64 animMoveExp_;	// １回のアニメで動く値
		//f64 animExp_;		// アニメ中の値


		// テキスト系
		TUIObj* paramNameTxt_[TM_PARAM_MAX];
		TUIObj* paramTxt_[TM_PARAM_MAX];
		TUIObj* paramUpTxt_[TM_PARAM_MAX];

		// 毎フレーム変化するような値はテクスチャを使う
		TUGTexNum ugExp_;
		TUGTexNum ugExpUp_;

		TUIObj* touchField_;	// スキップ用タッチ領域

		TUIGenLayer* pPlayer;

		s64 id_chara_;
		s64 mid_team_;
		s64 id_team_;
		s32 posNo_;
		s64 teamLv_;

		BOOL isLvMax_;

		std::string strName_;
		ChState nowSt_;
		ChState prevSt_;
		ChState upSt_;
		s64 nextExp_;

#ifdef __TM_STATE_PLATE_DEBUG__
		void DebugUpdate(void);
#endif
	};
}
