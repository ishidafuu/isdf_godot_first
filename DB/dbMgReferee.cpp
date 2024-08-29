#include "dbMgReferee.h"
#include "dbGame.h"

namespace db
{
  //コンストラクタ
  TMgReferee::TMgReferee(TGame* pGame)
    :TGameObjGen(pGame)
  {

  }

  //コンストラクタ
  TMgReferee::~TMgReferee()
  {

  }

  BOOL TMgReferee::IsLimitTime()
  {
    //プレビュー以外せいげんじかんあり
    return (IsPrev() == FALSE);
    ////EtcSS
    //s32 time = 0;
    ////        switch (pmgEO_->mgStSn_.EtcSS.dbgm)
    ////        {
    ////            case dbgmKachinuki:
    ////                time = pmgEO_->mgStSn_.OptionSS.GetSt(cstmKaTime);
    ////                break;
    ////            case dbgmTaikou:
    ////                time = pmgEO_->mgStSn_.OptionSS.GetSt(cstmTaTime);
    ////                break;
    ////        }
    //
    //time = pmgEO_->mgStSn_.OptionSS.GetSt(cstmTime);
    //
    //
    //return ((pmgSG_->stRf_.Demo_f == FALSE) //デモは試合時間なし
    //        && (pmgEO_->stShiai_.IsTuto() == FALSE)//チュートリアル
    //        && (pmgEO_->mgDt_.dtOption_.GetDt(rkopGAMETIME, time) > 0));
  }

  BOOL TMgReferee::IsShiai()//操作可能か
  {
    return (pmgSG_->stRf_.DBSSt == dbssShiai);
  }
  BOOL TMgReferee::IsAngeling()//天使になるか
  {
    ////制限時間あり、サドンデスではない、時間切れ
    //if (IsLimitTime()
    //  && (pmgSG_->stRf_.Suddun_f == FALSE)
    //  && (pmgSG_->stRf_.LimitTime == 0))
    //{
    //  return FALSE;
    //}
    //elsePrev_f
    //{
    //  return TRUE;
    //}

    return TRUE;
  }

  BOOL TMgReferee::IsPrev()//プレビューか
  {
    return (pmgSG_->stRf_.Prev_f);
  }
  BOOL TMgReferee::IsTour()//疑似対人か
  {
    return (pmgSG_->stRf_.Tour_f);
  }
  //BOOL TMgReferee::IsTuto()//チュートリアルか
  //{
  //  return (pmgEO_->stShiai_.IsTuto());
  //}
  //BOOL TMgReferee::IsFirstTuto()//最初のチュートリアルか
  //{
  //  return (pmgEO_->stShiai_.IsTuto() && pmgSG_->stRf_.FirstTuto_f);
  //}

