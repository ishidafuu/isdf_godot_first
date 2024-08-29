#include "dbLyShRank.h"
#include "dbSnShiai.h"
#include "dbUGPrev.h"
//======================================================================================================
//
//	ランキング演出シーン
//
//======================================================================================================
namespace db
{
	enum
	{
		pg_visit = 0,
		pg_ranker,
		pg_nowlist,
		pg_nextlist,
		pg_prev,
		pg_kantoku,
		pg_idle,
		pg_out,
	};

	enum enConnect
	{
		cn_ranker = 0,
	};


	//--------------------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------------------
	TLyShRank::TLyShRank(TSnShiai* pScene)
		:TLyShGen(pScene)
		, endBtn_(this)
		//, ugKantoku_(this)
		, ugRanker_(this)
		, ugNowReward_(this)
		, ugNextReward_(this)
		, prevBtn_(this)
		, backBtn_(this)
		, prevPage_(NGNUM)
		, ugProf_(this)
	{
	}

	//--------------------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------------------
	TLyShRank::~TLyShRank()
	{
	}

	//--------------------------------------------------------------------------------------------
	// DoLoad
	//--------------------------------------------------------------------------------------------
	void TLyShRank::DoLoad()
	{
		// 終わらせるボタン
		endBtn_.MakeGroup(field_, enUGGrp::ug_btn_ok);
		endBtn_.SetFont("おわる");
		endBtn_.SetPos(0, 95);
		endBtn_.SetDepth(UIDP_01BGOBJ);
		endBtn_.SetDraw(FALSE);

		//ugKantoku_.MakeGroup(field_, FALSE);
#ifdef __K_DEBUG_USER_DATA__
		// イベントの種類
		mid_ranking_ = 13;

		nowRank_ = 1;
		prevRank_ = 6;
#else
		s64 mid_event = pmgEO_->stSeq_.GetMidEvent();
		mid_ranking_ = NGNUM;
		for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Ranking_.GetLineNum(); i++)
		{
			// 該当のイベントではない
			if (pmgEO_->mgCoM_.mdm_Ranking_.GetLineDt(i, enmdm_Ranking::mdm_ranking_id_m_event) != mid_event){ continue; }
			mid_ranking_ = pmgEO_->mgCoM_.mdm_Ranking_.GetLineDt(i, enmdm_Ranking::mdm_ranking_pk);
			break;
		}
		
		// 0番目に必ずプレイヤーの試合前のランクが入るようになっている
		s64 prevRankPK = 0;
		s64 mbgid = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_mbgID);
		for (s32 i = 0; i < pmgEO_->mgCoU_.mdre_Ranker_.GetLineNum(); i++)
		{
			s64 tmpPk = pmgEO_->mgCoU_.mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_pk);
			if (tmpPk == prevRankPK){ continue; }

			s64 tmpMbgID = pmgEO_->mgCoU_.mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_mbgID);
			if (mbgid == tmpMbgID)
			{
				nowRank_ = pmgEO_->mgCoU_.mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_rank);
				break;
			}
		}
		prevRank_ = pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(prevRankPK, enmdre_Ranker::mdre_ranker_rank);
