#include "dbUGTipsManager.h"
#include "dbUGChTipsPerson.h"
#include "dbUILayer.h"
//=====================================================================================================
// 
//
//
//=====================================================================================================

namespace db
{
	// Tips吹き出し位置
	const s32 SERIFU_POS_X = -80;
	const s32 SERIFU_POS_Y = -70;

	// Tipsキャラ出現インターバル定数
	static const s32 PERSON_INTERVAL	 = 60;
	static const s32 PERSON_INTERVAL_RND = 1000;

	// 出現Y位置
	static const s32 PERSON_LINE_POS_Y[PERSON_LINE_MAX] =
	{
		40, 60, 80,
	};

	const char* PERSON_NAME_REPLACE = "$PERSON_NAME$";

	const s32 CHECK_ITV = 600;

	TUGTipsManager* TUGTipsManager::pInstance_ = NULL;

	//--------------------------------------------------------------------------------------------
	// 生成
	//--------------------------------------------------------------------------------------------
	TUGTipsManager* TUGTipsManager::Create(TUIGenLayer* player, TUIObj* parent)
	{
		if (!pInstance_)
		{
			pInstance_ = new TUGTipsManager(player, parent);
		}
		return pInstance_;
	}

	//--------------------------------------------------------------------------------------------
	// 取得
	//--------------------------------------------------------------------------------------------
	TUGTipsManager* TUGTipsManager::GetInstance(void)
	{
		if (!pInstance_)
		{
			return NULL;
			//pInstance_ = DebugUDManager::Create();
		}
		return pInstance_;
	}