  BOOL TMgReferee::IsNoOverLine()//オーバラインなし
  {
    
    return (pmgSG_->stRf_.Prev_f
      || (pmgEO_->stShiai_.IsTuto() && (pmgSG_->stRf_.TutoNo < sta_ShotAct)));
  }
  BOOL TMgReferee::IsNoDamage()//ダメージ無し
  {
    return ((pmgEO_->stShiai_.IsTuto() && (pmgSG_->stRf_.TutoNo < sta_GameAct)));
  }
  BOOL TMgReferee::IsOKDodge()//避けＯＫ
  {
    return ((pmgEO_->stShiai_.IsTuto() == FALSE)
      || (pmgSG_->stRf_.TutoNo == sta_DodgeAct)
      || (pmgSG_->stRf_.TutoNo >= sta_GameAct));
  }
  BOOL TMgReferee::IsOKCatch()//キャッチＯＫ
  {
    return ((pmgEO_->stShiai_.IsTuto() == FALSE)
      || (pmgSG_->stRf_.TutoNo == sta_CatchAct)
      || (pmgSG_->stRf_.TutoNo >= sta_GameAct));
  }
  BOOL TMgReferee::IsOKWalk()//あるきＯＫ
  {
    return ((pmgEO_->stShiai_.IsTuto() == FALSE)
      || (pmgSG_->stRf_.TutoNo == sta_WalkAct)
      || (pmgSG_->stRf_.TutoNo == sta_DashAct)
      || (pmgSG_->stRf_.TutoNo == sta_ShotAct)
      || (pmgSG_->stRf_.TutoNo >= sta_GameAct));
  }
  BOOL TMgReferee::IsOKDash()//ダッシュＯＫ
  {
    return ((pmgEO_->stShiai_.IsTuto() == FALSE)
      || (pmgSG_->stRf_.TutoNo == sta_WalkAct)
      || (pmgSG_->stRf_.TutoNo == sta_ShotAct)
      || (pmgSG_->stRf_.TutoNo >= sta_GameAct));
  }
  BOOL TMgReferee::IsOKJump()//ジャンプＯＫ
  {
    return ((pmgEO_->stShiai_.IsTuto() == FALSE)
      || (pmgSG_->stRf_.TutoNo == sta_JumpAct)
      || (pmgSG_->stRf_.TutoNo == sta_ShotAct)
      || (pmgSG_->stRf_.TutoNo >= sta_GameAct));
  }
  BOOL TMgReferee::IsOKShot()//シュートＯＫ
  {
    return ((pmgEO_->stShiai_.IsTuto() == FALSE)
      || (pmgSG_->stRf_.TutoNo == sta_ShotAct)
      || (pmgSG_->stRf_.TutoNo >= sta_GameAct));
  }
  BOOL TMgReferee::IsOKPass()//パスＯＫ
  {
    return ((pmgEO_->stShiai_.IsTuto() == FALSE)
      || (pmgSG_->stRf_.TutoNo >= sta_GameAct));
  }
  BOOL TMgReferee::IsCapOnlyCtrl()//キャプテンのみ操作可能
  {
    return ((pmgEO_->stShiai_.IsTuto()) && (pmgSG_->stRf_.TutoNo < sta_GameAct));
  }
  BOOL TMgReferee::GetTutoActBtn()
  {
    return pmgSG_->stRf_.actbtn_f;
  }
  BOOL TMgReferee::IsReturnNoSide()
  {
    return FALSE;
    //return (pmgEO_->stShiai_.IsTuto()
    //  && ((pmgSG_->stRf_.TutoNo == sta_Intro)
    //  || (pmgSG_->stRf_.TutoNo == sta_Walk)
    //  || (pmgSG_->stRf_.TutoNo == sta_WalkSub)
    //  || (pmgSG_->stRf_.TutoNo == sta_Jump)
    //  || (pmgSG_->stRf_.TutoNo == sta_Dash)
    //  || (pmgSG_->stRf_.TutoNo == sta_Shot)
    //  ));
  }
  BOOL TMgReferee::IsReturnMySide()
  {
    return (pmgEO_->stShiai_.IsTuto()
      && (
      //(pmgSG_->stRf_.TutoNo == sta_Walk)
      //|| (pmgSG_->stRf_.TutoNo == sta_WalkSub)
      ((pmgSG_->stRf_.TutoNo == sta_Dash) && (pmgEO_->stSeq_.IsTutoRetro() == FALSE))
      || ((pmgSG_->stRf_.TutoNo == sta_DashAct) && (pmgEO_->stSeq_.IsTutoRetro() == FALSE))
      //|| (pmgSG_->stRf_.TutoNo == sta_Jump)
      //|| (pmgSG_->stRf_.TutoNo == sta_JumpSub)
      || (pmgSG_->stRf_.TutoNo == sta_Shot)
      || (pmgSG_->stRf_.TutoNo == sta_ShotAct)));
  }
  BOOL TMgReferee::IsReturnEnSide()
  {
    
    return (pmgEO_->stShiai_.IsTuto()
      && (
      (pmgSG_->stRf_.TutoNo == sta_Intro)
      || (pmgSG_->stRf_.TutoNo == sta_Walk)
      || (pmgSG_->stRf_.TutoNo == sta_WalkAct)
      || ((pmgSG_->stRf_.TutoNo == sta_Dash) && (pmgEO_->stSeq_.IsTutoRetro()))
      || ((pmgSG_->stRf_.TutoNo == sta_DashAct) && (pmgEO_->stSeq_.IsTutoRetro()))
      || (pmgSG_->stRf_.TutoNo == sta_Jump)
      || (pmgSG_->stRf_.TutoNo == sta_JumpAct)
      || (pmgSG_->stRf_.TutoNo == sta_Dodge)
      || (pmgSG_->stRf_.TutoNo == sta_DodgeAct)
      || (pmgSG_->stRf_.TutoNo == sta_Catch)
      || (pmgSG_->stRf_.TutoNo == sta_CatchAct)
      ));
  }
  BOOL TMgReferee::IsCOMMove()//ＣＯＭ動く
  {
    return ((pmgEO_->stShiai_.IsTuto() == FALSE)
      || (pmgSG_->stRf_.TutoNo == sta_DodgeAct)
      || (pmgSG_->stRf_.TutoNo == sta_CatchAct)
      || (pmgSG_->stRf_.TutoNo >= sta_GameAct));
  }
  BOOL TMgReferee::IsFinTuto()
  {
    return ((pmgEO_->stShiai_.IsTuto())
      && (pmgSG_->stRf_.TutoNo == sta_Fin));
  }
  void TMgReferee::SetTutoActBtn(enSendTutoAct tutono)//チュートリアルＮＧ
  {
    if (pmgEO_->stShiai_.IsTuto() == FALSE) return;
    //チュートリアルの要件と合っている場合
    if (pmgSG_->stRf_.TutoNo == tutono)
    {
      pmgSG_->stRf_.actbtn_f = TRUE;//チュートアクション発生（毎フレームオフにする）
    }
  }
  void TMgReferee::SetTutoNG(enSendTutoAct tutono, s32 ngno)//チュートリアルＮＧ
  {
    if (pmgEO_->stShiai_.IsTuto() == FALSE) return;
    //チュートリアルの要件と合っている場合
    if (pmgSG_->stRf_.TutoNo == tutono)
    {
      pmgSG_->stRf_.TutoSend_f = TRUE;//チュートアクション発生（毎フレームオフにする）
      pmgSG_->stRf_.TutoSend = ngno;//チュートアクション番号
      pmgSG_->stRf_.actbtn_f = FALSE;//アクションボタンクリア
    }
  }
  void TMgReferee::SetTutoOK(enSendTutoAct tutono)//チュートリアルＯＫ
  {
    if (pmgEO_->stShiai_.IsTuto() == FALSE) return;
    //チュートリアルの要件と合っている場合
    if (pmgSG_->stRf_.TutoNo == tutono)
    {
      pmgSG_->stRf_.TutoSend_f = TRUE;//チュートアクション発生（毎フレームオフにする）
      pmgSG_->stRf_.TutoSend = 0;//チュートアクション番号
      pmgSG_->stRf_.actbtn_f = FALSE;//アクションボタンクリア
      pmgSG_->stRf_.act_c = 0;//アクションボタンクリア
    }
  }
  void TMgReferee::SetPrevReturn()//プレビュー最低1回当たった
  {
    pmgSG_->stRf_.prevreturn_f = TRUE;
  }
  enSendTutoAct TMgReferee::GetTutoNo()//チュートリアル番号
  {
    return (enSendTutoAct)pmgSG_->stRf_.TutoNo;
  }
  void TMgReferee::IncTutoAct_c(enSendTutoAct tutono)//アクションカウンタ増加
  {
    if (pmgEO_->stShiai_.IsTuto() == FALSE) return;
    //チュートリアルの要件と合っている場合
    if (pmgSG_->stRf_.TutoNo == tutono)
    {
      ++pmgSG_->stRf_.act_c;//アクションボタンクリア
      //if (tutono == sta_WalkSub)
      {
        if (pmgSG_->stRf_.act_c > 60)
        {
          SetTutoOK(tutono);
        }
      }
    }
  }


