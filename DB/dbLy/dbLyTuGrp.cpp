//utf8
#include "dbLyTuGrp.h"
#include "dbSnTutorial.h"
//=========================================================================
// 
//	チュートリアルシーン
//		リソースクラス
//
//=========================================================================
namespace db
{
	//コンストラクタ
	TLyTuGrp::TLyTuGrp(TSnTutorial* pScene) :TUILayer(pScene, pScene->frame_)
	{

	}

	TLyTuGrp::~TLyTuGrp()
	{

	}

	void TLyTuGrp::DoLoad()
	{
		//監督とキャプテン読み込み
		pmgEO_->mgGrp_.pmgcg_kantoku_->SetMyKantokuDt();//てきとう

		pmgEO_->mgGrp_.MakeKantokuTexture();
		pmgEO_->mgGrp_.pmgcg_kantoku_->SetGrTile( pmgEO_->mgGrp_.pgr_Kantoku_,
												  pmgEO_->mgGrp_.pgr_KantokuFace_,
												  pmgEO_->mgGrp_.pgr_KantokuHair_,
												  &pmgEO_->mgDt_.dtCell_kantoku_ );


		//bar_obj_ = LoadGrp("bar", "bar_obj");//オブジェ
		home_bg_ = LoadGrpPart("home", "home_bg", 456, 168);
		home_board_ = LoadGrp("home", "home_board");
		//home_challenge_ = LoadGrp("home", "home_challenge");
		//home_flower_ = LoadGrp("home", "home_flower");
		home_shadow_ = LoadGrp("home", "home_shadow");
		//home_tv_ = LoadGrpPart("home", "home_tv", 40, 32);
		home_window_ = LoadGrp("home", "home_window");


    shop_door_[0] = LoadGrpPart("shop", "shop_door0", 48, 40);
    shop_door_[1] = LoadGrpPart("shop", "shop_door1", 48, 40);
    shop_door_[2] = LoadGrpPart("shop", "shop_door2", 24, 40);
    shop_front_[0] = LoadGrp("shop", "shop_front0");
    shop_front_[1] = LoadGrp("shop", "shop_front1");
    shop_front_[2] = LoadGrp("shop", "shop_front2");

    center_shutter_ = LoadGrpPart("center", "center_shutter", 216, 36);
    center_bbadoor_ = LoadGrpPart("center", "center_bbadoor", 20, 32);
    center_dockdoor_ = LoadGrpPart("center", "center_dockdoor", 48, 28);
    center_track_ = LoadGrp("center", "center_track");
    center_pole_ = LoadGrp("center", "center_pole");
	}
}