	//--------------------------------------------------------------------------------------------
	//　破棄
	//--------------------------------------------------------------------------------------------
	void TUGTipsManager::Destroy(void)
	{
		if (pInstance_)
		{
			delete pInstance_;
		}
		pInstance_ = NULL;
	}


	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	void TUGTipsManager::Setup(void)
	{
		for (s32 i = 0; i < PERSON_LINE_MAX; i++)
		{
			ugVecTipsPerson_[i]->SetChDtRnd();
			ugVecTipsPerson_[i]->MakeGroup(pParent_);
			ugVecTipsPerson_[i]->SetPos_Full(80 + (i * 32), 168 + 35);
			ugVecTipsPerson_[i]->SetDraw(FALSE);
		}

		for (s32 i = 0; i < PERSON_LINE_MAX; i++)
		{
			intervalTimer_[i] = mid::midGetRandMn(PERSON_INTERVAL_RND) + PERSON_INTERVAL;
			//SetupParam(i);
		}
	}

	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	BOOL TUGTipsManager::SetupParam(s32 no)
	{
		// ここに抽選処理
#ifdef __K_DEBUG_ROOT__
		tipsParam_[no].pPlayer_ = pPlayer_;
		switch (no)
		{
			case 0:	tipsParam_[no].SetupDebugParam1(); break;
			case 1:	tipsParam_[no].SetupDebugParam2(); break;
			case 2:	tipsParam_[no].SetupDebugParam3(); break;
		}
#else
		BOOL isChoose = ChooseParam(no);
		// 何も選ばれなかったので失敗
		if (!isChoose)
		{
			intervalTimer_[no] = mid::midGetRandMn(PERSON_INTERVAL_RND) + PERSON_INTERVAL;
			ugVecTipsPerson_[no]->SetDraw(FALSE);
			return FALSE;
		}
#endif
		std::string name = tipsParam_[no].GetName();

		s32 hair = tipsParam_[no].GetHairNo();
		s32 face = tipsParam_[no].GetFaceNo();
		s32 haircol = tipsParam_[no].GetColorHair();
		s32 skincol = tipsParam_[no].GetColorSkin();
		s32 eyecol = tipsParam_[no].GetColorEye();
		s32 acccol = tipsParam_[no].GetColorAccessories();
		s32 wearcol = tipsParam_[no].GetColorWear();
		s32 pantscol = tipsParam_[no].GetColorPants();
		s32 sidecol = tipsParam_[no].GetColorSide();
		s32 undercol = tipsParam_[no].GetColorUnder();
		s32 shoecol = tipsParam_[no].GetColorShoe();
		BOOL gb_f = tipsParam_[no].IsColorGB();
		enPersonType personType = (enPersonType)tipsParam_[no].GetPersonType();
		
		//enPersonActType moveType = (enPersonActType)tipsParam_[no].GetMoveType();
		//s32 item = tipsParam_[no].GetItem();

		ugVecTipsPerson_[no]->SetChDt(hair, face, haircol, skincol, eyecol, acccol, wearcol, pantscol, sidecol, undercol, shoecol, gb_f);
		ugVecTipsPerson_[no]->SetPersonType(personType);
		ugVecTipsPerson_[no]->SetPersonActType(enPersonActType::pat_Run);
		ugVecTipsPerson_[no]->SetName(name);

		return TRUE;
	}
	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	void TUGTipsManager::ClearParam(s32 no)
	{
		// とりあえずpkだけクリアしておく
		tipsParam_[no].SetID(NGNUM);
		tipsParam_[no].SetEndTips(FALSE);
	}

	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	BOOL TUGTipsManager::IsSelect(void)
	{
		for (s32 i = 0; i < PERSON_LINE_MAX; i++)
		{
			if (ugVecTipsPerson_[i]->IsSelect()) { return TRUE; }
		}
		return FALSE;
	}
	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	s32 TUGTipsManager::GetSelectNo(void)
	{
		for (s32 i = 0; i < PERSON_LINE_MAX; i++)
		{
			if (ugVecTipsPerson_[i]->IsSelect()) { return i; }
		}
		return NGNUM;
	}
	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	f32 TUGTipsManager::GetPersonPosX(s32 no)
	{
		f32 x = 0;
		f32 y = 0;
		ugVecTipsPerson_[no]->GetField()->z_GetPos(&x, &y);
		return x;
	}
	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	f32 TUGTipsManager::GetPersonPosY(s32 no)
	{
		f32 x = 0;
		f32 y = 0;
		ugVecTipsPerson_[no]->GetField()->z_GetPos(&x, &y);
		return y;
	}

	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	void TUGTipsManager::SetStopPerson(s32 no)
	{
		ugVecTipsPerson_[no]->z_SetStop();
		ugVecTipsPerson_[no]->SetPersonActType(enPersonActType::pat_Stand);
	}

	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	void TUGTipsManager::SetPersonFrameOutR(s32 no)
	{
		//if (no == NGNUM){ return; }
		ugVecTipsPerson_[no]->SetDrawBtn(FALSE);

		f32 speed = 0.0f;
		switch (tipsParam_[no].GetSpeedType())
		{
		case 0: speed = PERSON_SPEED_A; break;
		case 1: speed = PERSON_SPEED_B; break;
		case 2: speed = PERSON_SPEED_C; break;
		case 3: speed = PERSON_SPEED_D; break;
		case 4: speed = PERSON_SPEED_E; break;
		case 5: speed = PERSON_SPEED_F; break;
		}

		s32 x = mid::midGetDotL_Hf();
		s32 size = ugVecTipsPerson_[no]->GetField()->b_GetAutoSize().width_;
		ugVecTipsPerson_[no]->z_MoveWork(0, x + size, PERSON_LINE_POS_Y[no], speed);
		ugVecTipsPerson_[no]->SetMirror(FALSE);
		ugVecTipsPerson_[no]->SetPersonActType(enPersonActType::pat_Run);
	}

	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	void TUGTipsManager::SetPersonFrameOutL(s32 no)
	{
		//if (no == NGNUM){ return; }
		ugVecTipsPerson_[no]->SetDrawBtn(FALSE);

		f32 speed = 0.0f;
		switch (tipsParam_[no].GetSpeedType())
		{
		case 0: speed = PERSON_SPEED_A; break;
		case 1: speed = PERSON_SPEED_B; break;
		case 2: speed = PERSON_SPEED_C; break;
		case 3: speed = PERSON_SPEED_D; break;
		case 4: speed = PERSON_SPEED_E; break;
		case 5: speed = PERSON_SPEED_F; break;
		}

		s32 x = mid::midGetDotL_Hf();
		s32 size = ugVecTipsPerson_[no]->GetField()->b_GetAutoSize().width_;
		ugVecTipsPerson_[no]->z_MoveWork(0, -x - size, PERSON_LINE_POS_Y[no], speed);
		ugVecTipsPerson_[no]->SetMirror(TRUE);
		ugVecTipsPerson_[no]->SetPersonActType(enPersonActType::pat_Run);
	}


	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	void TUGTipsManager::ShowTips(s32 no, BOOL isMirror)
	{
		ugVecTipsPerson_[no]->SetMirror(isMirror);
		msgNo_ = 0;
		SetMsg(no, tipsParam_[no].GetText(msgNo_));
		ugSerifu_->SetDraw(TRUE);
		ugSerifu_->ClearAct();
		tipsPersonNo_ = no;
	}
	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	BOOL TUGTipsManager::IsEndTips(s32 no)
	{
		return tipsParam_[no].IsEndTips();
	}
	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	void TUGTipsManager::CloseTips(void)
	{
		msgNo_ = NGNUM;
		serifuVec_.clear();
		ugSerifu_->SetDraw(FALSE);
		tipsPersonNo_ = NGNUM;
	}

	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	void TUGTipsManager::SetDrawTipsBtnALL(BOOL isDraw)
	{
		for (s32 i = 0; i < PERSON_LINE_MAX; i++)
		{
			SetDrawTipsBtn(i, isDraw);
		}
		isUnDrawBtn_ = !isDraw;
	}
	void TUGTipsManager::SetDrawTipsBtn(s32 no, BOOL isDraw)
	{
		if (IsEndTips(no)){ return; }
		ugVecTipsPerson_[no]->SetDrawBtn(isDraw);
	}

