#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
	class TUILayer;

	struct TUGShSetSt
	{
		s64 id_shset_;
		s32 rarity_;
		void Init()
		{
			Zeromem(this);
			id_shset_ = NGNUM;
			rarity_ = NGNUM;
		}
	};

	//リスト
	class TUGShSet : public TUGGen
	{
	public:
		//コンストラクタ
		explicit TUGShSet(TUIGenLayer* player);
		virtual ~TUGShSet(void);
		void MakeGroup(TUIObj* parent);
		void Refresh(s64 mid_team, s64 id_team, s32 rarity);
		TUGShSetSt stShSet_;

	protected:
		void DoUpdate(BOOL draw_f);

		TUIObj* uiShotNum_;	// 装備してるシュート数
		TUIObj* uiTotalShSt_; // 装備してるシュートの総ステータス

	};

}
