#include "dbUGChStatePlate.h"
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
	TUGChStatePlate::TUGChStatePlate(TUIGenLayer* player)
		: TUGGen(player)
		, pPlayer(player)
		, ugExp_(player)
		, ugExpUp_(player)
	{
		nowSt_.Init();
		prevSt_.Init();
		nextExp_ = 0;
		isExpAnim_ = FALSE;
		isLvMax_ = FALSE;
	}

	//--------------------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------------------
	TUGChStatePlate::~TUGChStatePlate()
	{
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGChStatePlate::MakeGroup(TUIObj* parent)
	{
		SetParent(parent);

		uiBg_ = MakeObj();
		uiBg_->o_SetParent(field_);
		uiBg_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_reward_bg));
		uiBg_->z_SetPos(0, 0);
		uiBg_->g_SetDraw(TRUE);

		uiBody_ = MakeObj();
		uiBody_->o_SetParent(uiBg_);
		uiBody_->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconBody_[0]);
		uiBody_->z_SetPos(8, 30);

		uiName_ = MakeObj();
		uiName_->o_SetParent(uiBg_);
		uiName_->z_SetPos(10, 6);
		uiName_->g_SetDraw(TRUE);

		const s32 PX = 0;
		const s32 PY = 1;
		const s32 INTERVAL = 3;
		INTVECTOR2 v =
		{
			// れべる
			{ 10, 24 },	// name
			{ 38, 30 },	// param
			{ 38, 22 }, // up

			// ぱわー
			{ 46, 40 },	// name
			{ 72, 46 },	// param
			{ 72, 38 }, // up

			// きあい
			{ 98, 40 },	// name
			{ 124, 46 },	// param
			{ 124, 38 }, // up

			// すぴん
			{ 46, 58 },	// name
			{ 72, 64 },	// param
			{ 72, 56 }, // up

			// まもり
			{ 98, 58 },	// name
			{ 124, 64 },	// param
			{ 124, 56 }, // up

			// けいけんち
			{ 88, 16 },	// name
			{ 110, 13 },	// param
			{ 106, 5 },  // up
		};

		for (s32 i = 0; i < TM_PARAM_MAX; i++)
		{
			s32 nameX = v[i*INTERVAL][PX];
			s32 nameY = v[i*INTERVAL][PY];
			s32 paramX = v[i*INTERVAL + 1][PX];
			s32 paramY = v[i*INTERVAL + 1][PY];
			s32 upX = v[i*INTERVAL + 2][PX];
			s32 upY = v[i*INTERVAL + 2][PY];

			paramNameTxt_[i] = MakeObj();
			paramNameTxt_[i]->o_SetParent(uiBg_);
			paramNameTxt_[i]->z_SetPos(nameX, nameY);
			paramNameTxt_[i]->g_SetDraw(TRUE);

			paramTxt_[i] = MakeObj();
			paramTxt_[i]->o_SetParent(uiBg_);
			paramTxt_[i]->z_SetPos(paramX, paramY + 3);
			paramTxt_[i]->g_SetDraw(TRUE);

			// 経験値のところだけは別のものを使う
			if (i == TM_PARAM_EXP)
			{
				ugExp_.MakeGroup(uiBg_);
				ugExp_.SetPos(paramX, paramY);
				ugExpUp_.MakeGroup(uiBg_);
				ugExpUp_.SetPos(upX, upY);

				paramUpTxt_[i] = NULL;
				continue;
			}

			paramUpTxt_[i] = MakeObj();
			paramUpTxt_[i]->o_SetParent(uiBg_);
			paramUpTxt_[i]->z_SetPos(upX, upY);
			paramUpTxt_[i]->g_SetDraw(TRUE);
		}

		// Exp枠
		uiExpBarBody_ = MakeObj();
		uiExpBarBody_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_bar_stamina));
		uiExpBarBody_->o_SetParent(uiBg_);
		uiExpBarBody_->z_SetPos(88, 22);

		//スタミナバー
		uiExpBar_ = MakeObj();
		uiExpBar_->o_SetObjFilterGrp(50, 3, 0xFF, 0xFF, 179, 15);//50の値が変動
		uiExpBar_->z_SetPos(1, 1);
		uiExpBar_->o_SetParent(uiExpBarBody_);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGChStatePlate::SetSt(s64 mid_team, s32 posNo, s32 overlap)
	{
		isLvMax_ = FALSE;
		mid_team_ = mid_team;
		posNo_ = posNo;

		id_team_ = NGNUM;
		teamLv_ = overlap;
		for (s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); ++i)
		{
			if (pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_m_team) == mid_team)
			{
				id_team_ = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk);
				if (teamLv_ == NGNUM)
				{
					teamLv_ = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_overlap);
				}
			}
		}

		// 試合後のドロップで手に入るチームはこの段階ではユーザーデータに入ってないので
		// ここのif文にひっかかる
		if (id_team_ == NGNUM)
		{
			// 初取得時なので1確定
			teamLv_ = 1;
		}

		prevSt_.Init();
		nowSt_.Init();
		upSt_.Init();

		id_chara_ = pmgEO_->mgCoM_.GetCharID(mid_team, posNo_);
		s64 Lv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team, posNo_);
		s64 exp = pmgEO_->mgCoU_.GetMyTmCharaExp(mid_team, posNo_);
		s64 nextExp = pmgEO_->mgCoM_.mdm_LvUp_.GetPKDt(Lv, enmdm_LvUp::mdm_lvup_exp_for_next);

		nowSt_.lv_  = Lv;
		nowSt_.exp_ = exp;

		strName_ = pmgEO_->mgDt_.dtChar_.GetName(id_chara_);
		{
			nowSt_.stPow_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Pow, Lv, teamLv_);
			nowSt_.stGuts_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Guts, Lv, teamLv_);
			nowSt_.stKire_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Kire, Lv, teamLv_);
			nowSt_.stTech_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Tech, Lv, teamLv_);
		}

		prevSt_ = nowSt_;

		// 限界に到達
		s64 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team_, enmdm_Team::mdm_team_rarity);
		if (pmgEO_->mgCoU_.IsCharaLvMax(nowSt_.lv_, rarity))
		{
			isLvMax_ = TRUE;
		}

		for (s32 i = 0; i < TM_PARAM_MAX; i++)
		{
			SetText(i);
		}

		// 現在の経験値状態をバーにセット
		ugExp_.SetNum(nextExp - exp, FALSE);
		SetExpBar(exp, nextExp);

		// 名前
		uiName_->f_MakeFont(strName_.c_str(), FC2_normal);
		// キャラグラフィック
		uiBody_->g_SetPartNo(posNo_);

		// もしアニメーション中なら中止
		isExpAnim_ = FALSE;
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGChStatePlate::SetAnim(f64 addExp)
	{
		const f64 ANIM_FRAME = 180.0f;

		// 続けてアニメーションすると前のデータが残ってしまうので
		// ここで一旦クリアしてしまう
		prevSt_ = nowSt_;
		upSt_.Init();
		for (s32 i = 0; i < TM_PARAM_MAX; i++)
		{
			SetText(i);
		}

		// もらえる経験値
		addExp_ = addExp;

		isLvMax_ = FALSE;

		// ここでレベルキャップにひかかったかのチェック
		if (pmgEO_->mgCoU_.IsMyTmCharaLvMax(mid_team_, posNo_))
		{
			s64 lv = nowSt_.lv_;
			s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team_, enmdm_Team::mdm_team_rarity);
			s64 needExp = 0;

			// 最大まで行くのであれば余分な経験値を削る
			while (!pmgEO_->mgCoU_.IsCharaLvMax(lv, rarity))
			{
				needExp += pmgEO_->mgCoM_.mdm_LvUp_.GetPKDt(lv, enmdm_LvUp::mdm_lvup_exp_for_next);
				lv++;
			}

			// もらえる経験値をレベルキャップ値までにする
			addExp_ = needExp - nowSt_.exp_;
		}
		
		//animExp_ = 0;
		upSt_.exp_ = 0;

		// １回で移動するゲージの値
		animMoveExp_ = (addExp_ < ANIM_FRAME) ? 1 : addExp_ / ANIM_FRAME;
		isExpAnim_ = TRUE;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGChStatePlate::FinishAnim(void)
	{
		// アニメしてなかったらなにもしない
		if(!isExpAnim_){ return; }

		// 今のレベルを取得
		s64 lv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team_, posNo_);

		// 今の経験値
		s64 now = pmgEO_->mgCoU_.GetMyTmCharaExp(mid_team_, posNo_);
		// 次のレベルに必要な経験値
		s64 next = pmgEO_->mgCoM_.mdm_LvUp_.GetPKDt(lv, enmdm_LvUp::mdm_lvup_exp_for_next);

		// 最新のレベルで値再計算
		nowSt_.lv_ = lv;
		nowSt_.stPow_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Pow, lv, teamLv_);
		nowSt_.stGuts_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Guts, lv, teamLv_);
		nowSt_.stKire_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Kire, lv, teamLv_);
		nowSt_.stTech_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Tech, lv, teamLv_);
		nowSt_.exp_ = now;

		// パラメータの増加分を加算
		upSt_.lv_ = nowSt_.lv_ - prevSt_.lv_;
		upSt_.stPow_ = nowSt_.stPow_ - prevSt_.stPow_;
		upSt_.stGuts_ = nowSt_.stGuts_ - prevSt_.stGuts_;
		upSt_.stKire_ = nowSt_.stKire_ - prevSt_.stKire_;
		upSt_.stTech_ = nowSt_.stTech_ - prevSt_.stTech_;
		upSt_.exp_ = addExp_;

		// 限界に到達
		s64 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team_, enmdm_Team::mdm_team_rarity);
		if (pmgEO_->mgCoU_.IsCharaLvMax(nowSt_.lv_, rarity))
		{
			isLvMax_ = TRUE;
		}

		for (s32 i = 0; i < TM_PARAM_MAX; i++)
		{
			SetText(i);
		}

		ugExp_.SetNum(next - now, FALSE);
		SetExpBar(now, next);

		// もしアニメーション中なら中止
		isExpAnim_ = FALSE;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGChStatePlate::SetText(s32 no)
	{
		std::string strName;
		std::string strParam;
		std::string strParamUp;
		s64 up = 0;

		switch (no)
		{
		case TM_PARAM_LV:
		{
			strName = "れべる";
			strParam = "＿" + lib_str::IntToStr(nowSt_.lv_);
			strParamUp = "＋" + lib_str::IntToStr(upSt_.lv_);
			up = upSt_.lv_;
		}
			break;
		case TM_PARAM_POW:
		{
			strName = "ぱわー";
			strParam = "＿" + lib_str::IntToStr(nowSt_.stPow_);
			strParamUp = "＋" + lib_str::IntToStr(upSt_.stPow_);
			up = upSt_.stPow_;
		}
			break;
		case TM_PARAM_GUTS:
		{
			strName = "きあい";
			strParam = "＿" + lib_str::IntToStr(nowSt_.stGuts_);
			strParamUp = "＋" + lib_str::IntToStr(upSt_.stGuts_);
			up = upSt_.stGuts_;
		}
			break;
		case TM_PARAM_KIRE:
		{
			strName = "すぴん";
			strParam = "＿" + lib_str::IntToStr(nowSt_.stKire_);
			strParamUp = "＋" + lib_str::IntToStr(upSt_.stKire_);
			up = upSt_.stKire_;
		}
			break;
		case TM_PARAM_TECH:
		{
			strName = "まもり";
			strParam = "＿" + lib_str::IntToStr(nowSt_.stTech_);
			strParamUp = "＋" + lib_str::IntToStr(upSt_.stTech_);
			up = upSt_.stTech_;
		}
			break;
		case TM_PARAM_EXP:
		{
			strName = "ＮＥＸＴ＿";
			if (isLvMax_)
			{
				strParam = "ＭＡＸ";
			}
			//else
			//{
			//	strParam = lib_str::IntToStr(nowSt_.exp_);
			//}
			//strParamUp = "＋" + lib_str::IntToStr(upSt_.exp_);
			up = upSt_.exp_;
		}
			break;
		}

		// 経験値の時だけミニフォント
		if (TM_PARAM_EXP == no)
		{
			paramNameTxt_[no]->f_MakeFont_Mini(strName.c_str(), FC2_mini);

			// 経験値の時はparamUpTxt_を使わない
			//paramUpTxt_[no]->g_SetDraw(FALSE);
			
			// 最大時の時には文字で出す
			if (isLvMax_)
			{
				paramTxt_[no]->f_MakeFont_Mini(strParam.c_str(), FC2_mini);
				paramTxt_[no]->g_SetDraw(TRUE);
				ugExp_.SetDraw(FALSE);
			}
			else
			{
				paramTxt_[no]->g_SetDraw(FALSE);
				ugExp_.SetDraw(TRUE);
				ugExp_.SetNum(nowSt_.exp_, FALSE);
			}

			BOOL isUp = FALSE;
			if (up > 0 && prevSt_.lv_ > 1)
			{
				ugExpUp_.SetNum(up, TRUE);
				isUp = TRUE;
			}
			ugExpUp_.SetDraw(isUp);
		}
		else
		{
			paramNameTxt_[no]->f_MakeFont(strName.c_str(), FC2_normal);
			paramTxt_[no]->f_MakeFont_Mini(strParam.c_str(), FC2_mini);

			BOOL isUp = FALSE;
			if (up > 0 && prevSt_.lv_ > 1)
			{
				paramUpTxt_[no]->f_MakeFont_Mini(strParamUp.c_str(), FC2_mini);
				isUp = TRUE;
			}
			paramUpTxt_[no]->g_SetDraw(isUp);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGChStatePlate::SetExpBar(s64 now, s64 next)
	{
		const s32 BARW = 50;
		s32 fw = BARW;//満タン

		f64 nowf = (f64)(now);
		f64 nextf = (f64)(next);


		f32 ratio = (now == 0 && next == 0) ? 0.0f : nowf / nextf;
		fw = floor(BARW * ratio);	// 小数点切り捨て

		// レベル最大
		if (isLvMax_)
		{
			fw = BARW;
		}
		uiExpBar_->o_SetObjFilterGrp(fw, 3, 0xFF, 0xFF, 179, 15);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGChStatePlate::UpdateExpAnim(void)
	{
		// １回で増える分の経験値を足す
		s64 now = nowSt_.exp_ + animMoveExp_;
		//animExp_ += animMoveExp_;// 増えた分を記憶
		upSt_.exp_ += animMoveExp_;	// 増えた分を記憶

		// 次のレベルに必要な経験値
		s64 next = pmgEO_->mgCoM_.mdm_LvUp_.GetPKDt(nowSt_.lv_, enmdm_LvUp::mdm_lvup_exp_for_next);


		// レベルアップ可能
		if (now >= next)
		{
			//prevSt_ = nowSt_;

			now -= next;
			nowSt_.lv_++;

			// 次のレベルに必要な経験値の再計算
			next = pmgEO_->mgCoM_.mdm_LvUp_.GetPKDt(nowSt_.lv_, enmdm_LvUp::mdm_lvup_exp_for_next);

			nowSt_.stPow_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Pow, nowSt_.lv_, teamLv_);
			nowSt_.stGuts_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Guts, nowSt_.lv_, teamLv_);
			nowSt_.stKire_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Kire, nowSt_.lv_, teamLv_);
			nowSt_.stTech_ = pmgEO_->CalcSt(mid_team_, id_team_, id_chara_, clv_Tech, nowSt_.lv_, teamLv_);

			// パラメータの増加分を加算
			upSt_.lv_ = nowSt_.lv_ - prevSt_.lv_;
			upSt_.stPow_ = nowSt_.stPow_ - prevSt_.stPow_;
			upSt_.stGuts_ = nowSt_.stGuts_ - prevSt_.stGuts_;
			upSt_.stKire_ = nowSt_.stKire_ - prevSt_.stKire_;
			upSt_.stTech_ = nowSt_.stTech_ - prevSt_.stTech_;


			// 限界に到達
			s64 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team_, enmdm_Team::mdm_team_rarity);
			if (pmgEO_->mgCoU_.IsCharaLvMax(nowSt_.lv_, rarity))
			{
				isLvMax_ = TRUE;
			}

			for (s32 i = 0; i < TM_PARAM_MAX; i++)
			{
				SetText(i);
			}
		}
		else
		{
			//SetText(TM_PARAM_EXP);
			ugExp_.SetNum(next-now, FALSE);

			//s64 up = animExp_;//nowSt_.exp_ - prevSt_.exp_;
			s64 up = upSt_.exp_;
			BOOL isUp = FALSE;
			if (up > 0 && prevSt_.lv_ > 1)
			{
				ugExpUp_.SetNum(up, TRUE);
				isUp = TRUE;
			}
			ugExpUp_.SetDraw(isUp);
		}

		SetExpBar(now, next);

		nowSt_.exp_ = now;

		// 全ての経験値を足し終えた == アニメーション修了
		if (addExp_ <= upSt_.exp_)
		{
			FinishAnim();
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGChStatePlate::DoUpdate(BOOL draw_f)
	{
		uiBody_->Update(draw_f);
		uiBg_->Update(draw_f);
		uiName_->Update(draw_f);
		uiExpBarBody_->Update(draw_f);
		uiExpBar_->Update(draw_f);

		for (s32 i = 0; i < TM_PARAM_MAX; i++)
		{
			paramNameTxt_[i]->Update(draw_f);
			paramTxt_[i]->Update(draw_f);
			if (i == TM_PARAM_EXP)
			{
				ugExp_.Update(draw_f);
				ugExpUp_.Update(draw_f);
			}
			else
			{
				paramUpTxt_[i]->Update(draw_f);
			}
		}

		if(isExpAnim_)
		{
			UpdateExpAnim();
		}
	}
}
