#include "dbLyHmhasebe.h"
#include "dbSnHome.h"
//=====================================================================================================
//
//
//
//
//=====================================================================================================
namespace db
{

	enum enPage
	{
		pg_Visit,
		pg_Menu,
		pg_TrTeamSelect,	// トレーニング：チーム選択
		pg_TrCharaSelect,	// トレーニング：キャラ選択
		pg_TrCoachSelect,	// トレーニング：コーチ選択
		pg_TrCheck,			// トレーニング：実行確認
		pg_TrCharaLvUp,		// トレーニング：キャラ成長
		pg_TrOneMoreCheck,	// トレーニング：再実行確認

		pg_TeamDisplay,		// 仮の最強チーム表示

		pg_TmMenu,			// チーム確認メニュートップ
		pg_TmCharaMenu,		// チームキャラ確認メニュー
		pg_TmSellCheck,		// チーム売却確認
		pg_TmSellRetry,		// チーム売却通信エラーのリトライ選択画面

		pg_TrTmFilter,		// トレーニングのチームソート
		pg_TmFilter,		// チーム売却のチームソート
		pg_Out,
	};

	enum enMenuList
	{
		ml_Training,		// キャラトレーニングメニュー
		ml_teamDisplay,		// 最強チーム表示

		ml_TeamMenu,		// チーム関連メニュー
		//ml_TeamShotMenu,	// チームのシュートメニュー
		//ml_Info,			// infoメニュー

		mlEND,
	};

	enum enCoachMenuList
	{
		cml_Coach1,		// ばくれつコーチ
		cml_Coach2,		// レインボーコーチ
		cml_Coach3,		// ルシファコーチ
		cmlEND,
	};

	enum enConnect
	{
		cn_Training,
		cn_TmSell,
	};

	const s32 DISP_RESULT_TIMER = 180;

