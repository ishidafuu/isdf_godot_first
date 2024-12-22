#include "dbUGRcGraphic.h"
#include "dbUILayer.h"
//=====================================================================================================
//
//
//
//=====================================================================================================
namespace db
{
	const s32 RECORDH = 48;
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGRcGraphic::TUGRcGraphic(TUIGenLayer* player)
		: TUGRcGen(player)
		, commonID_(NGNUM)
	{

	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGRcGraphic::~TUGRcGraphic(void)
	{

	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcGraphic::SetDt(base::TGrp* pgrp, const char* msg)
	{
		banner_->o_SetObjGrp(pgrp);
		banner_->g_SetDraw(TRUE);
		
		const s32 MAXLEN = 17;
		name_->f_MakeFont_Len(msg, FC2_normal, MAXLEN);
		name_->z_SetPos(8, 33);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	s32 TUGRcGraphic::DoMakeGroupSub(void)
	{
		//TUGRcGen::MakeGroupから呼ばれる
		basegrp_ = pmgUI_->GetUGGrp(ug_list_recordmem);
		//basegrp2_ = pmgUI_->GetUGGrp(ug_list_recordmem);
		field_->o_SetObjGrp(basegrp_);
		name_ = MakeObj();
		banner_ = MakeObj();
		//nokori_ = MakeObj();

		items_.push_back(name_);
		items_.push_back(banner_);
		//items_.push_back(nokori_);

		for (s32 i = 0; i < items_.size(); ++i) items_[i]->o_SetParent(field_);

		//レコード高さを返す
		return RECORDH;
	}
}
