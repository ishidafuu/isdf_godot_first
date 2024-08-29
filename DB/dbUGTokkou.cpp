#include "dbUGTokkou.h"
#include "dbUILayer.h"

namespace db
{
	//コンストラクタ
	TUGTokkou::TUGTokkou(TUIGenLayer* player) : TUGGen(player)//アクションなし
	{
		stTokkou_.Init();

	}

	TUGTokkou::~TUGTokkou()
	{

	}
	void TUGTokkou::MakeGroup(TUIObj* parent)
	{
		SetParent(parent);
		field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_item));
		field_->g_SetPartNo(ugi_Tokkou);
		field_->g_SetCenter(TRUE);
		field_->f_SetCenter(TRUE);
		field_->f_FontPos(3, 2);
		field_->g_SetDraw(FALSE);//デフォはアイコン無し
	}

	void TUGTokkou::SetTokkou(s64 id_team, s32 mid_team, s32 mid_event)
	{
		tokkouMag_ = NGNUM;
		tokkouMagStr_ = "";

		BOOL draw_f = FALSE;
		s32 tokkouNo = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, mdm_event_tokkouNo);
		for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Tokkou_.GetLineNum(); ++i)
		{
			if (tokkouNo == pmgEO_->mgCoM_.mdm_Tokkou_.GetLineDt(i, mdm_tokkou_tokkouNo))
			{
				if (mid_team == pmgEO_->mgCoM_.mdm_Tokkou_.GetLineDt(i, mdm_tokkou_id_m_team))
				{
					draw_f = TRUE;
					//特効倍率
					s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
					tokkouMag_ = pmgEO_->mgCoM_.GetTokkouMag(id_team, mid_event);

					tokkouMagStr_ = "Ｘ" + lib_str::IntToStr(tokkouMag_ / 100) + "．" + lib_str::IntToStr((tokkouMag_ % 100) / 1);
					//field_->f_MakeFont_Mini(strtokkou.c_str(), FC2_mini);
					break;
				}
			}
		}

		field_->g_SetDraw(draw_f);
	}

	//現在開催中の特効イベント
	void TUGTokkou::SetTokkouNow(s64 id_team, s32 mid_team)
	{
		tokkouMag_ = NGNUM;
		tokkouMagStr_ = "";

		BOOL draw_f = FALSE;
		LONGVECTOR mid_event_now = pmgEO_->mgCoU_.GetNowTokkouEvent();

		for (auto& item : mid_event_now)//リストに写す
		{
			s32 tokkouNo = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(item, mdm_event_tokkouNo);
			for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Tokkou_.GetLineNum(); ++i)
			{
				if (tokkouNo == pmgEO_->mgCoM_.mdm_Tokkou_.GetLineDt(i, mdm_tokkou_tokkouNo))
				{
					if (mid_team == pmgEO_->mgCoM_.mdm_Tokkou_.GetLineDt(i, mdm_tokkou_id_m_team))
					{
						draw_f = TRUE;
						//特効倍率
						s32 overlap = 1;
						if (id_team != 0) overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);

						tokkouMag_ = pmgEO_->mgCoM_.GetTokkouMag(mid_team, tokkouNo, overlap);

						tokkouMagStr_ = "Ｘ" + lib_str::IntToStr(tokkouMag_ / 100) + "．" + lib_str::IntToStr((tokkouMag_ % 100) / 1);
						//field_->f_MakeFont_Mini(strtokkou.c_str(), FC2_mini);
						break;
					}
				}
			}
		}
		field_->g_SetDraw(draw_f);
		field_->g_Flash();
	}

	void TUGTokkou::DoUpdate(BOOL draw_f)
	{
		field_->Update(draw_f);
	}

}