  BOOL TMgReferee::IsSuddunDeath()
  {
    return (pmgSG_->stRf_.Suddun_f);
  }

  //参照設定
  void TMgReferee::RefSet()
  {
    //for (s32 i = 0; i < DBSIDE; ++i)
    //{
    //    pmgGO_->pmgTm_[i] = pmgGO_->pmgTm_[i];
    //}
    //pmgGO_->pmgBa_ = pmgGO_->pmgBa_;
  }
  void TMgReferee::Retry()
  {
    //const s32 MINCVT = (60 * 1);
    //ここで初期化
    pmgSG_->stRf_.Init();
    pmgSG_->stRf_.WinSide = NGNUM;
    pmgSG_->stRf_.NextScene = snShiai;
    pmgSG_->stRf_.DBSSt = dbssShiai;


    pmgSG_->stRf_.LimitTime = (pmgEO_->stShiai_.GetShiaiSec() * 60);// (pmgEO_->mgDt_.dtOption_.GetDt(rkopGAMETIME, time) * MINCVT);
    SetMotion(dbrfOvLine, mL);

	// カウントアップ表記
	if (pmgEO_->stSeq_.IsEventTimeAttack())
	{
		pmgSG_->stRf_.isTimeAttackMode_ = TRUE;
	}

    pmgEO_->mgNSF_.PlayID(pmgEO_->stShiai_.GetMidBGM());

    //最初だけ
    //pmgSG_->stRf_.Motion = dbrfShortWhistle;

    //pmgGO_->pmgBa_->baCommon_.ResetRefPos(NGNUM, jbJpball, FALSE);
  }

  //ステータス初期化
  void TMgReferee::Ready(BOOL demo_f, BOOL tour_f)
  {
    //s32 tWinner = (tAGE_f) ? pmgSG_->stRf_.WinSide : NGNUM;
    //const s32 MINCVT = (60 * 1);
    //ここで初期化
    pmgSG_->stRf_.Init();
    pmgSG_->stRf_.Demo_f = demo_f;
    pmgSG_->stRf_.NoSE_f = demo_f;//まずどんなときもTRUE
    pmgSG_->stRf_.WinSide = NGNUM;
    pmgSG_->stRf_.Tour_f = tour_f;
    pmgSG_->stRf_.NextScene = snShiai;
    pmgSG_->stRf_.DBSSt = dbssReady;

    //s32 time = pmgEO_->mgStSn_.OptionSS.GetSt(cstmTime);

    pmgSG_->stRf_.LimitTime = (pmgEO_->stShiai_.GetShiaiSec() * 60);// (pmgEO_->mgDt_.dtOption_.GetDt(rkopGAMETIME, time) * MINCVT);
    SetMotion(dbrfJumpBallWait, mN);

	// カウントアップ表記
	if (pmgEO_->stSeq_.IsEventTimeAttack())
	{
		pmgSG_->stRf_.isTimeAttackMode_ = TRUE;
	}

    //最初だけ
    pmgSG_->stRf_.Motion = (demo_f)
      ? dbrfJumpBall
      : dbrfJumpBallWait;

    pmgGO_->pmgBa_->baCommon_.ResetRefPos(NGNUM, jbJpball, demo_f);

  }

  //プレビュー
  void TMgReferee::ReadyPrev()
  {
    //ここで初期化
    pmgSG_->stRf_.Init();
    pmgSG_->stRf_.Prev_f = TRUE;
    pmgSG_->stRf_.WinSide = NGNUM;
    pmgSG_->stRf_.NextScene = snShiai;
    pmgSG_->stRf_.DBSSt = dbssShiai;
    pmgSG_->stRf_.LimitTime = 99999999;
    SetMotion(dbrfPrevReset, mN);
    pmgSG_->stRf_.Motion = dbrfStand;
    //pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev();

  }

  //チュートリアル
  void TMgReferee::ReadyTuto(BOOL first_f, BOOL retro_f)
  {
    //ここで初期化
    pmgSG_->stRf_.Init();
    pmgSG_->stRf_.WinSide = NGNUM;
    pmgSG_->stRf_.NextScene = snShiai;
    pmgSG_->stRf_.DBSSt = dbssShiai;
    pmgSG_->stRf_.LimitTime = 99999999;
    SetMotion(dbrfTutoReset, mN);
    pmgSG_->stRf_.Motion = dbrfStand;
    //pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev();
  }

