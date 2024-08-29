#include "dbUGShiai.h"
#include "dbUILayer.h"
#include "dbScene.h"

namespace db
{

  //コンストラクタ
  TUGShiai::TUGShiai(TUIGenLayer* player) :TUGGen(player)
    , ugbtn_time_(player)
    , ugcount_(player)
    , ugbtn_ctrl_(player)
    , ugcatch_(player)
    , ugkantoku_(player)
    , ugai_(player)
    , ugpad_shot_(player)
    , ugpad_pass_(player)
    , ugpad_jump_(player)
    , uglever_(player)
	, ugKillBtn_(player)
  {
    stShiai_.Init();
  }
  TUGShiai::~TUGShiai()
  {
  }

  void TUGShiai::MakeGroup(TUIObj* parent)
  {
    const s32 BARW = 512;

    //ヘッダフッタ間フィルタ
    field_ = MakeObj();
    field_->o_SetParent(parent);


    const s32 SAKUSENX = 190 - 24;
    const s32 HEADH = 44;//ヘッダサイズ

    header_ = MakeObj();
    header_->o_SetParent(field_);
    header_->o_SetObjFilterGrp(512, HEADH, 0xFF, 0, 0, 0);
    header_->z_SetPos_Full(0, 0);
    header_->g_SetDepth(UIDP_04HEADER);

    //監督

    //order_ = MakeObj();
    //order_->o_SetParent(field_);
    //order_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_game_order));
    //order_->z_SetPos_SafeL(24, -24);
    //order_->b_SetBtn(TRUE);
    //order_->g_SetDepth(UIDP_05BUTTON);

    ugkantoku_.MakeGroup(field_);
    ugbtn_time_.MakeGroupShTime(field_);
    ugcount_.MakeGroup(field_);
    ugbtn_ctrl_.MakeGroupShCtrl(field_);
    ugcatch_.MakeGroup(field_, &ugkantoku_);
    ugai_.MakeGroup(field_);
    ugpad_shot_.MakeGroup_Shot(field_);
    ugpad_pass_.MakeGroup_Pass(field_);
    ugpad_jump_.MakeGroup_Jump(field_);
    uglever_.MakeGroup(field_, FALSE);

	if (mid::midIsDebug())
	{
		ugKillBtn_.MakeGroup(field_, enUGGrp::ug_btn_ok);
		ugKillBtn_.SetFont("ころす");
		ugKillBtn_.SetPos(0, 95);
		ugKillBtn_.SetDepth(UIDP_05BUTTON);
	}

    SetDraw(TRUE);
  }

  void TUGShiai::Ready_Shiai()
  {
    player_->pGame_->Ready_Shiai();
    //stShiai_.tuto_f_ = FALSE;
    stShiai_.vs_f_ = FALSE;


    switch (pmgEO_->stShiai_.GetCtrl())
    {
    case 0:ugbtn_ctrl_.SetRetro(); break;
    case 1:ugbtn_ctrl_.SetSmart(); break;
	case 4:ugbtn_ctrl_.SetDebug(); break;
    default: ugbtn_ctrl_.SetRetro(); break;
    }
  }
  void TUGShiai::Ready_Tuto(BOOL retro_f)
  {
    player_->pGame_->Ready_Tuto(retro_f);
    ugbtn_time_.SetSkipTuto();
    ugbtn_time_.SetDraw(FALSE);
    ugbtn_ctrl_.SetDraw(FALSE);
    //stShiai_.tuto_f_ = TRUE;
    stShiai_.vs_f_ = FALSE;
    //各チュートリアルの操作モードに切り替え

    if (pmgEO_->stSeq_.IsTutoRetro())
    {
      ugbtn_ctrl_.SetRetro();
    }
    else if (pmgEO_->stSeq_.IsTutoSmart())
    {
      ugbtn_ctrl_.SetSmart();
    }

    SetHideScore(TRUE);
  }
  void TUGShiai::Ready_VS()
  {
    player_->pGame_->Ready_VS();
    ugbtn_time_.SetDisable(TRUE);//wifiでなければ出す
    //stShiai_.tuto_f_ = FALSE;
    stShiai_.vs_f_ = TRUE;

    switch (pmgEO_->stShiai_.GetCtrl())
    {
    case 0:ugbtn_ctrl_.SetRetro(); break;
    case 1:ugbtn_ctrl_.SetSmart(); break;
    default: ugbtn_ctrl_.SetRetro(); break;
    }
  }
  void TUGShiai::CanselPause()
  {
    stShiai_.cancelPause_f_ = TRUE;
  }
  void TUGShiai::Retry()
  {
    //復活処理
    player_->pGame_->Retry();
    //タイム表示など戻す
    PauseSetDraw(TRUE);
    ugbtn_time_.RefreshRetry();
  }
  void TUGShiai::SendTutoAct(s32 tutoact)
  {
    player_->pGame_->SetTutoAct(tutoact);
  }
  void TUGShiai::SendTutoGameAct(BOOL tonormal_f)
  {
    player_->pGame_->SetTutoGameAct();
    SetHideScore(FALSE);
    ugbtn_time_.SetDraw(TRUE);
    if (tonormal_f)
    {
      //stShiai_.tuto_f_ = FALSE;
      //時間の更新
      player_->pGame_->mgSG_.stRf_.LimitTime = 240 * 60;
      ugbtn_time_.Refresh();
    }
  }
  void TUGShiai::SetHideScore(BOOL hide_f)
  {
    stShiai_.hidescore_f_ = hide_f;
  }


