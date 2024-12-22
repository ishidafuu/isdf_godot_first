#include "dbUGBtnShTime.h"
#include "dbMgUI.h"
#include "dbUIGenLayer.h"
#include "dbGame.h"

namespace db
{
  //コンストラクタ
  TUGBtnShTime::TUGBtnShTime(TUIGenLayer* player) :TUGButton(player)
  {
    stBtnShTime_.Init();

	if (pmgEO_->stSeq_.IsEventTimeAttack())
	{
		const s32 SEC = 60;
		stBtnShTime_.time_ = NGNUM * SEC;
	}
  }
  TUGBtnShTime::~TUGBtnShTime(void)
  {
  }
  void TUGBtnShTime::MakeGroupShTime(TUIObj* parent)
  {
    GenMakeGroup(parent, FALSE);//センタリングしない
    SetPos(124, -108);
    SetDepth(UIDP_05BUTTON);
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_game_time));
    field_->b_SetModal(TRUE);
    field_->g_SetPartNo(0);//0番画像
    field_->f_FontPos(4, 1);
  }


  //文字列セット
  void TUGBtnShTime::Refresh()
  {
    if (stBtnShTime_.timeover_f_) return;
    const s32 SEC = 60;
    const s32 SEC_HF = SEC / 2;
    field_->g_SetPartNo(0);

	//表示が違う
	// タイムアタック時
	if (player_->pGame_->mgSG_.stRf_.isTimeAttackMode_)
	{
		BOOL blink_f = (player_->pGame_->mgSG_.stRf_.LimitTime / SEC) - (player_->pGame_->mgSG_.stRf_.PlayTime_c / SEC) <= 30;
		BOOL needrefresh_f = ((player_->pGame_->mgSG_.stRf_.PlayTime_c / SEC) != (stBtnShTime_.time_ / SEC));
		stBtnShTime_.time_ = player_->pGame_->mgSG_.stRf_.PlayTime_c;

		BOOL blinkcol_f = (blink_f && ((player_->pGame_->mgSG_.stRf_.PlayTime_c % SEC_HF) == 0));

		if (needrefresh_f || blinkcol_f)
		{
			s32 fontcol = (blinkcol_f && (needrefresh_f == FALSE))
				? FC2_rareB
				: FC2_btn_normal;

			std::string st = base::MinSecString(player_->pGame_->mgSG_.stRf_.PlayTime_c, FALSE);
			field_->f_MakeFont(st.c_str(), fontcol);
			if (stBtnShTime_.skip_f_)
			{
				stBtnShTime_.skip_f_ = FALSE;
				SetPos(124, -108);
				field_->f_FontPos(4, 1);
				field_->f_SetCenter(FALSE);
				field_->g_SetCenter(FALSE);
			}
		}
	}
	else
	{
		BOOL blink_f = (player_->pGame_->mgSG_.stRf_.LimitTime / SEC) <= 30;
		BOOL needrefresh_f = ((player_->pGame_->mgSG_.stRf_.LimitTime / SEC) != (stBtnShTime_.time_ / SEC));
		stBtnShTime_.time_ = player_->pGame_->mgSG_.stRf_.LimitTime;

		BOOL blinkcol_f = (blink_f && ((player_->pGame_->mgSG_.stRf_.LimitTime % SEC_HF) == 0));

		if (needrefresh_f || blinkcol_f)
		{
			s32 fontcol = (blinkcol_f && (needrefresh_f == FALSE))
				? FC2_rareB
				: FC2_btn_normal;

			std::string st = base::MinSecString(player_->pGame_->mgSG_.stRf_.LimitTime, TRUE);
			field_->f_MakeFont(st.c_str(), fontcol);
			if (stBtnShTime_.skip_f_)
			{
				stBtnShTime_.skip_f_ = FALSE;
				SetPos(124, -108);
				field_->f_FontPos(4, 1);
				field_->f_SetCenter(FALSE);
				field_->g_SetCenter(FALSE);
			}
		}
	}

  }
  void TUGBtnShTime::RefreshTimeOver(BOOL overtime_f)
  {
    if (stBtnShTime_.timeover_f_) return;
    const s32 SEC = 60;
    if (overtime_f)
    {
      std::string st = base::MinSecString(0, TRUE);// (player_->pGame_->mgSG_.stRf_.LimitTime / SEC));
      field_->f_MakeFont(st.c_str(), FC2_btn_normal);
    }
    else
    {
      field_->g_SetDraw(FALSE);
    }
    stBtnShTime_.timeover_f_ = TRUE;

  }
  void TUGBtnShTime::RefreshRetry()
  {
    stBtnShTime_.Init();

	if (pmgEO_->stSeq_.IsEventTimeAttack())
	{
		const s32 SEC = 60;
		stBtnShTime_.time_ = NGNUM * SEC;
	}
    field_->g_SetDraw(TRUE);
    Refresh();
  }
  //ポーズ切り替え
  void TUGBtnShTime::SwitchPause(BOOL pause_f)
  {
    if (pause_f)
    {
      field_->g_SetPartNo(1);
      field_->f_SetFontDraw(FALSE);
    }
    else
    {
      field_->g_SetPartNo(0);
      field_->f_SetFontDraw(TRUE);
    }
  }
  void TUGBtnShTime::SetDisable(BOOL disable_f)
  {
    if (disable_f)
    {
      field_->g_Dark();
    }
    else
    {
      field_->g_EfReset();
    }
    field_->b_SetBtn(!disable_f);
  }
  void TUGBtnShTime::SetSkipTuto()
  {
    field_->g_SetPartNo(2);
    std::string st = "スキップ";
    field_->f_FontPos(0, 0);
    field_->f_SetCenter(TRUE);
    field_->g_SetCenter(TRUE);
    SetPos(124 + 18, -108 + 12);
    field_->f_MakeFont(st.c_str(), FC2_btn_normal);
    field_->f_SetFontDraw(TRUE);
    stBtnShTime_.skip_f_ = TRUE;
  }

  //更新
  void TUGBtnShTime::DoUpdateSub(BOOL draw_f)
  {
    //gb_ggz_->Update(draw_f);
  }

}