#include "dbUGChCaptain.h"
#include "dbUILayer.h"

namespace db
{
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	TUGChCaptain::TUGChCaptain(TUIGenLayer* player)
		:TUGChBaseCaptain(player)
	{
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	TUGChCaptain::~TUGChCaptain()
	{

	}

	//-----------------------------------------------------------------
	// 今のチームでキャプテンテクスチャ再構築
	//-----------------------------------------------------------------
	void TUGChCaptain::SetupMgCharGrp(void)
	{
#ifdef __K_DEBUG_USER_DATA__
		s64 teamMID = kdebug::DebugUDManager::GetInstance()->GetLastTeamID();
#else
		s64 teamMID = pmgEO_->mgCoU_.GetLastMIDTeam();
#endif

		BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(teamMID, mdm_team_gb_f);
		s64 charID = pmgEO_->mgCoM_.GetCharID(teamMID, 0);
		pmgEO_->mgGrp_.pmgcg_captain_->SetCharDt(charID, teamMID, gb_f);
		pmgEO_->mgGrp_.MakeCaptainTexture();
	}
	//-----------------------------------------------------------------
	// 
	//-----------------------------------------------------------------
	void TUGChCaptain::MakeGroup(TUIObj* parent)
	{
		BASE::MakeGroup(parent, player_->pmgEO_->mgGrp_.pmgcg_captain_);
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChCaptain::DoActionSub(void)
	{
		BASE::DoActionSub();
	}
}