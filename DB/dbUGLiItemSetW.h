#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGLiGenW.h"
#include "dbUGRcNormal.h"
//==========================================================================
//
//
//
//==========================================================================
namespace db
{
	//struct TUGListWSt
	//{
	//  void Init()
	//  {
	//    Zeromem(this);
	//  }
	//};

	//リスト
	class TUGLiItemSetW :public TUGLiGenW
	{
	public:
		//コンストラクタ
		explicit TUGLiItemSetW(TUIGenLayer* player);
		virtual ~TUGLiItemSetW(void);

		s64 GetCommonID(void);
		s64 GetEtcNum(void);
		LONGVECTOR GetData(void);

		void Refresh(LONGVECTOR2 data);

		std::vector<TUGRcNormal*> records_;
		//TUGListWSt stLiItemGet_;

	protected:
		void DoUpdateSub(BOOL draw_f);
		void DoActionSub();
		void DoClearRecord();
		void DoMakeGroupSub();
	};

}