	//--------------------------------------------------------------------------------------------
	//　更新
	//--------------------------------------------------------------------------------------------
	void TUGTipsManager::Update(BOOL draw_f)
	{
		for (s32 i = 0; i < PERSON_LINE_MAX; i++)
		{
			if (intervalTimer_[i] > 0)
			{
				intervalTimer_[i]--;
				if (intervalTimer_[i] <= 0)
				{
					BOOL isChoose = SetupParam(i);
					if (!isChoose){ continue; }		// セットアップ失敗

					s32 x = mid::midGetDotL_Hf();
					s32 size = ugVecTipsPerson_[i]->GetField()->b_GetAutoSize().width_;
					s32 muki = mid::midGetRandMn(2);
					s32 y = PERSON_LINE_POS_Y[i];


					// ここだけデバッグそのまま
					f32 speed = 0.0f;
					switch (tipsParam_[i].GetSpeedType())
					{
					case 0: speed = PERSON_SPEED_A; break;
					case 1: speed = PERSON_SPEED_B; break;
					case 2: speed = PERSON_SPEED_C; break;
					case 3: speed = PERSON_SPEED_D; break;
					case 4: speed = PERSON_SPEED_E; break;
					case 5: speed = PERSON_SPEED_F; break;
					}


					// 0:左から右へ
					if (muki == 0)
					{
						ugVecTipsPerson_[i]->SetPos(-x - size, y);
						ugVecTipsPerson_[i]->z_MoveWork(0, x + size, y, speed);
						ugVecTipsPerson_[i]->SetMirror(FALSE);
					}
					// 1:右から左へ
					else
					{
						ugVecTipsPerson_[i]->SetPos(x + size, y);
						ugVecTipsPerson_[i]->z_MoveWork(0, -x - size, y, speed);
						ugVecTipsPerson_[i]->SetMirror(TRUE);
					}
					ugVecTipsPerson_[i]->SetPersonActType(enPersonActType::pat_Run);
					ugVecTipsPerson_[i]->SetDraw(TRUE);
					if (!isUnDrawBtn_)
					{
						ugVecTipsPerson_[i]->SetDrawBtn(TRUE);
					}
					intervalTimer_[i] = -1;
				}
			}
			else
			{
				if (ugVecTipsPerson_[i]->GetActRes() == enUGChGenAct_MoveStop)
				{
					intervalTimer_[i] = mid::midGetRandMn(PERSON_INTERVAL_RND) + PERSON_INTERVAL;
					ugVecTipsPerson_[i]->SetDraw(FALSE);
					ClearParam(i);
				}
			}
			ugVecTipsPerson_[i]->Update(draw_f);
		}

		if (ugSerifu_->IsDraw())
		{
			// メッセージの次へが押された
			if (ugSerifu_->GetActRes() == enUGSerifuAct_WaitEnd)
			{
				msgNo_++;
				STRVECTOR2 v = tipsParam_[tipsPersonNo_].GetText();
				if (v.size() > msgNo_)
				{
					SetMsg(tipsPersonNo_, v[msgNo_]);
				}
				else
				{
					tipsParam_[tipsPersonNo_].SetEndTips(TRUE);
				}
			}
			// 喋り終わった
			else if (ugSerifu_->GetActRes() == enUGSerifuAct_ReadEnd)
			{
			}
			// メッセージスキップ処理
			else if (pParent_->sc_IsCamEnter())
			{
				if (ugSerifu_->IsReadLineEnd())
				{
					ugSerifu_->NextSerifu();
				}
				else
				{
					ugSerifu_->SkipSerifu();
				}
			}
		}
		ugSerifu_->Update(draw_f);
	}

