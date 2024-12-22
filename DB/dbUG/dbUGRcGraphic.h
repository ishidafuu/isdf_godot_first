#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcGen.h"
//=====================================================================================================
//
//
//
//=====================================================================================================
namespace db
{

	class TUGRcGraphic :public TUGRcGen
	{
	public:
		explicit TUGRcGraphic(TUIGenLayer* player);
		virtual ~TUGRcGraphic(void);
		void SetDt(base::TGrp* pgrp, const char* msg);
		s64 GetCommonID(void){ return commonID_; }
		void SetCommonID(s64 id){ commonID_ = id; }

	protected:
		s32 DoMakeGroupSub(void);

		TUIObj* banner_;
		TUIObj* name_;
		s64 commonID_;
	};
}
