//utf8
#pragma once
#include "dbInclude.h"
#include "dbUILayer.h"
//=========================================================================
// 
//	チュートリアルシーン
//		リソースクラス
//
//=========================================================================
namespace db
{
	// 前方参照
	class TSnTutorial;

	class TLyTuGrp :public TUILayer//自動開放読み込みするためTUILayerを継承
	{
	public:

		//コンストラクタ
		explicit TLyTuGrp(TSnTutorial* pScene);
		virtual ~TLyTuGrp(void);

		//base::TGrp* bar_obj_;
		base::TGrPart* home_bg_;
		base::TGrp* home_board_;
		//base::TGrp* home_challenge_;
		base::TGrp* home_flower_;
		base::TGrp* home_shadow_;
		//base::TGrPart* home_tv_;
		base::TGrp* home_window_;

    base::TGrPart* shop_door_[3];
    base::TGrp* shop_front_[3];

    base::TGrPart* center_shutter_;
    base::TGrPart* center_bbadoor_;
    base::TGrPart* center_dockdoor_;
    base::TGrp* center_track_;
    base::TGrp* center_pole_;

	protected:

		//オーバーロード
		void DoLoad();
	};

}