	//-----------------------------------------------------------------
	// 
	//-----------------------------------------------------------------
	BOOL TUGTipsManager::ChooseParam(s32 no)
	{
		tipsParam_[no];

		s32 dataNum = pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineNum();
		INTVECTOR2 v2;

		//タイマー切れは適当にインターバルとる
		if (timecheck_c_ > 0) --timecheck_c_;


		for (s32 i = 0; i < dataNum; i++)
		{
			INTVECTOR v;
#ifndef __K_DEBUG_ROOT__
			//タイマー切れ
      if (timecheck_c_ <= 0)
      {
        if (pPlayer_->pmgEO_->mgCoM_.IsTimer(pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(i, mdm_tips_id_m_timer)) == FALSE) continue;
      }
#endif
			s32 osType =  pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(i, mdm_tips_osType);
			if (mid::midIsIOS())
			{
				if(osType == enOSType_Android){ continue; }
			}
			else if(mid::midIsADR())
			{
				if (osType == enOSType_IOS){ continue; }
			}

			v.push_back(i);
			v.push_back(pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(i, mdm_tips_selectPercent));
			v2.push_back(v);
		}

    if (timecheck_c_ <= 0) timecheck_c_= CHECK_ITV;

		// 該当データ無し
		if (v2.size() <= 0){ return FALSE; }

		// まず全ての抽選値を足す
		s32 maxPercent = 0;
		for (s32 i = 0; i < v2.size(); i++)
		{
			maxPercent += v2[i][1];
		}

		s32 pk = NGNUM;
		s32 missCnt = 0;
		while (1)
		{
			pk = NGNUM;

			// 抽選
			s32 select = mid::midGetRandMn(maxPercent);
			s32 min = 0;
			s32 max = v2[0][1];
			for (s32 i = 0; i < v2.size()-1; i++)
			{
				if (min <= select && select < max)
				{
					pk = v2[i][0];
					break;
				}
				else if (i+1 >= v2.size()-1)
				{
					pk = v2[i + 1][0];
				}
				min += v2[i][1];
				max += v2[i+1][1];
			}

			if (pk == NGNUM)
			{
				// ３回やってダメだったらあきらめる
				missCnt++;
				if (missCnt >= 3){ return FALSE; }

				continue;
			}

			// 重複チェック
			BOOL isOK = TRUE;
			for (s32 i = 0; i < PERSON_LINE_MAX; i++)
			{
				if (i == no){ continue; }
				// 他のキャラと被った
				if (tipsParam_[i].GetID() == pk)
				{
					isOK = FALSE;
					break;
				}
			}
			if (isOK){ break; }

			
			// データが３つ以下の時は諦める
			if (v2.size() < 3){ return FALSE; }
		}

		tipsParam_[no].SetID(pk);
		tipsParam_[no].SetName(pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDtStr(pk, mdm_tips_name_c32));
		tipsParam_[no].SetPersonType(pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_personType));
		tipsParam_[no].SetMoveType(pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_moveType));
		tipsParam_[no].SetItem(pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtItem));
		tipsParam_[no].SetChDt(
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtHair),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtFace),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtHairCol),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtSkinCol),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtEyeCol),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtAccCol),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtShirCol),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtPantCol),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtSideLineCol),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtUnderLineCol),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_avtShoeCol),
			pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDtBOOL(pk, mdm_tips_gb_f));
			

		tipsParam_[no].SetSpeedType(pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_speedType));
		//tipsParam_[no].SetPercent(pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_selectPercent));
		tipsParam_[no].SetTerminalType(pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDt(pk, mdm_tips_osType));
		
		INTVECTOR2 iv =
		{
			{
				mdm_tips_text0_c128,
				mdm_tips_text1_c128,
			},
			{
				mdm_tips_text2_c128,
				mdm_tips_text3_c128,
			},
			{
				mdm_tips_text4_c128,
				mdm_tips_text5_c128,
			},
		};

		STRVECTOR2 sv2;
		sv2.clear();
		std::string strNone = "＿";
		for (s32 i = 0; i < iv.size(); i++)
		{
			STRVECTOR sv;
			for (s32 j = 0; j < iv[i].size(); j++)
			{
				std::string s = pPlayer_->pmgEO_->mgCoM_.mdm_Tips_.GetLineDtStr(pk, (enmdm_TipsStr)iv[i][j]);
				if (strNone.compare(s.c_str()) == 0) { continue; }
				sv.push_back(s);
			}
			if (sv.size() <= 0){ continue; }
			sv2.push_back(sv);
		}

		tipsParam_[no].SetText(sv2);
		return TRUE;
	}

	//-----------------------------------------------------------------
	// 
	//-----------------------------------------------------------------
	void TUGTipsManager::SetMsg(s32 no, STRVECTOR strVec)
	{
		if (strVec.size() == 1)
		{
			SetMsg(no, strVec[0].c_str(), NULL, NULL);
		}
		else if (strVec.size() == 2)
		{
			SetMsg(no, strVec[0].c_str(), strVec[1].c_str(), NULL);
		}
		else
		{
			SetMsg(no, strVec[0].c_str(), strVec[1].c_str(), strVec[2].c_str());
		}
	}
	void TUGTipsManager::SetMsg(s32 no, const char* text0, const char* text1, const char* text2)
	{
		std::string strNone = "";

		serifuVec_.clear();
		serifuVec_.push_back(text0);
		if (text1 && strNone.compare(text1) != 0)
		{
			serifuVec_.push_back(text1);
		}
		if (text2 && strNone.compare(text2) != 0)
		{
			serifuVec_.push_back(text2);
		}

		for (s32 i = 0; i < serifuVec_.size(); i++)
		{
			ConvPersonName(no, &serifuVec_[i]);
		}
		ugSerifu_->SetSerifu(serifuVec_);
	}

	//-----------------------------------------------------------------
	// 
	//-----------------------------------------------------------------
	void TUGTipsManager::ConvPersonName(s32 no, std::string* s)
	{
		std::string name;
		name = tipsParam_[no].GetName();
		std::size_t pos = s->find(PERSON_NAME_REPLACE);
		if (pos == std::string::npos) return;
		std::size_t size = std::string(PERSON_NAME_REPLACE).size();
		s->replace(pos, size, name);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	TUGTipsManager::TUGTipsManager(db::TUIGenLayer* player, db::TUIObj* parent)
		: pPlayer_(player)
		, pParent_(parent)
		, ugVecTipsPerson_(PERSON_LINE_MAX, NULL)
		, tipsParam_(PERSON_LINE_MAX)
		, isUnDrawBtn_(FALSE)
	  , timecheck_c_(0)
	{
		for (s32 i = 0; i < PERSON_LINE_MAX; i++)
		{
			ugVecTipsPerson_[i] = new TUGChTipsPerson(player, i);
			intervalTimer_[i] = mid::midGetRandMn(PERSON_INTERVAL_RND) + PERSON_INTERVAL;	// ランダム値設定
		}

		ugSerifu_ = new TUGSerifu(player);
		ugSerifu_->MakeGroup(pParent_, 2, enSerifuFrame_Wide);
		ugSerifu_->SetMsgChangeMode(enMsgChangeMode_Touch);
		ugSerifu_->SetPos(SERIFU_POS_X, SERIFU_POS_Y);
		ugSerifu_->SetDraw(FALSE);
	}

	//--------------------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------------------
	TUGTipsManager::~TUGTipsManager(void)
	{
		for (s32 i = 0; i < ugVecTipsPerson_.size(); i++){ Freemem(ugVecTipsPerson_[i]); }
		ugVecTipsPerson_.clear();
	}
}