  //ジングルから開始まで
  void TMgReferee::JglCount()
  {
    if (pmgSG_->stRf_.Demo_f)
    {
      //const s32 JGLEND = 3;//30;
      const s32 JGLEND = 30;
      if (lib_num::UpToR(&pmgSG_->stRf_.Jgl_c, JGLEND))
      {
        pmgSG_->stRf_.DBSSt = dbssShiai;//ゲーム開始
        //pscObj_->ResetNG_f = FALSE;
      }
    }
    else
    {
      if (pmgSG_->stRf_.JglSE_f == FALSE)
      {
        pmgEO_->mgSound_.StopALL();

        const s32 JGLSTART = 10;
        if (lib_num::UpToR(&pmgSG_->stRf_.Jgl_c, JGLSTART))
        {
          pmgSG_->stRf_.JglSE_f = TRUE;
          pmgSG_->stRf_.Jgl_c = 0;
          //pmgEO_->mgSound_.PlayJGL(jglStart);
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
		  pmgEO_->mgNSF_.PlayID(pmgEO_->stShiai_.GetReadyMidBGM());
#else
          pmgEO_->mgNSF_.Play(nsf_jgl_ready);
#endif
        }

      }
      else
      {
        const s32 JGLEND = 200;
        if (lib_num::UpToR(&pmgSG_->stRf_.Jgl_c, JGLEND))
        {
          //if (pmgEO_->mgStSn_.EtcSS.Nazo_f)
          //{
          //  pmgEO_->mgSound_.PlayBGM(mdCourt_Nazo);//謎に差し替える
          //}
          //else
          //{
          //  switch (pmgEO_->mgStSn_.EtcSS.CourtNo)
          //  {
          //    case c0Koutei: pmgEO_->mgSound_.PlayBGM(mdCourt_Taiikukan);break;
          //    case c1Taiikukan: pmgEO_->mgSound_.PlayBGM(mdCourt_Koutei);break;
          //    case c2Okujou: pmgEO_->mgSound_.PlayBGM(mdCourt_Okujou);break;
          //    case c3Jinja: pmgEO_->mgSound_.PlayBGM(mdCourt_Jinja);break;
          //    case c4Kouen: pmgEO_->mgSound_.PlayBGM(mdCourt_Kouen);break;
          //    case c5Kaigan: pmgEO_->mgSound_.PlayBGM(mdCourt_Kaigan);break;
          //    case c6Sentou: pmgEO_->mgSound_.PlayBGM(mdCourt_Sentou);break;
          //    case c7Stadium: pmgEO_->mgSound_.PlayBGM(mdCourt_Stadium);break;
          //  }
          //}

          pmgEO_->mgNSF_.PlayID(pmgEO_->stShiai_.GetMidBGM());

          pmgSG_->stRf_.DBSSt = dbssShiai;//ゲーム開始
        }
      }
    }
  }

  void TMgReferee::SetSYS(enSYSID sys)
  {
    if (pmgSG_->stRf_.Demo_f == FALSE)
    {
      pmgEO_->mgSound_.PlaySYS(sys);
    }
  }

  void TMgReferee::SetSE(enSEID se)
  {
    if (pmgSG_->stRf_.Demo_f == FALSE)
    {
      pmgEO_->mgSound_.PlaySE(se);
    }
  }

  void TMgReferee::SetMotion(enRfMotion tMotion)
  {
    SetMotion(tMotion, pmgSG_->stRf_.Muki);
  }

  void TMgReferee::SetMotion(enRfMotion tMotion, enMukiType tMuki)
  {
    pmgSG_->stRf_.M_c = 0;
    pmgSG_->stRf_.Motion = tMotion;
    pmgSG_->stRf_.Muki = tMuki;

    s32 side = 0;

    const s32 LWTIME = 180;
    const s32 SWTIME = 60;
    enJumpBall jb;
    switch (tMotion)
    {
    case dbrfPrevReset://プレビュー
      pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev(TRUE);
      break;

    case dbrfTutoReset://チュートリアル
      //pmgGO_->pmgBa_->baCommon_.ResetRefPos(NGNUM, jbJpball, FALSE);
      pmgGO_->pmgBa_->baCommon_.ResetRefPos_Tuto(TRUE, SIDE1);
      break;

    case dbrfJumpBallWait:
      switch (tMuki)
      {
      case mL:
        SetSYS(sysJudgeShort);
        side = 0;
        jb = jbSide0;
        break;
      case mR:
        SetSYS(sysJudgeShort);
        side = 1;
        jb = jbSide1;
        break;
      default:
        side = NGNUM;
        jb = jbNormal;
        break;
      }
      pmgGO_->pmgBa_->baCommon_.ResetRefPos(side, jb, pmgSG_->stRf_.Demo_f);
      break;

    case dbrfOvLine:

      switch (tMuki)
      {
      case mL:
        //SetSYS(sysJudgeShort);
        side = 0;
        jb = jbSide0;
        break;
      case mR:
        //SetSYS(sysJudgeShort);
        side = 1;
        jb = jbSide1;
        break;
      default:
        side = NGNUM;
        jb = jbNormal;
        break;
      }
      pmgGO_->pmgBa_->baCommon_.ResetRefPos(side, jb, pmgSG_->stRf_.Demo_f);
      break;

    case dbrfJumpBall:

      switch (tMuki)
      {
      case mL:
        side = 0;
        break;
      case mR:
        side = 1;
        break;
      default:
        side = NGNUM;
        break;
      }
      break;

    case dbrfLongWhistle:
      SetSYS(sysJudgeLong);
      pmgSG_->stRf_.Whistle_c = LWTIME;
      break;

    case dbrfShortWhistle:
      SetSYS(sysJudgeShort);
      pmgSG_->stRf_.Whistle_c = SWTIME;
      break;

    case dbrfIOChange:

      switch (tMuki)
      {
      case mL:
        SetSYS(sysJudgeShort);
        side = 0;
        jb = jbSide0;
        break;
      case mR:
        SetSYS(sysJudgeShort);
        side = 1;
        jb = jbSide1;
        break;
      default:
        side = NGNUM;
        jb = jbNormal;
        break;
      }
      //pmgGO_->pmgBa_->baCommon_.ResetRefPos(side, jb, pmgSG_->stRf_.Demo_f);
      break;

    }
  }

