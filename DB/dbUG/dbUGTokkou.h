#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
	class TUILayer;

	struct TUGTokkouSt
	{
		void Init()
		{
			Zeromem(this);
		}
	};

	//リスト
	class TUGTokkou : public TUGGen
	{
	public:
		TUGTokkouSt stTokkou_;
		void MakeGroup(TUIObj* parent);
		void SetTokkou(s64 id_team, s32 mid_team, s32 mid_event);
		void SetTokkouNow(s64 id_team, s32 mid_team);

		s64 GetTokkouMag(void){ return tokkouMag_; }
		std::string GetTokkouMagStr(void) { return tokkouMagStr_; }

		//コンストラクタ
		explicit TUGTokkou(TUIGenLayer* player);
		virtual ~TUGTokkou(void);
	protected:
		void DoUpdate(BOOL draw_f);
		
		s64 tokkouMag_;
		std::string tokkouMagStr_;
	};
}
