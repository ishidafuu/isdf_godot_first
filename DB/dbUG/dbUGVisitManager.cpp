#include "dbUGVisitManager.h"
#include "dbUGChAnotherKantoku.h"
#include "dbUGChAnotherCaptain.h"
//=====================================================================================================
// 
//
//
//=====================================================================================================

namespace db
{
	// いいかんじのポジション
	const s32 posTbl[PLAYERNUM][2] =
	{
		{ 90, 165 },
		{ 230, 170 },
		{ 285, 205 },
		{ 325, 175 },
	};

#ifdef __K_DEBUG_ROOT__
	static f32 px = 200.0f;
	static f32 py = 150.0f;
	static s32 num = 0;
#endif


	TUGVisitManager* TUGVisitManager::pInstance_ = NULL;

	//--------------------------------------------------------------------------------------------
	// 生成
	//--------------------------------------------------------------------------------------------
	TUGVisitManager* TUGVisitManager::Create(TUIGenLayer* player, TUIObj* parent)
	{
		if (!pInstance_)
		{
			pInstance_ = new TUGVisitManager(player, parent);
		}
		return pInstance_;
	}

	//--------------------------------------------------------------------------------------------
	// 取得
	//--------------------------------------------------------------------------------------------
	TUGVisitManager* TUGVisitManager::GetInstance(void)
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
	void TUGVisitManager::Destroy(void)
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
	void TUGVisitManager::SetupVisitChara(void)
	{
		// 適当に監督ならべる
		for (s32 i = 0; i < POSITION_NUM; i++)
		{
			s32 posNo = 0;
			if (ugVecAnyKantoku_[i])
			{
				// ランダムでキャラセット
				ugVecAnyKantoku_[i]->SetChDtRnd();
				ugVecAnyKantoku_[i]->MakeGroup(pParent_);

				// 座標もテーブルからランダムで決める
				while (1)
				{
					posNo = mid::midGetRandMn(PLAYERNUM);
					if (!isUsePosition_[posNo])
					{
						isUsePosition_[posNo] = TRUE;
						dbgUsePositionTeam_[posNo] = i;		// デバッグ用
						break;
					}
				}

				//s32 x = 60;
				//ugVecAnyKantoku_[i]->SetPos_Full(160 + (i*x), 150);
				ugVecAnyKantoku_[i]->SetPos_Full(posTbl[posNo][0], posTbl[posNo][1]);
				// ついでにモーションもらんだむで
				ugVecAnyKantoku_[i]->SetMType((enKantokuMotionType)(mid::midGetRandMn(kmt_TameUpper)));
				ugVecAnyKantoku_[i]->SetMirror(FALSE);
			}

			if (ugVecAnyCaptain_[i])
			{
				ugVecAnyCaptain_[i]->SetChDtRnd();
				ugVecAnyCaptain_[i]->MakeGroup(pParent_);
				//s32 x = 60;
				//ugVecAnyCaptain_[i]->SetPos_Full(160 + (i*x)+ 32, 150);
				ugVecAnyCaptain_[i]->SetPos_Full(posTbl[posNo][0] + 24, posTbl[posNo][1]);
				ugVecAnyCaptain_[i]->SetKantoku(ugVecAnyKantoku_[i]);
				ugVecAnyCaptain_[i]->SetWalkType(cwt_FOLLOW);
				ugVecAnyCaptain_[i]->UpdateMemPos();
				ugVecAnyCaptain_[i]->SetMType((enChCaptainMotionType)(mid::midGetRandMn(cmt_Anger)));
				ugVecAnyCaptain_[i]->SetMirror(TRUE);
				ugVecAnyCaptain_[i]->SetFollowPos(mid::midGetRandMn(5) + 10);	// 距離 5～15
			}
		}
	}

#ifdef __K_DEBUG_PAD_MODE__
	//--------------------------------------------------------------------------------------------
	//　
	//--------------------------------------------------------------------------------------------
	void TUGVisitManager::SetupDebug(void)
	{
		num = 0;
		for (s32 i = 0; i < POSITION_NUM; i++)
		{
			isUsePosition_[i] = FALSE;
		}
		SetupVisitChara();
	}
#endif