  void TUGShiai::MvAuto()
  {
    stShiai_.enterCatch_f_ = FALSE;
    stShiai_.changeAI_f_ = FALSE;

    if (ugai_.IsAct())
    {
        s32 aino = ugai_.GetActRes();
      //選択中と違うAI
      if (stShiai_.aiNo_ != aino)//関数に
      {
        stShiai_.aiNo_ = aino;
        stShiai_.changeAI_f_ = TRUE;
      }
    }

    ////シュートオーダー切り替え
    //if (order_->b_IsDecide())
    //{
    //  stShiai_.order_f_ = !stShiai_.order_f_;
    //  if (stShiai_.order_f_) order_->g_SetPartNo(1);
    //  else order_->g_SetPartNo(0);
    //}
    //else
    {
      //画面タップ
      if ((ugai_.IsAct() == FALSE)
        && (ugbtn_time_.IsAct() == FALSE)
        //&& (order_->b_IsEnter(TRUE) == FALSE)
        )
      {
        if (field_->sc_IsCamEnter()) stShiai_.enterCatch_f_ = TRUE;//キャッチ命令
      }
    }

  }
  void TUGShiai::MvSmart()
  {

  }
  void TUGShiai::MvRetro()
  {
    //stShiai_.manualB_f_ = (ugpadbtn_[0]->IsAct());
    //stShiai_.manualA_f_ = (ugpadbtn_[1]->IsAct());
    //stShiai_.manualJ_f_ = (ugpadbtn_[2]->IsAct());
    //for (s32 i = 0; i < PADBTN; ++i)
    //{
    //  if (ugpadbtn_[i]->GetField()->b_IsEnter(TRUE) == FALSE) ugpadbtn_[i]->GetField()->g_SetAlp(FALSE, 128);
    //}
  }
  void TUGShiai::MvTech()
  {

  }
  void TUGShiai::MvDebug(void)
  {
	  MvSmart();

	if (ugKillBtn_.IsAct())
	{
		// 今いるやつを全員殺す
		s32 enemySide = 1;
		for (s32 i = 0; i < DBMEMBER_INF; i++)
		{
			player_->pGame_->mgGO_.pmgCh_[enemySide][i]->st_.pstMyCh_->ANGEL_f = TRUE;
			player_->pGame_->mgGO_.pmgCh_[enemySide][i]->st_.pstMyCh_->HP = 0;

			//player_->pGame_->mgGO_.pmgCh_[enemySide][i]->chCommon_.AddRec(recDeath_c);//
			pmgEO_->stResult_.IncTmpKillNum(0);
		}
		player_->pGame_->mgGO_.pmgTm_[enemySide]->CheckChangePos();
		// 一人を残して全員殺す
		for (s32 i = 0; i < DBMEMBER_INF - 2; i++)
		{
			player_->pGame_->mgGO_.pmgCh_[enemySide][i]->st_.pstMyCh_->ANGEL_f = TRUE;
			player_->pGame_->mgGO_.pmgCh_[enemySide][i]->st_.pstMyCh_->HP = 0;

			//player_->pGame_->mgGO_.pmgCh_[enemySide][i]->chCommon_.AddRec(recDeath_c);//
			pmgEO_->stResult_.IncTmpKillNum(0);
		}
		player_->pGame_->mgGO_.pmgTm_[enemySide]->CheckChangePos();
	}
  }