  //審判モーション
  void TMgReferee::IncMotion()
  {
    pmgSG_->stRf_.M_c++;

    switch (pmgSG_->stRf_.Motion)
    {
    case dbrfStand:

      if (pmgSG_->stRf_.M_c % 20 == 0)
      {
        s32 CNTAGL = 0;// ((DBCRT_WAL - pmgSG_->stBa_.Zahyou.Z) / 2);

        if (pmgSG_->stBa_.Zahyou.X < (DBCRT_CLXL - CNTAGL))
        {
          pmgSG_->stRf_.Muki = mL;
        }
        else if (pmgSG_->stBa_.Zahyou.X > (DBCRT_CLXR + CNTAGL))
        {
          pmgSG_->stRf_.Muki = mR;
        }
        else
        {
          pmgSG_->stRf_.Muki = mN;
        }
      }

      break;

    case dbrfJumpBallWait:
      if (pmgSG_->stRf_.M_c >= (JPBALLANIF * 6))
      {
        SetMotion(dbrfJumpBall);
      }

      break;

    case dbrfOvLine:
    case dbrfJumpBall:
      if (pmgSG_->stRf_.M_c == (JPBALLANIF * 3))
      {
        pmgGO_->pmgBa_->baCommon_.RefThrow();
        SetSYS(sysJudgeThrow);
      }

      if (pmgSG_->stRf_.M_c >= (JPBALLANIF * 5))
      {
        SetMotion(dbrfStand, pmgSG_->stRf_.Muki);
      }

      break;

    case dbrfShortWhistle:
    case dbrfLongWhistle:
      if (lib_num::AprTo0(&pmgSG_->stRf_.Whistle_c))
      {
        SetMotion(dbrfStand, pmgSG_->stRf_.Muki);
      }
      break;

    case dbrfIOChange:
      if (lib_num::AprTo0(&pmgSG_->stRf_.Whistle_c))
      {
        SetMotion(dbrfStand, pmgSG_->stRf_.Muki);
      }

      break;
    }
  }

  //レフェリーメッセージ
  void TMgReferee::RefereeMsg(std::string msg)
  {
    pmgGO_->pmgCommon_->PopupMsg(pu_Damage, msg.c_str(), NGNUM, DBCRT_CL, 0, DBCRT_WAL, FC2_shiai, FALSE);
  }

  void TMgReferee::Mv_TutoRcv()
  {
    //BOOL res = FALSE;

    if (pmgEO_->stShiai_.IsTuto() == FALSE) return;

    //送信情報初期化
    pmgSG_->stRf_.TutoSend_f = FALSE;
    pmgSG_->stRf_.TutoSend = 0;

    //レイヤから数値を受信したとき
    if (pmgSG_->stRf_.TutoRcv != 0)
    {
      pmgSG_->stRf_.TutoNo = pmgSG_->stRf_.TutoRcv;
      //受信情報ごとの処理

      ////実戦開始タイミング
      //if (pmgSG_->stRf_.TutoRcv == sta_Game)
      //{
      //  res = TRUE;
      //}

      //最後にクリアする//保存して現状扱いにする
      pmgSG_->stRf_.TutoRcv = 0;
    }

    //return res;
  }