	//--------------------------------------------------------------------------------------------
	//　更新
	//--------------------------------------------------------------------------------------------
	void TUGVisitManager::Update(BOOL draw_f)
	{
#ifdef __K_DEBUG_ROOT__
		kdebug::DebugController* dc = kdebug::DebugController::GetInstance();

		if (dc->IsTriggerButton(XINPUT_BUTTON_RIGHT_SHOULDER))
		{
			if (num < PLAYERNUM)
			{
				ugVecAnyKantoku_[num++]->SetScreenOut(1, 2);
			}
		}
		else if (dc->IsTriggerButton(XINPUT_BUTTON_LEFT_SHOULDER))
		{
			if (num < PLAYERNUM)
			{
				ugVecAnyKantoku_[num++]->SetScreenOut(0, 2);
			}
		}
		else if (dc->IsTriggerButton(XINPUT_BUTTON_RIGHT_TRIGGER))
		{
			if (num < PLAYERNUM)
			{
				s32 posnum = dbgUsePositionTeam_[num];
				ugVecAnyKantoku_[num++]->SetScreenIn(1, posTbl[posnum][0], posTbl[posnum][1], 2);
			}
		}
		else if (dc->IsTriggerButton(XINPUT_BUTTON_LEFT_TRIGGER))
		{
			if (num < PLAYERNUM)
			{
				s32 posnum = dbgUsePositionTeam_[num];
				ugVecAnyKantoku_[num++]->SetScreenIn(0, posTbl[posnum][0], posTbl[posnum][1], 2);
			}
		}

		if (dc->IsRepeatButton(XINPUT_BUTTON_DPAD_RIGHT))
		{
			px += 5.0f;
			ugVecAnyKantoku_[0]->SetPos_Full(px, py);
		}
		else if (dc->IsRepeatButton(XINPUT_BUTTON_DPAD_LEFT))
		{
			px -= 5.0f;
			ugVecAnyKantoku_[0]->SetPos_Full(px, py);
		}
		else if (dc->IsRepeatButton(XINPUT_BUTTON_DPAD_UP))
		{
			py -= 5.0f;
			ugVecAnyKantoku_[0]->SetPos_Full(px, py);
		}
		else if (dc->IsRepeatButton(XINPUT_BUTTON_DPAD_DOWN))
		{
			py += 5.0f;
			ugVecAnyKantoku_[0]->SetPos_Full(px, py);
		}
		else if (dc->IsTriggerButton(XINPUT_BUTTON_B))
		{
			SetupDebug();
			return;
		}
		for (s32 i = 0; i < ugVecAnyKantoku_.size(); i++){ ugVecAnyKantoku_[i]->Update(draw_f); }
		for (s32 i = 0; i < ugVecAnyCaptain_.size(); i++){ ugVecAnyCaptain_[i]->Update(draw_f); }


		kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
		char str[256];
		sprintf_s(str, "px:%f py:%f", px, py);
		kdebug::DebugText::GetInstance()->SetDebugText(5, 20, str);
#endif
	}


	//--------------------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------------------
	TUGVisitManager::TUGVisitManager(db::TUIGenLayer* player, db::TUIObj* parent)
		: pPlayer_(player)
		, pParent_(parent)
		, ugVecAnyKantoku_(PLAYERNUM, NULL)
		, ugVecAnyCaptain_(PLAYERNUM, NULL)
	{
		for (s32 i = 0; i < POSITION_NUM; i++)
		{
			isUsePosition_[i] = FALSE;
		}
		for (s32 i = 0; i < PLAYERNUM; i++)
		{
			TUGChAnotherKantoku* k = new TUGChAnotherKantoku(pPlayer_, i);
			ugVecAnyKantoku_[i] = k;

			TUGChAnotherCaptain* c = new TUGChAnotherCaptain(pPlayer_, i);
			ugVecAnyCaptain_[i] = c;
		}
	}

	//--------------------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------------------
	TUGVisitManager::~TUGVisitManager(void)
	{
		for (s32 i = 0; i < ugVecAnyKantoku_.size(); i++){ Freemem(ugVecAnyKantoku_[i]); }
		ugVecAnyKantoku_.clear();
		for (s32 i = 0; i < ugVecAnyCaptain_.size(); i++){ Freemem(ugVecAnyCaptain_[i]); }
		ugVecAnyCaptain_.clear();
	}
}