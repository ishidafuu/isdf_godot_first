#include "dbUGRanker.h"
//======================================================================================================
//
//	ランキング演出クラス
//
//======================================================================================================
namespace db
{
	const s32 RECORD_WAIT = 60;
	const s32 RECORD_IN_OUT_FRAME = 120;	// 自分のレコードが出入りする速度

	const s32 RECORD_MOVE_FRAME = 120;

	const s32 RANK_TOP_POSITION = 0;
	const s32 RANK_UNDER_POSITION = 4;

	const s32 ANOTHER_MODE_MOVE_NUM = RECORD_DISP_MAX - 2; // 特殊な動きをするときの移動値

	const s32 RECORD_SCROLL_ACTIVE_NUM = 5;	// リストのスクロールが可能になるデータ数


	enum enPage
	{
		pg_wait,
		pg_visit,

		// ランキング演出アニメーション
		pg_Anim1,
		pg_Anim2,
		pg_Anim3,
		pg_AnimEnd,

		pg_out,
	};

	//--------------------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------------------
	TUGRanker::TUGRanker(TUIGenLayer* player)
		: TUGGen(player)
		, pPlayer_(player)
		, ugLiRanker_(player)
		, ugMyRcRanker_(player)
		, frameCnt_(NGNUM)
		, isRankUP_(FALSE)
		, isUseDummy_(FALSE)
		, pageNo_(pg_wait)
		, isEnd_(FALSE)
#ifdef __RANKER_DEBUG__
		, editMode_(0)
		, rankerNum_(100)
#endif
	{
	}

