#include "dbUGChBaseCaptain.h"
#include "dbUILayer.h"
#include "dbUGChSensyu.h"	// キャプテン連れて歩く用
#include "dbUGChKantoku.h"

namespace db
{
	enum MemPos
	{
		MP_X = 0,
		MP_Y,
		MP_MAX,
	};
	const s32 MEM_NUM = 15;
	const s32 CAPTAIN_OFFSET_X = 26;
	const s32 CAPTAIN_SPEED = 2;

	//コンストラクタ
	TUGChBaseCaptain::TUGChBaseCaptain(TUIGenLayer* player)
		:TUGChGen(player)
		, ugKantoku_(NULL)
		, memPos(MEM_NUM, std::vector<f32>(MP_MAX))
		, isWalk_(FALSE)
		, type_(cwt_FOLLOW)	// デフォルトは追跡モード
		, cpPos_(MEM_NUM - 1)
	{
	}

	TUGChBaseCaptain::~TUGChBaseCaptain()
	{

	}

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	void TUGChBaseCaptain::MakeGroup(TUIObj* parent, TMgCharGrp* pmgcg)
	{
		GenMakeGroup(parent, pmgcg, &player_->pmgEO_->mgDt_.dtBM_sensyu_, 0);
	}

	void TUGChBaseCaptain::SetMType(enChCaptainMotionType mtype)
	{
		SetGenMType(mtype);
	}

	void TUGChBaseCaptain::z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd)
	{
		z_MoveWork(moveno, tagx, tagy, spd, cmk_None);
	}

	void TUGChBaseCaptain::z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki)
	{
		field_->z_MoveSetspd(tagx, tagy, spd);
		stChar_.moveNo_ = moveno;
		stChar_.muki_ = muki;
		if (field_->stSO_.stZahyou_.eat_ > 1)
		{
			SetMType(cmt_Wk);
			SetMirror((field_->stSO_.stZahyou_.dx_ < 0));
		}
	}
	void TUGChBaseCaptain::z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd)
	{
		z_MoveWork(moveno, tagx - mid::midFullDotL_Hf(), tagy - mid::midFullDotS_Hf(), spd, cmk_None);
	}
	void TUGChBaseCaptain::z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki)
	{
		z_MoveWork(moveno, tagx - mid::midFullDotL_Hf(), tagy - mid::midFullDotS_Hf(), spd, muki);
	}
	void TUGChBaseCaptain::z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd, f32 lookx)
	{
		enUGChGenMuki muki = (tagx < lookx) ? cmk_Right : cmk_Left;
		z_MoveWork_Full(moveno, tagx, tagy, spd, muki);
	}

	void TUGChBaseCaptain::SetWalkType(enChCaptainWalkType type)
	{
		type_ = type;

		// フォローなら最新の状態に
		if (type == cwt_FOLLOW)
		{
			f32 x = 0;
			f32 y = 0;
			ugKantoku_->GetField()->z_GetPos(&x, &y);

			f32 mx = 0;
			f32 my = 0;
			GetField()->z_GetPos(&mx, &my);

			// 徐々に監督に位置になるようにセット
			for (s32 i = 0; i < MEM_NUM; i++)
			{
				if (i == 0)
				{
					memPos[i][MP_X] = x;
				}
				else
				{
					// float型のキャストがうまくいかないので
					f32 tmp1 = x - mx;
					f32 tmp2 = (f32)(MEM_NUM - 1);
					f32 tmp3 = (f32)(tmp2 - i);
					memPos[i][MP_X] = mx + tmp1 * (tmp3 / tmp2);
				}
				memPos[i][MP_Y] = my;
			}
		}
	}

	void TUGChBaseCaptain::SetKantoku(TUGChBaseKantoku* kantoku)
	{
		ugKantoku_ = kantoku;
		if (!ugKantoku_){ return; }

		f32 x = 0;
		f32 y = 0;
		ugKantoku_->GetField()->z_GetPos(&x, &y);

		f32 mx = 0;
		f32 my = 0;
		GetField()->z_GetPos(&mx, &my);

		// 徐々に監督に位置になるようにセット
		for (s32 i = 0; i < MEM_NUM; i++)
		{
			if (i == 0)
			{
				memPos[i][MP_X] = x;
			}
			else
			{
				// float型のキャストがうまくいかないので
				f32 tmp1 = x - mx;
				f32 tmp2 = (f32)(MEM_NUM - 1);
				f32 tmp3 = (f32)(tmp2 - i);
				memPos[i][MP_X] = mx + tmp1 * (tmp3 / tmp2);
			}
			memPos[i][MP_Y] = my;
		}
	}

	void TUGChBaseCaptain::UpdateMemPos(void)
	{

		f32 x = 0;
		f32 y = 0;
		ugKantoku_->GetField()->z_GetPos(&x, &y);


		if (memPos[0][MP_X] == x &&
			memPos[0][MP_Y] == y)
		{
			if (isWalk_)
			{
				isWalk_ = FALSE;
				SetMType(cmt_St);
			}
			// 変化がないならなにもしない
			return;
		}
		else
		{
			if (!isWalk_)
			{
				isWalk_ = TRUE;
				SetMType(cmt_Wk);
			}
		}

		for (s32 i = MEM_NUM - 1; i >= 0; i--)
		{
			if (i == 0)
			{
				memPos[i][MP_X] = x;
				memPos[i][MP_Y] = y;
			}
			else
			{
				// 座標更新の際に向く方向を決める
				if (i == MEM_NUM - 1)
				{
					if ((memPos[i][MP_X] - memPos[i - 1][MP_X]) >= 0)
					{
						SetMirror(TRUE);
					}
					else
					{
						SetMirror(FALSE);
					}
				}

				memPos[i][MP_X] = memPos[i - 1][MP_X];
				memPos[i][MP_Y] = memPos[i - 1][MP_Y];
			}
		}
	}

	void TUGChBaseCaptain::SetFollowPos(s32 pos)
	{
		if (pos < 0) { return; }
		if (pos >= MEM_NUM) { return; }
		cpPos_ = pos;
		UpdateMemPos();
		DoActionSub();
	}

	void TUGChBaseCaptain::DoActionSub()
	{
		if (type_ == cwt_FOLLOW && ugKantoku_)
		{
			SetPos(memPos[cpPos_][MP_X], memPos[cpPos_][MP_Y]);
			UpdateMemPos();
		}
		else
		{
			isWalk_ = FALSE;
		}
		if (IsAct())
		{
			switch (GetActRes())
			{
			case enUGChGenAct_MotihonEnd:
			case enUGChGenAct_MoveStop:
				SetMType(cmt_St);
				break;
			}
		}

	}

}
