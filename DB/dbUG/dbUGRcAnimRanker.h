#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
namespace db
{

	class TUGRcAnimRanker :public TUGRcNormal
	{
	public:
		//コンストラクタ
		explicit TUGRcAnimRanker(TUIGenLayer* player);
		virtual ~TUGRcAnimRanker(void);
		void SetRankerDt(s64 id_ranker, s32 lineno);	// RANKERNUM人まで対応バージョン
		void SetRankerDtDummy(s64 dataRank, s64 dispRank);

		// 指定座標に移動
		void SetMoveTargetX(s32 tx, s32 frame);
		void SetMoveTargetY(s32 ty, s32 frame);

		// 指定値だけ移動
		void SetMoveX(s32 mx, s32 frame);
		void SetMoveY(s32 my, s32 frame);
		BOOL IsMove(void){ return isMove_; }

	protected:

		std::vector<std::vector<f32>> moveList_;
		BOOL isMove_;
		s32 frameCnt_;
		INTVECTOR initPos;
		INTVECTOR lastPos;

		void SetScore(s64 id_ranker);
		void DoMakeGroupSub2();
		void DoSetLimY2(s32 limu, s32 hhh);
		void DoUpdateSub2(BOOL draw_f);
	};
}
