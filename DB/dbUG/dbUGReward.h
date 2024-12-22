#pragma once
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
#include "dbUGButton.h"
#include "dbUGTitle.h"

//#define __REWARD_DEBUG__
namespace db
{
	class TUGReward : public TUGGen
	{
	public:
		//コンストラクタ
		explicit TUGReward(TUIGenLayer* player);
		virtual ~TUGReward(void);

		void Init(void);
		void SetPos(s32 x, s32 y);

		void SetNowData(s32 rank);	// 今の順位の報酬
		void SetNextData(s32 rank, s32 next); // 次の順位の報酬
		void SetRankOutData(void);	// 今の順位に何も報酬がないとき
		void SetEtcData(void);	// 順位とか関係なしのバージョン

		void SetDraw(BOOL isDraw); // UIパーツの表示

		void SetFadeHide(BOOL isHide); // 報酬を見てる時用のUIを全て消す
		void SetFade(void);	// 報酬を見てる時用(外から呼んでもらう)
		void OutFade(void); // 報酬を見終わった時用(外から呼んでもらう)

	protected:
		s32 pageNo_;
		void MvPage_Idle(void);
		void MvPage_List(void);
		void MvPage_Fade(void);
		void MvPage_Out(void);

		TUIObj* uiBg_;			// 枠
		//base::TGrPart* pGrpBg_;	// 枠のデータ


		TUGButton* ugBtn_; // 中身見るボタン

		TUIObj* fade_;	// 報酬見てる時用のフェード
		TUGTitle* ugTitleBar_;

		// テキスト系
		TUIObj* title_;
		TUIObj* text_;
		TUIObj* text2_;

		BOOL isUseText2_;
		BOOL isTopRank_;
		BOOL isEnd_;

		TUIGenLayer* pPlayer;

		void DoChangePage(s32 pageno);
		void DoUpdate(BOOL draw_f);

#ifdef __REWARD_DEBUG__
		void DebugUpdate(void);
#endif
	};
}
