#include "dbUGRcAnimRanker.h"

namespace db
{
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGRcAnimRanker::TUGRcAnimRanker(TUIGenLayer* player)
		: TUGRcNormal(player)
		, isMove_(FALSE)
	{
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGRcAnimRanker::~TUGRcAnimRanker(void)
	{

	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcAnimRanker::SetRankerDt(s64 id_ranker, s32 lineno)
	{
		SetCommonID(id_ranker);
		ugname_.SetNameRankerByData(id_ranker);

		s32 rank = pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_rank);
		std::string rankstr = "ＲＡＮＫ＿" + lib_str::IntToStr(rank);
		SetSubName(rankstr.c_str());

		SetScore(id_ranker);
		//s32 score = pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_score);
		//SetValue(lib_str::IntToStr(score).c_str());

		s64 createdate = pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_create_date);
		std::string datestr = base::UnixtimeToDateString(createdate, TRUE);
		//SubValue
		SetSubValue(datestr.c_str());
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcAnimRanker::SetRankerDtDummy(s64 dataRank, s64 dispRank)
	{
		s64 id_ranker = NGNUM;
		for (s32 i = 0; i < pmgEO_->mgCoU_.rankerData_.GetLineNum(); i++)
		{
			s64 pk = pmgEO_->mgCoU_.rankerData_.GetLineDt(i, enmdre_Ranker::mdre_ranker_pk);
			s64 rank = pmgEO_->mgCoU_.rankerData_.GetPKDt(pk, enmdre_Ranker::mdre_ranker_rank);
			if (dataRank == rank)
			{
				id_ranker = pk;
				break;
			}
		}

		SetCommonID(id_ranker);
		ugname_.SetNameRankerByData(id_ranker);

		std::string rankstr = "ＲＡＮＫ＿" + lib_str::IntToStr(dispRank);
		SetSubName(rankstr.c_str());

		SetScore(id_ranker);
		//s32 score = pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_score);
		//SetValue(lib_str::IntToStr(score).c_str());

		s64 createdate = pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_create_date);
		std::string datestr = base::UnixtimeToDateString(createdate, TRUE);
		//SubValue
		SetSubValue(datestr.c_str());
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcAnimRanker::SetMoveTargetX(s32 tx, s32 frame)
	{
		f32 x = 0.0f;
		f32 y = 0.0f;
		field_->z_GetPos(&x, &y);

		s32 mx = tx - x;

		SetMoveX(mx, frame);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcAnimRanker::SetMoveTargetY(s32 ty, s32 frame)
	{
		f32 x = 0.0f;
		f32 y = 0.0f;
		field_->z_GetPos(&x, &y);

		s32 my = ty - y;

		SetMoveY(my, frame);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcAnimRanker::SetMoveX(s32 mx, s32 frame)
	{
		const f32 _RADIAN = 3.1415f / 180;

		f32 x = 0.0f;
		f32 y = 0.0f;
		field_->z_GetPos(&x, &y);

		// 今の位置を記憶
		initPos.clear();
		initPos.push_back(x);
		initPos.push_back(y);

		// 最後の位置を記憶
		lastPos.clear();
		lastPos.push_back(x + mx);
		lastPos.push_back(y);

		// 移動フレーム数より移動値が少なければ移動値をフレーム数とする
		s32 newFrame = frame;
		//s32 newFrame = abs(mx - x);
		//newFrame = (newFrame > frame) ? frame : newFrame;

		moveList_.clear();
		moveList_.resize(newFrame);

		f32 a = 90.0f / newFrame;
		for (s32 i = 0; i < newFrame; i++)
		{
			moveList_[i].push_back(sin(i * a * _RADIAN) * mx);
			moveList_[i].push_back(0);
		}

		frameCnt_ = 0;
		isMove_ = TRUE;
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcAnimRanker::SetMoveY(s32 my, s32 frame)
	{
		const f32 _RADIAN = 3.1415f / 180;

		f32 x = 0.0f;
		f32 y = 0.0f;
		field_->z_GetPos(&x, &y);

		// 今の位置を記憶
		initPos.clear();
		initPos.push_back(x);
		initPos.push_back(y);

		// 最後の位置を記憶
		lastPos.clear();
		lastPos.push_back(x);
		lastPos.push_back(y + my);

		// 移動フレーム数より移動値が少なければ移動値をフレーム数とする
		s32 newFrame = frame;
		//s32 newFrame = abs(my - y);
		//newFrame = (newFrame > frame) ? frame : newFrame;

		moveList_.clear();
		moveList_.resize(newFrame);

		f32 a = 90.0f / newFrame;
		for (s32 i = 0; i < newFrame; i++)
		{
			moveList_[i].push_back(0);
			moveList_[i].push_back(sin(i * a * _RADIAN) * my);
		}

		frameCnt_ = 0;
		isMove_ = TRUE;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcAnimRanker::SetScore(s64 id_ranker)
	{
		s32 mid_Event = pmgEO_->stSeq_.GetMidEvent();
		s64 mid_ranking = pmgEO_->mgCoM_.GetEventRanking(pmgEO_->mgCoU_.GetNowEvent(), mid_Event);
		s32 scoretype = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDt(mid_ranking, mdm_ranking_scoreType);

		s32 score = pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_score);
		std::string scoreStr = "";
		
		if (scoretype == 8)	// タイムアタック
		{
			scoreStr = base::MinSecMSecString(score, FALSE);
		}
		else
		{
			scoreStr = lib_str::IntToStr(score);
		}
		SetValue(scoreStr.c_str());
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcAnimRanker::DoMakeGroupSub2()
	{
		basegrp_ = pmgUI_->GetUGGrp(ug_list_record);
		field_->o_SetObjGrp(basegrp_);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcAnimRanker::DoSetLimY2(s32 limu, s32 hhh)
	{
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRcAnimRanker::DoUpdateSub2(BOOL draw_f)
	{
		if (isMove_)
		{
			if (frameCnt_ >= moveList_.size())
			{
				SetPos(lastPos[0], lastPos[1]);
				isMove_ = FALSE;
				return;
			}
			SetPos(initPos[0] + moveList_[frameCnt_][0], initPos[1] + moveList_[frameCnt_][1]);
			frameCnt_++;
		}
	}

}
