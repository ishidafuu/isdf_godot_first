#include "dbUGTmBoard.h"
#include "dbUILayer.h"
//======================================================================================================
//
//	ホーム画面に配置される所持チーム中の最強チーム表示掲示板
//
//======================================================================================================
namespace db
{
	const s32 ICON_TEAM_BASE = 0;
	const s32 ICON_SHOT_BASE = 7;


	//--------------------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------------------
	TUGTmBoard::TUGTmBoard(TUIGenLayer* player)
		: TUGGen(player)
		, pPlayer(player)
		, uiTeamIcon_(player)
		, uiTeamShotIcon_(player)
	{
	}

	//--------------------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------------------
	TUGTmBoard::~TUGTmBoard()
	{
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTmBoard::MakeGroup(TUIObj* parent)
	{
		SetParent(parent);

		uiBg_ = MakeObj();
		uiBg_->o_SetParent(field_);
		uiBg_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_home_board));
		uiBg_->z_SetPos(0, 0);
		uiBg_->g_SetDraw(TRUE);


		s32 BASE_X = 18;
		s32 BASE_Y = 10;
		s32 OFFSET_X = 21;
		s32 OFFSET_Y1 = -4;
		s32 OFFSET_Y2 = 3;


		uiTeamIcon_.MakeGroup(uiBg_);
		uiTeamIcon_.SetPos(BASE_X, BASE_Y);
		uiTeamIcon_.SetDraw(TRUE);
		
		uiTeamName_ = MakeObj();
		uiTeamName_->o_SetParent(uiBg_);
		uiTeamName_->f_MakeFont_Mini("ＴＥＡＭ", FC2_mini);
		uiTeamName_->z_SetPos(BASE_X + OFFSET_X, BASE_Y + OFFSET_Y1);
		uiTeamName_->g_SetDraw(TRUE);

		uiTeamState_ = MakeObj();
		uiTeamState_->o_SetParent(uiBg_);
		uiTeamState_->z_SetPos(BASE_X + OFFSET_X, BASE_Y + OFFSET_Y2);
		uiTeamState_->g_SetDraw(TRUE);

		
		BASE_X = 18;
		BASE_Y = 28;
		
		uiTeamShotIcon_.MakeGroup(uiBg_);
		uiTeamShotIcon_.SetPos(BASE_X, BASE_Y);
		uiTeamShotIcon_.SetDraw(TRUE);

		uiTeamShotName_ = MakeObj();
		uiTeamShotName_->o_SetParent(uiBg_);
		uiTeamShotName_->f_MakeFont_Mini("ＳＨＯＴ", FC2_mini);
		uiTeamShotName_->z_SetPos(BASE_X + OFFSET_X, BASE_Y + OFFSET_Y1);
		uiTeamShotName_->g_SetDraw(TRUE);

		uiTeamShotState_ = MakeObj();
		uiTeamShotState_->o_SetParent(uiBg_);
		uiTeamShotState_->z_SetPos(BASE_X + OFFSET_X, BASE_Y + OFFSET_Y2);
		uiTeamShotState_->g_SetDraw(TRUE);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTmBoard::SetTopTeamDt(void)
	{
		s64 id_team = pmgEO_->GetTopTeamByUserID();
		SetTeamDt(id_team);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTmBoard::SetTeamDt(s64 id_team)
	{
		s64 mid_team = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_m_team);
		//s64 tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);

		s64 totalSt = 0;
		for (s32 i = 0; i < DBMEMBER_ALL; i++)
		{
			// 実際に計算する
			s64 stPow  = pmgEO_->GetSt(id_team, i, clv_Pow);
			s64 stGuts = pmgEO_->GetSt(id_team, i, clv_Guts);
			s64 stKire = pmgEO_->GetSt(id_team, i, clv_Kire);
			s64 stTech = pmgEO_->GetSt(id_team, i, clv_Tech);
			totalSt += (stPow + stGuts + stKire + stTech);
		}
		// チームの総ステータス
		uiTeamState_->f_MakeFont_Mini(lib_str::IntToStr(totalSt).c_str(), FC2_mini);
		uiTeamIcon_.SetIconTeam(mid_team);

		// チームの総シュートステータス
		s32 totalShotSt = 0;
		for (s32 i = 0; i < DBMEMBER_ALL; i++)
		{
			s32 pow = 0;
			s32 spin = 0;
			pmgEO_->mgCoU_.GetShotTotalPowSpin(mid_team, i, &pow, &spin);
			totalShotSt += (pow + spin);
		}
		uiTeamShotState_->f_MakeFont_Mini(lib_str::IntToStr(totalShotSt).c_str(), FC2_mini);
		uiTeamShotIcon_.SetIcon(ugi_Ball);
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTmBoard::DoUpdate(BOOL draw_f)
	{
		uiBg_->Update(draw_f);
		uiTeamIcon_.Update(draw_f);
		uiTeamName_->Update(draw_f);
		uiTeamState_->Update(draw_f);

		uiTeamShotIcon_.Update(draw_f);
		uiTeamShotName_->Update(draw_f);
		uiTeamShotState_->Update(draw_f);
	}
}
