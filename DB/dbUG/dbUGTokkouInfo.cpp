#include "dbUGTokkouInfo.h"

namespace db
{
	enum RcType
	{
		RcType_BGM = 0,
		RcType_Haikei,
		RcType_Max,
	};

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGTokkouInfo::TUGTokkouInfo(TUIGenLayer* player)
		: TUGGen(player)
		, ugList_(player)
		, ugrMsg_(player)
	{

	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGTokkouInfo::~TUGTokkouInfo()
	{
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTokkouInfo::MakeGroup(TUIObj* parent)
	{
		field_ = MakeObj();
		field_->o_SetParent(parent);

		filter_ = MakeObj();
		filter_->z_SetPos_SafeLF(0, 0);
		filter_->o_SetObjFilterGrp(512, 512, 0xc0, 0x00, 0x00, 0x00);

		uiPlate_ = MakeObj();
		uiPlate_->o_SetParent(field_);
		uiPlate_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_panelavatar_long));
		uiPlate_->z_SetPos(-150, -82);
		uiPlate_->g_SetDraw(TRUE);

		ugList_.MakeGroup(field_, FALSE);
		ugList_.SetDraw(TRUE);

		for (s32 i = 0; i < RcType_Max; i++)
		{
			TUGRcNormal* record = ugList_.MakeObj_Record();
			switch (i)
			{
			case RcType_BGM:
				record->SetName("とっこうＢＧＭ＿かくにん");
				record->ugname_.ugicon_.SetIconTapeTitle(1);
				break;
			case RcType_Haikei:
				record->SetName("とっこうはいけい＿かくにん");
				record->ugname_.ugicon_.SetIconHaikei(1);
				break;
			}
			record->SetCommonID(i);
		}


		uiIcon_ = MakeObj();
		uiIcon_->o_SetParent(uiPlate_);
		uiIcon_->g_SetDraw(FALSE);

		ugrMsg_.MakeGroup_Mini(field_, FALSE);
		ugrMsg_.SetPos(-154, 22);
		ugrMsg_.SetDraw(FALSE);
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTokkouInfo::Refresh(s64 mid_event)
	{
		ClearText();
		uiIcon_->g_SetDraw(FALSE);

		std::string text;

		s32 posY = 4;

		uiPlate_->f_MakeFont("心＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿心", FC2_normal);
		uiPlate_->f_FontPos(4, posY);

		posY += 8;

		//------------------------------------------------------------------------------
		// タイトル部分
		s64 mid_supply = NGNUM;
		for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Ranking_.GetLineNum(); i++)
		{
			// イベント番号が違う
			if (pmgEO_->mgCoM_.mdm_Ranking_.GetLineDt(i, enmdm_Ranking::mdm_ranking_id_m_event) != mid_event) { continue; }

			// 種類がアイテムではない
			if (pmgEO_->mgCoM_.mdm_Ranking_.GetLineDt(i, enmdm_Ranking::mdm_ranking_scoreType) != enScoreType::enScoreType_Item){ continue; }


			mid_supply = pmgEO_->mgCoM_.mdm_Ranking_.GetLineDt(i, enmdm_Ranking::mdm_ranking_id_m_supply);
			break;
		}

		uiIcon_->g_SetDraw(FALSE);

		STRVECTOR v;
		v.clear();
		v.push_back("たくさん＿あつめれば＿ＧＥＴできる");

		// 登録がないならデフォルトテキスト
		if (mid_supply == NGNUM)
		{
			text = "スコア＿とっこう";
			AddText(text.c_str(), 72, posY, TRUE);

			v.push_back("スコアが＿ふえていき");
		}
		else
		{
			/*
			uiIcon_->z_SetPos(4, posY);
			if (mid_supply >= 20000)
			{
				s32 iconno = ((mid_supply % 10) % 4);
				uiIcon_->o_SetObjGrp(pmgUI_->GetEvIcon(iconno));
				uiIcon_->g_SetDraw(TRUE);
			}
			else
			{
				s32 iconno = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply, mdm_supply_iconNo);
				uiIcon_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_supply));
				uiIcon_->g_SetPartNo(iconno);
				uiIcon_->g_SetDraw(TRUE);
			}
			*/
			text = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, enmdm_SupplyStr::mdm_supply_name_c32) + "＿とっこう";
			AddText(text.c_str(), 72, posY, TRUE);

			v.push_back(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, enmdm_SupplyStr::mdm_supply_name_c32) + "が＿ふえていき");
		}
		v.push_back("ランキングが＿アップしやすくなるよ");
		ugrMsg_.SetData(&v);
		ugrMsg_.SetDraw(TRUE);


		posY += 12;
		s64 haveBGM = pmgEO_->mgCoM_.GetHaveTokkouBgmNum(mid_event);
		s64 totalBGM = pmgEO_->mgCoM_.GetTokkouBgmNum(mid_event);
		text = "ＢＧＭ＿＿＿＿" + lib_str::IntToStr(haveBGM, 2) + "／" + lib_str::IntToStr(totalBGM, 2);
		AddText(text.c_str(), 4, posY, FALSE);

		posY += 12;
		s64 haveHaikei = pmgEO_->mgCoM_.GetHaveTokkouHaikeiNum(mid_event);
		s64 totalHaikei = pmgEO_->mgCoM_.GetTokkouHaikeiNum(mid_event);
		text = "はいけい＿＿＿" + lib_str::IntToStr(haveHaikei, 2) + "／" + lib_str::IntToStr(totalHaikei, 2);
		AddText(text.c_str(), 4, posY);

		posY += 12;
		s64 haveAll = haveBGM + haveHaikei;
		s64 totalAll = totalBGM + totalHaikei;
		text = "ごうけい＿＿＿" + lib_str::IntToStr(haveAll, 2) + "／" + lib_str::IntToStr(totalAll, 2);
		AddText(text.c_str(), 4, posY);

		// 該当の特効番号を取得
		s64 tokkouNo = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, enmdm_Event::mdm_event_tokkouNo);

		// 特効の設定なし
		if (tokkouNo == 0)
		{
			posY += 6;
			posY += 12;
			text = "とっこう＿こうかは＿ありません";
			AddText(text.c_str(), 4, posY);
			return;
		}


		// まず今回の特効効果だけを抽出
		LONGVECTOR targetTokkouItemMagVec;
		targetTokkouItemMagVec.clear();
		for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetLineNum(); i++)
		{
			// 対象の特効効果ではない
			if (pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetLineDt(i, enmdm_TokkouItemMag::mdm_tokkouitemmag_tokkouNo) != tokkouNo) continue;

			targetTokkouItemMagVec.push_back(pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetLineDt(i, enmdm_TokkouItemMag::mdm_tokkouitemmag_pk));
		}

		// 特効の効果の設定なし
		if (targetTokkouItemMagVec.size() == 0)
		{
			posY += 6;
			posY += 12;
			text = "とっこう＿こうかの＿せっていは＿ありません";
			AddText(text.c_str(), 4, posY);
			return;
		}


		// そこから今の効果、次の効果などの情報を抽出
		s64 nowPK = NGNUM;
		s64 nextPK = NGNUM;
		s64 effect = 0;
		s64 nextEffect = 0;
		for (s32 i = 0; i < targetTokkouItemMagVec.size(); i++)
		{
			s64 pk = targetTokkouItemMagVec[i];

			// 対象の特効の所持数に満たないならそこで終わり
			if (pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetPKDt(pk, enmdm_TokkouItemMag::mdm_tokkouitemmag_haveNum) > haveAll)
			{
				// 終わる前にここが次になるので保存
				nextEffect = pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetPKDt(pk, enmdm_TokkouItemMag::mdm_tokkouitemmag_effect);
				nextPK = pk;
				break;
			}

			effect = pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetPKDt(pk, enmdm_TokkouItemMag::mdm_tokkouitemmag_effect);
			nowPK = pk;
		}

		// 一番高い設定
		//s64 lastPK = targetTokkouItemMagVec[targetTokkouItemMagVec.size() - 1];
		//s64 maxEffect = pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetPKDt(lastPK, enmdm_TokkouItemMag::mdm_tokkouitemmag_effect);

		posY += 6;
		// １個ももってない
		if (haveAll <= 0 || effect <= 0)
		{
			posY += 12;
			text = "とっこうは＿はつどう＿していない";
			AddText(text.c_str(), 4, posY);
		}
		else
		{
			posY += 12;
			text = "とっこう＿" + lib_str::IntToStr(effect) + "％＿はっきちゅう";
			AddText(text.c_str(), 4, posY);
		}


		// 次が無い状態
		if (nextPK == NGNUM || nextEffect == NGNUM)
		{
			posY += 12;
			if (mid_supply == NGNUM)
			{
				text = "スコア＿とっこうＭＡＸ";
			}
			else
			{
				text = pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, enmdm_SupplyStr::mdm_supply_name_c32) + "＿とっこうＭＡＸ";
			}
			AddText(text.c_str(), 4, posY);
		}
		else
		{
			posY += 12;
			s32 nowNum = pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetPKDt(nowPK, enmdm_TokkouItemMag::mdm_tokkouitemmag_haveNum);
			s32 nextNum = pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetPKDt(nextPK, enmdm_TokkouItemMag::mdm_tokkouitemmag_haveNum);
			s32 differenceNum = nextNum - nowNum;
			text = "あと＿" + lib_str::IntToStr(differenceNum) + "こ＿ＧＥＴで＿＋" + lib_str::IntToStr(nextEffect) + "％";
			AddText(text.c_str(), 4, posY);
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTokkouInfo::DoUpdate(BOOL draw_f)
	{
		uiPlate_->Update(draw_f);
		ugList_.Update(draw_f);
		uiIcon_->Update(draw_f);
		ugrMsg_.Update(draw_f);

		for (s32 i = 0; i < textVec_.size(); i++)
		{
			textVec_[i]->Update(draw_f);
		}

		if (draw_f == FALSE)
		{
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTokkouInfo::DoAction(void)
	{
		if (ugList_.IsAct())
		{
			switch (ugList_.GetCommonID())
			{
			case RcType_BGM: 	SetAct(TIA_Select_BGM); break;
			case RcType_Haikei: SetAct(TIA_Select_Haikei); break;
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTokkouInfo::ClearText(void)
	{
		for (s32 i = 0; i < textVec_.size(); i++)
		{
			Freemem(textVec_[i]);
		}
		textVec_.clear();
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTokkouInfo::AddText(std::string text, s32 x, s32 y, BOOL isCenter)
	{
		TUGRName* ugname;
		ugname = new TUGRName(player_);
		ugname->SetParent(uiPlate_);
		const s32 MAXLEN = 17;
		ugname->SetName(text.c_str(), MAXLEN);
		ugname->SetPos(x, y);
		ugname->GetField()->f_SetCenter(isCenter);
		ugname->SetDraw(TRUE);

		textVec_.push_back(ugname);
	}
}