	STRVECTOR CHARA_LV_MAX_MSG =
	{
		"れべるが＿さいだいちに＿なりました",
		"このせんしゅは＿これいじょう＿あげれません",
	};


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TLyHmHasebe::TLyHmHasebe(TSnHome* pScene)
		:TLyHmGen(pScene)
		, ugsp_Hasebe_(this)
		, uglistMenu_(this)
		, ugprof_(this)
		, ugMenuTitle_(this)
		, ugteam_(this)
		, ugtitle_(this)
		, ugDrTeam_(this)
		, uglistTmFilterL_(this)
		, uglistTmFilterR_(this)
		, uglistCoachMenu_(this)
		, ugCoachMenuTitle_(this)
		, uglistMember_(this)
		, ugChStatePlate_(this)
		, isChangePriceType_(FALSE)
	{
		stHasebe_.Init();
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TLyHmHasebe::~TLyHmHasebe(void)
	{
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::DoLoad(void)
	{
		//field_ = MakeObj();

		//ショップフィールド全生成
		ugsp_Hasebe_.MakeGroup(&pScene_->lyHmHome_->ughasebe_);

		//ここの設定はuglistにうつす
		uglistMenu_.MakeGroup(field_, FALSE);
		uglistCoachMenu_.MakeGroup(field_, FALSE);
		ugprof_.MakeGroup(field_, FALSE);

		//---------------------------------------------------------------------
		//メニューレコード
		const s32 MAXLEN = 14;
		for (s32 i = 0; i < mlEND; ++i)
		{
			// キャラクタートレーニング塞ぎ処理
			if (i == ml_Training && !pmgEO_->mgDt_.dtCharaSet_.IsOpenTraingMenu())
			{
				continue;
			}


#ifdef __K_MAKE_HIDE_TEAM_SELL_MENU__
			if (i == ml_TeamMenu) { continue; }
#endif


			TUGRcNormal* newrecord = (TUGRcNormal*)uglistMenu_.MakeObj_Record();
			newrecord->SetCommonID(i);
			switch (i)
			{
			case ml_Training:newrecord->ugname_.SetNameIcon("キャラクタートレーニング", ugi_Info, MAXLEN); break;
			case ml_TeamMenu:newrecord->ugname_.SetNameIcon("チーム＿かくにん・ばいきゃく", ugi_Info, MAXLEN); break;
				//case ml_TeamShotMenu:newrecord->ugname_.SetNameIcon("チーム＿シュートそうび", ugi_Info, MAXLEN); break;
				//case ml_Info:newrecord->ugname_.SetNameIcon("チーム＿きょうかについて", ugi_Info, MAXLEN); break;
			case ml_teamDisplay:newrecord->ugname_.SetNameIcon("さいきょう＿チーム＿かくにん", ugi_Info, MAXLEN); break;
			}
		}

		// メニュータイトル
		ugMenuTitle_.MakeGroup(field_);
		ugMenuTitle_.SetName("トレーニング");
		ugMenuTitle_.SetPos(100, -80);
		ugMenuTitle_.SetDepth(enUIDepth::UIDP_07BUTTONTOP);
		ugMenuTitle_.SetDraw(TRUE);


		//---------------------------------------------------------------------
		//メニューレコード
		// コーチメニューの中身更新
		RefreshCoachMenu();


		// メニュータイトル
		ugCoachMenuTitle_.MakeGroup(field_);
		ugCoachMenuTitle_.SetName("トレーニング");
		ugCoachMenuTitle_.SetPos(100, -80);
		ugCoachMenuTitle_.SetDepth(enUIDepth::UIDP_07BUTTONTOP);
		ugCoachMenuTitle_.SetDraw(FALSE);


		// チームリスト
		uglistTmFilterL_.MakeGroup(field_, TRUE);
		uglistTmFilterR_.MakeGroup(field_, FALSE);
		ugDrTeam_.MakeGroup(field_, TRUE, SIDE0, &uglistTmFilterL_);
		ugDrTeam_.SetDraw(FALSE);

		// メンバーリスト
		uglistMember_.MakeGroup(field_, FALSE);

		// キャラステータスプレート
		ugChStatePlate_.MakeGroup(field_);
		ugChStatePlate_.SetPos(-160, -60);
		ugChStatePlate_.SetDepth(enUIDepth::UIDP_03_5);
		ugChStatePlate_.SetDraw(FALSE);

		//-------------------------------------------------------------------------
		// とりあえずもの
		ugtitle_.MakeGroup(field_, TRUE);
		ugtitle_.SetMessage("げんざいの＿さいきょう＿チーム");
		ugtitle_.SetLRPos(FALSE);
		ugtitle_.SetDraw(FALSE);

		ugteam_.MakeGroup(field_, SIDE0);
		ugteam_.SetDepth(UIDP_09MSGBOX);
		ugteam_.SetDraw(FALSE);


		listvec_.clear();
		listvec_.push_back(&uglistMenu_);
		listvec_.push_back(&uglistCoachMenu_);
		listvec_.push_back(&uglistTmFilterL_);
		listvec_.push_back(&uglistTmFilterR_);
		listvec_.push_back(&uglistMember_);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::DoReady(void)
	{
		stHasebe_.Init();
		uginfo_ = &pScene_->lyHmHome_->uginfo_;

		ugsp_Hasebe_.SetMsg(hsm_Visit);
		DrawList(NULL);
		ugbtn_back_->SetFont("もどる");
		ugbtn_ok_->SetFont("おーけー");
		ugbtn_prev_->SetFont("まえのページ");
		ugbtn_next_->SetFont("つぎのページ");
		ugbtn_back_->SetDraw(FALSE);

		id_supply_ = NGNUM;
		id_team_select_ = NGNUM;
		mid_team_select_ = NGNUM;
		posNoSelect_ = NGNUM;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::DoDown(void)
	{
		pScene_->lyHmHome_->Ready();
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::DrawList(TUGLiGen* drawlist)
	{
		for (s32 i = 0; i < listvec_.size(); ++i)
		{
			listvec_[i]->SetDraw(FALSE);
		}
		if (drawlist != NULL)
		{
			drawlist->SetDraw(TRUE);
			//drawlist->SetZeroPos();
		}
		ugMenuTitle_.SetDraw(uglistMenu_.IsDraw());
		ugCoachMenuTitle_.SetDraw(uglistCoachMenu_.IsDraw());
		ResetObj();
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::ResetObj(void)
	{
		ugbtn_back_->SetDraw(FALSE);
		ugbtn_ok_->SetDraw(FALSE);
		ugbtn_mid_->SetDraw(FALSE);
		ugbtn_next_->SetDraw(FALSE);
		ugbtn_prev_->SetDraw(FALSE);

		ugprof_.SetDraw(FALSE);

		ugDrTeam_.SetDraw(FALSE);
		ugChStatePlate_.SetDraw(FALSE);

		ugtitle_.SetDraw(FALSE);
		ugteam_.SetDraw(FALSE);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::RefreshCoachMenu(void)
	{
		uglistCoachMenu_.ClearRecord();

		LONGVECTOR coachMenuPKVec;
		coachMenuPKVec.clear();
		for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineNum(); i++)
		{
			// ショップタイプがコーチのものだけ抽出
			if (pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, enmdm_SpSyouhin::mdm_spsyouhin_shopType) == enShopType_Coach)
			{
				coachMenuPKVec.push_back(pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, enmdm_SpSyouhin::mdm_spsyouhin_pk));
			}
		}

		// メニューの登録
		for (s32 i = 0; i < coachMenuPKVec.size(); i++)
		{
			TUGRcNormal* newrecord = (TUGRcNormal*)uglistCoachMenu_.MakeObj_Record();

			s64 pk = coachMenuPKVec[i];
			s64 itemID = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(pk, enmdm_SpSyouhin::mdm_spsyouhin_itemID);
			s32 iconNo = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(itemID, enmdm_Supply::mdm_supply_iconNo);
			std::string strName = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(itemID, enmdm_SupplyStr::mdm_supply_name_c32);
			s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(itemID, enmdm_Supply::mdm_supply_price);
			s32 pricetype = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(itemID, enmdm_Supply::mdm_supply_priceType);

			// スター
			if (pricetype == enPriceType_Star)
			{
				// 自分自身を持っていればそれを使用する
				if (pmgEO_->mgCoU_.IsCoachKenUse(itemID))
				{
					price = 1;		// 必要枚数は１固定
					newrecord->SetValue_Coach(itemID, price);
					newrecord->ugname_.ugicon_.SetIconSupply(itemID);
				}
				else
				{
					newrecord->SetValue_Star(price);
					newrecord->ugname_.ugicon_.SetIconSupply(itemID);
				}
				newrecord->ugname_.SetName(strName.c_str());
			}
			// トッド
			else if (pricetype == enPriceType_Todd)
			{
				// 自分自身を持っていればそれを使用する
				if (pmgEO_->mgCoU_.IsCoachKenUse(itemID))
				{
					price = 1;		// 必要枚数は１固定
					newrecord->SetValue_Coach(itemID, price);
					newrecord->ugname_.ugicon_.SetIconSupply(itemID);
				}
				else
				{
					newrecord->SetValue_Todd(price);
					newrecord->ugname_.ugicon_.SetIconSupply(itemID);
				}
				newrecord->ugname_.SetName(strName.c_str());
			}
			// 特殊
			else
			{
				// 自分自身を持っていればそれを使用する
				if (pmgEO_->mgCoU_.IsCoachKenUse(itemID))
				{
					price = 1;		// 必要枚数は１固定
					newrecord->SetValue_Coach(itemID, price);
					newrecord->ugname_.ugicon_.SetIconSupply(itemID);
					newrecord->ugname_.SetName(strName.c_str());
				}
				else
				{
					price = NGNUM;
					//newrecord->SetValue_Coach(itemID, price);
					newrecord->SetValue("もってない");
					newrecord->ugname_.ugicon_.SetIconSupply(itemID);
					newrecord->ugname_.SetName(strName.c_str());
					newrecord->SetDisable(TRUE);
				}
			}

			newrecord->SetCommonID(itemID);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::DoChangePage(s32 pageno)
	{
		switch ((enPage)pageno)
		{
			//----------------------------------------------
		case db::pg_Visit:
			DrawList(&uglistMenu_);
			break;
			//----------------------------------------------
		case db::pg_Menu:
			DrawList(&uglistMenu_);
			ugsp_Hasebe_.SetMsg(hsm_Menu);
			ugbtn_back_->SetDraw(TRUE);
			break;
			//----------------------------------------------
		case db::pg_TrTeamSelect:
			DrawList(NULL);
			ugbtn_back_->SetDraw(TRUE);
			ugbtn_ok_->SetDraw(TRUE);
			ugbtn_ok_->SetFont("このチーム");
			ugbtn_prev_->SetFont("ならべかえ");
			ugbtn_prev_->SetDraw(TRUE);
			ugsp_Hasebe_.SetMsg(hsm_SelectTeam);
			ugDrTeam_.SetLRPos(FALSE);
			ugDrTeam_.SetDraw(TRUE);
			uglistTmFilterL_.Ready(NGNUM);
			ugDrTeam_.ReadyFilter(&uglistTmFilterL_);
			ugDrTeam_.SetDepth(enUIDepth::UIDP_03_5);
			ugDrTeam_.RefreshParam();
			break;
			//----------------------------------------------
		case db::pg_TrCharaSelect:
			DrawList(&uglistMember_);
			ugbtn_back_->SetDraw(TRUE);
			ugbtn_ok_->SetFont("このキャラ");
			ugsp_Hasebe_.SetMsg(hsm_SelectChara);
			uglistMember_.Refresh(id_team_select_, mid_team_select_, FALSE);
			break;
			//----------------------------------------------
		case db::pg_TrCoachSelect:
		{
			DrawList(&uglistCoachMenu_);
			RefreshCoachMenu();

			ugr_star_->RefreshMid();
			ugr_todd_->Refresh();

			ugbtn_back_->SetDraw(TRUE);
			ugChStatePlate_.SetSt(mid_team_select_, posNoSelect_, NGNUM);
			ugChStatePlate_.SetDraw(TRUE);
			s64 charaID = pmgEO_->mgCoM_.GetCharID(mid_team_select_, posNoSelect_);
			ugsp_Hasebe_.SetMsg(hsm_SelectTraining, charaID);
		}
			break;
			//----------------------------------------------
		case db::pg_TrCheck:
		{
			isChangePriceType_ = FALSE;

			ugr_star_->RefreshMid();
			ugr_todd_->Refresh();

			std::string name = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(id_supply_, enmdm_SupplyStr::mdm_supply_name_c32);
			s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(id_supply_, enmdm_Supply::mdm_supply_price);
			s32 pricetype = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(id_supply_, enmdm_Supply::mdm_supply_priceType);

			std::string priceStr;

			// 券をもっているかどうか
			if (pmgEO_->mgCoU_.IsCoachKenUse(id_supply_))
			{
				//const s32 NEED_COACH_KEN = 1;
				//priceStr = lib_str::IntToStr(NEED_COACH_KEN);
				//priceStr += "チケット＿つかいます";


				std::string coachName = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(id_supply_, enmdm_SupplyStr::mdm_supply_name_c32);
				s32 haveNum = pmgEO_->mgCoU_.GetCoachKenNum(id_supply_);
				priceStr = coachName;
				priceStr += "：";
				priceStr += lib_str::IntToStr(haveNum);

				// 残り１枚なら連続購入確認画面に行かないようフラグを立てておく
				if (haveNum == 1)
				{
					isChangePriceType_ = TRUE;
				}
			}
			else
			{
				switch (pricetype)
				{
				case enPriceType::enPriceType_Star:
				{
					priceStr = lib_str::IntToStr(price);
					priceStr += "スター＿つかいます";
				}
					break;

				case enPriceType::enPriceType_Todd:
				{
					priceStr = lib_str::IntToStr(price);
					priceStr += "トッド＿つかいます";
				}
					break;
				}
			}

			s64 effect = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(id_supply_, enmdm_Supply::mdm_supply_effect);
			std::string expStr = "ＥＸＰ＋" + lib_str::IntToStr(effect);
			STRVECTOR v =
			{
				"トレーニングを＿しますか？",
				expStr,
				priceStr,
			};
			SetHudMsg(TRUE, TRUE, &v);
		}
			break;
			//----------------------------------------------
		case db::pg_TrCharaLvUp:
		{
			ugr_star_->RefreshMid();
			ugr_todd_->Refresh();

			s64 getExp = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(id_supply_, enmdm_Supply::mdm_supply_effect);
			ugChStatePlate_.SetAnim(getExp);

			subPage_ = 0;

			// 操作不可にしてもどるボタンも消す
			pmgUI_->SetModal(TRUE);
			ugbtn_back_->SetDraw(FALSE);
		}
			break;
			//----------------------------------------------
		case db::pg_TrOneMoreCheck:
		{
			ugr_star_->RefreshMid();
			ugr_todd_->Refresh();

			isChangePriceType_ = FALSE;

			std::string name = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(id_supply_, enmdm_SupplyStr::mdm_supply_name_c32);
			s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(id_supply_, enmdm_Supply::mdm_supply_price);
			s32 pricetype = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(id_supply_, enmdm_Supply::mdm_supply_priceType);

			std::string priceStr;

			// 券をもっているかどうか
			if (pmgEO_->mgCoU_.IsCoachKenUse(id_supply_))
			{
				//const s32 NEED_COACH_KEN = 1;
				//priceStr = lib_str::IntToStr(NEED_COACH_KEN);
				//priceStr += "チケット＿つかいます";


				std::string coachName = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(id_supply_, enmdm_SupplyStr::mdm_supply_name_c32);
				s32 haveNum = pmgEO_->mgCoU_.GetCoachKenNum(id_supply_);
				priceStr = coachName;
				priceStr += "：";
				priceStr += lib_str::IntToStr(haveNum);

				// 残り１枚なら連続購入確認画面に行かないようフラグを立てておく
				if (haveNum == 1)
				{
					isChangePriceType_ = TRUE;
				}
			}
			else
			{
				switch (pricetype)
				{
				case enPriceType::enPriceType_Star:
				{
					priceStr = lib_str::IntToStr(price);
					priceStr += "スター＿つかいます";
				}
					break;

				case enPriceType::enPriceType_Todd:
				{
					priceStr = lib_str::IntToStr(price);
					priceStr += "トッド＿つかいます";
				}
					break;
				}
			}

			s64 effect = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(id_supply_, enmdm_Supply::mdm_supply_effect);
			std::string expStr = "ＥＸＰ＋" + lib_str::IntToStr(effect);
			STRVECTOR v =
			{
				"つづけて＿トレーニングを＿しますか？",
				expStr,
				priceStr,
			};
			SetHudMsg(TRUE, TRUE, &v);
		}
			break;
			//----------------------------------------------
		case db::pg_TeamDisplay:
		{
			DrawList(NULL);
			ugsp_Hasebe_.SetMsg(hsm_TeamDisplay);
			ugbtn_back_->SetDraw(TRUE);
			ugtitle_.SetDraw(TRUE);
			ugteam_.SetDraw(TRUE);
			s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
			s64 mid_team = pmgEO_->GetTopTeamByMasterID();

			ugteam_.SetTeamView(id_kantoku, mid_team, FALSE);
			//ugteam_.SetItemCnt(mid_team, FALSE);
		}
			break;


			//----------------------------------------------
		case db::pg_TmMenu:
		{
			DrawList(&uglistMember_);
			ugbtn_back_->SetDraw(TRUE);
			ugbtn_ok_->SetFont("うる");
			ugbtn_ok_->SetDraw(TRUE);

			ugbtn_prev_->SetFont("ならべかえ");
			ugbtn_prev_->SetDraw(TRUE);

			ugbtn_next_->SetFont("くわしく");
			ugbtn_next_->SetDraw(TRUE);

			/*
			ugbtn_mid_->SetFont("プレビュー");
			ugbtn_mid_->SetDraw(TRUE);
			*/

			// ドラム表示
			ugDrTeam_.SetLRPos(TRUE);
			ugDrTeam_.SetDraw(TRUE);
			uglistTmFilterR_.Ready(NGNUM);
			ugDrTeam_.ReadyFilter(&uglistTmFilterR_);
			ugDrTeam_.SetDepth(enUIDepth::UIDP_03_5);
			ugDrTeam_.RefreshParam();

			// 今のチームを記憶
			id_team_select_ = ugDrTeam_.GetIDTeam();
			mid_team_select_ = ugDrTeam_.GetMIDTeam();

			// チームメンバーリスト表示
			uglistMember_.Refresh(id_team_select_, mid_team_select_, FALSE);
		}
			break;

			//----------------------------------------------
		case db::pg_TmCharaMenu:
		{
			DrawList(NULL);
			ugbtn_back_->SetDraw(TRUE);
		}
			break;

			//----------------------------------------------
		case db::pg_TmSellCheck:
		{
			// チーム名
			std::string teamStr = pmgEO_->mgCoM_.mdm_Team_.GetPKDtStr(mid_team_sell_, enmdm_TeamStr::mdm_team_name_c32);
			teamStr += "＿を";

			// レアリティ
			s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team_sell_, enmdm_Team::mdm_team_rarity);

			// チームレベル
			s64 overlap = 0;
			for (s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); i++)
			{
				if (pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, enmdTeam::mdteam_id_m_team) == mid_team_sell_)
				{
					overlap = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, enmdTeam::mdteam_overlap);
					break;
				}
			}

			// 売却価格
			s64 price = 0;
			for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Rarity_.GetLineNum(); i++)
			{
				if (pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt(i, enmdm_Rarity::mdm_rarity_pk) == rarity)
				{
					price = pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt(i, enmdm_Rarity::mdm_rarity_priceTeamSell);
					price *= overlap;
					break;
				}
			}
			std::string priceStr = lib_str::IntToStr(price);
			priceStr += "トッドで＿うります";

			STRVECTOR v =
			{
				teamStr,
				priceStr,
				"ほんとうに＿よろしいですか？",
			};
			SetHudMsg(TRUE, TRUE, &v);
		}
			break;

			//----------------------------------------------
		case db::pg_TmSellRetry:
		{
			//1回更新入れる
			frame_->Update(FALSE);

			STRVECTOR v =
			{
				"つうしんに＿しっぱいしました",
				"おーけー＿で＿さいつうしん",
				"ダメダメ＿で＿たいとるに＿もどります",
			};
			SetHudMsg(TRUE, FALSE, &v);
		}
			break;

			//----------------------------------------------
			// チームソート
		case db::pg_TrTmFilter:
			DrawList(&uglistTmFilterL_);
			ugDrTeam_.SetDraw(TRUE);
			ugbtn_back_->SetDraw(TRUE);
			break;

		case db::pg_TmFilter:
			DrawList(&uglistTmFilterR_);
			ugDrTeam_.SetDraw(TRUE);
			ugbtn_back_->SetDraw(TRUE);
			break;

			//----------------------------------------------
		case db::pg_Out:
			UndrawBtn();
			ugsp_Hasebe_.SetMsg(hsm_Out);
			break;

			//----------------------------------------------
		default:
			break;
		}

	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_Visit(void)
	{
		if (ugbtn_back_->IsAct())
		{
			ChangePage(pg_Out);
		}
		else if (ugsp_Hasebe_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
		{
			ChangePage(pg_Menu);
		}
		else if (uglistMenu_.IsAct())
		{
			s32 no = uglistMenu_.GetCommonID();
			switch (no)
			{
			case ml_Training:
				ChangePage(pg_TrTeamSelect);
				break;

			case ml_TeamMenu:
				ChangePage(pg_TmMenu);
				break;
				/*
				case ml_TeamShotMenu:
				break;
				case ml_Info:
				break;
				*/
			case ml_teamDisplay:
				ChangePage(pg_TeamDisplay);
				break;
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_Menu(void)
	{
		if (ugbtn_back_->IsAct())
		{
			ChangePage(pg_Out);//退出
		}
		else if (uglistMenu_.IsAct())
		{
			s32 no = uglistMenu_.GetCommonID();
			switch (no)
			{
			case ml_Training:
				ChangePage(pg_TrTeamSelect);
				break;

			case ml_TeamMenu:
				ChangePage(pg_TmMenu);
				break;

				/*
							case ml_TeamShotMenu:
							break;
							case ml_Info:
							break;
							*/
			case ml_teamDisplay:
				ChangePage(pg_TeamDisplay);
				break;
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// チーム選択
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TrTeamSelect(void)
	{
		if (ugbtn_back_->IsAct())
		{
			ChangePage(pg_Menu);//退出
		}
		else if (ugbtn_ok_->IsAct())
		{
			mid_team_select_ = ugDrTeam_.GetMIDTeam();
			id_team_select_ = ugDrTeam_.GetIDTeam();
			ChangePage(pg_TrCharaSelect);
		}
		else if (ugbtn_prev_->IsAct())
		{
			ChangePage(pg_TrTmFilter);
		}
	}

	//--------------------------------------------------------------------------------------------
	// キャラ選択
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TrCharaSelect(void)
	{
		if (ugbtn_back_->IsAct())
		{
			ChangePage(pg_TrTeamSelect);
		}
		// キャラを選択した
		else if (uglistMember_.IsAct() && uglistMember_.IsSelected())
		{
			ugbtn_ok_->SetDraw(TRUE);
		}
		// キャラの選択を解除した
		else if (uglistMember_.IsAct() && !uglistMember_.IsSelected())
		{
			ugbtn_ok_->SetDraw(FALSE);
		}
		else if (ugbtn_ok_->IsAct())
		{
			posNoSelect_ = uglistMember_.GetSelect();

			// キャラレベル上限チェック
			if (pmgEO_->mgCoU_.IsMyTmCharaLvMax(mid_team_select_, posNoSelect_))
			{
				SetHudMsg(FALSE, FALSE, &CHARA_LV_MAX_MSG);
			}
			else
			{
				ChangePage(pg_TrCoachSelect);
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// コーチ選択
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TrCoachSelect(void)
	{
		if (ugbtn_back_->IsAct())
		{
			ChangePage(pg_TrCharaSelect);
		}
		else if (uglistCoachMenu_.IsAct())
		{
			id_supply_ = uglistCoachMenu_.GetCommonID();
			if (id_supply_ == NGNUM)
			{
				// ブラウザオープン
			}
			else
			{
				ChangePage(pg_TrCheck);
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// 確認画面
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TrCheck(void)
	{
		if (IsHudOK())
		{
			Post_Training();
		}
		else if (IsHudCancel())
		{
			id_supply_ = NGNUM;
			ChangePage(pg_TrCoachSelect);
		}
	}

	//--------------------------------------------------------------------------------------------
	// キャラ成長画面
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TrCharaLvUp(void)
	{
		enum
		{
			LOCAL_PAGE_ANIM = 0,	// アニメ中
			LOCAL_PAGE_WAIT,		// 結果見せ画面中
			LOCAL_PAGE_LIMIT,		// レベルキャップ到達
		};

		switch (subPage_)
		{
		case LOCAL_PAGE_ANIM:

			// 画面をタッチしたら
			// 演出は強制終了
			if (field_->sc_IsCamEnter())
			{
				//ugChStatePlate_.SetSt(mid_team_select_, posNoSelect_, NGNUM);
				ugChStatePlate_.FinishAnim();
			}

			// 演出が終わったら結果を見せる時間を用意
			if (!ugChStatePlate_.IsAnim())
			{
				waitTimer_ = DISP_RESULT_TIMER;
				subPage_ = LOCAL_PAGE_WAIT;
			}
			break;

		case LOCAL_PAGE_WAIT:

			// 結果表示画面が終わったら
			// もしくは画面をタッチしたら再実行確認画面へ
			waitTimer_--;
			if (waitTimer_ < 0 || field_->sc_IsCamEnter())
			{
				// 操作可能にしてもどるボタンも表示する
				pmgUI_->SetModal(FALSE);
				ugbtn_back_->SetDraw(TRUE);

				// レベル制限にひかかったら終わり
				if (pmgEO_->mgCoU_.IsMyTmCharaLvMax(mid_team_select_, posNoSelect_))
				{
					SetHudMsg(FALSE, FALSE, &CHARA_LV_MAX_MSG);
					subPage_ = LOCAL_PAGE_LIMIT;
					return;
				}

				// 今の支払いタイプから変わるようならコーチ選択画面に戻す
				if (isChangePriceType_)
				{
					ChangePage(pg_TrCoachSelect);
				}
				else
				{
					ChangePage(pg_TrOneMoreCheck);
				}
			}
			break;

		case LOCAL_PAGE_LIMIT:
			if (IsHudOK())
			{
				ChangePage(pg_TrCharaSelect);
			}
			break;
		}

	}

	//--------------------------------------------------------------------------------------------
	// 再実行確認画面
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TrOneMoreCheck(void)
	{
		if (IsHudOK())
		{
			Post_Training();
		}
		else if (IsHudCancel())
		{
			id_supply_ = NGNUM;
			ChangePage(pg_TrCoachSelect);
		}
	}


	//--------------------------------------------------------------------------------------------
	//チーム表示
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TeamDisplay(void)
	{
		if (ugbtn_back_->IsAct())
		{
			ChangePage(pg_Menu);//退出
		}
	}



	//--------------------------------------------------------------------------------------------
	// チームメニュートップ
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TmMenu(void)
	{
		if (ugbtn_back_->IsAct())
		{
			ChangePage(pg_Menu);
		}
		else if (ugbtn_ok_->IsAct())
		{
			// 残り1チームなら売れない
			if (ugDrTeam_.GetRecordNum() == 1)
			{
				STRVECTOR v =
				{
					"１チームしか＿もっていないので",
					"うることが＿できません",
				};
				SetHudMsg(FALSE, FALSE, &v);
				return;
			}

			// 売る処理
			mid_team_sell_ = ugDrTeam_.GetMIDTeam();
			ChangePage(pg_TmSellCheck);
		}
		else if (ugDrTeam_.GetActRes() == enUGDrGenAct_Stop)
		{
			// 今のチームを記憶
			id_team_select_ = ugDrTeam_.GetIDTeam();
			mid_team_select_ = ugDrTeam_.GetMIDTeam();

			// チームメンバーリスト表示
			uglistMember_.Refresh(id_team_select_, mid_team_select_, FALSE);
			uglistMember_.SetDraw(TRUE);
		}
		else if (ugbtn_next_->IsAct())
		{
			ugDrTeam_.ura_.SetTeamUraDt(ugDrTeam_.GetMIDTeam());
			ugDrTeam_.RevUra();
		}
		else if (ugbtn_prev_->IsAct())
		{
			ChangePage(pg_TmFilter);
		}
	}
	//--------------------------------------------------------------------------------------------
	// チーム売却確認
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TmCharaMenu(void)
	{
		if (ugbtn_back_->IsAct())
		{
			ChangePage(pg_TmMenu);
		}
	}

	//--------------------------------------------------------------------------------------------
	// チーム売却確認
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TmSellCheck(void)
	{
		if (IsHudOK())
		{
			Post_TmSell();
		}
		else if (IsHudCancel())
		{
			ChangePage(pg_TmMenu);
		}
	}


	//--------------------------------------------------------------------------------------------
	// チーム売却通信再送信
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TmSellRetry(void)
	{
		// 再通信
		if (IsHudOK())
		{
			Post_TmSell();
		}
		// あきらめてタイトルへ
		else if (IsHudCancel())
		{
			Down();
			pScene_->lyHmBar_->SetFilterAll(FALSE);
			pScene_->SetGotoTitle();
		}
	}


	//--------------------------------------------------------------------------------------------
	//共通チームフィルター
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_TmFilter()
	{
		if (ugbtn_back_->IsAct())//おーけー
		{
			if(stLy_.pageNo_ == pg_TmFilter)
			{
				ChangePage(pg_TmMenu);
			}
			else if (stLy_.pageNo_ == pg_TrTmFilter)
			{
				ChangePage(pg_TrTeamSelect);
			}
		}
		else if (uglistTmFilterR_.IsActRes2(enUGLiTmFilter_Filter) ||
				 uglistTmFilterL_.IsActRes2(enUGLiTmFilter_Filter))
		{
			ugDrTeam_.ReadyFilter();
		}
		else if (uglistTmFilterL_.IsActRes2(enUGLiTmFilter_Sort) ||
				 uglistTmFilterR_.IsActRes2(enUGLiTmFilter_Sort))
		{
			ugDrTeam_.ReadySort();
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::MvPage_Out(void)
	{
		if (ugsp_Hasebe_.IsAct())
		{
			Down();
		}
	}

	//--------------------------------------------------------------------------------------------
	// POST トレーニング
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::Post_Training(void)
	{
		if (pmgEO_->stSeq_.IsNoSv())
		{
#ifdef __K_DEBUG_USER_DATA__
			// 通信データを偽装
			kdebug::DebugUDManager::GetInstance()->RunCharaLv(id_supply_, mid_team_select_, posNoSelect_);
#endif
			Connect_Training();
		}
		else
		{
			StartConnect(cn_Training);
			pmgEO_->mgPo_.poHome_.Ready_training(id_supply_, mid_team_select_, posNoSelect_);
		}
	}

	//--------------------------------------------------------------------------------------------
	// POST チーム売却
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::Post_TmSell(void)
	{
		if (pmgEO_->stSeq_.IsNoSv())
		{
#ifdef __K_DEBUG_USER_DATA__
			// 通信データを偽装
			kdebug::DebugUDManager::GetInstance()->RunTeamSell(mid_team_sell_);
#endif
			Connect_TmSell();
		}
		else
		{
			StartConnect(cn_TmSell);
			pmgEO_->mgPo_.poHome_.Ready_sellteam(mid_team_sell_);
		}
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::Connect_Training(void)
	{
#ifdef __K_DEBUG_USER_DATA__
		kdebug::DebugUDManager::GetInstance()->UseItem(id_supply_);
#endif
		// チームのステータス更新
		pmgEO_->RefreshStateTbl(mid_team_select_);
		pScene_->lyHmHome_->RefreshTopTeam();		// ここで最強チームの表示も更新
		ChangePage(pg_TrCharaLvUp);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::Connect_TmSell(void)
	{
		// チームのステータス更新
		pmgEO_->RefreshStateTbl();
		pScene_->lyHmHome_->RefreshTopTeam();		// ここで最強チームの表示も更新
		ChangePage(pg_TmMenu);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::DoConnect(BOOL rqok_f)
	{
		if (rqok_f)//ＯＫのみ
		{
			//code0
			if (pmgEO_->mgPo_.IsCodeZero())
			{
				switch (stLy_.connect_)
				{
				default:
				case cn_Training: Connect_Training(); break;
				case cn_TmSell:	Connect_TmSell(); break;
				}
			}
			else
			{
				switch (stLy_.connect_)
				{
				default:
				case cn_Training:
				case cn_TmSell:
					{
						ChangePage(pg_Menu);
					}
					break;
				}
				//エラーメッセージ(ここでstLy_.connect_のクリアが入ってしまう)
				EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
			}
		}
		else
		{
			//通信エラー
			SetHudEnd();
			switch (stLy_.connect_)
			{
			default:
				case cn_Training:
					ChangePage(pg_TmMenu);
					break;
				case cn_TmSell:
					ChangePage(pg_TmSellRetry);
					break;
			}
		}
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TLyHmHasebe::DoUpdate(BOOL draw_f)
	{
		switch (stLy_.pageNo_)
		{
		case pg_TrCharaSelect:
			ugsp_Hasebe_.Update(draw_f);
			uglistMember_.Update(draw_f);
			break;

		case pg_TeamDisplay:
			ugsp_Hasebe_.Update(draw_f);
			ugtitle_.Update(draw_f);
			ugteam_.Update(draw_f);
			break;

		case pg_TmMenu:
			//ugsp_Hasebe_.Update(draw_f);
			ugDrTeam_.Update(draw_f);
			uglistMember_.Update(draw_f);
			break;

		case pg_TmCharaMenu:
			uglistMember_.Update(draw_f);
			break;

		case pg_TmSellCheck:
		case pg_TmSellRetry:
			break;

		case pg_TrTmFilter:
			uglistTmFilterL_.Update(draw_f);
			ugDrTeam_.Update(draw_f);
			break;
		case pg_TmFilter:
			uglistTmFilterR_.Update(draw_f);
			ugDrTeam_.Update(draw_f);
			break;

		case pg_Out:
			ugsp_Hasebe_.Update(draw_f);
			break;

		default:
			ugsp_Hasebe_.Update(draw_f);
			uglistMenu_.Update(draw_f);
			uglistCoachMenu_.Update(draw_f);
			ugMenuTitle_.Update(draw_f);
			ugCoachMenuTitle_.Update(draw_f);
			ugprof_.Update(draw_f);
			ugChStatePlate_.Update(draw_f);

			ugDrTeam_.Update(draw_f);
			break;
		}

		if (draw_f == FALSE)
		{
			switch (stLy_.pageNo_)
			{
			case pg_Visit: MvPage_Visit(); break;
			case pg_Menu: MvPage_Menu(); break;
			case pg_TrTeamSelect: MvPage_TrTeamSelect(); break;
			case pg_TrCharaSelect: MvPage_TrCharaSelect(); break;
			case pg_TrCoachSelect: MvPage_TrCoachSelect(); break;
			case pg_TrCheck: MvPage_TrCheck(); break;
			case pg_TrCharaLvUp: MvPage_TrCharaLvUp(); break;
			case pg_TrOneMoreCheck: MvPage_TrOneMoreCheck(); break;
			case pg_TeamDisplay: MvPage_TeamDisplay(); break;
			case pg_TmMenu: MvPage_TmMenu(); break;
			case pg_TmCharaMenu: MvPage_TmCharaMenu(); break;
			case pg_TmSellCheck: MvPage_TmSellCheck(); break;
			case pg_TmSellRetry: MvPage_TmSellRetry(); break;
			
			case pg_TrTmFilter:
			case pg_TmFilter:MvPage_TmFilter(); break;
			
			case pg_Out: MvPage_Out(); break;
			}
		}

	}
}