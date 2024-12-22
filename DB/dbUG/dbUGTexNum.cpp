#include "dbUGTexNum.h"
#include "dbUILayer.h"
//======================================================================================================
//
//	チームステータス表示クラス
//
//======================================================================================================
namespace db
{
	const s32 SIGN_PLUS  = 10;
	const s32 SIGN_MINUS = 11;


	//--------------------------------------------------------------------------------------------
	// コンストラクタ
	//--------------------------------------------------------------------------------------------
	TUGTexNum::TUGTexNum(TUIGenLayer* player)
		: TUGGen(player)
	{
	}

	//--------------------------------------------------------------------------------------------
	// デストラクタ
	//--------------------------------------------------------------------------------------------
	TUGTexNum::~TUGTexNum()
	{
		ClearTex();
	}


	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTexNum::MakeGroup(TUIObj* parent)
	{
		SetParent(parent);
		ClearTex();

		sign_ = MakeObj();
		sign_->o_SetParent(field_);
		sign_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_level_num));
		sign_->z_SetPos(0, 0);
		sign_->g_SetDraw(FALSE);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTexNum::SetNum(s64 num)
	{
		SetNum(num, isDispPlus_);
	}
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTexNum::SetNum(s64 num, BOOL isDispPlus)
	{
		s32 keta = 1;
		s64 tmp = abs(num);

		// 桁数計算の為10で割っていく
		while(tmp >= 10)
		{
			tmp *= 0.1f;
			keta++;
		}

		// 桁数が足りない
		if(keta > texNum_.size())
		{
			// 一旦削除
			ClearTex();
			// 新規作成
			CreateTex(keta);
		}

		
		//符号
		s32 sineno = (num < 0) ? SIGN_MINUS : SIGN_PLUS;
		sign_->g_SetPartNo(sineno);

		// 符号の表示はマイナスの時かプラス値(プラス表記使うフラグＯＮ)の時
		BOOL isDispSign = FALSE;
		if (num < 0 || (isDispPlus  && num > 0))
		{
			isDispSign = TRUE;
		}
		sign_->g_SetDraw(isDispSign);

		const s32 TEXW = 4;
		s32 numPos = 0;
		
		// 符号があるなら１文字文横にずらす
		if(isDispSign)
		{
			numPos += TEXW;
		}

		// 各桁(左から順に)設定
		tmp = abs(num);
		s32 tmpKeta = keta - 1;
		for(s32 i = 0; i < texNum_.size(); i++)
		{
			// その桁に数値はないので非表示
			if(keta <= i)
			{
				texNum_[i]->g_SetDraw(FALSE);
				continue;
			}

			s32 no = 0;
			s32 k = pow(10, tmpKeta);
			// 1の位は10で割った余りでセット
			if(keta == (i + 1))
			{
				no = abs(tmp);
			}
			// それ以外はその桁数の10のべき乗で割る
			else
			{
				no = abs(tmp / k);
			}
			texNum_[i]->g_SetPartNo(no);
			texNum_[i]->z_SetPos(numPos + (i * TEXW), 0);
			texNum_[i]->g_SetDraw(TRUE);

			// 今表示した桁数以下の値をセットして頭にもどる
			tmp = abs(tmp % k);
			tmpKeta--;
		}
		isDispPlus_ = isDispPlus;
		num_ = num;
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTexNum::SetDispPlus(BOOL isDispPlus)
	{
		SetNum(num_, isDispPlus);
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTexNum::CreateTex(s32 keta)
	{
		// 生成
		for (s32 i = 0; i < keta; ++i)
		{
			TUIObj* newnum = new TUIObj(pmgEO_, pmgUI_);
			texNum_.push_back(newnum);
			newnum->o_SetParent(field_);
			newnum->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_level_num));
		}
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTexNum::ClearTex(void)
	{
		for(s32 i = 0; i < texNum_.size(); i++)
		{
			Freemem(texNum_[i]);
		}
		texNum_.clear();
	}

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTexNum::DoUpdate(BOOL draw_f)
	{
		sign_->Update(draw_f);
		for(s32 i = 0; i < texNum_.size(); i++)
		{
			texNum_[i]->Update(draw_f);
		}
#ifdef __TEX_NUM_DEBUG__
		UpdateDebug();
#endif
	}

#ifdef __TEX_NUM_DEBUG__
	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void TUGTexNum::UpdateDebug(void)
	{
		if (kdebug::DebugController::GetInstance()->IsPressButton(XINPUT_BUTTON_RIGHT_SHOULDER))
		{
			num_++;
			SetNum(num_);
		}
		else if (kdebug::DebugController::GetInstance()->IsPressButton(XINPUT_BUTTON_LEFT_SHOULDER))
		{
			num_--;
			SetNum(num_);
		}
		else if (kdebug::DebugController::GetInstance()->IsPressButton(XINPUT_BUTTON_RIGHT_TRIGGER))
		{
			num_ += 100;
			SetNum(num_);
		}
		else if (kdebug::DebugController::GetInstance()->IsPressButton(XINPUT_BUTTON_LEFT_TRIGGER))
		{
			num_ -= 100;
			SetNum(num_);
		}
		else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_DPAD_RIGHT))
		{
			num_++;
			SetNum(num_);
		}
		else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_DPAD_LEFT))
		{
			num_--;
			SetNum(num_);
		}
		else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_DPAD_UP))
		{
			num_ += 100;
			SetNum(num_);
		}
		else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_DPAD_DOWN))
		{
			num_ -= 100;
			SetNum(num_);
		}
		else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_START))
		{
			isDispPlus_ = !isDispPlus_;
			SetDispPlus(isDispPlus_);
		}

		kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
		char str_c[256];
		sprintf_s(str_c, "num:%d", num_);
		kdebug::DebugText::GetInstance()->SetDebugText(10, 30, str_c);
		if (isDispPlus_)
		{
			sprintf_s(str_c, "dispPlus:on");
		}
		else
		{
			sprintf_s(str_c, "dispPlus:off");
		}
		kdebug::DebugText::GetInstance()->SetDebugText(10, 38, str_c);
	}
#endif
}