  void TMgReferee::Mv_Shiai()
  {
    IncMotion();
    //経過時間
    ++pmgSG_->stRf_.PlayTime_c;

    //制限時間
    if (IsLimitTime() && (pmgSG_->stRf_.Suddun_f == FALSE))
    {
#ifdef __K_DEBUG_SHIAI__
		// タイマーストップフラグがFALSEなら通常処理
		BOOL ret = FALSE;
		if(!kdebug::DebugSystem::GetInstance()->IsStopTiemr())
		{
			ret = lib_num::AprTo0(&pmgSG_->stRf_.LimitTime);
		}

		if(ret)
#else
		BOOL ret = FALSE;
		if (pmgSG_->stRf_.isTimeAttackMode_)
		{
			// カウントアップ
			ret = (pmgSG_->stRf_.PlayTime_c > pmgSG_->stRf_.LimitTime) ? TRUE : FALSE;
		}
		else
		{
			// カウントダウン
			ret = lib_num::AprTo0(&pmgSG_->stRf_.LimitTime);
		}

      if (ret)
#endif
	  {
        //シナリオの時
        if (pmgEO_->stShiai_.IsVSHum() == FALSE)
        {
          //時間切れは相手のかち
          pmgSG_->stRf_.WinSide = 1;
          SetMotion(dbrfLongWhistle);
          pmgSG_->stRf_.GameSet_f = TRUE;
          pmgSG_->stRf_.TimeUp_f = TRUE;
        }
        else//対戦
        {
          //ドロー
          if (pmgGO_->pmgTm_[0]->DeadNum() == pmgGO_->pmgTm_[1]->DeadNum())
          {
            {
              pmgSG_->stRf_.Suddun_f = TRUE;//サドン状態
              pmgGO_->pmgTm_[0]->SuddunDeath();
              pmgGO_->pmgTm_[1]->SuddunDeath();

              //審判の元へ
              if (pmgSG_->stBa_.TimerSide == 0)
              {
                SetMotion(dbrfOvLine, mL);
                pmgGO_->pmgBa_->baCommon_.ResetTimer(NGNUM, FALSE);
              }
              else if (pmgSG_->stBa_.TimerSide == 1)
              {
                SetMotion(dbrfOvLine, mR);
                pmgGO_->pmgBa_->baCommon_.ResetTimer(NGNUM, FALSE);
              }
              else
              {
                SetMotion(dbrfShortWhistle);
              }

              RefereeMsg("サドンデス");
            }
          }
          else if (pmgGO_->pmgTm_[0]->DeadNum() < pmgGO_->pmgTm_[1]->DeadNum())
          {
            pmgSG_->stRf_.WinSide = 0;
          }
          else if (pmgGO_->pmgTm_[0]->DeadNum() > pmgGO_->pmgTm_[1]->DeadNum())
          {
            pmgSG_->stRf_.WinSide = 1;
          }

          if (pmgSG_->stRf_.Suddun_f == FALSE)
          {
            SetMotion(dbrfLongWhistle);
            pmgSG_->stRf_.GameSet_f = TRUE;
            pmgSG_->stRf_.TimeUp_f = TRUE;
          }
        }



      }
      else
      {
        //カウントダウン
        const s32 SEC = 60;
        if ((pmgSG_->stRf_.LimitTime == (SEC * 1))
          || (pmgSG_->stRf_.LimitTime == (SEC * 2))
          || (pmgSG_->stRf_.LimitTime == (SEC * 3)))
        {
          SetSE(seCountDown);
        }
      }
    }

    //サドンデス状態
    if (pmgSG_->stRf_.Suddun_f)
    {
      if (pmgGO_->pmgTm_[0]->DeadNum() < pmgGO_->pmgTm_[1]->DeadNum())
      {
        pmgSG_->stRf_.WinSide = 0;
        SetMotion(dbrfLongWhistle);
        pmgSG_->stRf_.GameSet_f = TRUE;
      }
      else if (pmgGO_->pmgTm_[0]->DeadNum() > pmgGO_->pmgTm_[1]->DeadNum())
      {
        pmgSG_->stRf_.WinSide = 1;
        SetMotion(dbrfLongWhistle);
        pmgSG_->stRf_.GameSet_f = TRUE;
      }
    }

    //全滅チェック
    if (pmgGO_->pmgTm_[0]->IsAllDead())
    {
      pmgSG_->stRf_.WinSide = 1;
      pmgSG_->stRf_.GameSet_f = TRUE;
    }
    else if (pmgGO_->pmgTm_[1]->IsAllDead())
    {
      pmgSG_->stRf_.WinSide = 0;
      pmgSG_->stRf_.GameSet_f = TRUE;
    }

    //決着
    if (pmgSG_->stRf_.GameSet_f)
    {
      switch (pmgSG_->stRf_.WinSide)
      {
      case 0:
        pmgGO_->pmgTm_[0]->st_.pstMyTm_->RType = rtWIN;
        pmgGO_->pmgTm_[1]->st_.pstMyTm_->RType = rtLOSE;
        break;
      case 1:
        pmgGO_->pmgTm_[0]->st_.pstMyTm_->RType = rtLOSE;
        pmgGO_->pmgTm_[1]->st_.pstMyTm_->RType = rtWIN;
        break;
      default:
        pmgGO_->pmgTm_[0]->st_.pstMyTm_->RType = rtTIMEUPDRAW;
        pmgGO_->pmgTm_[1]->st_.pstMyTm_->RType = rtTIMEUPDRAW;
        break;
      }

      //試合結果
      if (pmgEO_->stShiai_.IsVSHum())
      {
        //切断が検知できてないときがある
        s32 disco = (mid::midIsDisconnect())
          ? 1000
          : 0;

        //念のため
        if (mid::midIsDisconnect())
        {
          pmgEO_->stShiai_.SetDisConnect(TRUE);
        }
        

        s32 myside = mid::midGetMyIdx();
        pmgEO_->stResult_.SetResultVSHum(mid::midGetMyIdx(),
          pmgGO_->pmgTm_[myside]->st_.pstMyTm_->RType,
          pmgGO_->pmgTm_[myside]->st_.pstMyTm_->Rec.Dt(recDeath_c),
          mid::midGetMyBM(),
          mid::midGetEnBM(),
          mid::midGetNGNum(),
          mid::midGetSyncErrorCode() + (mid::midGetGKErrorCode() * 10) + disco//0以外の数値が入っていたときに切断と見なす
          );
      }
	  else if (pmgEO_->stShiai_.IsTimeAttack())
	  {
		  s32 myside = mid::midGetMyIdx();
		  s64 time = pmgSG_->stRf_.PlayTime_c;
		  pmgEO_->stResult_.SetResult(mid::midGetMyIdx(),
			  pmgGO_->pmgTm_[myside]->st_.pstMyTm_->RType,
			  pmgGO_->pmgTm_[myside]->st_.pstMyTm_->Rec.Dt(recDeath_c),
			  time,
			  pmgGO_->pmgTm_[myside]->st_.pstMyTm_->Rec.Dt(recMaxdam),
			  pmgGO_->pmgTm_[myside]->st_.pstMyTm_->Rec.Dt(recTotaldam));

		  //シナリオの時は勝ったときのみアイテム表示
		  if ((pmgEO_->stShiai_.IsVSHum() == FALSE)
			  && (pmgEO_->stShiai_.IsTour() == FALSE)
			  && (pmgGO_->pmgTm_[myside]->st_.pstMyTm_->RType == rtWIN))
		  {
			  pmgEO_->stDrop_.SetDrop();
		  }
	  }
      else
      {
        s32 myside = mid::midGetMyIdx();
        pmgEO_->stResult_.SetResult(mid::midGetMyIdx(),
          pmgGO_->pmgTm_[myside]->st_.pstMyTm_->RType,
          pmgGO_->pmgTm_[myside]->st_.pstMyTm_->Rec.Dt(recDeath_c),
          ((pmgEO_->stShiai_.GetShiaiSec() * 60) - pmgSG_->stRf_.LimitTime),
          pmgGO_->pmgTm_[myside]->st_.pstMyTm_->Rec.Dt(recMaxdam),
          pmgGO_->pmgTm_[myside]->st_.pstMyTm_->Rec.Dt(recTotaldam));

        //シナリオの時は勝ったときのみアイテム表示
        if ((pmgEO_->stShiai_.IsVSHum() == FALSE)
          && (pmgEO_->stShiai_.IsTour() == FALSE)
          && (pmgGO_->pmgTm_[myside]->st_.pstMyTm_->RType == rtWIN))
        {
          pmgEO_->stDrop_.SetDrop();
        }
      }

      pmgSG_->stRf_.Win_c = 0;
      pmgSG_->stRf_.DBSSt = dbssGameEnd;

      if (pmgSG_->stRf_.Demo_f == FALSE)
      {
        pmgEO_->mgSound_.StopBGM();
      }
    }
  }

