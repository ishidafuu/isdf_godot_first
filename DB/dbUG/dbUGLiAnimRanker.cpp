#include "dbUGLiAnimRanker.h"
#include "dbUILayer.h"

namespace db
{
	const s32 RECORD_SIZE_HEIGHT = 24;

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGLiAnimRanker::TUGLiAnimRanker(TUIGenLayer* player) :TUGLiGen(player)
		//, uglistend_(player)
	{
		stLiRanker_.Init();
		initY_ = 0;
		isMove_ = FALSE;
		moveNo_ = 0;
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGLiAnimRanker::~TUGLiAnimRanker()
	{
		//解放時必ず呼ぶ
		ClearRecord();
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGLiAnimRanker::Refresh(BOOL myscore_f)
	{
		ClearRecord();
		stLiRanker_.myscore_f_ = myscore_f;

		for (s32 i = 0; i < pmgEO_->mgCoU_.rankerData_.GetLineNum(); ++i)
		{
			TUGRcAnimRanker* newrecord = new TUGRcAnimRanker(player_);
			s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
			s64 id_ranker = pmgEO_->mgCoU_.rankerData_.GetLineDt(i, mdre_ranker_pk);
			newrecord->SetRankerDt(id_ranker, i);
			records_.push_back(newrecord);
			GenMakeObj_Record(newrecord, rh);

			//1行目を入れ終わった段階でマイスコア表示必要なとき入れる
			if ((i == 0) && myscore_f && (pmgEO_->mgCoU_.mdlog_Event_.GetLineNum() > 0))
			{
				s32 mid_Event = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_id_m_event);
				s64 mid_ranking = pmgEO_->mgCoM_.GetEventRanking(pmgEO_->mgCoU_.GetNowEvent(), mid_Event);
				s32 scoretype = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDt(mid_ranking, mdm_ranking_scoreType);
				if (scoretype > 0)
				{

					switch (scoretype)
					{
					case 1://1勝数
					{
						TUGRcNormal* newrecord = MakeScoreRecord();
						s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_win);
						std::string strscore = "しょうりすう";
						newrecord->ugname_.SetNameIcon(strscore.c_str(), ugi_RankBest);
						newrecord->SetValue(lib_str::IntToStr(score).c_str());
					}
					break;
					case 2://2連勝数
					{
						TUGRcNormal* newrecord = MakeScoreRecord();
						s32 wins = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_wins);
						std::string strwins = "さいだいれんしょう";
						newrecord->ugname_.SetNameIcon(strwins.c_str(), ugi_RankBest);
						newrecord->SetValue(lib_str::IntToStr(wins).c_str());
					}
					{
						TUGRcNormal* newrecord = MakeScoreRecord();
						s32 nowwins = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_nowwins);
						//ここで挑戦中がみえてるということは０
						if (pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_play_f))
						{
							nowwins = 0;
						}
						std::string strnowwins = "げんざいれんしょう";
						newrecord->ugname_.SetNameIcon(strnowwins.c_str(), ugi_RankNow);
						newrecord->SetValue(lib_str::IntToStr(nowwins).c_str());
					}
					break;
					case 3:// 3最大ダメ
					{
						TUGRcNormal* newrecord = MakeScoreRecord();
						s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_maxdam);
						std::string strscore = "さいだいダメージ";
						newrecord->ugname_.SetNameIcon(strscore.c_str(), ugi_RankBest);
						newrecord->SetValue(lib_str::IntToStr(score).c_str());
					}
					break;
					case 4://4累積ダメ
					{
						TUGRcNormal* newrecord = MakeScoreRecord();
						s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_totaldam);
						std::string strscore = "ごうけいダメージ";
						newrecord->ugname_.SetNameIcon(strscore.c_str(), ugi_RankBest);
						newrecord->SetValue(lib_str::IntToStr(score).c_str());
					}
					break;
					case 5://5倒した数
					{
						TUGRcNormal* newrecord = MakeScoreRecord();
						s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_kill);
						std::string strscore = "たおしたかず";
						newrecord->ugname_.SetNameIcon(strscore.c_str(), ugi_RankBest);
						newrecord->SetValue(lib_str::IntToStr(score).c_str());
					}
					break;
					case 6://6アイテム
					{
						//s32 mid_supply = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDt(mid_ranking, mdm_ranking_id_m_supply);
						//s32 iconno = (mid_supply % 10) % 4;
						TUGRcNormal* newrecord = MakeScoreRecord();

						s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_itemnum);
						newrecord->ugname_.SetNameRankingItem(mid_ranking);
						newrecord->SetValue(lib_str::IntToStr(score).c_str());
					}
					break;
					case 8://8タイムアタック
					{
						TUGRcNormal* newrecord = MakeScoreRecord();
						s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_itemnum);
						std::string strscore = "しあいじかん";
						newrecord->ugname_.SetNameIcon(strscore.c_str(), ugi_RankBest);

						// 99:59以上は　負負：負負　で表示
						// フレーム数として、１００：００になるような値を初回アクセス時入れるようにするので
						// 60 * 60 * 100以上のとき判断でよいかとおもいます
						s64 INVALID_NUM = 60 * 60 * 100;
						std::string st;
						if (score >= INVALID_NUM)
						{
							st = "負負" + EtcWord(base::ewColon) + "負負" + EtcWord(base::ewColon) + "負負";
						}
						else
						{
							st = base::MinSecMSecString(score, FALSE);
						}
						newrecord->SetValue(st.c_str());
					}
					break;
					default: // その他
						break;
					}
				}
			}
		}

		SetZeroPos();
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGLiAnimRanker::ChangeListEnd(BOOL listend_f)
	{
		//uglistend_.SetDraw(listend_f);
		for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
		for (s32 i = 0; i < records2_.size(); ++i) records2_[i]->SetDraw(!listend_f);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	s64 TUGLiAnimRanker::GetCommonID()
	{
		s64 res = NGNUM;
		if (stLiRanker_.myscore_f_)
		{
			if (GetActIndex() >= records2_.size())
			{
				res = records_[GetActIndex() - records2_.size()]->GetCommonID();
			}
		}
		else
		{
			res = records_[GetActIndex()]->GetCommonID();
		}
		return res;
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	s32 TUGLiAnimRanker::GetRecordLimU(void)
	{
		return records_[0]->GetField()->stSO_.stGrp_.limU_;
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	s32 TUGLiAnimRanker::GetRecordLimD(void)
	{
		return records_[0]->GetField()->stSO_.stGrp_.limD_;
	}

	//--------------------------------------------------------------------------------------------
	// 登録されているレコードの番号を取得
	//--------------------------------------------------------------------------------------------
	s32 TUGLiAnimRanker::GetRecordNoByRank(s64 rank)
	{
		s32 ret = NGNUM;
		for (s32 i = 0; i < records_.size(); i++)
		{
			s32 id = records_[i]->GetCommonID();
			s32 rnk = pmgEO_->mgCoU_.rankerData_.GetPKDt(id, mdre_ranker_rank);

			if (rank == rnk)
			{
				ret = i;
				break;
			}
		}
		return ret;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	s32 TUGLiAnimRanker::GetRecordPosX(void)
	{
		f32 x = 0.0f;
		f32 y = 0.0f;

		records_[0]->GetField()->z_GetGlobalPos(&x, &y);
		return x;
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	s32 TUGLiAnimRanker::GetPosYByListNo(s32 no)
	{
		f32 x = 0.0f;
		f32 y = 0.0f;

		records_[0]->GetField()->z_GetGlobalPos(&x, &y);
		y += RECORD_SIZE_HEIGHT * no;
		y += slider_->stSO_.stLineSw_.posY_;
		return y;
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGLiAnimRanker::SetScrollPosByRank(s64 rank)
	{
		for (s32 i = 0; i < records_.size(); i++)
		{
			s32 id = records_[i]->GetCommonID();
			s32 rnk = pmgEO_->mgCoU_.rankerData_.GetPKDt(id, mdre_ranker_rank);

			if (rank == rnk)
			{
				slider_->stSO_.stLineSw_.posY_ = RECORD_SIZE_HEIGHT * i;
				break;
			}
		}
		// 内部の変数にセットされるだけなので
		// updateを呼ばないと実際の座標に反映されない
		Update(FALSE);
	}


	//--------------------------------------------------------------------------------------------
	// 描画
	//--------------------------------------------------------------------------------------------
	void TUGLiAnimRanker::SetDrawByRank(s64 rank, BOOL draw_f)
	{
		for (s32 i = 0; i < records_.size(); i++)
		{
			s32 id = records_[i]->GetCommonID();
			s32 rnk = pmgEO_->mgCoU_.rankerData_.GetPKDt(id, mdre_ranker_rank);

			if (rank == rnk)
			{
				records_[i]->SetDraw(draw_f);
				break;
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	BOOL TUGLiAnimRanker::IsMove(void)
	{
		return isMove_;
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGLiAnimRanker::SetMoveEnd(void)
	{
		isMove_ = FALSE;
		slider_->stSO_.stLineSw_.posY_ = initY_ + moveList_[moveList_.size() - 1];
		moveNo_ = moveList_.size();
		Update(FALSE);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGLiAnimRanker::SetScrollMoveByRank(s32 rank, s32 frame)
	{
		s32 move = 0;
		for (s32 i = 0; i < records_.size(); i++)
		{
			s32 id = records_[i]->GetCommonID();
			s32 rnk = pmgEO_->mgCoU_.rankerData_.GetPKDt(id, mdre_ranker_rank);

			if (rank == rnk)
			{
				move = RECORD_SIZE_HEIGHT * i;
				break;
			}
		}
		move -= slider_->stSO_.stLineSw_.posY_;

		AutoMove(frame, move);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGLiAnimRanker::AutoMove(s32 frame, s32 dist)
	{
		if (isMove_){ return; }

		s32 newDist = dist;
		s32 afterPosY = slider_->stSO_.stLineSw_.posY_ + dist;

		// リストの上限チェック
		if (afterPosY < 0)
		{
			newDist = -slider_->stSO_.stLineSw_.posY_;
		}
		// 下限チェック
		// 24:１レコードの画像縦サイズ
		s32 max = (records_.size() - 5) * RECORD_SIZE_HEIGHT;
		if (afterPosY > max)
		{
			newDist = max - slider_->stSO_.stLineSw_.posY_;
		}

		const f32 _RADIAN = 3.1415f / 180;

		s32 newFrame = frame;
		//s32 newFrame = abs(newDist);
		//newFrame = (newFrame > frame) ? frame : newFrame;

		moveList_.clear();
		moveList_.resize(newFrame);

		f32 a = 90.0f / newFrame;
		for (s32 i = 0; i < newFrame; i++)
		{
			moveList_[i] = sin(i * a * _RADIAN) * newDist;
		}

		initY_ = slider_->stSO_.stLineSw_.posY_;

		moveNo_ = 0;
		isMove_ = TRUE;
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGLiAnimRanker::DoUpdateSub(BOOL draw_f)
	{
		//uglistend_.Update(draw_f);

		if (isMove_)
		{
			if (moveNo_ >= moveList_.size())
			{
				isMove_ = FALSE;
				return;
			}
			slider_->stSO_.stLineSw_.posY_ = initY_ + moveList_[moveNo_];
			moveNo_++;
		}
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	//リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
	void TUGLiAnimRanker::DoClearRecord()
	{
		stLiRanker_.Init();
		for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);

		records_.clear();
		for (s32 i = 0; i < records2_.size(); ++i) Freemem(records2_[i]);

		records2_.clear();
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGLiAnimRanker::DoMakeGroupSub()
	{

		//ev_icon_.push_back(player_->LoadGrp("event", "ev_icon0"));
		//ev_icon_.push_back(player_->LoadGrp("event", "ev_icon1"));
		//ev_icon_.push_back(player_->LoadGrp("event", "ev_icon2"));
		//ev_icon_.push_back(player_->LoadGrp("event", "ev_icon3"));
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGRcNormal* TUGLiAnimRanker::MakeScoreRecord()
	{
		TUGRcNormal* newrecord = new TUGRcNormal(player_);
		s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
		records2_.push_back(newrecord);
		GenMakeObj_Record(newrecord, rh);

		return newrecord;
	}
}
