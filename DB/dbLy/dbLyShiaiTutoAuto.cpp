#include "dbLyShiaiTutoAuto.h"
#include "dbSnShiai.h"
namespace db
{
	enum enPage
	{
		pg_Intro,
		pg_Dodge,
		pg_DodgeAct,
		pg_DodgeEnd,
		pg_Walk,
		pg_WalkAct,
		pg_WalkEnd,
		pg_Jump,
		pg_JumpAct,
		pg_JumpEnd,
		pg_Catch,
		pg_CatchAct,
		pg_CatchEnd,
		pg_Dash,
		pg_DashAct,
		pg_DashEnd,
		pg_Shot,
		pg_ShotAct,
		pg_ShotEnd,
		pg_Game,
		pg_GameAct,
		//pg_Fin,
		pg_Idle,
	};


	enum enTutoMsgShAdv
	{
		tma_dodge,
		tma_walk,
		tma_jump,
		tma_jump2,
		tma_catch,
		tma_dash,
		tma_dash2,
		tma_shot,
		tma_qpass,
		tma_tpass,
	};

	const s32 FX = 112;
	const s32 FY = 64;
	const s32 FYPASS = -32;

	//コンストラクタ
	TLyShiaiTutoAuto::TLyShiaiTutoAuto(TSnShiai* pScene, TUGShiai* ugshiai) :TLyShiaiTutoGen(pScene, ugshiai)
	{
	}

	TLyShiaiTutoAuto::~TLyShiaiTutoAuto()
	{
		Freemem(ugtuto_);
	}

	void TLyShiaiTutoAuto::DoLoadSub()
	{
		ugtuto_ = new TUGTutoMsgAuto(this);
		ugtuto_->MakeGroup(field_);
	}
	void TLyShiaiTutoAuto::DoReadySub()
	{
		ugtuto_->Ready();
	}
	//ページ切り替え
	void TLyShiaiTutoAuto::DoChangePage(s32 pageno)
	{
		ugfinger_.SetDraw(FALSE);

		switch (pageno)
		{
		case db::pg_Intro:
			SetPageData(pageno, sta_Intro, tpt_Info);
			ugshiai_->ugbtn_time_.SetDraw(TRUE);
			break;
		case db::pg_Dodge:SetPageData(pageno, sta_Dodge, tpt_Info); break;
		case db::pg_DodgeAct:SetPageData(pageno, sta_DodgeAct, tpt_Act); break;
		case db::pg_DodgeEnd:SetPageData(pageno, sta_Dodge, tpt_End); break;
		case db::pg_Walk:SetPageData(pageno, sta_Walk, tpt_Info); break;
		case db::pg_WalkAct:SetPageData(pageno, sta_WalkAct, tpt_Act); break;
		case db::pg_WalkEnd:SetPageData(pageno, sta_Walk, tpt_End); break;
		case db::pg_Jump:SetPageData(pageno, sta_Jump, tpt_Info); break;
		case db::pg_JumpAct:SetPageData(pageno, sta_JumpAct, tpt_Act); break;
		case db::pg_JumpEnd:SetPageData(pageno, sta_Jump, tpt_End); break;
		case db::pg_Catch:SetPageData(pageno, sta_Catch, tpt_Info); break;
		case db::pg_CatchAct:SetPageData(pageno, sta_CatchAct, tpt_Act); break;
		case db::pg_CatchEnd:SetPageData(pageno, sta_Catch, tpt_End); break;
		case db::pg_Dash:SetPageData(pageno, sta_Dash, tpt_Info); break;
		case db::pg_DashAct:SetPageData(pageno, sta_DashAct, tpt_Act); break;
		case db::pg_DashEnd:SetPageData(pageno, sta_Dash, tpt_End); break;
		case db::pg_Shot:SetPageData(pageno, sta_Shot, tpt_Info); break;
		case db::pg_ShotAct:SetPageData(pageno, sta_ShotAct, tpt_Act); break;
		case db::pg_ShotEnd:SetPageData(pageno, sta_Shot, tpt_End); break;
		case db::pg_Game:SetPageData(pageno, sta_Game, tpt_Info); break;
		case db::pg_GameAct:SetPageData(pageno, sta_GameAct, tpt_Act); break;
			//case db::pg_Fin:SetPageData(pageno, sta_Fin, tpt_Info); break;
		}

		switch (pageno)
		{
		case db::pg_DodgeAct:ugfinger_.SetMotionLongTouch(FX, FYPASS); break;
		case db::pg_WalkAct:ugfinger_.SetMotionGrgr(FX, FY); break;
		case db::pg_JumpAct:ugfinger_.SetMotionLongTouch(FX, FY); break;
		case db::pg_CatchAct:ugfinger_.SetMotionTouch(FX, FY); break;
		case db::pg_DashAct:ugfinger_.SetMotionTouch(FX, FY); break;
		case db::pg_ShotAct:ugfinger_.SetMotionTouch(FX, FY); break;
		case db::pg_GameAct:
			ugshiai_->SendTutoGameAct(TRUE);
			pmgEO_->stShiai_.SetTutoToNormal();
			ugtuto_->SetDraw(FALSE);
			//ugtuto_->ugserifu_.SetMsgChangeMode(enMsgChangeMode_Wait2);//スマホの時は追加アドバイス
			break;
		}

	}
	void TLyShiaiTutoAuto::DoUpdateSub(BOOL draw_f)
	{

		if (draw_f == FALSE)
		{
			switch (stLy_.pageNo_)
			{
			case db::pg_GameAct: MvPage_GameAct(); break;
			case db::pg_Idle: MvPage_Idle(); break;
			default:
				switch (stTuto_.pagetype_)
				{
				case tpt_Info:MvPage_Info(); break;
				case tpt_Act:MvPage_Act(); break;
				case tpt_End:MvPage_End(); break;
				}

				if (ugshiai_->IsActRes(enUGShiaiAct_pause))
				{
					if (stLy_.pageNo_ < pg_GameAct)
					{
						ChangePage(pg_GameAct);
					}
					else
					{
						pScene_->lyShBar_->MoveLayer(shl_Result);
					}
				}
				break;
			}

			if (stLy_.pageNo_ != db::pg_GameAct
				&& stLy_.pageNo_ != db::pg_Idle)
			{
				ugtuto_->CheckSkipMsg();
			}
		}
	}


	//実戦開始
	void TLyShiaiTutoAuto::MvPage_GameAct()
	{
		if (ugshiai_->IsActRes(enUGShiaiAct_pause))//ポーズ
		{
			pScene_->lyShBar_->MoveLayer(shl_Pause);
		}
		else if (ugshiai_->IsActRes(enUGShiaiAct_retry))//リトライ
		{
			pScene_->lyShBar_->MoveLayer(shl_Retry);
		}
		else if (ugshiai_->IsActRes(enUGShiaiAct_result))//結果
		{
			pScene_->lyShBar_->MoveLayer(shl_Result);
			ChangePage(pg_Idle);
		}
	}
}
