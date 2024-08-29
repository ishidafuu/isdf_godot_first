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
	class TUGLiVSHumReward :public TUGLiMenu
	{
	public:
		//コンストラクタ
		explicit TUGLiVSHumReward(TUIGenLayer* player);
		virtual ~TUGLiVSHumReward(void);
		TUGRcNormal* MakeObj_Record();

	protected:
		void DoMakeGroupSub();
	};

}
