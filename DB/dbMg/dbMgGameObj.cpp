#include "dbMgGameObj.h"
#include "dbGame.h"

namespace db
{
  const s32 SLOWTIME = 60;

  //コンストラクタ
  TMgGameObj::TMgGameObj(TGame* pGame)
    : pGame_(pGame), pmgEO_(pGame->pmgEO_), pmgSG_(&pGame->mgSG_)
  {
    //各GameObj生成
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      pmgTm_[i] = new TMgTeam(pGame, i);
      pmgPad_[i] = new TMgPad(pGame, pGame->pmgEO_->mgInput_.pPadGM_[i]);//ゲームのパッドを渡す

      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {
        pmgCh_[i][i2] = new TMgChar(pGame, i, i2);
      }
    }

    for (s32 i = 0; i < DBETCOBJ; ++i)
    {
      pmgEtc_[i] = new TMgEtc(pGame, i);
    }

    pmgRf_ = new TMgReferee(pGame);
    pmgBa_ = new TMgBall(pGame);
    pmgCommon_ = new TMgCommon(pGame);


    //各GameObj生成後に参照セット

    //各GameObj生成
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      pmgTm_[i]->RefSet();

      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {
        pmgCh_[i][i2]->RefSet();
      }
    }

    pmgRf_->RefSet();
    pmgBa_->RefSet();
  }


  TMgGameObj::~TMgGameObj(void)
  {
    Freemem(pmgCommon_);
    Freemem(pmgRf_);
    Freemem(pmgBa_);

    for (s32 i = 0; i < DBSIDE; ++i)
    {
      Freemem(pmgTm_[i]);
      Freemem(pmgPad_[i]);

      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {
        Freemem(pmgCh_[i][i2]);
      }
    }

    for (s32 i = 0; i < DBETCOBJ; ++i)
    {
      Freemem(pmgEtc_[i]);
    }
  }

  //管理しているオブジェの初期化
  void TMgGameObj::RefSet()
  {
    pmgRf_->RefSet();
    pmgBa_->RefSet();
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      pmgTm_[i]->RefSet();
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {
        pmgCh_[i][i2]->RefSet();
      }
    }

  }
  //リトライ
  void TMgGameObj::Retry()
  {
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      pmgTm_[i]->Ready(FALSE);
    }

    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {
        pmgCh_[i][i2]->chCommon_.RespawnInit();//復活
      }
    }
    pmgRf_->Retry();

    pmgBa_->baCommon_.ResetCamPos();
  }
  //切断
  void TMgGameObj::Dissconnect()
  {
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      pmgTm_[i]->SetSidePad();
    }
  }
  //管理しているオブジェの試合直前初期化
  void TMgGameObj::Ready_Shiai(BOOL demo_f, BOOL tour_f)
  {
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      pmgTm_[i]->Ready(demo_f);
    }

    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {
        pmgCh_[i][i2]->Ready();
      }
    }
    pmgRf_->Ready(demo_f, tour_f);

    if (demo_f) pmgBa_->baCommon_.ResetCamPos();
  }
  //プレビュー
  void TMgGameObj::Ready_Prev()
  {
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      pmgTm_[i]->Ready(FALSE);
    }

    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_INF; ++i2)
      {
        pmgCh_[i][i2]->Ready();
        if (i ==SIDE0) break;
      }
    }

    pmgRf_->ReadyPrev();

    //pmgBa_->baCommon_.ResetCamPos();
  }
  //チュートリアル
  void TMgGameObj::Ready_Tuto(BOOL first_f, enGOType type)
  {
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      pmgTm_[i]->Ready(FALSE);
    }

    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {
        pmgCh_[i][i2]->Ready();


        if (i2 != 0)
        {
          pmgCh_[i][i2]->st_.pstMyCh_->Dead_f = TRUE;
        }
      }
    }
    //チュートリアル
	pmgRf_->ReadyTuto(first_f, type);
  }

  void TMgGameObj::Ready2_TutoJissen()
  {
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {

        if (i2 == 0) continue;

        pmgCh_[i][i2]->Ready();
        pmgCh_[i][i2]->st_.pstMyCh_->Dead_f = FALSE;
        pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.X = 0;// -9600;
        pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.Z = (DBCRT_FL + (i2 * 12 * XYMAG));
        pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.Muki = (i2 < 4)
          ? mR
          : mL;
        switch (pmgCh_[i][i2]->st_.posNo_)
        {
        case (s32)dbpoO2: pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.MukiZ = mzF; break;
        case (s32)dbpoO3: pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.MukiZ = mzB; break;
        default: pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.MukiZ = mzF; break;
        }
        if (pmgCh_[i][i2]->st_.mysideNo_ == 1)
        {
          pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.X = (DBCRT_W);// -pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.X);
          pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.Muki = pmgCh_[i][i2]->chCommon_.RevMuki(pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.Muki);
        }
        pmgCh_[i][i2]->chCommon_.SetMtypeReset(dbmtSt);

      }
    }
  }


  //管理しているオブジェの試合直前初期化
  void TMgGameObj::Ready(enGOType gotype)
  {
    gotype_ = gotype;

    //オリジナル必殺クリア
    pmgEO_->mgDt_.dtShSyn_.ClearOrgShName();

    switch (gotype)
    {
    case goTitle: Ready_Shiai(TRUE, FALSE); break;
    case goShiai: Ready_Shiai(FALSE, FALSE); break;
    case goShiaiTour: Ready_Shiai(FALSE, TRUE); break;
    case goPrev: Ready_Prev(); break;
	case goTutoFirst: Ready_Tuto(TRUE, enGOType::goTutoFirst); break;
	case goTutoRetro: Ready_Tuto(FALSE, enGOType::goTutoRetro); break;
	case goTutoSmart: Ready_Tuto(FALSE, enGOType::goTutoSmart); break;
	case goTutoAuto: Ready_Tuto(FALSE, enGOType::goTutoAuto); break;
    default: break;
    }
  }

  void TMgGameObj::ShiaiObjMove_Cap()
  {
    //パッド
    for (s32 i = 0; i < DBSIDE; ++i) pmgPad_[i]->Update();

    //チーム
    for (s32 i = 0; i < DBSIDE; ++i) pmgTm_[i]->TeamMove();

    //キャラカウンタ
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoCount();
    }

    //ボールカウンタ
    pmgBa_->DoCount();

    //キャラ入力
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoInput();
    }

    //キャラ動作
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoMove();
    }

    //ボール動作
    pmgBa_->DoMove();

    //キャラ再動作
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoRemove();
    }

    //キャラ判定枠
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoSetRect();
    }

    //ボール判定枠
    pmgBa_->DoSetRect();

    //ボール判定
    pmgBa_->DoJudge();

    //キャラ再動作
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoRemove();
    }

    //小物オブジェ
    for (s32 i = 0; i < DBETCOBJ; ++i) pmgEtc_[i]->Update();

  }

  //試合★★
  void TMgGameObj::ShiaiObjMove()
  {
    //パッド
    for (s32 i = 0; i < DBSIDE; ++i) pmgPad_[i]->Update();

    //チーム
    for (s32 i = 0; i < DBSIDE; ++i) pmgTm_[i]->TeamMove();

#ifdef __K_DEBUG_SHIAI__
    // ここで必殺シュートデバッグを実行
    pmgTm_[0]->Debug_SetShot();
#endif	// #ifdef __K_DEBUG_SHIAI__

    //キャラカウンタ
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoCount();
    }

    //ボールカウンタ
    pmgBa_->DoCount();

    //キャラ入力
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoInput();
    }

    //キャラ動作
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoMove();
    }

    //ボール動作
    pmgBa_->DoMove();

    //キャラ再動作
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoRemove();
    }

    //キャラ判定枠
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoSetRect();
    }

    //ボール判定枠
    pmgBa_->DoSetRect();

    //ボール判定
    pmgBa_->DoJudge();

    //キャラ再動作
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoRemove();
    }

    //小物オブジェ
    for (s32 i = 0; i < DBETCOBJ; ++i) pmgEtc_[i]->Update();
  }

  //チュート終了待機
  void TMgGameObj::TutoEndMove()
  {
    //チーム
    for (s32 i = 0; i < DBSIDE; ++i) pmgTm_[i]->TeamMove();

    //キャラカウンタ
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoCount();
    }
    //キャラ入力
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoInput();
    }
    ////キャラ動作
    //for (s32 i = 0; i < DBSIDE; ++i)
    //{
    //  for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoMove();
    //}

    //ボールカウンタ
    pmgBa_->DoCount();

    //ボール動作
    pmgBa_->DoMove();

    //小物オブジェ
    for (s32 i = 0; i < DBETCOBJ; ++i) pmgEtc_[i]->Update();
  }

  //試合終了
  void TMgGameObj::ResultObjMove()
  {
    //チーム
    for (s32 i = 0; i < DBSIDE; ++i) pmgTm_[i]->TeamMove();

    //試合終了モーション
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      enDBMotionType ResultMType = dbmtWIN;

      switch (pmgTm_[i]->st_.pstMyTm_->RType)
      {
      case rtWIN: ResultMType = dbmtWIN; break;
      case rtLOSE: ResultMType = dbmtLOSE; break;
      case rtTIMEUPDRAW: ResultMType = dbmtDRAW; break;
      }

      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {
        pmgCh_[i][i2]->chCommon_.SetMtypeReset(ResultMType);
      }
    }

    //キャラカウンタ
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoCount();
    }

    //ボールカウンタ
    pmgBa_->DoCount();

    //小物オブジェ
    for (s32 i = 0; i < DBETCOBJ; ++i) pmgEtc_[i]->Update();

  }

  //デモ帰宅
  void TMgGameObj::GameSetObjMove()
  {

    //キャラカウンタ
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoCount();
    }

    //キャラ入力
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoInput();
    }

    //キャラ動作
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoMove();
    }

    //小物オブジェ
    for (s32 i = 0; i < DBETCOBJ; ++i) pmgEtc_[i]->Update();
  }

  void TMgGameObj::Update_Shiai()
  {

    if (pmgSG_->stRf_.NextScene == snShiai)
    {
      if (pmgSG_->stRf_.Pause_f == FALSE)
      {
        switch (pmgSG_->stRf_.DBSSt)
        {
          //レディ
        case dbssReady:
          ////レフェリージングル
          //pmgRf_->JglCount();
          break;
        case dbssShiai://試合中
          ShiaiObjMove();
          break;
        case dbssGameEnd://決着
          if (pmgSG_->stRf_.TimeUp_f)//タイムアップの時はスロー無し
          {
            ShiaiObjMove();
          }
          else
          {
            ++pmgSG_->stRf_.Slow_c;
            if ((pmgSG_->stRf_.Slow_c > SLOWTIME)//スロー時間過ぎてる
              || (pmgTm_[0]->IsNoFly() && pmgTm_[1]->IsNoFly())//誰も飛んでない
              || (pmgSG_->stRf_.Slow_c % 3 == 0))//スロー速度
            {
              ShiaiObjMove();
            }
          }
          break;
          //試合終了
        case dbssResult:
          ResultObjMove();
          break;

          //デモ帰宅
        case dbssGohome:
          GameSetObjMove();
          break;
        }
      }

      //レフェリー試合
      pmgRf_->ShiaiMove();

    }
  }

  void TMgGameObj::Update_Prev()
  {
    //パッド
    for (s32 i = 0; i < DBSIDE; ++i) pmgPad_[i]->Update();

    //チーム
    for (s32 i = 0; i < DBSIDE; ++i) pmgTm_[i]->TeamMove();

    //キャラカウンタ
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      //for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoCount();
      for (s32 i2 = 0; i2 < DBMEMBER_INF; ++i2)
      {
        pmgCh_[i][i2]->DoCount();
        if (i == SIDE0) break;
      }
    }

    //ボールカウンタ
    pmgBa_->DoCount();

    //キャラ入力
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      //for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoInput();
      for (s32 i2 = 0; i2 < DBMEMBER_INF; ++i2)
      {
        pmgCh_[i][i2]->DoInput();
        if (i == SIDE0) break;
      }
    }

    //キャラ動作
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      //for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoMove();
      for (s32 i2 = 0; i2 < DBMEMBER_INF; ++i2)
      {
        pmgCh_[i][i2]->DoMove();
        if (i == SIDE0) break;
      }
    }

    //ボール動作
    pmgBa_->DoMove();

    //キャラ再動作
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      //for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoRemove();
      for (s32 i2 = 0; i2 < DBMEMBER_INF; ++i2)
      {
        pmgCh_[i][i2]->DoRemove();
        if (i == SIDE0) break;
      }
    }

    //キャラ判定枠
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      //for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoSetRect();
      for (s32 i2 = 0; i2 < DBMEMBER_INF; ++i2)
      {
        pmgCh_[i][i2]->DoSetRect();
        if (i == SIDE0) break;
      }
    }

    //ボール判定枠
    pmgBa_->DoSetRect();

    //ボール判定
    pmgBa_->DoJudge();

    //キャラ再動作
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      //for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoRemove();
      for (s32 i2 = 0; i2 < DBMEMBER_INF; ++i2)
      {
        pmgCh_[i][i2]->DoRemove();
        if (i == SIDE0) break;
      }
    }

    //小物オブジェ
    for (s32 i = 0; i < DBETCOBJ; ++i) pmgEtc_[i]->Update();
  }

  //チュートリアル
  void TMgGameObj::Update_Tuto()
  {
    if (pmgSG_->stRf_.NextScene == snShiai)
    {
      //ここでチュート送受信情報処理するか
      //pmgRf_->
      pmgRf_->Mv_TutoRcv();

      //実戦スタンバイ
      if (pmgSG_->stRf_.TutoGameAct_f)
      {
        pmgSG_->stRf_.TutoGameAct_f = FALSE;
        Ready2_TutoJissen();
      }

      //チュート終了
      if (pmgRf_->IsFinTuto() && pmgEO_->stSeq_.IsFirstTuto())
      {
        TutoEndMove();
      }
      else  if (pmgRf_->IsCapOnlyCtrl())
      {
        //試合
        ShiaiObjMove_Cap();
      }
      else
      {
        if (pmgSG_->stRf_.Pause_f == FALSE)
        {
          switch (pmgSG_->stRf_.DBSSt)
          {
            //レディ
          case dbssReady:
            break;
          case dbssShiai://試合中
            ShiaiObjMove();
            break;
          case dbssGameEnd://決着
            if (pmgSG_->stRf_.TimeUp_f)//タイムアップの時はスロー無し
            {
              ShiaiObjMove();
            }
            else
            {
              ++pmgSG_->stRf_.Slow_c;
              if ((pmgSG_->stRf_.Slow_c > SLOWTIME)//スロー時間過ぎてる
                || (pmgTm_[0]->IsNoFly() && pmgTm_[1]->IsNoFly())//誰も飛んでない
                || (pmgSG_->stRf_.Slow_c % 3 == 0))//スロー速度
              {
                ShiaiObjMove();
              }
            }
            break;
            //試合終了
          case dbssResult:
            ResultObjMove();
            break;
          }
        }
      }
      //レフェリー試合
      pmgRf_->ShiaiMove();//これをチュート用に書き換える
    }
  }


  //更新
  void TMgGameObj::Update()
  {
    s32 ctrlpos_bf = 0;

    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {
        if (i2 == pmgTm_[i]->st_.pstMyTm_->CtrlNo)
        {
          ctrlpos_bf += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.X;
          ctrlpos_bf += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.Y;
          ctrlpos_bf += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.Z;
        }
      }
    }


    switch (gotype_)
    {
    case goPrev: Update_Prev(); break;
    case goTutoFirst:
    case goTutoRetro:
    case goTutoSmart:
	case goTutoAuto:
      Update_Tuto();
      break;
    default: Update_Shiai(); break;
    }

    //通信同期ズレチェック用ボール位置保存
    s32 ballpos = pmgSG_->stBa_.Zahyou.X + pmgSG_->stBa_.Zahyou.Y + pmgSG_->stBa_.Zahyou.Z;
    //s32 charpos = 0;
    s32 ctrlpos = 0;
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++)
      {
        if (i2 == pmgTm_[i]->st_.pstMyTm_->CtrlNo)
        {
          ctrlpos += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.X;
          ctrlpos += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.Y;
          ctrlpos += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.Z;
          
          //charpos += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.dX;
          //charpos += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.dY;
          //charpos += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.dZ;
        }
//        else
//        {
//          charpos += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.X;
//          charpos += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.Y;
//          charpos += pmgCh_[i][i2]->st_.pstMyCh_->Zahyou.Z;
//        }
      }
    }
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      if (pmgPad_[i]->ppad_->Nowdxpad() != pmgPad_[i]->ppad_->NowBuf_D(0).keyBit_)
      {
        s32 asdf = 0;
      }
    }
    //この値をチェックする
    //pmgPad_[0]->ppad_->Nowdxpad();


    mid::midSetNowCheckPos(ctrlpos, ctrlpos_bf);
  }


  void TMgGameObj::Draw_Shiai()
  {
    //試合時間と背景
    pmgRf_->Draw();

    //チーム全員の名前と顔
    for (s32 i = 0; i < DBSIDE; ++i) pmgTm_[i]->Draw();

    //キャラ描画
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoDraw();
    }

    //ボール描画
    pmgBa_->DoDraw();

    //小物オブジェ描画
    for (s32 i = 0; i < DBETCOBJ; ++i) pmgEtc_[i]->Draw();
  }
  void TMgGameObj::Draw_Prev()
  {
    //キャラ描画
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      //pmgCh_[i][0]->DoDraw();
      for (s32 i2 = 0; i2 < DBMEMBER_INF; ++i2)
      {
        pmgCh_[i][i2]->DoDraw();
        if (i == SIDE0) break;
      }
    }

    //ボール描画
    pmgBa_->DoDraw();

  }
  void TMgGameObj::Draw_Tuto()
  {
    //試合時間と背景
    pmgRf_->Draw();

    //チーム全員の名前と顔
    for (s32 i = 0; i < DBSIDE; ++i) pmgTm_[i]->Draw();

    if (pmgRf_->IsCapOnlyCtrl())
    {
      //キャラ描画
      for (s32 i = 0; i < DBSIDE; ++i)
      {
        for (s32 i2 = 0; i2 < 1; i2++) pmgCh_[i][i2]->DoDraw();
      }
    }
    else
    {
      //キャラ描画
      for (s32 i = 0; i < DBSIDE; ++i)
      {
        for (s32 i2 = 0; i2 < DBMEMBER_ALL; i2++) pmgCh_[i][i2]->DoDraw();
      }
    }

    //ボール描画
    if (pmgRf_->IsFinTuto() == FALSE)  pmgBa_->DoDraw();//チュート終了時に消す

    //小物オブジェ描画
    for (s32 i = 0; i < DBETCOBJ; ++i) pmgEtc_[i]->Draw();
  }

  //描画★★
  void TMgGameObj::Draw()
  {
    switch (gotype_)
    {
    case goPrev: Draw_Prev(); break;
    case goTutoRetro: Draw_Tuto(); break;
    case goTutoSmart: Draw_Tuto(); break;
	case goTutoAuto: Draw_Tuto(); break;
    default: Draw_Shiai(); break;
    }
  }
}
