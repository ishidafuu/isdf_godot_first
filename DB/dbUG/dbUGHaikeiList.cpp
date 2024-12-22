#include "dbUGHaikeiList.h"
//=====================================================================================================
//	
//
//
//=====================================================================================================
namespace db
{
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGHaikeiList::TUGHaikeiList(TUIGenLayer* player) 
		: TUGGen(player)
		, ugdrum_haikei_(player)
		, uglistHaikei_(player)
		, ugfilter_(player)
	{
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGHaikeiList::~TUGHaikeiList()
	{
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGHaikeiList::MakeGroup(TUIObj* parent)
	{
		field_ = MakeObj();
		field_->o_SetParent(parent);

		stHaikei_.Init();

		ugfilter_.MakeGroup(field_);
		ugfilter_.OffFilter(2, FALSE);

		nowtitle_ = MakeObj();
		nowtitle_->o_SetParent(field_);
		nowtitle_->g_SetDepth(UIDP_05BUTTON);
		nowtitle_->z_SetPos_Full(mid::midFullDotL_Hf(), 34);
		nowtitle_->f_SetCenter(TRUE);
		//nowtitle_->f_MakeFont("てすと");
		nowcourt_ = MakeObj();
		nowcourt_->o_SetParent(field_);
		nowcourt_->g_SetDepth(UIDP_05BUTTON);
		nowcourt_->z_SetPos_FullBtm(mid::midFullDotL_Hf() - 156, -42);
		//nowcourt_->f_MakeFont("てすと");
		uglistHaikei_.MakeGroup(field_, TRUE);
		uglistHaikei_.Refresh();

		//ドラム
		ugdrum_haikei_.MakeGroup(field_, FALSE, pmgUI_->GetUGGrPart(ugp_icon_films));

#ifdef __K_DEBUG_USER_DATA__
		kdebug::DebugUDManager::GetInstance()->GetAllBG();
#endif

	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	BOOL TUGHaikeiList::IsAct(void)
	{
		return (GetActRes() == enUGHaikeiAct_Select);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGHaikeiList::Ready(void)
	{
		Ready(NGNUM);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGHaikeiList::Ready(s64 mid_event)
	{
		mid_event_ = mid_event;
		stHaikei_.Init();
		uglistHaikei_.Refresh(mid_event);
		SetTitle(0);

		ugdrum_haikei_.SetDraw(stHaikei_.dramdraw_f_);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGHaikeiList::Down()
	{
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGHaikeiList::SetTitle(s32 recordno)
	{
		s32 titleid = uglistHaikei_.records_[recordno]->GetCommonID();
		s32 iconno = pmgEO_->mgDt_.dtTitle_.GetDtPK(titleid, enDtTitle_iconno);

		if (stHaikei_.titleid_ == titleid) return;

		stHaikei_.titleid_ = titleid;
		stHaikei_.iconno_ = iconno;

		mid_haikei_vec_.clear();
		player_->ClearGrp();//pgrvec_をクリア

		//ヘッダタイトル
		nowtitle_->f_MakeFont_Ht(pmgEO_->mgDt_.dtTitle_.GetStrPK(titleid, enDtTitleStr_shortname).c_str(), FC2_normal);


		LONGVECTOR haikeiVec;
		haikeiVec.clear();

		BOOL isSet = FALSE;

		// イベント指定なので特効はいけいを取得してくる
		if (mid_event_ != NGNUM)
		{
			haikeiVec = pmgEO_->mgCoM_.GetTokkouHaikeiByTitle(mid_event_, titleid);
			if (haikeiVec.size() > 0)
			{
				isSet = TRUE;
			}
		}
		
		// セットされてなければとりあえず落ちないようにデフォルトの全部表示
		if (!isSet)
		{
			for (s32 i = 0; i < pmgEO_->mgDt_.dtCourt_.GetLineNum(); ++i)
			{
				//タイトルが違う奴は飛ばす
				if (pmgEO_->mgDt_.dtCourt_.GetDtLine(i, enDtCourt_titleid) != titleid) continue;

				// とりあえず全部出す
				haikeiVec.push_back(pmgEO_->mgDt_.dtCourt_.GetDtLine(i, enDtCourt::enDtCourt_pk));
			}
		}

		//画像を読み込む
		for (s32 i = 0; i < haikeiVec.size(); i++)
		{
			s64 pk = haikeiVec[i];
			std::string thumbname = "t_" + pmgEO_->mgDt_.dtCourt_.GetStrPK(pk, enDtCourtStr_filename);
			player_->LoadGrp_NoColKey("thumb", thumbname.c_str());//レイヤに画像を保存（ドラムにリンク）
			//s64 id_haikei = pmgEO_->mgCoU_.mdHaikei_.GetLineDt(i2, mdhaikei_pk);
			mid_haikei_vec_.push_back(pk);//mid_haikei_vec_に現在のドラムにはいってるＩＤを保存
		}
		ugdrum_haikei_.Ready(player_->GetPgrVec().size());
		//０のときはドラム表示しない
		stHaikei_.dramdraw_f_ = (mid_haikei_vec_.size() != 0);

		//現状表示されてるドラムに画像を割り当てる
		RefreshHaikeiDt(TRUE);

		//ugdrum_haikei_.SetDraw(stHaikei_.dramdraw_f_);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGHaikeiList::RefreshHaikeiDt(BOOL first_f)
	{
		ugdrum_haikei_.SetDraw(stHaikei_.dramdraw_f_);

		if (stHaikei_.dramdraw_f_ == FALSE) return;

		for (s32 i = 0; i < DRRECNUM; ++i)
		{
			s32 dtno = ugdrum_haikei_.stDrGen_.drrecdtno_[i];

			//同じ画像ならとばす
			if ((!first_f) && (ugdrum_haikei_.records_[i]->IsSameGrp((player_->GetPgrVec())[dtno])))
			{
				continue;
			}

			//s64 mid_haikei = pmgEO_->mgCoU_.mdHaikei_.GetPKDt(id_haikei_vec_[dtno], mdhaikei_id_m_haikei);
			ugdrum_haikei_.records_[i]->SetHaikeiDt((player_->GetPgrVec())[dtno], mid_haikei_vec_[dtno]);

			// 持ってる
			BOOL isHide = TRUE;
			if (pmgEO_->mgCoU_.IsHaikei(mid_haikei_vec_[dtno]))
			{
				// 持ってるなら隠さない
				isHide = FALSE;
			}
			ugdrum_haikei_.records_[i]->SetHide(isHide);
		}
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGHaikeiList::MvPage_Haikei()
	{
		if (uglistHaikei_.IsAct())//りすと
		{
			//背景レコードタイトル検索
			SetTitle(uglistHaikei_.GetActIndex());
			s32 mid_haikei = mid_haikei_vec_[ugdrum_haikei_.GetDataNo()];
		}
		else
		{
			//ドラム回転＆カウンタ処理
			if (ugdrum_haikei_.IsActRes(enUGDrHaikeiAct_Roll))
			{
				RefreshHaikeiDt(FALSE);
				s32 mid_haikei = mid_haikei_vec_[ugdrum_haikei_.GetDataNo()];
			}
			//読み込みが必要なタイミング
			else if (ugdrum_haikei_.IsActRes(enUGDrHaikeiAct_Stop))
			{
				if (mid_haikei_vec_.size() > 0)
				{
					stHaikei_.mid_haikei_ = mid_haikei_vec_[ugdrum_haikei_.GetDataNo()];
					std::string courtname = pmgEO_->mgDt_.dtCourt_.GetStrPK(stHaikei_.mid_haikei_, enDtCourtStr_name);
					nowcourt_->f_MakeFont(courtname.c_str(), FC2_normal);
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGHaikeiList::DoUpdate(BOOL draw_f)
	{
		nowtitle_->Update(draw_f);
		nowcourt_->Update(draw_f);
		uglistHaikei_.Update(draw_f);
		ugdrum_haikei_.Update(draw_f);
		ugfilter_.Update(draw_f);

		if (draw_f == FALSE)
		{
			MvPage_Haikei();
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGHaikeiList::DoAction(void)
	{
		//ドラム回転＆カウンタ処理
		if (ugdrum_haikei_.IsActRes(enUGDrHaikeiAct_Stop))
		{
			SetAct(enUGHaikeiAct_Select);
		}
	}
}
