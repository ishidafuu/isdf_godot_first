#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcGraphic.h"
//=====================================================================================================
//
//
//
//=====================================================================================================
namespace db
{
	class TUGLiGraphic :public TUGLiGen
	{
	public:
		//コンストラクタ
		explicit TUGLiGraphic(TUIGenLayer* player);
		virtual ~TUGLiGraphic(void);
		TUGRcGraphic* MakeObj_Record(void);

		s64 GetCommonID(void);

	protected:
		void DoActionSub(void);
		void DoClearRecord(void);

		std::vector<TUGRcGraphic*> records_;

	};

}