#endif
		nowRewardRank_ = NGNUM;
		nextRewardRank_ = NGNUM;

		// 今回のイベントの報酬をまとめる
		INTVECTOR rkRewardVec;
		rkRewardVec.clear();
		for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_RkReward_.GetLineNum(); i++)
		{
			// 該当のイベントではない
			if (pmgEO_->mgCoM_.mdm_RkReward_.GetLineDt(i, enmdm_RkReward::mdm_rkreward_id_m_ranking) != mid_ranking_) { continue; }
			rkRewardVec.push_back(pmgEO_->mgCoM_.mdm_RkReward_.GetLineDt(i, enmdm_RkReward::mdm_rkreward_pk));
		}

		for (s32 i = 0; i < rkRewardVec.size(); i++)
		{
			s64 pk = rkRewardVec[i];
			// 該当の順位ではない
			s32 top = pmgEO_->mgCoM_.mdm_RkReward_.GetPKDt(pk, enmdm_RkReward::mdm_rkreward_top);
			s32 bottom = pmgEO_->mgCoM_.mdm_RkReward_.GetPKDt(pk, enmdm_RkReward::mdm_rkreward_bottom);

			// 今の報酬の番号
			if (nowRank_ >= top && bottom >= nowRank_)
			{
				// 自分の報酬ランク
				nowRewardRank_ = bottom;

				// 1位だったときは特にセットしない
				if (i != 0)
				{
					// 今の報酬の次の報酬
					s64 nextPK = rkRewardVec[i - 1];
					nextRewardRank_ = pmgEO_->mgCoM_.mdm_RkReward_.GetPKDt(nextPK, enmdm_RkReward::mdm_rkreward_bottom);
				}
			}
		}

		//-------------------------------------------------------------------------------------------------------
		// 今の報酬
		ugNowReward_.Init();
		ugNowReward_.SetPos(-150, 0);
		ugNowReward_.SetNowData(nowRank_);
		ugNowReward_.SetDraw(FALSE);

		// 今のランクに報酬がない
		if (nowRewardRank_ == NGNUM)
		{
			ugNowReward_.SetRankOutData();

			// 今のランクが分からないと次がどれか分からないので
			// もしランキングに報酬が存在するのであれば
			// 一番後ろのデータを次のデータとしてセットしておく
			if (rkRewardVec.size())
			{
				s64 nextPK = rkRewardVec[rkRewardVec.size() - 1];
				nextRewardRank_ = pmgEO_->mgCoM_.mdm_RkReward_.GetPKDt(nextPK, enmdm_RkReward::mdm_rkreward_bottom);
			}
		}
		// あるときはそのまま今のランクをセット
		else
		{
			ugNowReward_.SetNowData(nowRank_);
		}
		
		//-------------------------------------------------------------------------------------------------------
		// 次の報酬
		ugNextReward_.Init();
		ugNextReward_.SetPos(-150, -80);
		ugNextReward_.SetDraw(FALSE);

		// 次のランクの報酬がない
		if (nextRewardRank_ == NGNUM)
		{
			ugNextReward_.SetNextData(nowRank_, nextRewardRank_);
		}
		// あるときはその順位差を渡す
		else
		{
			ugNextReward_.SetNextData(nowRank_, nowRank_ - nextRewardRank_);
		}

		ugRanker_.Init(prevRank_, nowRank_);

		backBtn_.MakeGroup_FtPosBack(field_);
		backBtn_.SetDepth(UIDP_09MSGBOX);//プライオリティあげて下地のもどるを隠す
		backBtn_.SetDraw(FALSE);

		// シュートプレビュー用
		prevBtn_.MakeGroup_FtPos(field_, 1, "プレビュー");
		prevBtn_.GetField()->z_SetPos_Full(352 - 64, 223);
		//ugbtn_intro_.SetFont("イントロ", FC2_btn_normal);
		prevBtn_.SetDepth(UIDP_09MSGBOX);
		prevBtn_.SetDraw(FALSE);

		// 監督プレビュー用
		ugProf_.MakeGroup(field_, TRUE);
		ugProf_.SetDepth(UIDP_09MSGBOX);
		ugProf_.SetDraw(FALSE);

		// ランクアップBGM
		if (prevRank_ > nowRank_)
		{
			pmgEO_->mgNSF_.Play(nsf_rank_up);
		}
		// ランクダウンBGM
		else if (prevRank_ < nowRank_)
		{
			pmgEO_->mgNSF_.Play(nsf_rank_down);
		}
		// ランク変化無しBGM
		else
		{
			pmgEO_->mgNSF_.Play(nsf_rank_same);
		}

		// プレビューを押されると上書きされるので
		// ここで自分で持っておく
		shiaiType_ = pmgEO_->stShiai_.GetShiaiType();
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::DoReady()
	{
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::DoDown()
	{
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::DoChangePage(s32 pageno)
	{
		switch (pageno)
		{
		default:
		case pg_visit:
			break;
		case pg_ranker:
			ugRanker_.Setup();
			break;
		case pg_idle:
			endBtn_.SetDraw(TRUE);
			ugNextReward_.SetDraw(TRUE);
			ugNowReward_.SetDraw(TRUE);
			break;
		case pg_nowlist:
			break;
		case pg_nextlist:
			break;
		case pg_prev:
		{
			SetHudDraw(FALSE);
			prevBtn_.SetDraw(FALSE);
			//backBtn_.SetDraw(TRUE);

			SOZAIDTVECTOR v;
			v.clear();
			TSozaiData sozai;
			// ここにシュートの素材ベースを決める処理を書く
			sozai.SetMidSozai(mid_sozai_);
			v.push_back(sozai);

			TPrevParam param;
			param.nonebackbtn_f = FALSE;
			param.nonemixbtn_f = FALSE;
			param.nonehavemixbtn_f = TRUE;
			param.sozaivec = &v;
			SetHudPrev(param);
		}
			break;
		case pg_kantoku:
			pmgUI_->SetModal(TRUE);//モーダル
			backBtn_.SetDraw(TRUE);
			backBtn_.SetModal(TRUE);
			break;
		case pg_out:
			break;
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::MvPage_Visit()
	{
		//Post_Ranker();
		ChangePage(pg_ranker);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::MvPage_Ranker()
	{
		if (ugRanker_.IsEnd())
		{
			ChangePage(pg_idle);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::MvPage_NowList()
	{
		if (IsHudItemView())
		{
			ugNowReward_.OutFade();
			ChangePage(pg_idle);
		}
		
		// 選択したものが素材
		if (GetHudItemView().type_ == enItemType_Sozai)
		{
			prevBtn_.SetDraw(TRUE);
		}
		else
		{
			prevBtn_.SetDraw(FALSE);
		}

		if (prevBtn_.IsAct())
		{
			ugNowReward_.SetFadeHide(FALSE);

			prevPage_ = pg_nowlist;
			mid_sozai_ = GetHudItemView().id_;
			ChangePage(pg_prev);
		}
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::MvPage_NextList()
	{
		if (IsHudItemView())
		{
			ugNextReward_.OutFade();
			ChangePage(pg_idle);
		}

		// 選択したものが素材
		if (GetHudItemView().type_ == enItemType_Sozai)
		{
			prevBtn_.SetDraw(TRUE);
		}
		else
		{
			prevBtn_.SetDraw(FALSE);
		}

		if (prevBtn_.IsAct())
		{
			ugNextReward_.SetFadeHide(FALSE);

			prevPage_ = pg_nextlist;
			mid_sozai_ = GetHudItemView().id_;
			ChangePage(pg_prev);
		}
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::MvPage_Preview(void)
	{
		//if (backBtn_.IsAct())
		if (IsHudPrevBack())
		{
			SetPrevEnd();
			prevBtn_.SetDraw(TRUE);
			backBtn_.SetDraw(FALSE);

			if (prevPage_ == pg_nowlist)
			{
				ugNowReward_.SetFadeHide(TRUE);
			}
			else if (prevPage_ == pg_nextlist)
			{
				ugNextReward_.SetFadeHide(TRUE);
			}

			SetHudDraw(TRUE);

			ChangePage(prevPage_);
			prevPage_ = NGNUM;
		}
		else if (IsHudPrevMix())//ランダムボタン
		{
			SetHudPrevMixSozai(mid_sozai_, TRUE);
		}
		else if (IsHudPrevMixClear())
		{
			SetHudPrevMixSozai(mid_sozai_, FALSE);

		}
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::MvPage_Kantoku(void)
	{
		if (backBtn_.IsAct())
		{
			ugRanker_.OutFade();
			pmgUI_->SetModal(FALSE);
			ugProf_.SetDraw(FALSE);
			backBtn_.SetDraw(FALSE);
			ChangePage(pg_idle);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::MvPage_Idle()
	{
		if (endBtn_.IsAct())
		{
			pmgEO_->stSeq_.SetShiaiRanking(FALSE);

			// タイムアタックの時はイベントホームに帰る
			if (shiaiType_ == TStSnShiai::enShiaiType::st_TimeAttack)
			{
				pScene_->lyShBar_->MoveLayer(shl_EventHome);
			}
			else
			{
				pScene_->lyShBar_->MoveLayer(shl_Map);
			}

			ChangePage(pg_out);
		}
		else if (ugNowReward_.IsAct())
		{
			// 報酬をアイテムビューワーにセット
			s32 rank = nowRank_;
			SetHudItemView_Reward(mid_ranking_, rank, FALSE, FALSE, FALSE);
			
			// フェードセット
			ugNowReward_.SetFade();

			ChangePage(pg_nowlist);
		}
		else if (ugNextReward_.IsAct())
		{
			// 報酬をアイテムビューワーにセット
			s32 rank = nextRewardRank_;
			SetHudItemView_Reward(mid_ranking_, rank, FALSE, FALSE, FALSE);
			
			// フェードセット
			ugNextReward_.SetFade();
			
			ChangePage(pg_nextlist);
		}
		else if (ugRanker_.GetRecordAct() != NGNUM)
		{
			s64 id_ranker = ugRanker_.GetRecordAct();
			ugProf_.SetProfRanker(id_ranker);
			ugProf_.SetDraw(TRUE);
			ugRanker_.SetFade();
			ChangePage(pg_kantoku);
		}

	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::MvPage_Out()
	{
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::DoUpdate(BOOL draw_f)
	{
		endBtn_.Update(draw_f);
		//ugKantoku_.Update(draw_f);
		ugRanker_.Update(draw_f);
		ugNowReward_.Update(draw_f);
		ugNextReward_.Update(draw_f);
		prevBtn_.Update(draw_f);
		backBtn_.Update(draw_f);
		ugProf_.Update(draw_f);
		
		if (draw_f == FALSE)
		{
			switch (stLy_.pageNo_)
			{
			case pg_visit:MvPage_Visit(); break;
			case pg_ranker:MvPage_Ranker(); break;
			case pg_nowlist:MvPage_NowList(); break;
			case pg_nextlist: MvPage_NextList(); break;
			case pg_prev:MvPage_Preview(); break;
			case pg_kantoku:MvPage_Kantoku(); break;
			case pg_idle:MvPage_Idle(); break;
			case pg_out:MvPage_Out(); break;
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::Post_Ranker(void)
	{
		if (!pmgEO_->stSeq_.IsNoSv())
		{
			StartConnect(cn_ranker);
			//pmgEO_->mgPo_.poHmShop_.Ready_hukucnt(stHuku_.mid_hukugrp_);
		}
		else
		{
			Connect_Ranker();
		}
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::Connect_Ranker()
	{
		ChangePage(pg_ranker);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyShRank::DoConnect(BOOL rqok_f)
	{
		if (rqok_f)//ＯＫのみ
		{
			//code0
			if (pmgEO_->mgPo_.IsCodeZero())
			{
				switch (stLy_.connect_)
				{
				case cn_ranker: Connect_Ranker(); break;
				}
			}
			else
			{
				//エラーメッセージ
				EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
			}
		}
		else
		{
			//通信エラー
		}
	}
}
