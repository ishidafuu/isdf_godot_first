#include "dbUGShSet.h"
#include "dbUILayer.h"

namespace db
{
	//コンストラクタ
	TUGShSet::TUGShSet(TUIGenLayer* player) : TUGGen(player)
	{
		stShSet_.Init();
	}

	TUGShSet::~TUGShSet()
	{

	}
	void TUGShSet::MakeGroup(TUIObj* parent)
	{
		SetParent(parent);
		field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_item));
		field_->g_SetPartNo(ugi_ShSetAuto);
		field_->g_SetDraw(TRUE);
		field_->f_FontPos(24, 6);

		uiShotNum_ = MakeObj();
		uiShotNum_->o_SetParent(field_);
		uiShotNum_->z_SetPos(0, 23);

		uiTotalShSt_ = MakeObj();
		uiTotalShSt_->o_SetParent(field_);
		uiTotalShSt_->z_SetPos(46, 23);
	}

	void TUGShSet::Refresh(s64 mid_team, s64 id_team, s32 rarity)
	{
		s32 id_shset = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_shset);
		BOOL makefont_f = FALSE;
		if ((stShSet_.id_shset_ != id_shset) || (stShSet_.rarity_ != rarity))
		{
			stShSet_.id_shset_ = id_shset;
			stShSet_.rarity_ = rarity;
			makefont_f = TRUE;
		}

		s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);

		s32 totalShotNum = 0;
		s32 totalSt = 0;
		for (s32 i = 0; i < DBMEMBER_ALL; i++)
		{
			s32 pow = 0;
			s32 spin = 0;
			pmgEO_->mgCoU_.GetShotTotalPowSpin(mid_team, i, &pow, &spin);
			totalSt += (pow + spin);

			s32 num = pmgEO_->mgCoU_.GetTotalSetShotNum(mid_team, i);
			totalShotNum += num;
		}

		std::string str = "ＳＨＯＴ＿" + lib_str::IntToStr(totalShotNum);
		uiShotNum_->f_MakeFont_Mini(str.c_str(), FC2_mini);
		uiTotalShSt_->f_MakeFont_Mini(lib_str::IntToStr(totalSt).c_str(), FC2_mini);

		s32 fccol = (FC2_rareD + rarity);

		if (id_shset == 0)
		{
			field_->g_SetPartNo(ugi_ShSetCstm);
			if (makefont_f) field_->f_MakeFont("カスタム", fccol);
		}
		else
		{
			for (s32 i = 0; i < SHSETNUM; ++i)
			{
				if (id_shset == pmgEO_->mgCoU_.mdShSet_.GetLineDt(i, mdshset_pk))
				{
					switch (i)
					{
					case 0:
						field_->g_SetPartNo(ugi_ShSetAuto);
						if (makefont_f)  field_->f_MakeFont("おまかせ", fccol);
						break;
					case 1:
						field_->g_SetPartNo(ugi_ShSet1);
						if (makefont_f)  field_->f_MakeFont(pmgEO_->mgCoU_.GetShSetName(1, FALSE).c_str(), fccol);
						break;
					case 2:
						field_->g_SetPartNo(ugi_ShSet2);
						if (makefont_f) field_->f_MakeFont(pmgEO_->mgCoU_.GetShSetName(2, FALSE).c_str(), fccol);
						break;
					case 3:
						field_->g_SetPartNo(ugi_ShSet3);
						if (makefont_f)  field_->f_MakeFont(pmgEO_->mgCoU_.GetShSetName(3, FALSE).c_str(), fccol);
						break;
					}
					break;
				}
			}
		}

	}


	void TUGShSet::DoUpdate(BOOL draw_f)
	{
		field_->Update(draw_f);
		uiShotNum_->Update(draw_f);
		uiTotalShSt_->Update(draw_f);
	}

}
