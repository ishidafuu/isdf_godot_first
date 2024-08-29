#pragma once
#include "dbInclude.h"
#include "dbUGLiMenu.h"
#include "dbUGRcNormal.h"
//=====================================================================================================
//
//
//
//=====================================================================================================
namespace db
{
	//
	class TUGLiRkReward :public TUGLiGen
	{
	public:
		//コンストラクタ
		explicit TUGLiRkReward(TUIGenLayer* player);
		virtual ~TUGLiRkReward(void);
		TUGRcNormal* MakeObj_Record();

		s64 GetCommonID(void);

	protected:
		void DoUpdateSub(BOOL draw_f);
		void DoMakeGroupSub();
		void DoClearRecord();

		std::vector<TUGRcNormal*> records_;
	};

}
