#include "dbUGChAnotherCaptain.h"
#include "dbUILayer.h"

namespace db
{
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	TUGChAnotherCaptain::TUGChAnotherCaptain(TUIGenLayer* player, s32 no)
		: TUGChBaseCaptain(player)
		, no_(no)
	{
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	TUGChAnotherCaptain::~TUGChAnotherCaptain()
	{

	}

	//-----------------------------------------------------------------
	// 今のチームでキャプテンテクスチャ再構築
	//-----------------------------------------------------------------
	void TUGChAnotherCaptain::SetChDt(s32 teamid, s32 charano)
	{
		BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(teamid, mdm_team_gb_f);
		s64 charID = pmgEO_->mgCoM_.GetCharID(teamid, charano);
		pmgEO_->mgGrp_.pmgcg_captains_[no_]->SetCharDt(charID, teamid, gb_f);
		pmgEO_->mgGrp_.MakeCaptainsTexture(no_);
	}
	//-----------------------------------------------------------------
	// 今のチームでキャプテンテクスチャ再構築
	//-----------------------------------------------------------------
	void TUGChAnotherCaptain::SetChDtRnd(void)
	{
		s32 teamMAX = pmgEO_->mgCoM_.mdm_Team_.GetLineNum();	// チーム最大数
		s32 teamID = mid::midGetRandMn(teamMAX);
		s32 charNo = mid::midGetRandMn(DBMEMBER_ALL);
		SetChDt(teamID, charNo);
	}
	//-----------------------------------------------------------------
	// 
	//-----------------------------------------------------------------
	void TUGChAnotherCaptain::MakeGroup(TUIObj* parent)
	{
		BASE::MakeGroup(parent, player_->pmgEO_->mgGrp_.pmgcg_captains_[no_]);
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChAnotherCaptain::DoActionSub(void)
	{
		BASE::DoActionSub();
	}
}
