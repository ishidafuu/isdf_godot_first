#include "dbUGBanner.h"
#include "dbUILayer.h"
//=====================================================================================================
//
//	バナークラス
//
//=====================================================================================================
namespace db
{
	enum enPage
	{
		pg_Init = 0,
		pg_BannerDiplay,
		pg_BannerChange,
	};

	// バナー画像の最大数
	const s32 BANNER_GRP_MAX = 9;

	// バルーンの位置
	const s32 BALOON_POSX = 110;
	const s32 BALOON_POSY = 1;

	// バルーンのテキスト
	const std::string BALOON_TEXT = "おすすめ";

	// タブ切替ボタン位置
	const s32 TAB_NEXT_POSX = 135;
	const s32 TAB_PREV_POSX = 13;
	const s32 TAB_POSY = 65;

	// ディスプレイの数
	const s32 DISPLAYS_NUM = 3;

	// ディスプレイの表示時間(というか画面切替演出時間)
	const s32 DISPLAYS_DRAW_TIME = 120;
	
	// ディスプレイの位置
  const INTVECTOR DISPLAYS_POSX = { -2 + 2, 46 + 2, 95 + 2};
	const INTVECTOR DISPLAYS_POSY = { 14, 14, 14 };

	// 砂嵐のアニメーション数
	const s32 SNOW_NOISE_ANIM_MAX = 3;

	// 枠描画位置
	const s32 FRAME_POSX = 0;
	const s32 FRAME_POSY = 11;

	// バナー絵位置
	const s32 BANNER_POSX = 0;
	const s32 BANNER_POSY = 14;

	// フォントサイズ
	const s32 FONT_SIZE = 8;

	// テキスト描画位置
	const s32 TEXT_POSX = 3 + 5;
	const s32 TEXT_POSY = 55;

	// タブページ切替ボタンの絵サイズ(画像サイズとは別)
	const s32 BTN_ICON_SIZE = 8;

	// テキストウェイト時間
	const s32 TEXT_WAIT_TIME = 120;

	// テキストスクロールスピード
	const f32 TEXT_SCROLL_SPEED = 0.3f;

	// テキストフラッシュ時間 // 奇数：非表示時間　偶数：表示時間
	const INTVECTOR TEXT_FLASH_TIME = { 120, 60, 120, 60, 60, 30, 60, 30, 60, 60 };

