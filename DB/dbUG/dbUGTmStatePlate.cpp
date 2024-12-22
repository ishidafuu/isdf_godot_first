#include "dbUGTmStatePlate.h"
#include "dbUILayer.h"
//======================================================================================================
//
//	チームステータス表示クラス
//
//======================================================================================================
namespace db
{
	//--------------------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------------------
	TUGTmStatePlate::TUGTmStatePlate(TUIGenLayer* player)
		: TUGGen(player)
		, pPlayer(player)
	{
		nowSt_.Init();
		prevSt_.Init();
	}

	//--------------------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------------------
	TUGTmStatePlate::~TUGTmStatePlate()
	{
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTmStatePlate::MakeGroup(TUIObj* parent)
	{
		SetParent(parent);

		uiBg_ = MakeObj();
		uiBg_->o_SetParent(field_);
		uiBg_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_serifu_frame));
		uiBg_->z_SetPos(0, 0);
		uiBg_->g_SetDraw(TRUE);

		for (s32 i = 0; i < TM_PARAM_MAX; i++)
		{
			s32 nameX = 0;
			s32 nameY = 0;
			s32 paramX = 0;
			s32 paramY = 0;
			s32 upX = 0;
			s32 upY = 0;

			// 上３つ、下３つで表示
			if (i < 3)
			{
				nameX	= 2 + i * 48;
				nameY	= 6;
				paramX	= 28 + i * 48;
				paramY	= 12;
				upX		= 28 + i * 48;
				upY		= 4;
			}
			else
			{
				nameX	= 2 + (i - 3) * 48;
				nameY	= 24;
				paramX	= 28 + (i - 3) * 48;
				paramY	= 30;
				upX		= 28 + (i - 3) * 48;
				upY		= 22;
			}

			paramNameTxt_[i] = MakeObj();
			paramNameTxt_[i]->o_SetParent(uiBg_);
			paramNameTxt_[i]->z_SetPos(nameX, nameY);
			paramNameTxt_[i]->g_SetDraw(TRUE);

			paramTxt_[i] = MakeObj();
			paramTxt_[i]->o_SetParent(uiBg_);
			paramTxt_[i]->z_SetPos(paramX, paramY);
			paramTxt_[i]->g_SetDraw(TRUE);

			paramUpTxt_[i] = MakeObj();
			paramUpTxt_[i]->o_SetParent(uiBg_);
			paramUpTxt_[i]->z_SetPos(upX, upY);
			paramUpTxt_[i]->g_SetDraw(TRUE);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTmStatePlate::SetLRPos(BOOL leftpos_f)
	{
		const s32 POSLX = -160;
		const s32 POSRX = 8;
		//const s32 POSY = -64;
		const s32 POSY = 38;

		s32 posx = (leftpos_f)
			? POSLX
			: POSRX;

		field_->z_SetPos(posx, POSY);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTmStatePlate::SetSt(s64 mid_team, s32 overlap, BOOL leftpos_f)
	{
		s64 id_team = NGNUM;
		s64 tmLv = overlap;
		for (s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); ++i)
		{
			if (pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_m_team) == mid_team)
			{
				id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk);
				if (tmLv == NGNUM)
				{
					tmLv = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_overlap);
				}
			}
		}

		// 試合後のドロップで手に入るチームはこの段階ではユーザーデータに入ってないので
		// ここのif文にひっかかる
		if(id_team == NGNUM)
		{
			// 初取得時なので1確定
			tmLv = 1;
		}

		prevSt_.Init();
		nowSt_.Init();

		prevSt_.lv_ = tmLv - 1;
		nowSt_.lv_ = tmLv;

		for (s32 i = 0; i < DBMEMBER_ALL; i++)
		{
			s64 charaid = pmgEO_->mgCoM_.GetCharID(mid_team, i);
			s64 Lv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team, i);

			// 一つ前のレベル時のパラメータ
			// ただし、tmLv == 1は初ゲット時なのでその時は計算しない
			if (prevSt_.lv_ > 1)
			{
				s64 stPow = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Pow, Lv, prevSt_.lv_);
				s64 stGuts = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Guts, Lv, prevSt_.lv_);
				s64 stKire = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Kire, Lv, prevSt_.lv_);
				s64 stTech = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Tech, Lv, prevSt_.lv_);
				s64 shotNum = pmgEO_->mgCoU_.GetOpenShotNum(mid_team, id_team, i, prevSt_.lv_);

				prevSt_.stPow_ += stPow;
				prevSt_.stGuts_ += stGuts;
				prevSt_.stKire_ += stKire;
				prevSt_.stTech_ += stTech;
				prevSt_.shotNum_ += shotNum;
			}


			// 今回分
			{
				s64 stPow = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Pow, Lv, nowSt_.lv_);
				s64 stGuts = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Guts, Lv, nowSt_.lv_);
				s64 stKire = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Kire, Lv, nowSt_.lv_);
				s64 stTech = player_->pmgEO_->CalcSt(mid_team, id_team, charaid, clv_Tech, Lv, nowSt_.lv_);
				s64 shotNum = pmgEO_->mgCoU_.GetOpenShotNum(mid_team, id_team, i, nowSt_.lv_);

				nowSt_.stPow_ += stPow;
				nowSt_.stGuts_ += stGuts;
				nowSt_.stKire_ += stKire;
				nowSt_.stTech_ += stTech;
				nowSt_.shotNum_ += shotNum;
			}
		}

		for (s32 i = 0; i < TM_PARAM_MAX; i++)
		{
			SetText(i);
		}

		SetLRPos(leftpos_f);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTmStatePlate::SetText(s32 no)
	{
		std::string strName;
		std::string strParam;
		std::string strParamUp;
		s64 up = 0;

		switch (no)
		{
		case TM_PARAM_LV:
			{
				strName		= "れべる";
				strParam	= "＿" + lib_str::IntToStr(nowSt_.lv_);
				up = nowSt_.lv_ - prevSt_.lv_;
				strParamUp  = "＋" + lib_str::IntToStr(up);
			}
			break;
		case TM_PARAM_POW:
			{
				strName = "ぱわー";
				strParam = "＿" + lib_str::IntToStr(nowSt_.stPow_);
				up = nowSt_.stPow_ - prevSt_.stPow_;
				strParamUp = "＋" + lib_str::IntToStr(up);
		}
			break;
		case TM_PARAM_GUTS:
			{
				strName = "きあい";
				strParam = "＿" + lib_str::IntToStr(nowSt_.stGuts_);
				up = nowSt_.stGuts_ - prevSt_.stGuts_;
				strParamUp = "＋" + lib_str::IntToStr(up);
			}
			break;
		case TM_PARAM_SHOTNUM:
			{
				strName = "そうび";
				strParam = "＿" + lib_str::IntToStr(nowSt_.shotNum_);
				up = nowSt_.shotNum_ - prevSt_.shotNum_;
				strParamUp = "＋" + lib_str::IntToStr(up);
			}
			break;
		case TM_PARAM_KIRE:
			{
				strName = "すぴん";
				strParam = "＿" + lib_str::IntToStr(nowSt_.stKire_);
				up = nowSt_.stKire_ - prevSt_.stKire_;
				strParamUp = "＋" + lib_str::IntToStr(up);
			}
			break;
		case TM_PARAM_TECH:
			{
				strName = "まもり";
				strParam = "＿" + lib_str::IntToStr(nowSt_.stTech_);
				up = nowSt_.stTech_ - prevSt_.stTech_;
				strParamUp = "＋" + lib_str::IntToStr(up);
			}
			break;
		}

		paramNameTxt_[no]->f_MakeFont(strName.c_str(), FC2_normal);
		paramTxt_[no]->f_MakeFont_Mini(strParam.c_str(), FC2_mini);

		BOOL isUp = FALSE;
		if (up > 0 && prevSt_.lv_ >= 1)
		{
			paramUpTxt_[no]->f_MakeFont_Mini(strParamUp.c_str(), FC2_mini);
			isUp = TRUE;
		}
		paramUpTxt_[no]->g_SetDraw(isUp);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTmStatePlate::DoUpdate(BOOL draw_f)
	{
		uiBg_->Update(draw_f);
		for (s32 i = 0; i < TM_PARAM_MAX; i++)
		{
			paramNameTxt_[i]->Update(draw_f);
			paramTxt_[i]->Update(draw_f);
			paramUpTxt_[i]->Update(draw_f);
		}
#ifdef __TM_STATE_PLATE_DEBUG__
		DebugUpdate();
#endif
	}

#ifdef __TM_STATE_PLATE_DEBUG__
	//--------------------------------------------------------------------------------------------
	// デバッグ
	//--------------------------------------------------------------------------------------------
	void TUGTmStatePlate::DebugUpdate(void)
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
