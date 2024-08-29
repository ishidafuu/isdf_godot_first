#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
#include "dbUGListEnd.h"
//==========================================================================
//
//
//
//==========================================================================
namespace db
{
	struct TUGLiItemSetSt
	{
		void Init()
		{
			Zeromem(this);
		}
	};

	//リスト
	class TUGLiItemSet :public TUGLiGen
	{
	public:
		//コンストラクタ
		explicit TUGLiItemSet(TUIGenLayer* player);
		virtual ~TUGLiItemSet(void);

		s64 GetCommonID(void);
		s64 GetEtcNum(void);
		LONGVECTOR GetData(void);

		void Refresh(LONGVECTOR2 data);

		std::vector<TUGRcNormal*> records_;
		TUGLiItemSetSt stLiItemGet_;

	protected:
		void DoUpdateSub(BOOL draw_f);
		void DoActionSub();
		void DoClearRecord();
		void DoMakeGroupSub();
	};
}