  void TUGShiai::SetPadAuto(TMgPad* pmgPad)
  {
    //セミオート
    pmgPad->SetBitAuto();
    //オーダー
    if (stShiai_.order_f_) pmgPad->SetBitOrder();
    //キャッチ
    if (stShiai_.enterCatch_f_) pmgPad->SetBitTouch();
    //ＡＩ
    pmgPad->SetBitCmd(stShiai_.aiNo_);
  }
  void TUGShiai::SetPadSmart(TMgPad* pmgPad)
  {
    //スマート
    pmgPad->SetBitSmart();
    //水平斬り入力
    mid::midSetUseHInput();
  }
  void TUGShiai::SetPadRetro(TMgPad* pmgPad)
  {
    //レトロ
    pmgPad->SetBitRetro();

    //四等分入力
    mid::midSetUseT1Input();
  }
  void TUGShiai::SetPadTech(TMgPad* pmgPad)
  {
    //てく
    pmgPad->SetBitTech();

    //四等分入力
    mid::midSetUseXInput();
  }
  void TUGShiai::SetHideRetro(BOOL hide_f)
  {
    stShiai_.hideretro_f_ = hide_f;
  }
  void TUGShiai::SetRetroStep(s32 totostep, TUGShPad::enPadMsgType msgtype)
  {
    stShiai_.retrotutostep_ = totostep;
    //TUGShPad::enPadMsgType msgtype = (of_f)
    //  ? TUGShPad::pmt_TutoOf
    //  : TUGShPad::pmt_TutoDf;
    ugpad_pass_.SetMsgType(msgtype);
    ugpad_shot_.SetMsgType(msgtype);
    ugpad_jump_.SetMsgType(msgtype);
  }
  void TUGShiai::Dissconnect()
  {
    ugbtn_time_.SetDisable(FALSE);//切断されたらポーズ可能に
    player_->pGame_->Dissconnect();
  }
  void TUGShiai::PauseSetDraw(BOOL draw_f)
  {
    //field_->Update(draw_f);
    header_->g_SetDraw(draw_f);
    //order_->g_SetDraw(draw_f);
    ugkantoku_.SetDraw(draw_f);
    ugai_.SetDraw(draw_f);
    ugbtn_ctrl_.SetDraw(draw_f);
    ugbtn_time_.SetDraw(draw_f);
    ugcatch_.SetDraw(draw_f);
    ugpad_shot_.SetDraw(draw_f);
    ugpad_pass_.SetDraw(draw_f);
    ugpad_jump_.SetDraw(draw_f);
    uglever_.SetDraw(draw_f);
	ugKillBtn_.SetDraw(draw_f);
  }

  void TUGShiai::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    header_->Update(draw_f);

    ugbtn_time_.Update(draw_f);

    if (stShiai_.hidescore_f_ == FALSE)
    {
      //捜査切り替えボタン試合後消す
      if ((player_->pGame_->IsGameSet() == FALSE) || (draw_f ==FALSE)) ugbtn_ctrl_.Update(draw_f);
      ugcount_.Update(draw_f);
    }


    if (ugbtn_ctrl_.IsSmart())
    {
    }
    else if (ugbtn_ctrl_.IsRetro())
    {
      if ((draw_f == FALSE) || (stShiai_.hideretro_f_ == FALSE))
      {
        if (pmgEO_->stShiai_.IsTuto())
        {
          BOOL dodge_f = (stShiai_.retrotutostep_ > 0);
          BOOL catch_f = (stShiai_.retrotutostep_ > 1);
          BOOL lever_f = (stShiai_.retrotutostep_ > 2);
          BOOL jump_f = (stShiai_.retrotutostep_ > 3);

          if (dodge_f) ugpad_pass_.Update(draw_f);
          if (catch_f) ugpad_shot_.Update(draw_f);
          if (jump_f) ugpad_jump_.Update(draw_f);
          if (lever_f) uglever_.Update(draw_f);
        }
        else
        {
          ugpad_shot_.Update(draw_f);
          ugpad_pass_.Update(draw_f);
          ugpad_jump_.Update(draw_f);
          uglever_.Update(draw_f);
        }
      }
    }
    else if (ugbtn_ctrl_.IsAuto())
    {
      //order_->Update(draw_f);//オーダー
      ugkantoku_.Update(draw_f);//監督
      ugcatch_.Update(draw_f);//キャッチ
      ugai_.Update(draw_f);//作戦
    }
    else if (ugbtn_ctrl_.IsTech())
    {
    }
	else if (ugbtn_ctrl_.IsDebug())
	{
		ugKillBtn_.Update(draw_f);
	}

