#include "dbUGChBasePerson.h"
#include "dbUILayer.h"
//=============================================================================================
//
//	かんとくベースクラス
//
//=============================================================================================
namespace db
{

	s32 motion = 0;
	enPersonMotionType personMotionTbl[pt_Max][pat_Max] =
	{
		{
			pmt_Sei_St,//制服立ち
			pmt_Sei_Ds,//制服走り
			pmt_Sei_Ds,//制服走り
		},
		{
			pmt_Org_St,//オリジナル立ち
			pmt_Org_Ds,//オリジナル走り
			pmt_Org_Ds,//オリジナル走り
		},
		{
			pmt_Soc_St,//サッカー立ち
			pmt_Soc_Ds,//サッカー走り
			pmt_Soc_Ds,//サッカー走り
		},
		{
			pmt_Uni_St,//ユニフォーム立ち
			pmt_Uni_Ds,//ユニフォーム走り
			pmt_Uni_Ds,//ユニフォーム走り
		},
		{
			pmt_Jid_St,//時代劇立ち
			pmt_Jid_Ds,//時代劇走り
			pmt_Jid_Ds,//時代劇走り
		},
		{
			pmt_Bsk_St,//バスケ立ち
			pmt_Bsk_Ds,//バスケ走り
			pmt_Bsk_Ds,//バスケ走り
		},
		{
			pmt_Mas_St,//マーシャル立ち
			pmt_Mas_Ds,//マーシャル走り
			pmt_Mas_Ds,//マーシャル走り
		},
		{
			pmt_Kan_St,//カンフー立ち
			pmt_Kan_Ds,//カンフー走り
			pmt_Kan_Ds,//カンフー走り
		},
		{
			pmt_Jud_St,//柔道家立ち
			pmt_Jud_Ds,//柔道家走り
			pmt_Jud_Ds,//柔道家走り
		},
		{
			pmt_Hok_St,//ホッケー立ち
			pmt_Hok_Ds,//ホッケー走り
			pmt_Hok_Ds,//ホッケー走り
		},
	};

	//-----------------------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------------------
	TUGChBasePerson::TUGChBasePerson(TUIGenLayer* player, s32 no)
		: TUGChGen(player)
		, no_(no)
	{
		// 自身の番号をキーとして扱う
		s32 key = no;
		if (pmgEO_->mgGrp_.pmgcg_person_.count(key) == 0)
		{
			// 新規作成
			pmgEO_->mgGrp_.pmgcg_person_[key] = new TMgCharGrp(pmgEO_);
		}
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	TUGChBasePerson::~TUGChBasePerson()
	{
	}

	//-----------------------------------------------------------------
	// 
	//-----------------------------------------------------------------
	void TUGChBasePerson::SetChDt(s32 teamid, s32 charano)
	{
		BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(teamid, mdm_team_gb_f);
		s64 charID = pmgEO_->mgCoM_.GetCharID(teamid, charano);
		pmgEO_->mgGrp_.pmgcg_person_[no_]->SetCharDt(charID, teamid, gb_f);
		pmgEO_->mgGrp_.MakePersonTexture(no_);
	}
	void TUGChBasePerson::SetChDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 wearcol, s32 pantscol, s32 sidecol, s32 undercol, s32 shoecol, BOOL gb_f)
	{
		pmgEO_->mgGrp_.pmgcg_person_[no_]->SetCharDt(hair, face, haircol, skincol, eyecol, acccol, wearcol, pantscol, sidecol, undercol, shoecol, gb_f);
		pmgEO_->mgGrp_.MakePersonTexture(no_);
	}

	//-----------------------------------------------------------------
	// 
	//-----------------------------------------------------------------
	void TUGChBasePerson::SetChDtRnd(void)
	{
		s32 teamMAX = pmgEO_->mgCoM_.mdm_Team_.GetLineNum();	// チーム最大数
		s32 teamID = mid::midGetRandMn(teamMAX);
		s32 charNo = mid::midGetRandMn(DBMEMBER_ALL);
		SetChDt(teamID, charNo);
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::MakeGroup(TUIObj* parent)
	{
		GenMakeGroup(parent, player_->pmgEO_->mgGrp_.pmgcg_person_[no_], &player_->pmgEO_->mgDt_.dtBM_person_, 0);
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::SetPersonType(enPersonType type)
	{
		personType_ = type;
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::SetPersonActType(enPersonActType type)
	{
		personActType_ = type;
		SetMType(personMotionTbl[personType_][personActType_]);
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::SetMType(enPersonMotionType mtype)
	{
		SetGenMType(mtype);
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd)
	{
		z_MoveWork(moveno, tagx, tagy, spd, cmk_None);
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki)
	{
		field_->z_MoveSetspd(tagx, tagy, spd);
	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd)
	{
		z_MoveWork(moveno, tagx - mid::midFullDotL_Hf(), tagy - mid::midFullDotS_Hf(), spd, cmk_None);
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::z_MoveWork_FullMuki(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki)
	{
		z_MoveWork(moveno, tagx - mid::midFullDotL_Hf(), tagy - mid::midFullDotS_Hf(), spd, muki);
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd, f32 lookx)
	{
		enUGChGenMuki muki = (tagx < lookx) ? cmk_Right : cmk_Left;
		z_MoveWork_FullMuki(moveno, tagx, tagy, spd, muki);
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::z_SetStop(void)
	{
		field_->z_SetStop();
	}
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBasePerson::DoActionSub()
	{
		if (IsAct())
		{
			switch (GetActRes())
			{
			case enUGChGenAct_MotihonEnd:
			case enUGChGenAct_MoveStop:
				break;
			}
		}
	}
}
