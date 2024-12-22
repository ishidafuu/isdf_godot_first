#pragma once
#include "dbLyTuGen.h"
#include "dbUGLiMenu.h"
#include "dbUGRMessage.h"
//=====================================================================================================
//	
//
//
//=====================================================================================================
namespace db
{
	enum enUGTokkouInfoAct
	{
		TIA_Select_BGM = 0,
		TIA_Select_Haikei,
	};


	class TUGTokkouInfo :public TUGGen
	{
	public:
		//コンストラクタ
		explicit TUGTokkouInfo(TUIGenLayer* player);
		virtual ~TUGTokkouInfo(void);

		void MakeGroup(TUIObj* parent);
		void Refresh(s64 mid_event);
	
	protected:
		//オーバーロード
		void DoUpdate(BOOL draw_f);
		void DoAction(void);

		void ClearText(void);
		void AddText(std::string text, s32 x, s32 y, BOOL isCenter = FALSE);
		std::vector<TUGRName*> textVec_;

		TUIObj* uiPlate_;
		TUIObj* uiIcon_;
		TUGRMessage ugrMsg_;
		TUGLiMenu ugList_;
	};

}
