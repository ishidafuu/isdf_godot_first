#pragma once
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
#include "dbUGButton.h"
#include "dbUGTitle.h"
//======================================================================================================
//
//	数字を画像で表示するクラス
//
//	毎フレーム変化するようなものはTUIObjクラスのf_MakeFont関数では表示できないのでこちらを使う
//
//======================================================================================================
#ifdef __K_DEBUG_ROOT__
//#define __TEX_NUM_DEBUG__
#endif
namespace db
{
	class TUGTexNum : public TUGGen
	{
	public:
		//コンストラクタ
		explicit TUGTexNum(TUIGenLayer* player);
		virtual ~TUGTexNum(void);

		void MakeGroup(TUIObj* parent);
		void SetNum(s64 num);
		void SetNum(s64 num, BOOL isDispPlus);
		void SetDispPlus(BOOL isDispPlus);

	protected:
		void CreateTex(s32 keta);
		void ClearTex(void);
		void DoUpdate(BOOL draw_f);

		BOOL isDispPlus_;	// ＋を表示するかどうか
		s64 num_;
		TUIObj* sign_;//符号
		std::vector<TUIObj*> texNum_;

#ifdef __TEX_NUM_DEBUG__
		void UpdateDebug(void);
#endif
	};
}