  void TMgReferee::Mv_GameEnd()
  {
    if (pmgGO_->pmgTm_[0]->IsAllStop() && pmgGO_->pmgTm_[1]->IsAllStop())
    {
      if (lib_num::UpToR(&pmgSG_->stRf_.Win_c, 90))
      {
        if (pmgSG_->stRf_.Demo_f == FALSE)
        {
          const s32 WINJGL = 450;
          const s32 LOSEJGL = 380;
          const s32 DRAWJGL = 280;

         
          //チュートの時はジングルなら左内
          if (pmgGO_->pmgRf_->IsFinTuto() == FALSE)
          {
            if (pmgSG_->stRf_.WinSide == NGNUM)
            {
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
				pmgEO_->mgNSF_.PlayID(pmgEO_->stShiai_.GetLoseMidBGM());
#else
				pmgEO_->mgNSF_.Play(nsf_jgl_lose);
#endif
              pmgSG_->stRf_.JglTime = DRAWJGL;//引き分け
            }
            else
            {
              if (pmgSG_->stRf_.WinSide == pmgEO_->stShiai_.GetMySide())
              {
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
				  pmgEO_->mgNSF_.PlayID(pmgEO_->stShiai_.GetWinMidBGM());
#else
				  pmgEO_->mgNSF_.Play(nsf_jgl_win);
#endif
                pmgSG_->stRf_.JglTime = WINJGL;//勝ち
              }
              else
              {
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
				  pmgEO_->mgNSF_.PlayID(pmgEO_->stShiai_.GetLoseMidBGM());
#else
				  pmgEO_->mgNSF_.Play(nsf_jgl_lose);
#endif
                pmgSG_->stRf_.JglTime = LOSEJGL;//負け
              }
            }

          }


          //立ちに戻す
          SetMotion(dbrfStand);
        }

        pmgSG_->stRf_.Win_c = 0;
        pmgSG_->stRf_.DBSSt = dbssResult;
      }
    }
  }
  void TMgReferee::Mv_Result()
  {
    ++pmgSG_->stRf_.Win_c;

    if (pmgSG_->stRf_.Demo_f == FALSE)
    {

      if (pmgSG_->stRf_.Win_c < pmgSG_->stRf_.JglTime)
      {
        for (s32 i = 0; i < DBSIDE; ++i)
        {
          //ボタン押すとジングルとばす
          if (pmgGO_->pmgTm_[i]->st_.pstMyTm_->MANSide_f)
          {
            if (pmgGO_->pmgTm_[0]->st_.pSidePad_->IsNext())
            {
              pmgSG_->stRf_.Win_c = pmgSG_->stRf_.JglTime;
            }
          }
        }
      }
      else
      {
        //勝ち抜き戦勝利or対戦試合
        pmgSG_->stRf_.NextScene = snScenario;//この値を外部から観る
        //pmgSG_->stRf_.WinSide = 0;
        //pmgEO_->mgSound_.StopALL();
      }
    }
    else
    {
      if (pmgSG_->stRf_.Win_c > 300)
      {
        pmgSG_->stRf_.DBSSt = dbssGohome;
      }
    }
  }
  void TMgReferee::Mv_GoHome()
  {
    if (pmgGO_->pmgTm_[0]->IsGohome() && pmgGO_->pmgTm_[1]->IsGohome())
    {
      pmgSG_->stRf_.NextScene = snTitle;
    }
  }
  void TMgReferee::Mv_Pause()
  {
    //ポーズ
    if (pmgGO_->pmgTm_[0]->st_.pSidePad_->IsPause())
    {
      pmgSG_->stRf_.DBSSt = dbssShiai;
    }
  }
  //決着処理
  void TMgReferee::ShiaiMove()
  {
    //ポーズ
    if (pmgGO_->pmgTm_[0]->st_.pSidePad_->IsPause())
    {
      pmgSG_->stRf_.Pause_f = !(pmgSG_->stRf_.Pause_f);
    }

    if (pmgSG_->stRf_.Pause_f) return;

    switch (pmgSG_->stRf_.DBSSt)
    {
    case dbssReady: JglCount(); break;
    case dbssShiai: Mv_Shiai(); break;//試合中
    case dbssGameEnd: Mv_GameEnd(); break;//決着からジングルまで
    case dbssResult: Mv_Result(); break;//勝ちポーズ
    case dbssGohome:Mv_GoHome(); break;
      //case dbssPause:Mv_Pause(); break;
    }
  }

