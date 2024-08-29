#pragma once
#include "dbLyShGen.h"
#include "dbUGChKantoku.h"
#include "dbUGRanker.h"
#include "dbUGReward.h"
#include "dbUGPlateProf.h"
#include "dbStSnShiai.h"

namespace db
{
	class TLyShRank :public TLyShGen
	{
	public:
		//コンストラクタ
		explicit TLyShRank(TSnShiai* pScene);
		virtual ~TLyShRank(void);

	protected:
		void MvPage_Visit(void);
		void MvPage_Ranker(void);
		void MvPage_NowList(void);
		void MvPage_NextList(void);
		void MvPage_Preview(void);
		void MvPage_Kantoku(void);
		void MvPage_Idle(void);
		void MvPage_Out(void);
		s32 prevPage_;

		s64 mid_ranking_;

		s64 nowRank_;			// 今のランク
		s64 prevRank_;			// 試合前のランク
		s64 nowRewardRank_;		// 今の報酬のランク
		s64 nextRewardRank_;	// 次の報酬のランク(bottom値)

		TUGButton endBtn_;
		TUGButton backBtn_;
		TUGButton prevBtn_;

		TUGRanker ugRanker_;
		TUGReward ugNowReward_;
		TUGReward ugNextReward_;

		// シュートプレビュー用
		s64 mid_sozai_;
		INTVECTOR syouhinVec_;
		SOZAIDTVECTOR prevSozaiVec_;


		TStSnShiai::enShiaiType shiaiType_;

		// 他プレイヤープレビュー用
		TUGPlateProf ugProf_;

		//オーバーロード
		void DoLoad();
		void DoReady();
		void DoDown();
		void DoChangePage(s32 pageno);
		void DoUpdate(BOOL draw_f);

		//void Post_NowReward(void);
		//void Post_NextReward(void);
		//void Connect_NowReward(void);
		//void Connect_NextReward(void);
		void Post_Ranker(void);
		void Connect_Ranker(void);
		void DoConnect(BOOL rqok_f);
	};
}