	// 空テキスト文字(登録しない文字)
	const std::string NULL_CHAR = "＿";


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGBanner::TUGBanner(TUIGenLayer* player)
		: TUGGen(player)
		, ugBln_(player)
		, ugBtnPrev_(player)
		, ugBtnNext_(player)
		, textDrawNo_(NGNUM)
		, textWaitTimer_(NGNUM)
		, textMoveX_(NGNUM)
		, textFlashTimer_(NGNUM)
		, textFlashNo_(NGNUM)
		, pDisplaysVec_(DISPLAYS_NUM, NULL)
		, dispChangeTypeVec_(DISPLAYS_NUM, NGNUM)
		, animNoVec_(DISPLAYS_NUM, NGNUM)
		, mid_banner_id_(NGNUM)
		, dispDrawTimer_(NGNUM)
		, pageNo_(NGNUM)
		, tabNo_(NGNUM)
		, isTabBack_(FALSE)
		, pBannerVec_(BANNER_GRP_MAX, NULL)
	{

	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGBanner::~TUGBanner()
	{
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::MakeGroup(TUIObj* parent)
	{
		SetParent(parent);
		SetDraw(TRUE);

		// バルーン
		ugBln_.MakeGroup_Balloon(field_, BALOON_TEXT.c_str());
		ugBln_.SetPos(BALOON_POSX, BALOON_POSY);

		// TV枠の画像をここで読み込む
		//pGrpBanner_ = player_->LoadGrp("banner", "tv");

		// TV枠
		pFrame_ = MakeObj();
		pFrame_->o_SetParent(field_);
		pFrame_->z_SetPos(FRAME_POSX, FRAME_POSY);
		pFrame_->o_SetObjGrp(pGrpBanner_);
		pFrame_->g_SetDepth(enUIDepth::UIDP_08MSGFILTER);
		//pFrame_->b_SetBtn(TRUE);

		// タブ切替用ボタン
		ugBtnPrev_.MakeGroup(field_, ug_btn_icon, ug_btn_icon2);
		//ugBtnPrev_.ugricon_.SetIcon(ugi_AllCheck);
		ugBtnPrev_.SetPos(TAB_PREV_POSX, TAB_POSY);
		ugBtnPrev_.SetDepth(UIDP_09MSGBOX);

		ugBtnNext_.MakeGroup(field_, ug_btn_icon, ug_btn_icon2);
		//ugBtnNext_.ugricon_.SetIcon(ugi_AllCheck);
		ugBtnNext_.SetPos(TAB_NEXT_POSX, TAB_POSY);
		ugBtnNext_.SetDepth(UIDP_09MSGBOX);

		// テキストエリア
		pText_ = MakeObj();
		pText_->o_SetParent(field_);
		pText_->z_SetPos(TEXT_POSX, TEXT_POSY);	// 後でちゃんとした画像に差し替えてから位置調整
		pText_->g_SetDepth(UIDP_09MSGBOX);

		// バナー絵
		for (s32 i = 0; i < BANNER_GRP_MAX; i++)
		{
			pBannerVec_[i] = MakeObj();
			pBannerVec_[i]->o_SetParent(field_);
			pBannerVec_[i]->g_SetDepth(enUIDepth::UIDP_06BUTTONHI);
			pBannerVec_[i]->z_SetPos(BANNER_POSX, BANNER_POSY);
			pBannerVec_[i]->g_SetDraw(FALSE);
			pBannerVec_[i]->b_SetBtn(TRUE);
		}
		// バナー絵更新
		Refresh();

		f32 myX = 0.0f;
		f32 myY = 0.0f;
		pText_->z_GetGlobalPos(&myX, &myY);
		s32 width = pFrame_->b_GetAutoSize().width_ - 10;	// 単純に横幅だとちょっと大きいので若干小さくしておく
		//pText_->g_SetLimX(myX, myX + width);


		//s32 tab1X = ugBtnPrev_.GetField()->b_GetAutoSize().width_;
		//s32 tab2X = ugBtnNext_.GetField()->b_GetAutoSize().width_;
		//pText_->g_SetLimX(myX, myX + width - tab1X - tab1X - tab2X);
		s32 tx = BTN_ICON_SIZE;
		pText_->g_SetLimX(myX, myX + width - tx - tx + 2); // + 2は補正値

		InitDispParam();
		// フレームにはめこむ3つの画面
		for (s32 i = 0; i < DISPLAYS_NUM; i++)
		{
			pDisplaysVec_[i] = MakeObj();
			pDisplaysVec_[i]->o_SetParent(field_);
			pDisplaysVec_[i]->z_SetPos(DISPLAYS_POSX[i], DISPLAYS_POSY[i]);
			pDisplaysVec_[i]->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_tv_noise));
			pDisplaysVec_[i]->g_SetPartNo(0);
			pDisplaysVec_[i]->g_SetDepth(enUIDepth::UIDP_07BUTTONTOP);

			// ついでにパラメータも初期化
			dispChangeTypeVec_[i] = edct_SnowNoise;	// とりあえずデフォは砂嵐
		}
		DoChangePage(pg_Init);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::SetParam(TUGBannerParam param)
	{
		tabParam_.push_back(param);
	}

	//--------------------------------------------------------------------------------------------
	// タブのデータをセット
	//--------------------------------------------------------------------------------------------
	BOOL TUGBanner::SetTab(s32 tabNo)
	{
		if (tabParam_.size() <= tabNo){ return FALSE; }

		ClearTextData();

		TUGBannerParam param = tabParam_[tabNo];

		// 表示時間切れ
		if (!param.IsDrawDate()){ return FALSE; }

		s32 bannerID = param.GetBannerID();
		STRVECTOR textVec = param.GetText();
		INTVECTOR dispTypeVec = param.GetTextDrawType();
		INTVECTOR changeTypeVec = param.GetDispChangeType();
		
		SetBanner(bannerID);
		SetTextData(textVec, dispTypeVec);
		SetDisplayChangeType(changeTypeVec);

		// 表示テキストをセット
		SetText(textDrawNo_);

		DoChangePage(pg_BannerDiplay);
		tabNo_ = tabNo;

		return TRUE;
	}
	//--------------------------------------------------------------------------------------------
	// タブのデータをセット
	//--------------------------------------------------------------------------------------------
	void TUGBanner::SetNextTab(void)
	{
		if (tabParam_.empty()){ return; }

		s32 now = tabNo_;
		s32 loopCnt = 0;
		s32 tabSize = tabParam_.size();
		while (1)
		{
			tabNo_++;
			if (tabSize <= tabNo_){ tabNo_ = 0; }

			if (SetTab(tabNo_)){ break; }

			// 1周した==表示するものがない
			// tabParamにデータはあるが全部表示期限切れだとnowに-1が入っていて1周したかわからないのでループカウンタも使う
			if (tabNo_ == now || loopCnt >= tabSize)
			{
				SetupEmpty();
				break;
			}
			loopCnt++;
		}
	}
	//--------------------------------------------------------------------------------------------
	// タブのデータをセット
	//--------------------------------------------------------------------------------------------
	void TUGBanner::SetPrevTab(void)
	{
		if (tabParam_.empty()){ return; }

		s32 now = tabNo_;
		s32 loopCnt = 0;
		s32 tabSize = tabParam_.size();
		while (1)
		{
			tabNo_--;
			if (0 > tabNo_){ tabNo_ = tabSize - 1; }
			
			if (SetTab(tabNo_)){ break; }
			
			// 1周した==表示するものがない
			// tabParamにデータはあるが全部表示期限切れだとnowに-1が入っていて1周したかわからないのでループカウンタも使う
			if (tabNo_ == now || loopCnt >= tabSize)
			{
				SetupEmpty();
				break;
			}
			loopCnt++;
		}
	}

	//--------------------------------------------------------------------------------------------
	// テキストデータのクリア(セットする際は最初に1回必ずクリアしておく)
	//--------------------------------------------------------------------------------------------
	void TUGBanner::ClearTextData(void)
	{
		strVec_.clear();
		textDrawTypeVec_.clear();
	}
	//--------------------------------------------------------------------------------------------
	// テキストデータのセット(1行)
	//--------------------------------------------------------------------------------------------
	void TUGBanner::SetTextData(std::string str, s32 type)
	{
		// "＿"だけだったら登録しない
		if (NULL_CHAR.compare(str) == 0){ return; }

		strVec_.push_back(str);
		textDrawTypeVec_.push_back(type);
	}
	//--------------------------------------------------------------------------------------------
	// テキストデータのセット(全部)
	//--------------------------------------------------------------------------------------------
	void TUGBanner::SetTextData(STRVECTOR strVec, INTVECTOR typeVec)
	{
		s32 size = strVec.size();
		for (s32 i = 0; i < size; i++)
		{
			SetTextData(strVec[i], typeVec[i]);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 画面切替方法のクリア(セットする際は最初に1回必ずクリアしておく)
	//--------------------------------------------------------------------------------------------
	void TUGBanner::ClearDisplayType(void)
	{
		for (s32 i = 0; i < DISPLAYS_NUM; i++)
		{
			dispChangeTypeVec_[i] = edct_SnowNoise;
		}
	}
	//--------------------------------------------------------------------------------------------
	// 画面切替方法セット(1個分)
	//--------------------------------------------------------------------------------------------
	void TUGBanner::SetDisplayChangeType(s32 no, s32 type)
	{
		dispChangeTypeVec_[no] = type;
	}
	//--------------------------------------------------------------------------------------------
	// 画面切替方法セット(全て)
	//--------------------------------------------------------------------------------------------
	void TUGBanner::SetDisplayChangeType(INTVECTOR typeVec)
	{
		s32 size = typeVec.size();
		for (s32 i = 0; i < size; i++)
		{
			SetDisplayChangeType(i, typeVec[i]);
		}
	}

	//--------------------------------------------------------------------------------------------
	// バナー画像のリフレッシュ	
	//--------------------------------------------------------------------------------------------
	void TUGBanner::Refresh(void)
	{
		// ロードしてばっかりだとヤバイのでクリアする
		player_->ClearGrp();

		// TV枠が上の関数で消えてしまうので再度読み込む
		pGrpBanner_ = player_->LoadGrp("banner", "bn_tv");
		pFrame_->o_SetObjGrp(pGrpBanner_);

		// 次へ前へボタン
		pGrpBtnR_ = player_->LoadGrp("banner", "bn_btn_r");
		ugBtnNext_.GetField()->o_SetObjGrp(pGrpBtnR_);

		pGrpBtnL_ = player_->LoadGrp("banner", "bn_btn_l");
		ugBtnPrev_.GetField()->o_SetObjGrp(pGrpBtnL_);

		for (s32 i = 0; i < BANNER_GRP_MAX; i++)
		{
			std::string bannername = "bn_" + lib_str::IntToStr1b(i, 5);//数値→1byte文字列
			base::TGrp* banner = player_->LoadGrp("banner", bannername.c_str());//pgrvec_の中に入る
			pBannerVec_[i]->o_SetObjGrp(banner);
			pBannerVec_[i]->g_SetDraw(FALSE);
		}
		pBannerVec_[0]->g_SetDraw(TRUE);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	BOOL TUGBanner::IsAct(void)
	{
		if (pageNo_ != pg_BannerDiplay) { return FALSE; }
		BOOL ret = ugBln_.IsAct();
		for (s32 i = 0; i < BANNER_GRP_MAX; i++)
		{
			ret |= pBannerVec_[i]->b_IsEnter(FALSE);
		}
		return ret;
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	s32 TUGBanner::GetBannerID(void)
	{
		if (tabParam_.empty()) { return NGNUM; }
		return tabParam_[tabNo_].GetBannerID();
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::DoChangePage(s32 page)
	{
		switch (page)
		{
			case pg_Init:
				InitTextParam();
				textDrawNo_ = 0;
				InitDispParam();
				if (tabParam_.empty())
				{
					// 表示するものが何もなければ砂嵐へ
					SetupEmpty();
				}
				else
				{
					if (isTabBack_){ SetPrevTab(); isTabBack_ = FALSE; }
					else 		   { SetNextTab(); }
				}
				return;

			case pg_BannerDiplay:
				ugBtnNext_.stBtn_.disable_f_ = FALSE;
				ugBtnPrev_.stBtn_.disable_f_ = FALSE;

				pText_->g_SetDraw(TRUE);
				ugBln_.SetDraw(TRUE);
				SetDrawDisplays(FALSE);
				break;

			case pg_BannerChange:
				ugBtnNext_.stBtn_.disable_f_ = TRUE;
				ugBtnPrev_.stBtn_.disable_f_ = TRUE;

				pText_->g_SetDraw(FALSE);
				ugBln_.SetDraw(FALSE);
				SetDrawDisplays(TRUE);
				dispDrawTimer_ = 0;
				break;
		}
		pageNo_ = page;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::DoUpdate(BOOL draw_f)
	{
		ugBln_.Update(draw_f);
		pFrame_->Update(draw_f);
		ugBtnPrev_.Update(draw_f);
		ugBtnNext_.Update(draw_f);
		pText_->Update(draw_f);

		for (s32 i = 0; i < BANNER_GRP_MAX; i++)
		{
			pBannerVec_[i]->Update(draw_f);
		}

		// フレームにはめこむ3つの画面
		for (s32 i = 0; i < DISPLAYS_NUM; i++)
		{
			pDisplaysVec_[i]->Update(draw_f);
		}

		switch(pageNo_)
		{
			case pg_BannerDiplay:
				if (ugBtnNext_.IsAct())
				{
					textDrawNo_ = 0;
					DoChangePage(pg_BannerChange);
				}
				else if (ugBtnPrev_.IsAct())
				{
					isTabBack_ = TRUE;
					textDrawNo_ = 0;
					DoChangePage(pg_BannerChange);
				}
				break;
			default:
				break;
		}


		switch (pageNo_)
		{
			default:	  break;
			case pg_Init: break;
			case pg_BannerDiplay: MvPage_BannerDisplay();	break;
			case pg_BannerChange: MvPage_BannerChange();	break;
		}
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::MvPage_Init(void)
	{
		
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::MvPage_BannerDisplay(void)
	{
		switch (textDrawTypeVec_[textDrawNo_])
		{
			case etdt_Scroll: UpdateTextScroll(); break;
			case etdt_Flash:  UpdateTextFlash(); break;
		}

	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::MvPage_BannerChange(void)
	{
		for (s32 i = 0; i < DISPLAYS_NUM; i++)
		{
			switch (dispChangeTypeVec_[i])
			{
				case edct_SnowNoise: UpdateDispSnowNoise(i); break;
				case edct_Custom:	 UpdateDispCustom(i); break;
			}
		}

		// 切替演出
		dispDrawTimer_++;
		if (dispDrawTimer_ >= DISPLAYS_DRAW_TIME)
		{
			//SetNextTab();
			DoChangePage(pg_Init);
		}
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::DoAction(void)
	{
		
	}

	//--------------------------------------------------------------------------------------------
	// 何もデータが入ってないとき用
	//--------------------------------------------------------------------------------------------
	void TUGBanner::SetupEmpty(void)
	{
		for (s32 i = 0; i < DISPLAYS_NUM; i++)
		{
			dispChangeTypeVec_[i] = edct_SnowNoise;
		}
		DoChangePage(pg_BannerChange);
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::SetBanner(s64 mid_banner_id)
	{
		mid_banner_id_ = mid_banner_id;
		for (s32 i = 0; i < BANNER_GRP_MAX; i++)
		{
			if (i == mid_banner_id)
			{
				pBannerVec_[i]->g_SetDraw(TRUE);
			}
			else
			{
				pBannerVec_[i]->g_SetDraw(FALSE);
			}
		}

		// バナー切替用画面は全部非表示
		SetDrawDisplays(FALSE);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::InitDispParam(void)
	{
		// 表示用タイマー初期化
		dispDrawTimer_ = 0;

		// 砂嵐用アニメーション番号初期化
		for (s32 i = 0; i < DISPLAYS_NUM; i++)
		{
			animNoVec_[i] = 0;
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::SetDrawDisplays(BOOL isDraw)
	{
		for (s32 i = 0; i < DISPLAYS_NUM; i++)
		{
			pDisplaysVec_[i]->g_SetDraw(isDraw);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::UpdateDispSnowNoise(s32 no)
	{
		s32 animNo = animNoVec_[no];
		animNo++;
		s32 n = animNo / 8;
		if (animNo >= SNOW_NOISE_ANIM_MAX*8-1) { animNo = 0; }
		pDisplaysVec_[no]->g_SetPartNo(n);
		animNoVec_[no] = animNo;
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::UpdateDispCustom(s32 no)
	{
		// ちょっと未定
	}
	
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::InitTextParam(void)
	{
		// ウェイト用タイマー初期化
		textWaitTimer_ = 0;
		
		// テキストスクロール用変数初期化
		textMoveX_ = 0.0f;

		// テキストフラッシュ用変数初期化
		textFlashTimer_ = 0;
		textFlashNo_ = 0;
	}

	//--------------------------------------------------------------------------------------------
	//  1バナー内のテキスト切替
	//--------------------------------------------------------------------------------------------
	BOOL TUGBanner::SetText(s32 no)
	{
		// 文字セットがない
		if (no >= strVec_.size()) { return FALSE; }

		pText_->g_SetDraw(TRUE);
		pText_->f_MakeFont(strVec_[no].c_str(), FC2_normal);
		textWidth_ = FONT_SIZE * (strVec_[no].size() / 3); // ついでに文字の横幅も調べておく

		// フラッシュのときはテキストを真ん中に
		if (textDrawTypeVec_[no] == etdt_Flash)
		{
			s32 textCenterPos = textWidth_ * 0.5f;
			s32 frameCenterPos = (pFrame_->b_GetAutoSize().width_ - 10) * 0.5f;
			s32 px = TEXT_POSX + frameCenterPos - textCenterPos;

			//s32 tab1X = ugBtnPrev_.GetField()->b_GetAutoSize().width_;
			//px -= tab1X;

			pText_->z_SetPos(px, TEXT_POSY);
		}
		// そうでなければ左詰め
		else
		{
			// 位置もリセット
			pText_->z_SetPos(TEXT_POSX, TEXT_POSY);
		}

		InitTextParam();

		DoChangePage(pg_BannerDiplay);
		return TRUE;
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::UpdateTextScroll(void)
	{
		// ウェイト
		if (TEXT_WAIT_TIME > textWaitTimer_)
		{
			textWaitTimer_++;
			return;
		}

		textMoveX_ -= TEXT_SCROLL_SPEED;
		pText_->z_SetPos(TEXT_POSX + textMoveX_, TEXT_POSY);

		// スクロール終了
		if (textMoveX_ < -textWidth_)
		{
			// スクロール位置リセット
			textMoveX_ = 0;

			// タイマー再度セット
			textWaitTimer_ = 0;

			// 次のテキストセット
			textDrawNo_++;
			
			// 1タブ分のメッセージ全部表示した
			if (textDrawNo_ >= strVec_.size())
			{
				textDrawNo_ = 0;
				DoChangePage(pg_BannerChange);
				return;
			}

			SetText(textDrawNo_);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGBanner::UpdateTextFlash(void)
	{
		textFlashTimer_++;
		if (textFlashTimer_ > TEXT_FLASH_TIME[textFlashNo_])
		{
			textFlashNo_++;
			BOOL isdraw = (textFlashNo_ % 2) == 0 ? TRUE : FALSE;
			pText_->g_SetDraw(isdraw);
			textFlashTimer_ = 0;

			// フラッシュ終了
			if (TEXT_FLASH_TIME.size() <= textFlashNo_)
			{
				textDrawNo_++;

				// 1タブ分のメッセージ全部表示した
				if (textDrawNo_ >= strVec_.size())
				{
					textDrawNo_ = 0;
					DoChangePage(pg_BannerChange);
					return;
				}
				SetText(textDrawNo_);
				return;
			}
		}
	}
}