  //審判
  void TMgReferee::RefereeDraw()
  {

    s32 baseline = (pmgGO_->pmgRf_->IsPrev())
      ? DRAWBASELINE_PREV
      : DRAWBASELINE;

    //カメラ位置
    s32 camposX = pmgEO_->mgCamera_.GetRevCamPosX();
    s32 camposY = pmgEO_->mgCamera_.GetRevCamPosY();

    s32 rz = 0;
    s32 ShadowX = (DBCRT_CL / XYMAG) + camposX;
    s32 ShadowY = -(DBCRT_WAL / XYMAG) + camposY + baseline;

    //描画情報
    mid::TStDraw dst;
    dst.Init();
    dst.centerOrigin_f_ = TRUE;
    dst.depth_ = base::DPT_3OBJ_SHADOW;
    dst.argb_.aaa_ = SDWALP;

    //u8 col = (pmgEO_->mgStSn_.EtcSS.Nazo_f)
    //? SHADOWCOL_NAZO
    u8 col = SHADOWCOL;
    dst.argb_.rrr_ = col;
    dst.argb_.ggg_ = col;
    dst.argb_.bbb_ = col;

    //影
    pmgEO_->mgGrp_.pmgEtcGrp_->OrderDrawTile(obmt_CharShadow, ShadowX, ShadowY, &dst);

    //審判
    s32 gNo = 0;
    BOOL LRMir_f = FALSE;

    const s32 STOFS = 0;//たち
    const s32 WSOFS = 2;//ホイッスル
    const s32 JBGNOOFS = 4;//ジャンプボール
    const s32 JBGNO = 5;//ジャンプボールコマ数

    switch (pmgSG_->stRf_.Muki)
    {
    case mL:
      gNo = 1;
      break;
    case mR:
      gNo = 1;
      LRMir_f = TRUE;
      break;
    default: gNo = 0;
      break;
    }

    dst.Init();
    dst.mirror_f_ = LRMir_f;
    dst.depth_ = base::DPT_3OBJ_SHADOW + 10;
    dst.under_f_ = TRUE;
    dst.centerOrigin_f_ = TRUE;
    s32 posX = ShadowX;// -(pmgEO_->mgGrp_.pgr_Referee_->GetPartW() / 2);
    s32 posY = ShadowY;// -pmgEO_->mgGrp_.pgr_Referee_->GetPartH();

    switch (pmgSG_->stRf_.Motion)
    {
    case dbrfStand:
      pmgEO_->mgGrp_.pgr_Referee_->OrderDrawTile_CS(STOFS + gNo, posX, posY, &dst);
      break;

    case dbrfShortWhistle:
    case dbrfLongWhistle:
      pmgEO_->mgGrp_.pgr_Referee_->OrderDrawTile_CS(WSOFS + gNo, posX, posY, &dst);
      break;

    case dbrfJumpBallWait:
      pmgEO_->mgGrp_.pgr_Referee_->OrderDrawTile_CS(JBGNOOFS + (JBGNO * gNo), posX, posY, &dst);
      break;

    case dbrfOvLine:
    case dbrfJumpBall:
      pmgEO_->mgGrp_.pgr_Referee_->OrderDrawTile_CS(JBGNOOFS + (JBGNO * gNo) + (pmgSG_->stRf_.M_c / JPBALLANIF),
        posX, posY, &dst);
      break;

    case dbrfIOChange:
      pmgEO_->mgGrp_.pgr_Referee_->OrderDrawTile_CS(JBGNOOFS + (JBGNO * gNo), posX, posY, &dst);
      break;

    }
  }

  //試合時間
  void TMgReferee::PlayTimeDraw()
  {
    //試合時間
    if (IsLimitTime())
    {
      const s32 SECFLAME = 60;
      //表示が違う
      s32 drawtime = ((pmgSG_->stRf_.LimitTime + (SECFLAME - 1)) / SECFLAME);
      if (pmgSG_->stRf_.DrawLimitTime != drawtime)
      {
        pmgSG_->stRf_.DrawLimitTime = drawtime;

        base::enFontCol2 fc = (pmgSG_->stRf_.LimitTime < (60 * (SECFLAME - 1)))
          ? (base::enFontCol2)FC2_shiai
          : (base::enFontCol2)FC2_shiai;

        std::string st = (IsLimitTime())
          ? base::MinSecString(pmgSG_->stRf_.LimitTime, TRUE)
          : base::MinSecString(0, TRUE);

        pmgEO_->mgFont_.OrderFontSprite((s32)fnsGameTime,
          st.c_str(),
          380 - 40,
          8,
          fc,
          base::fd_None,
          base::DPT_4OVER_OBJ);
      }
    }
  }


  //描画
  void TMgReferee::Draw()
  {
    if ((pmgSG_->stRf_.Demo_f == FALSE) && (IsPrev() == FALSE))
    {
      //審判
      RefereeDraw();
    }
    //背景描画
    if (IsPrev() == FALSE)  pmgEO_->mgGrp_.pgr_Court_->OrderDrawGameBG();
  }
}

