#pragma once
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
#include "dbUGRcAnimRanker.h"
#include "dbUGLiAnimRanker.h"

//#define __RANKER_DEBUG__
namespace db
{
	// 画面に同時に表示される最大人数 7人
	static const s32 RECORD_DISP_MAX = 7;

	class TUGRanker : public TUGGen
	{
	public:
		//コンストラクタ
		explicit TUGRanker(TUIGenLayer* player);
		virtual ~TUGRanker(void);

		void Init(s32 prev, s32 now);
		void Setup(void);
		BOOL IsEnd(void){ return isEnd_; }

		s32 GetRecordAct(void);
		void SetFade(void);	// 監督を見てる時用(外から呼んでもらう)
		void OutFade(void); // 監督を見終わった時用(外から呼んでもらう)

	protected:
		s32 pageNo_;
		void MvPage_Visit(void);
		void MvPage_Anim1(void);	// X方向に動くアニメーション
		void MvPage_Anim2(void);	// Y方向(orスクロール)アニメーション
		void MvPage_Anim3(void);	// 
		void MvPage_AnimEnd(void);  // 全ての演出終わった後
		void MvPage_Out(void);


		void SetRank(s32 prev, s32 now);
		void SetScrollPos(s32 rank);
		void SetDummyRanker(s32 prev, s32 now, BOOL isDummy);

		// ランカーの表示切替(val1～val2が対象)
		void SetDispRanker(s32 val, s32 val2, BOOL disp);

		// ランキング（ユーザー）のリスト
		TUGLiAnimRanker ugLiRanker_;

		// アニメーション用フレームカウンタ
		s32 frameCnt_;

		// ダミーを使うかどうか
		BOOL isUseDummy_;
		
		BOOL isRankUP_;
		s32 prevRank_;
		s32 nowRank_;
		TUGRcAnimRanker ugMyRcRanker_;

		std::vector<TUGRcAnimRanker*> ugDummyRcRanker_;	


		TUIObj* fade_;	// 監督見てる時用のフェード

		TUIGenLayer* pPlayer_;
		BOOL isEnd_;

		// データサイズ
		s32 dataSize_;

		void DoChangePage(s32 pageno);
		void DoUpdate(BOOL draw_f);

#ifdef __RANKER_DEBUG__
		s32 editMode_;
		s32 rankerNum_;
		void DebugUpdate(void);
#endif
	};
}
