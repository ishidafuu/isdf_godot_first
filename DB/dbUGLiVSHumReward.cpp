﻿#include "dbUGLiVSHumReward.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"
//=====================================================================================================
//
//
//
//=====================================================================================================
namespace db
{
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGLiVSHumReward::TUGLiVSHumReward(TUIGenLayer* player) :TUGLiMenu(player)
	{
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGLiVSHumReward::~TUGLiVSHumReward()
	{
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGRcNormal* TUGLiVSHumReward::MakeObj_Record()
	{
		TUGRcNormal* newrecord = TUGLiMenu::MakeObj_Record();
		newrecord->SetShopRecord();

		//TUGRcNormal* newrecord = new TUGRcNormal(player_);
		//s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
		//records_.push_back(newrecord);
		//GenMakeObj_Record(newrecord, rh);

		return newrecord;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGLiVSHumReward::DoMakeGroupSub()
	{
		uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
		field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
	}

}
