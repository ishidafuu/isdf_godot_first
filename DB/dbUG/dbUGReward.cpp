#include "dbUGReward.h"
#include "dbUILayer.h"
//======================================================================================================
//
//	ランキング報酬クラス
//
//======================================================================================================
namespace db
{
	enum enPage
	{
		pg_idle,
		pg_list,
		pg_fade,
		pg_out,
	};


	//--------------------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------------------
	TUGReward::TUGReward(TUIGenLayer* player)
		: TUGGen(player)
		, pPlayer(player)
		, pageNo_(pg_idle)
		, isEnd_(FALSE)
		, isTopRank_(FALSE)
	{
	}

	//--------------------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------------------
	TUGReward::~TUGReward()
	{
		Freemem(ugBtn_);
		Freemem(ugTitleBar_);
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::DoChangePage(s32 pageno)
	{
		switch (pageno)
		{
		default:
		case pg_idle:
			break;
		case pg_list:
			break;
		case pg_out:
			break;
		}
		pageNo_ = pageno;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::Init(void)
	{
		//pGrpBg_ = player_->LoadGrpPart("hmshiai", "hmshiai_next", 152, 80);

		uiBg_ = MakeObj();
		uiBg_->o_SetParent(field_);
		uiBg_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_reward_bg));
		uiBg_->g_SetPartNo(4);
		uiBg_->z_SetPos(0, 0);

		title_ = MakeObj();
		title_->o_SetParent(uiBg_);
		title_->z_SetPos(75, 20);
		title_->f_SetCenter(TRUE);

		text_ = MakeObj();
		text_->o_SetParent(uiBg_);
		text_->z_SetPos(75, 40);
		text_->f_SetCenter(TRUE);
		isUseText2_ = FALSE;

		text2_ = MakeObj();
		text2_->o_SetParent(uiBg_);
		text2_->z_SetPos(75, 56);
		text2_->f_SetCenter(TRUE);
		text2_->g_SetDraw(FALSE);

		ugBtn_ = new TUGButton(pPlayer);
		ugBtn_->MakeGroup(uiBg_, enUGGrp::ug_btn_ok);
		ugBtn_->SetFont("かくにんする");
		ugBtn_->SetPos(75, 60);
		ugBtn_->SetDraw(FALSE);

		fade_ = MakeObj();
		fade_->z_SetPos_Full(0, 0);
		fade_->o_SetObjFilterGrp(mid::midGetDispL(), mid::midGetDispS(), 0xFF, 0, 0, 0);
		fade_->g_SetAlp(FALSE, 0x80);
		fade_->g_SetDepth(UIDP_08MSGFILTER);
		fade_->g_SetDraw(FALSE);

		ugTitleBar_ = new TUGTitle(pPlayer);
		ugTitleBar_->MakeGroup(fade_, FALSE);
		ugTitleBar_->SetPos(230, 35);
		ugTitleBar_->SetDraw(FALSE);

	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::SetPos(s32 x, s32 y)
	{
		uiBg_->z_SetPos(x, y);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::SetNowData(s32 rank)
	{
		std::string str = "げんざい＿" + lib_str::IntToStr(rank) + "い";
		title_->f_MakeFont(str.c_str(), FC2_normal);
		text_->f_MakeFont("もらえる＿ほうしゅうは＿こちら", FC2_normal);
		
		text2_->g_SetDraw(FALSE);
		isUseText2_ = FALSE;

		ugTitleBar_->SetMessage("げんざいの＿ランクの＿ほうしゅう");
        
        // 同じなのでボタン表示しない
        ugBtn_->SetDraw(TRUE);
        isTopRank_ = FALSE;
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::SetNextData(s32 rank, s32 next)
	{
		// 現在の順位が最高(次がないとき)
		if (rank <= 1)
		{
			title_->f_MakeFont("ランキング＿とっぷ＿です心", FC2_normal);
			text_->f_MakeFont("いまの＿じゅんいを＿キープして", FC2_normal);
			text2_->f_MakeFont("ほうしゅうを＿てにいれよう！", FC2_normal);
			text2_->g_SetDraw(TRUE);
			isUseText2_ = TRUE;

			// 同じなのでボタン表示しない
			ugBtn_->SetDraw(FALSE);
			isTopRank_ = TRUE;
		}
		// 報酬がひとつしかない時の表示
		else if (next == NGNUM)
		{
			title_->f_MakeFont("ランキング＿じょうい＿です心", FC2_normal);
			text_->f_MakeFont("いまの＿じゅんいを＿キープして", FC2_normal);
			text2_->f_MakeFont("ほうしゅうを＿てにいれよう！", FC2_normal);
			text2_->g_SetDraw(TRUE);
			isUseText2_ = TRUE;

			// 同じなのでボタン表示しない
			ugBtn_->SetDraw(FALSE);
			isTopRank_ = TRUE;
		}
		else
		{
			std::string str = lib_str::IntToStr(next) + "い＿ランクアップすると";
			title_->f_MakeFont(str.c_str(), FC2_normal);
			text_->f_MakeFont("ほうしゅうが＿ごうかになります", FC2_normal);

			text2_->g_SetDraw(FALSE);
			isUseText2_ = FALSE;

			ugTitleBar_->SetMessage("つぎの＿ランクの＿ほうしゅう");
            
            ugBtn_->SetDraw(TRUE);
            isTopRank_ = FALSE;
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::SetRankOutData(void)
	{
		title_->f_MakeFont("ランキング＿けんがい", FC2_normal);
		text_->f_MakeFont("ほうしゅう＿たいしょうがいです", FC2_normal);
		text2_->f_MakeFont("イベントを＿がんばろう！", FC2_normal);
		text2_->g_SetDraw(TRUE);
		isUseText2_ = TRUE;
		
		ugBtn_->SetDraw(FALSE);
		isTopRank_ = TRUE;
	}



	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::SetEtcData(void)
	{
		SetDraw(FALSE);
		ugTitleBar_->SetMessage("ほうしゅう＿いちらん");
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::SetDraw(BOOL isDraw)
	{
		uiBg_->g_SetDraw(isDraw);
		title_->g_SetDraw(isDraw);
		text_->g_SetDraw(isDraw);
		if (!isTopRank_){ ugBtn_->SetDraw(isDraw); }
		if (isUseText2_){ text2_->g_SetDraw(isDraw); }
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::SetFadeHide(BOOL isHide)
	{
		fade_->g_SetDraw(isHide);
		ugTitleBar_->SetDraw(isHide);
	}

	//--------------------------------------------------------------------------------------------
	// 報酬を見てる時用(外から呼んでもらう)
	//--------------------------------------------------------------------------------------------
	void TUGReward::SetFade(void)
	{
		fade_->g_SetDraw(TRUE);
		ugTitleBar_->SetDraw(TRUE);
		DoChangePage(pg_fade);
	}
	//--------------------------------------------------------------------------------------------
	// 報酬を見終わった時用(外から呼んでもらう)
	//--------------------------------------------------------------------------------------------
	void TUGReward::OutFade(void)
	{
		fade_->g_SetDraw(FALSE);
		ugTitleBar_->SetDraw(FALSE);
		DoChangePage(pg_idle);
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::MvPage_Idle()
	{
		if (ugBtn_->IsAct())
		{
			// とりあえずアクションをセット
			SetAct(0);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::MvPage_List()
	{
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::MvPage_Fade()
	{

	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::MvPage_Out()
	{

	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGReward::DoUpdate(BOOL draw_f)
	{
		uiBg_->Update(draw_f);
		title_->Update(draw_f);
		text_->Update(draw_f);
		text2_->Update(draw_f);
		ugBtn_->Update(draw_f);
		fade_->Update(draw_f);
		ugTitleBar_->Update(draw_f);

#ifdef __REWARD_DEBUG__
		DebugUpdate();
#endif

		if (draw_f == FALSE)
		{
			switch (pageNo_)
			{
			default:
			case pg_idle:MvPage_Idle(); break;
			case pg_list:MvPage_List(); break;
			case pg_fade:MvPage_Fade(); break;
			case pg_out:MvPage_Out(); break;
			}
		}
	}

#ifdef __REWARD_DEBUG__
	//--------------------------------------------------------------------------------------------
	// デバッグ
	//--------------------------------------------------------------------------------------------
	void TUGReward::DebugUpdate(void)
	{
		if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_LEFT_SHOULDER))
		{
		}

		kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
		char str_c[256];

		sprintf(str_c, "報酬デバッグ");
		kdebug::DebugText::GetInstance()->SetDebugText(16, 50, str_c);
	}
#endif
}