	//--------------------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------------------
	TUGRanker::~TUGRanker()
	{
		for (s32 i = 0; i < RECORD_DISP_MAX; i++){ Freemem(ugDummyRcRanker_[i]); }
		ugDummyRcRanker_.clear();
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::Init(s32 prev, s32 now)
	{
#ifdef __K_DEBUG_USER_DATA__
		kdebug::DebugUDManager::GetInstance()->SetupRankingData(now, prev, 7);
#endif

		// ランキングデータを整理
		pmgEO_->mgCoU_.SetupRankerDataByAnimBefore();

		// ランキングリスト
		ugLiRanker_.MakeGroup(field_, FALSE);
		ugLiRanker_.SetDepth(UIDP_01BGOBJ);
		ugLiRanker_.Refresh(TRUE);
		pmgEO_->mgCoU_.MakeChestRankerByData();//ランカーアイコン作成
		ugLiRanker_.SetDraw(TRUE);

		// スクロール位置のセット
		// 自分のランキングをセット
		prevRank_ = prev;
		nowRank_ = now;

		isRankUP_ = FALSE;
		if (prevRank_ > nowRank_)
		{
			isRankUP_ = TRUE;
		}

		// 画面内での移動かどうかのチェック
		s32 moveRank = abs(prev - now);

		// スクロールしなくてよい(画面内移動)
		if (ANOTHER_MODE_MOVE_NUM >= moveRank)
		{
			// スクロール位置のセット
			if (isRankUP_)
			{
				SetScrollPos(nowRank_);
			}
			else
			{
				SetScrollPos(prevRank_);
			}
		}
		// リストのスクロールが必要(画面外移動) 
		else
		{
			// スクロール位置のセット
			if (isRankUP_)
			{
				SetScrollPos(prev - RANK_UNDER_POSITION);
			}
			else
			{
				SetScrollPos(prev);
			}
		}


		// ダミーランカー
		ugDummyRcRanker_.clear();
		for (s32 i = 0; i < RECORD_DISP_MAX; i++)
		{
			TUGRcAnimRanker* record = new TUGRcAnimRanker(pPlayer_);
			record->MakeGroup(field_, 0, 0, 0, 0);
			record->SetLimY(0, 0);
			record->SetDepth(enUIDepth::UIDP_08MSGFILTER);
			record->SetDraw(FALSE);
			ugDummyRcRanker_.push_back(record);
		}

		// 自分用ダミーランカー
		ugMyRcRanker_.MakeGroup(field_, 0, 0, 0, 0);
		ugMyRcRanker_.SetLimY(0, 0);
		ugMyRcRanker_.SetDepth(enUIDepth::UIDP_08MSGFILTER);
		ugMyRcRanker_.SetDraw(FALSE);

		//ugLiRanker_.GetField()->sc_CamSwipe(FALSE);

		fade_ = MakeObj();
		fade_->z_SetPos_Full(0, 0);
		fade_->o_SetObjFilterGrp(mid::midGetDispL(), mid::midGetDispS(), 0xFF, 0, 0, 0);
		fade_->g_SetAlp(FALSE, 0x80);
		fade_->g_SetDepth(UIDP_08MSGFILTER);
		fade_->g_SetDraw(FALSE);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::Setup(void)
	{
		for (s32 i = 0; i < RECORD_DISP_MAX; i++)
		{
			ugDummyRcRanker_[i]->SetDraw(FALSE);
		}


		dataSize_ = pmgEO_->mgCoU_.rankerData_.GetLineNum();
		if (dataSize_ > RANKERNUM){ dataSize_ = RANKERNUM; }

		// 古いランクを取得
		//SetRank(0, 4);
		//SetRank(4, 0);
		SetRank(prevRank_, nowRank_);

		DoChangePage(pg_visit);
		isEnd_ = FALSE;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	s32 TUGRanker::GetRecordAct(void)
	{
		s32 ret = NGNUM;
		if (ugLiRanker_.IsAct())
		{
			ret = ugLiRanker_.GetCommonID();
		}
		return ret;
	}
	//--------------------------------------------------------------------------------------------
	// 報酬を見てる時用(外から呼んでもらう)
	//--------------------------------------------------------------------------------------------
	void TUGRanker::SetFade(void)
	{
		fade_->g_SetDraw(TRUE);
	}
	//--------------------------------------------------------------------------------------------
	// 報酬を見終わった時用(外から呼んでもらう)
	//--------------------------------------------------------------------------------------------
	void TUGRanker::OutFade(void)
	{
		fade_->g_SetDraw(FALSE);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::DoChangePage(s32 pageno)
	{
		switch (pageno)
		{
		default:
		case pg_visit:
			frameCnt_ = RECORD_WAIT;
			break;
		case pg_Anim1:
		{
			ugMyRcRanker_.SetMoveX(-150, RECORD_IN_OUT_FRAME);
		}
		break;
		case pg_Anim2:
		{
			s32 size = ugLiRanker_.records_.size();
			f32 y;
			s32 num;
			s32 listNo;
			s32 shift;

			// リストのスクロール制限番号(n番目のリストを先頭にスクロール調整できない時)
			s32 limitPosition = size - RECORD_SCROLL_ACTIVE_NUM;
			limitPosition = (limitPosition < 0) ? 0 : limitPosition;

			// リストの番号を取得
			s32 nowListPosition = ugLiRanker_.GetRecordNoByRank(nowRank_);
			s32 prevListPosition = ugLiRanker_.GetRecordNoByRank(prevRank_);

			//-------------------------------------------------------------------------------------
			// ランクアップ時の動き
			if (isRankUP_)
			{
				// スクロール制限番号から何番目か
				// 0番ではなく1番スタートなので-1する
				shift = nowListPosition - limitPosition;

				num = prevRank_ - nowRank_;
				if (isUseDummy_)
				{
					// この時だけ画面に見切れているものも動く
					if (num == ANOTHER_MODE_MOVE_NUM)
					{
						// 上側
						y = ugLiRanker_.GetPosYByListNo(RANK_TOP_POSITION - 1);
						ugDummyRcRanker_[0]->SetMoveTargetY(y, RECORD_IN_OUT_FRAME);

						// 下側
						y = ugLiRanker_.GetPosYByListNo(RANK_UNDER_POSITION + 2);
						ugDummyRcRanker_[RECORD_DISP_MAX - 1]->SetMoveTargetY(y, RECORD_IN_OUT_FRAME);
					}

					for (s32 i = 0; i < num; i++)
					{
						listNo = RANK_TOP_POSITION + 1 + i;
						// RANK_TOP_POSITIONの位置を基準にできない時の位置調整
						if ((size <= RECORD_SCROLL_ACTIVE_NUM) ||
							(size > RECORD_SCROLL_ACTIVE_NUM && (size - nowListPosition) < RECORD_SCROLL_ACTIVE_NUM))
						{
							listNo += shift;
						}
						y = ugLiRanker_.GetPosYByListNo(listNo);
						ugDummyRcRanker_[i + 1]->SetMoveTargetY(y, RECORD_IN_OUT_FRAME);
					}
				}
				else
				{
					// ダミーを使わないということはリストがスクロールする
					ugLiRanker_.SetScrollMoveByRank(nowRank_, RECORD_MOVE_FRAME);
				}

				listNo = RANK_TOP_POSITION;
				// RANK_TOP_POSITIONの位置を基準にできない時の位置調整
				if ((size <= RECORD_SCROLL_ACTIVE_NUM) ||
					(size > RECORD_SCROLL_ACTIVE_NUM && (size - nowListPosition) < RECORD_SCROLL_ACTIVE_NUM))
				{
					listNo += shift;
				}
				y = ugLiRanker_.GetPosYByListNo(listNo);
				ugMyRcRanker_.SetMoveTargetY(y, RECORD_MOVE_FRAME);
			}
			//-------------------------------------------------------------------------------------
			// ランクダウン時の動き
			else
			{
				// スクロール制限番号から何番目か
				// 0番ではなく1番スタートなので-1する
				shift = prevListPosition - limitPosition;

				num = nowRank_ - prevRank_;
				if (isUseDummy_)
				{
					// この時だけ画面に見切れているものも動く
					if (num == ANOTHER_MODE_MOVE_NUM)
					{
						// 上側
						y = ugLiRanker_.GetPosYByListNo(RANK_TOP_POSITION - 2);
						ugDummyRcRanker_[0]->SetMoveTargetY(y, RECORD_IN_OUT_FRAME);

						// 下側
						y = ugLiRanker_.GetPosYByListNo(RANK_UNDER_POSITION + 1);
						ugDummyRcRanker_[RECORD_DISP_MAX - 1]->SetMoveTargetY(y, RECORD_IN_OUT_FRAME);
					}

					for (s32 i = 0; i < num; i++)
					{
						listNo = RANK_TOP_POSITION + i;

						// RANK_TOP_POSITIONの位置を基準にできない時の位置調整
						if ((size <= RECORD_SCROLL_ACTIVE_NUM) ||
							(size > RECORD_SCROLL_ACTIVE_NUM && (size - prevListPosition) < RECORD_SCROLL_ACTIVE_NUM))
						{
							listNo += shift;
						}

						// この時だけ２つ分移動する
						if (num == ANOTHER_MODE_MOVE_NUM)
						{
							listNo = RANK_TOP_POSITION - 1 + i;
						}

						y = ugLiRanker_.GetPosYByListNo(listNo);
						ugDummyRcRanker_[i + 1]->SetMoveTargetY(y, RECORD_IN_OUT_FRAME);
					}
				}
				else
				{
					// ダミーを使わないということはリストがスクロールする
					ugLiRanker_.SetScrollMoveByRank(nowRank_ - RANK_UNDER_POSITION, RECORD_MOVE_FRAME);
				}

				listNo = RANK_TOP_POSITION + num;

				// RANK_TOP_POSITIONの位置を基準にできない時の位置調整
				if ((size <= RECORD_SCROLL_ACTIVE_NUM) ||
					(size > RECORD_SCROLL_ACTIVE_NUM && (size - prevListPosition) < RECORD_SCROLL_ACTIVE_NUM))
				{
					listNo += shift;
				}

				// RANK_UNDER_POSITIONより下にはさせない
				if (listNo >= RANK_UNDER_POSITION)
				{
					listNo = RANK_UNDER_POSITION;
				}

				y = ugLiRanker_.GetPosYByListNo(listNo);
				ugMyRcRanker_.SetMoveTargetY(y, RECORD_MOVE_FRAME);
			}
		}
		break;
		case pg_Anim3:
		{
			f32 x = ugLiRanker_.GetRecordPosX();
			ugMyRcRanker_.SetMoveTargetX(x, RECORD_IN_OUT_FRAME);
		}
		break;
		case pg_AnimEnd:
		{
			// スクロールがおわって無ければ強制終了
			if (ugLiRanker_.IsMove())
			{
				ugLiRanker_.SetMoveEnd();
			}

			// スクロール位置のセット
			if (isRankUP_)
			{
				SetScrollPos(nowRank_);
			}
			else
			{
				//------------------------------------------------
				// この時だけ特殊な動きが必要
				s32 num = 0;
				num = nowRank_ - prevRank_;
				if (num == ANOTHER_MODE_MOVE_NUM)
				{
					ugLiRanker_.SetScrollPosByRank(nowRank_ - RANK_UNDER_POSITION);
				}
				else
				{
					SetScrollPos(nowRank_);
				}
			}

			for (s32 i = 0; i < ugDummyRcRanker_.size(); i++){ ugDummyRcRanker_[i]->SetDraw(FALSE); }
			ugMyRcRanker_.SetDraw(FALSE);
			SetDispRanker(prevRank_, nowRank_, TRUE);
		}
		break;
		case pg_out:
			break;
		}
		pageNo_ = pageno;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::MvPage_Visit()
	{
		frameCnt_--;
		if (frameCnt_ < 0)
		{
			DoChangePage(pg_Anim1);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::MvPage_Anim1()
	{
		if (ugMyRcRanker_.IsMove()) { return; }
		DoChangePage(pg_Anim2);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::MvPage_Anim2()
	{
		if (isUseDummy_)
		{
			for (s32 i = 0; i < ugDummyRcRanker_.size(); i++)
			{
				if (ugDummyRcRanker_[i]->IsMove()){ return; }
			}
		}
		else
		{
			if (ugLiRanker_.IsMove()){ return; }
		}
		if (ugMyRcRanker_.IsMove())	{ return; }

		DoChangePage(pg_Anim3);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::MvPage_Anim3()
	{
		if (!ugMyRcRanker_.IsMove())
		{
			DoChangePage(pg_AnimEnd);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::MvPage_AnimEnd()
	{
		isEnd_ = TRUE;

		// 現在のリストは演出用なので元のサイズに戻しておく
		pmgEO_->mgCoU_.SetupRankerDataByAnimAfter();

		ugLiRanker_.Refresh(TRUE);
		pmgEO_->mgCoU_.MakeChestRankerByData();//ランカーアイコン作成

		// スクロール位置のセット
		if (isRankUP_)
		{
			SetScrollPos(nowRank_);
		}
		else
		{
			//------------------------------------------------
			// この時だけ特殊な動きが必要
			s32 num = 0;
			num = nowRank_ - prevRank_;
			if (num == ANOTHER_MODE_MOVE_NUM)
			{
				ugLiRanker_.SetScrollPosByRank(nowRank_ - RANK_UNDER_POSITION);
			}
			else
			{
				SetScrollPos(nowRank_);
			}
		}
		DoChangePage(pg_out);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::MvPage_Out()
	{

	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::SetRank(s32 prev, s32 now)
	{
		// 自分のランキングをセット
		prevRank_ = prev;
		nowRank_ = now;

		isRankUP_ = FALSE;
		if (prevRank_ > nowRank_)
		{
			isRankUP_ = TRUE;
		}

		// 画面内での移動かどうかのチェック
		s32 moveRank = abs(prev - now);

		// スクロールしなくてよい(画面内移動)
		if (ANOTHER_MODE_MOVE_NUM >= moveRank)
		{
			// スクロール位置のセット
			if (isRankUP_)
			{
				SetScrollPos(nowRank_);
			}
			else
			{
				SetScrollPos(prevRank_);
			}

			// まず対象エリアの正しいランキングを消す
			SetDispRanker(prev, now, FALSE);

			// ダミーのランカーをセット
			SetDummyRanker(prev, now, TRUE);
		}
		// リストのスクロールが必要(画面外移動) 
		else
		{
			// スクロール位置のセット
			if (isRankUP_)
			{
				SetScrollPos(prev - RANK_UNDER_POSITION);
			}
			else
			{
				SetScrollPos(prev);
			}

			// まず対象エリアの正しいランキングを消す
			SetDispRanker(prev, prev, FALSE);	// 申し訳ないがここの人には消えてもらっておく
			SetDispRanker(now, now, FALSE);

			// ダミーのランカーをセット
			SetDummyRanker(prev, now, FALSE);
		}
		prevRank_ = prev;
		nowRank_ = now;
	}
	//--------------------------------------------------------------------------------------------
	// 
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::SetScrollPos(s32 rank)
	{
		s32 size = ugLiRanker_.records_.size();
		s32 no = NGNUM;
		for (s32 i = 0; i < size; i++)
		{
			s32 id = ugLiRanker_.records_[i]->GetCommonID();
			s32 rnk = pmgEO_->mgCoU_.rankerData_.GetPKDt(id, mdre_ranker_rank);

			if (rank == rnk)
			{
				no = i;
				break;
			}
		}


		if (size <= RECORD_SCROLL_ACTIVE_NUM)
		{
			// 一番上のデータにあわせる
			ugLiRanker_.SetScrollPosByRank(pmgEO_->mgCoU_.rankerData_.GetLineDt(0, enmdre_Ranker::mdre_ranker_rank));
		}
		else if ((size - no) >= RECORD_SCROLL_ACTIVE_NUM)
		{
			ugLiRanker_.SetScrollPosByRank(rank);
		}
		else
		{
			s32 lineNo = size - RECORD_SCROLL_ACTIVE_NUM;
			s32 tmpRank = pmgEO_->mgCoU_.rankerData_.GetLineDt(lineNo, mdre_ranker_rank);
			ugLiRanker_.SetScrollPosByRank(tmpRank);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 偽ランカーの情報セット
	// 位置はリストの位置をセットするがスライダーの影響(スワイプ座標)は反映させない
	//--------------------------------------------------------------------------------------------
	void TUGRanker::SetDummyRanker(s32 prev, s32 now, BOOL isDummy)
	{
		f32 x;
		f32 y;
		s32 num;
		s32 listNo;
		s32 shift;

		// スライダーのスクロール制限値
		s32 limu = ugLiRanker_.GetRecordLimU();
		s32 hhh = ugLiRanker_.GetRecordLimD() - limu;

		// リストのサイズ
		s32 size = ugLiRanker_.records_.size();

		// リストのスクロール制限番号(n番目のリストを先頭にスクロール調整できない時)
		s32 limitPosition = size - RECORD_SCROLL_ACTIVE_NUM;
		limitPosition = (limitPosition < 0) ? 0 : limitPosition;

		// リストの番号を取得
		s32 nowListPosition = ugLiRanker_.GetRecordNoByRank(now);
		s32 prevListPosition = ugLiRanker_.GetRecordNoByRank(prev);

		//------------------------------------------------------------------------------------------------------
		// ランクアップ時
		if (isRankUP_)
		{
			// スクロール制限番号から何番目か
			// 0番ではなく1番スタートなので-1する
			shift = nowListPosition - limitPosition;
			
			// いくつズレるか
			num = prev - now;

			if (isDummy)
			{
				//------------------------------------------------
				// この時だけ特殊な動きが必要
				if (num == ANOTHER_MODE_MOVE_NUM)
				{
					// 上限
					if (nowListPosition > 0)
					{
						x = ugLiRanker_.GetRecordPosX();
						y = ugLiRanker_.GetPosYByListNo(RANK_TOP_POSITION - 2);
						ugDummyRcRanker_[0]->SetPos(x, y);
						ugDummyRcRanker_[0]->SetRankerDtDummy(now - 1, now - 1);
						ugDummyRcRanker_[0]->SetDraw(TRUE);
						ugDummyRcRanker_[0]->SetLimY(limu, hhh);
					}

					// 下側
					if (prevListPosition + 1 < ugLiRanker_.records_.size())
					{
						x = ugLiRanker_.GetRecordPosX();
						y = ugLiRanker_.GetPosYByListNo(RANK_UNDER_POSITION + 1);
						ugDummyRcRanker_[RECORD_DISP_MAX - 1]->SetPos(x, y);
						ugDummyRcRanker_[RECORD_DISP_MAX - 1]->SetRankerDtDummy(prev + 1, prev + 1);
						ugDummyRcRanker_[RECORD_DISP_MAX - 1]->SetDraw(TRUE);
						ugDummyRcRanker_[RECORD_DISP_MAX - 1]->SetLimY(limu, hhh);
					}
				}

				for (s32 i = 0; i < num; i++)
				{
					// RANK_TOP_POSITIONの位置を基準にできない時の位置調整
					listNo = RANK_TOP_POSITION + i;
					if ((size <= RECORD_SCROLL_ACTIVE_NUM) ||
						(size > RECORD_SCROLL_ACTIVE_NUM && (size - nowListPosition) < RECORD_SCROLL_ACTIVE_NUM))
					{
						listNo += shift;
					}
					
					// この時だけ特殊な動きが必要
					if (num == ANOTHER_MODE_MOVE_NUM)
					{
						listNo -= 1;
					}

					s32 no = now + i;
					x = ugLiRanker_.GetRecordPosX();
					y = ugLiRanker_.GetPosYByListNo(listNo);
					ugDummyRcRanker_[i + 1]->SetPos(x, y);
					ugDummyRcRanker_[i + 1]->SetRankerDtDummy(no + 1, no);
					ugDummyRcRanker_[i + 1]->SetDraw(TRUE);
					ugDummyRcRanker_[i + 1]->SetLimY(limu, hhh);
				}
			}

			// ランクアップ前のポジションセット
			listNo = RANK_TOP_POSITION + num;
			
			// RANK_TOP_POSITIONの位置を基準にできない時の位置調整
			if ((size <= RECORD_SCROLL_ACTIVE_NUM) ||
				(size > RECORD_SCROLL_ACTIVE_NUM && (size - nowListPosition) < RECORD_SCROLL_ACTIVE_NUM))
			{
				listNo += shift;
			}
			// RANK_UNDER_POSITIONより下にはさせない
			if (listNo >= RANK_UNDER_POSITION)
			{
				listNo = RANK_UNDER_POSITION;
			}
			y = ugLiRanker_.GetPosYByListNo(listNo);
			x = ugLiRanker_.GetRecordPosX();

			ugMyRcRanker_.SetPos(x, y);
			ugMyRcRanker_.SetRankerDtDummy(now, prev);
			ugMyRcRanker_.SetDraw(TRUE);
		}
		//------------------------------------------------------------------------------------------------------
		// ランクダウン時
		else
		{
			// スクロール制限番号から何番目か
			// 0番ではなく1番スタートなので-1する
			shift = prevListPosition - limitPosition;

			// いくつズレるか
			num = now - prev;

			if (isDummy)
			{
				//------------------------------------------------
				// この時だけ特殊な動きが必要
				if (num == ANOTHER_MODE_MOVE_NUM)
				{
					// 上側
					if (prevListPosition > 0)
					{
						x = ugLiRanker_.GetRecordPosX();
						y = ugLiRanker_.GetPosYByListNo(RANK_TOP_POSITION - 1);
						ugDummyRcRanker_[0]->SetPos(x, y);
						ugDummyRcRanker_[0]->SetRankerDtDummy(prev - 1, prev - 1);
						ugDummyRcRanker_[0]->SetDraw(TRUE);
						ugDummyRcRanker_[0]->SetLimY(limu, hhh);
					}

					// 下側
					if (nowListPosition + 1 < ugLiRanker_.records_.size())
					{
						x = ugLiRanker_.GetRecordPosX();
						y = ugLiRanker_.GetPosYByListNo(RANK_UNDER_POSITION + 2);
						ugDummyRcRanker_[RECORD_DISP_MAX - 1]->SetPos(x, y);
						ugDummyRcRanker_[RECORD_DISP_MAX - 1]->SetRankerDtDummy(now + 1, now + 1);
						ugDummyRcRanker_[RECORD_DISP_MAX - 1]->SetDraw(TRUE);
						ugDummyRcRanker_[RECORD_DISP_MAX - 1]->SetLimY(limu, hhh);
					}
				}

				for (s32 i = 0; i < num; i++)
				{
					// RANK_TOP_POSITIONの位置を基準にできない時の位置調整
					listNo = RANK_TOP_POSITION + 1 + i;
					if ((size <= RECORD_SCROLL_ACTIVE_NUM) ||
						(size > RECORD_SCROLL_ACTIVE_NUM && (size - prevListPosition) < RECORD_SCROLL_ACTIVE_NUM))
					{
						listNo += shift;
					}
					
					s32 no = prev + i + 1;
					x = ugLiRanker_.GetRecordPosX();
					y = ugLiRanker_.GetPosYByListNo(listNo);
					ugDummyRcRanker_[i + 1]->SetPos(x, y);
					ugDummyRcRanker_[i + 1]->SetRankerDtDummy(no - 1, no);
					ugDummyRcRanker_[i + 1]->SetDraw(TRUE);
					ugDummyRcRanker_[i + 1]->SetLimY(limu, hhh);
				}
			}

			// ランクアップ前のポジションセット
			listNo = RANK_TOP_POSITION;

			// RANK_TOP_POSITIONの位置を基準にできない時の位置調整
			if ( (size <= RECORD_SCROLL_ACTIVE_NUM) || 
				(size > RECORD_SCROLL_ACTIVE_NUM && (size - prevListPosition) < RECORD_SCROLL_ACTIVE_NUM))
			{
				listNo += shift;
			}
			y = ugLiRanker_.GetPosYByListNo(listNo);
			x = ugLiRanker_.GetRecordPosX();
			ugMyRcRanker_.SetPos(x, y);
			ugMyRcRanker_.SetRankerDtDummy(now, prev);
			ugMyRcRanker_.SetDraw(TRUE);
		}
		isUseDummy_ = isDummy;
	}

	//--------------------------------------------------------------------------------------------
	// ランカーの表示切替
	//--------------------------------------------------------------------------------------------
	void TUGRanker::SetDispRanker(s32 val1, s32 val2, BOOL disp)
	{
		//s32 max = ugLiRanker_.records_.size();

		s32 start = (val1 > val2) ? val2 : val1;
		s32 end = (val1 > val2) ? val1 : val2;

		// 特殊処理
		if ((end - start) == ANOTHER_MODE_MOVE_NUM)
		{
			start -= 1;
			end += 1;
		}

		for (s32 i = start; i <= end; i++)	// endも含む
		{
			if (i <= 0){ continue; }
			//if (i > max){ continue; }
			ugLiRanker_.SetDrawByRank(i, disp);
		}
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGRanker::DoUpdate(BOOL draw_f)
	{
		ugLiRanker_.Update(draw_f);
		ugMyRcRanker_.Update(draw_f);
		for (s32 i = 0; i < RECORD_DISP_MAX; i++) { ugDummyRcRanker_[i]->Update(draw_f); }
		fade_->Update(draw_f);


		// スキップ
		switch (pageNo_)
		{
		case pg_visit:
		case pg_Anim1:
		case pg_Anim2:
		case pg_Anim3:
			if (field_->sc_IsCamEnter())
			{
				DoChangePage(pg_AnimEnd);
			}
			break;
		}

#ifdef __RANKER_DEBUG__
		DebugUpdate();
#endif

		if (draw_f == FALSE)
		{
			switch (pageNo_)
			{
			case pg_visit:MvPage_Visit(); break;
			case pg_Anim1:MvPage_Anim1(); break;
			case pg_Anim2:MvPage_Anim2(); break;
			case pg_Anim3:MvPage_Anim3(); break;
			case pg_AnimEnd:MvPage_AnimEnd(); break;
			case pg_out:MvPage_Out(); break;
			}
		}
	}

#ifdef __RANKER_DEBUG__
	//--------------------------------------------------------------------------------------------
	// デバッグ
	//--------------------------------------------------------------------------------------------
	void TUGRanker::DebugUpdate(void)
	{
		enum EDIT_MODE
		{
			mode_prev = 0,
			mode_now,
			mode_data_num,
			mode_max,
		};

		s32* editData = NULL;
		switch(editMode_)
		{
		case mode_prev:	editData = &prevRank_; break;
		case mode_now:	editData = &nowRank_; break;
		case mode_data_num: editData = &rankerNum_;	break;
		}

		if (isEnd_)
		{
			if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_LEFT_SHOULDER))
			{
				(*editData)--;
				if (editMode_ == mode_data_num) { if ((*editData) < 1){ (*editData) = 1; }	}
				else {	if ((*editData) < 1){ (*editData) = 1; } }
			}
			else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_RIGHT_SHOULDER))
			{
				(*editData)++;
				if (editMode_ == mode_data_num) { if ((*editData) >= 200){ (*editData) = 200; } }
				else { if ((*editData) >= pmgEO_->mgCoU_.rankerData_.GetLineNum()){ (*editData) = pmgEO_->mgCoU_.rankerData_.GetLineNum(); } }
			}
			else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_LEFT_TRIGGER))
			{
				(*editData) -= 10;
				if (editMode_ == mode_data_num) { if ((*editData) < 1){ (*editData) = 1; } }
				else { if ((*editData) < 1){ (*editData) = 1; } }
			}
			else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_RIGHT_TRIGGER))
			{
				(*editData) += 10;
				if (editMode_ == mode_data_num) { if ((*editData) >= 200){ (*editData) = 200; } }
				else { if ((*editData) >= pmgEO_->mgCoU_.rankerData_.GetLineNum()){ (*editData) = pmgEO_->mgCoU_.rankerData_.GetLineNum(); } }
			}
			else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_A))
			{
				pmgEO_->mgCoU_.SetupRankerDataByAnimBefore();
				ugLiRanker_.Refresh(TRUE);
				pmgEO_->mgCoU_.MakeChestRankerByData();//ランカーアイコン作成
				Setup();
			}
			else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_LEFT_STICK_DOWN))
			{
				editMode_++;
				if (editMode_ >= mode_max){ editMode_ = mode_prev; }
			}
			else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_LEFT_STICK_UP))
			{
				editMode_--;
				if (editMode_ < 0){ editMode_ = mode_data_num; }
			}
			else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_B))
			{
				s32 tmp = nowRank_;
				nowRank_ = prevRank_;
				prevRank_ = tmp;
			}
			else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_START))
			{
				prevRank_ = 1;
				nowRank_ = 1;
				kdebug::DebugUDManager::GetInstance()->SetupRankingData(nowRank_, prevRank_, rankerNum_);
				pmgEO_->mgCoU_.SetupRankerDataByAnimBefore();
				ugLiRanker_.Refresh(TRUE);
				pmgEO_->mgCoU_.MakeChestRankerByData();//ランカーアイコン作成
			}
		}

		kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
		char str_c[256];
		sprintf(str_c, "PrevRank:%d", prevRank_);
		kdebug::DebugText::GetInstance()->SetDebugText(16, 10, str_c);

		sprintf(str_c, "NowRank:%d", nowRank_);
		kdebug::DebugText::GetInstance()->SetDebugText(16, 20, str_c);

		sprintf(str_c, "RankerMax:%d", rankerNum_);
		kdebug::DebugText::GetInstance()->SetDebugText(16, 30, str_c);

		sprintf(str_c, "→");


		switch (editMode_)
		{
		case mode_prev:	kdebug::DebugText::GetInstance()->SetDebugText(5, 10, str_c); break;
		case mode_now:	kdebug::DebugText::GetInstance()->SetDebugText(5, 20, str_c); break;
		case mode_data_num: kdebug::DebugText::GetInstance()->SetDebugText(5, 30, str_c);	break;
		}

		if (!isEnd_)
		{
			sprintf(str_c, "アニメーション中です");
			kdebug::DebugText::GetInstance()->SetDebugText(16, 50, str_c);
		}
	}
#endif
}
