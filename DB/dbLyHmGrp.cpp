#include "dbLyHmGrp.h"
#include "dbSnHome.h"

namespace db
{
  //コンストラクタ
  TLyHmGrp::TLyHmGrp(TSnHome* pScene) :TUILayer(pScene, pScene->frame_)
  {

  }

  TLyHmGrp::~TLyHmGrp()
  {

  }

  void TLyHmGrp::DoLoad()
  {
    //監督とキャプテン読み込み
    pmgEO_->mgGrp_.pmgcg_kantoku_->SetMyKantokuDt();//てきとう

    pmgEO_->mgGrp_.MakeKantokuTexture();
    pmgEO_->mgGrp_.pmgcg_kantoku_->SetGrTile(
      pmgEO_->mgGrp_.pgr_Kantoku_,
      pmgEO_->mgGrp_.pgr_KantokuFace_,
      pmgEO_->mgGrp_.pgr_KantokuHair_,
      &pmgEO_->mgDt_.dtCell_kantoku_);

    pmgEO_->mgGrp_.pmgcg_enemy_->SetMyKantokuDt();//てきとう

    pmgEO_->mgGrp_.MakeEnemyTexture();
    pmgEO_->mgGrp_.pmgcg_enemy_->SetGrTile(
      pmgEO_->mgGrp_.pgr_Enemy_,
      pmgEO_->mgGrp_.pgr_EnemyFace_,
      pmgEO_->mgGrp_.pgr_EnemyHair_,
      &pmgEO_->mgDt_.dtCell_kantoku_);

	for (s32 i = 0; i < PLAYERNUM; i++)
	{
		pmgEO_->mgGrp_.pmgcg_enemies_[i]->SetCharDt(i+1, 1, FALSE);	// てきとう
		pmgEO_->mgGrp_.MakeEnemiesTexture(i);
		pmgEO_->mgGrp_.pmgcg_enemies_[i]->SetGrTile(
			pmgEO_->mgGrp_.pgr_Enemies_[i],
			pmgEO_->mgGrp_.pgr_EnemiesFace_[i],
			pmgEO_->mgGrp_.pgr_EnemiesHair_[i],
			&pmgEO_->mgDt_.dtCell_kantoku_);
	}


#ifdef __K_DEBUG_USER_DATA__
    s64 teamMID = kdebug::DebugUDManager::GetInstance()->GetLastTeamID();
#else
    s64 teamMID = pmgEO_->mgCoU_.GetLastMIDTeam();
#endif
    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(teamMID, mdm_team_gb_f);
    s64 charID = pmgEO_->mgCoM_.GetCharID(teamMID, 0);
    pmgEO_->mgGrp_.pmgcg_captain_->SetCharDt(charID, teamMID, gb_f);//てきとう
    pmgEO_->mgGrp_.MakeCaptainTexture();
    pmgEO_->mgGrp_.pmgcg_captain_->SetGrTile(
      pmgEO_->mgGrp_.pgr_Captain_,
      pmgEO_->mgGrp_.pgr_CaptainFace_,
      pmgEO_->mgGrp_.pgr_CaptainHair_,
      &pmgEO_->mgDt_.dtCell_sensyu_);// dtCell_captain_

	for (s32 i = 0; i < PLAYERNUM; i++)
	{
		BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(teamMID, mdm_team_gb_f);
		s64 charID = pmgEO_->mgCoM_.GetCharID(teamMID, 0);
		pmgEO_->mgGrp_.pmgcg_captains_[i]->SetCharDt(charID, teamMID, gb_f);//てきとう
		pmgEO_->mgGrp_.MakeCaptainsTexture(i);
		pmgEO_->mgGrp_.pmgcg_captains_[i]->SetGrTile(
			pmgEO_->mgGrp_.pgr_Captains_[i],
			pmgEO_->mgGrp_.pgr_CaptainsFace_[i],
			pmgEO_->mgGrp_.pgr_CaptainsHair_[i],
			&pmgEO_->mgDt_.dtCell_sensyu_);// dtCell_captain_
	}


    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
    {
      pmgEO_->mgGrp_.pmgcg_sensyu_[i2]->SetCharDt(i2 + 1, 1, FALSE);//てきとう

      pmgEO_->mgGrp_.MakeSensyuTexture(i2);

      pmgEO_->mgGrp_.pmgcg_sensyu_[i2]->SetGrTile(
        pmgEO_->mgGrp_.pgr_Sensyu_[i2],
        pmgEO_->mgGrp_.pgr_SensyuFace_[i2],
        pmgEO_->mgGrp_.pgr_SensyuHair_[i2],
        &pmgEO_->mgDt_.dtCell_sensyu_);
    }

    //女子高生と店員は開始時読み込みしてある

    //bar_obj_ = LoadGrp("bar", "bar_obj");//オブジェ
    home_bg_ = LoadGrpPart("home", "home_bg", 456, 168);
    //home_board_ = LoadGrp("home", "home_board");
    //home_challenge_ = LoadGrp("home", "home_challenge");
    //home_flower_ = LoadGrp("home", "home_flower");
    home_shadow_ = LoadGrp("home", "home_shadow");
    //home_tv_ = LoadGrpPart("home", "home_tv", 40, 32);
    home_window_ = LoadGrp("home", "home_window");

    team_banner_ = LoadGrp("team", "team_banner");
    team_bcolors_ = LoadGrpPart("team", "team_bcolors", 112, 16);
    team_boombox_ = LoadGrpPart("team", "team_boombox", 40, 24);
    team_camera_ = LoadGrp("team", "team_camera");
    team_cardside_ = LoadGrp("team", "team_cardside");
    //team_films_ = LoadGrpPart("team", "team_films", 16, 16);
    team_ball_ = LoadGrpPart("team", "team_ball", 16, 16);

    //team_album_ = LoadGrpPart("team", "team_album", 22, 16);
    //team_tapes_ = LoadGrpPart("team", "team_tapes", 18, 12);
    //team_vol_ = LoadGrp("team", "team_vol");
    //team_volbtn_ = LoadGrp("team", "team_volbtn");
    //stage_picture_ = LoadGrp("team", "stage_picture");

    //hmshiai_event_ = LoadGrpPart("hmshiai", "hmshiai_event", 152, 80);
    hmshiai_next_ = LoadGrpPart("hmshiai", "hmshiai_next", 152, 80);
    //hmshiai_select_ = LoadGrpPart("hmshiai", "hmshiai_select", 152, 80);
    hmshiai_banner_ = LoadGrpPart("hmshiai", "hmshiai_banner", 144, 32);
    //hmshiai_lock_ = LoadGrp("hmshiai", "hmshiai_lock");

	hmshiai_menu_wifi_ = LoadGrpPart("hmshiai", "hmshiai_menu_wifi", 144, 32);
	hmshiai_menu_bt_ = LoadGrpPart("hmshiai", "hmshiai_menu_bt", 144, 32);

    center_shutter_ = LoadGrpPart("center", "center_shutter", 216, 36);
    center_bbadoor_ = LoadGrpPart("center", "center_bbadoor", 20, 32);
    center_dockdoor_ = LoadGrpPart("center", "center_dockdoor", 48, 28);
    center_track_ = LoadGrp("center", "center_track");
    center_pole_ = LoadGrp("center", "center_pole");

    shop_door_[0] = LoadGrpPart("shop", "shop_door0", 48, 40);
    shop_door_[1] = LoadGrpPart("shop", "shop_door1", 48, 40);
    shop_door_[2] = LoadGrpPart("shop", "shop_door2", 24, 40);
    shop_front_[0] = LoadGrp("shop", "shop_front0");
    shop_front_[1] = LoadGrp("shop", "shop_front1");
    shop_front_[2] = LoadGrp("shop", "shop_front2");

  }
}