    if (draw_f)
    {
      player_->pGame_->Draw();
    }
  }

  void TUGShiai::PauseBtnAct()
  {
    if (player_->pGame_->IsGameSet())
    {
      ugbtn_time_.RefreshTimeOver(player_->pGame_->IsTimeOver());
      return;
    }

    if (pmgEO_->stShiai_.IsTuto())
    {
      if (ugbtn_time_.IsAct()) SetAct(enUGShiaiAct_pause);//チュートスキップ
    }
    else
    {
      //時間の更新
      ugbtn_time_.Refresh();

      if (ugbtn_time_.IsAct())//ポーズ
      {
        stShiai_.puase_f_ = TRUE;
        ugbtn_time_.SwitchPause(TRUE);
        SetAct(enUGShiaiAct_pause);//UGAct
        PauseSetDraw(FALSE);
      }
      else if (stShiai_.cancelPause_f_)//ポーズからの復帰
      {
        stShiai_.puase_f_ = TRUE;
        stShiai_.cancelPause_f_ = FALSE;
        ugbtn_time_.SwitchPause(FALSE);
        PauseSetDraw(TRUE);
      }
      else
      {
        stShiai_.puase_f_ = FALSE;

        //入力切り替え
        ugbtn_ctrl_.NextCtrl();

        if (ugbtn_ctrl_.IsSmart()) MvSmart();
        else if (ugbtn_ctrl_.IsRetro()) MvRetro();
        else if (ugbtn_ctrl_.IsAuto()) MvAuto();
        else if (ugbtn_ctrl_.IsTech()) MvTech();
		else if (ugbtn_ctrl_.IsDebug()) MvDebug();
      }
    }
  }

  void TUGShiai::DoAction()
  {

    //ＵＩ入力を反映（全ての操作タイプでおそらく共通）
    mid::midSetUseUIInput();
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      TMgPad* pmgPad = player_->pGame_->mgGO_.pmgPad_[i];
      //ポーズ 両サイドはいらない
      if ((i == 0) && stShiai_.puase_f_)
      {
        pmgPad->SetBitPause();
      }

      if (ugbtn_ctrl_.IsSmart())  SetPadSmart(pmgPad);
      else if (ugbtn_ctrl_.IsRetro())  SetPadRetro(pmgPad);
      else if (ugbtn_ctrl_.IsAuto()) SetPadAuto(pmgPad);
      else if (ugbtn_ctrl_.IsTech()) SetPadTech(pmgPad);
	  else if (ugbtn_ctrl_.IsDebug())  SetPadSmart(pmgPad);
    }

    //ポーズボタン
    PauseBtnAct();

    //ボールカウンタリフレッシュ
    //ugcount_.Refresh();

    ////キャッチリフレッシュ
    //ugcatch_.Refresh();

    //ゲームの更新
    player_->pGame_->Update();

    //決着時と敗北時もここでアクションとして受け取る
    if (player_->pGame_->GetNextScene() != snShiai)
    {
      if (stShiai_.vs_f_)//VS
      {
        SetAct(enUGShiaiAct_result);//UGAct
        PauseSetDraw(FALSE);
      }
      else
      {
        if (player_->pGame_->IsWin()  || 
			pmgEO_->stShiai_.IsTour() ||//疑似対戦もリザルトへ
			pmgEO_->stShiai_.IsTimeAttack() ) // タイムアタックもリザルトへ
        {
          SetAct(enUGShiaiAct_result);//勝ったらリザルト
        }
        else
        {
          SetAct(enUGShiaiAct_retry);//それ以外はリトライ
        }
        PauseSetDraw(FALSE);
      }

    }
    else
    {
      if (player_->pGame_->IsGameSet())
      {
        SetAct(enUGShiaiAct_gameset);
      }
      else if (pmgEO_->stShiai_.IsTuto())//チュートリアル用
      {
        if (player_->pGame_->GetTutoAct() != NGNUM)
        {
          SetAct(enUGShiaiAct_tuto, player_->pGame_->GetTutoAct());//チュートアクション
        }
      }
    }
  }

  BOOL TUGShiai::IsActRes(enUGShiaiAct res)
  {
    return (GetActRes() == res);
  }
  s32 TUGShiai::GetCtrl()
  {
	  // もしもの時用
	  if (ugbtn_ctrl_.IsDebug())
	  {
		  return 1;	// スマホ操作と嘘をつく
	  }
    return ugbtn_ctrl_.GetCtrl();
  }
}
