#include "dbChAction.h"
#include "dbGame.h"

namespace db
{
  //モーションカウンタ進める////////////////////////////////

  //コンストラクタ
  TChCount::TChCount(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo)
    : TChHaveCommon(pGame, pCommon, sideNo, charNo)
  {

  }

  TChCount::~TChCount()
  {
  }

  //キャッチカウンタ増やす＆キャッチ時間を超えているか
  BOOL TChCount::IncCatchFrm()
  {
    //const s32 CATCHEDDIV = 4;
    s32 tCaFrm = 0;
    BOOL res = FALSE;

    //キャッチカウンタ増やす
    ++st_.pstMyCh_->Catch_c;

    //対敵シュートボール
    if ((pmgSG_->stBa_.Motion == bmShoot)
      && (pmgSG_->stBa_.PichTNo != st_.mysideNo_))
    {
      tCaFrm = pmgEO_->mgDt_.dtSet_.GetDtCatch(setCaMotionFrm);//固定テスト★
    }
    else//シュート以外
    {
      const s32 PACAFRM = 8;
      tCaFrm = PACAFRM;
    }

    res = (st_.pstMyCh_->Catch_c > tCaFrm);

    if (res)  st_.pstMyCh_->Catch_c = NGNUM;

    return res;
  }
  //ジャンプ
  void TChCount::Jumping()
  {
    s32 dXSign;
    s32 dZSign;

    pCommon_->SetMtype(dbmtJUp);
    pCommon_->SESet(seJump);

    //Y初速
    st_.pstMyCh_->Zahyou.dY = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
      ? pmgEO_->mgDt_.dtSet_.GetDtJump(setDashJump_vY0)
      : pmgEO_->mgDt_.dtSet_.GetDtJump(setJump_vY0);

    //頂点タイミング
    st_.pstMyCh_->TopTiming = (st_.pstMyCh_->Zahyou.dY / pmgEO_->mgDt_.dtSet_.GetDtGrv(setGRV_Y));//pmgEO_->mgDt_.dtOption_.GetDt(rkopGRV_Y, tGrvLv));

    //斜めジャンプは内野のみ
    if ((IsCtrl() == FALSE)
      && (IsDashman() == FALSE)
      && st_.pstMyCh_->VJump_f)
    {
      st_.pstMyCh_->Zahyou.dX = 0;
      st_.pstMyCh_->Zahyou.dZ = 0;
    }
    else
    {
      //tJType = 0;
      dXSign = 0;
      dZSign = 0;

      //攻撃時は現在の向きに依存
      //ＤＪの時はダッシュ方向に依存
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
      {
        //enCrsType longJpCrs = base::cNone;


        switch (st_.pstMyCh_->Zahyou.DsMuki)
        {
        case mL:
          dXSign = -1;
          break;
        case mR:
          dXSign = +1;
          break;
        default:
          break;
        }

        BOOL longJp_f = FALSE;

        //Ｒ押しでロングジャンプ(IOSでは加速ダッシュのままジャンプでロングジャンプ)
        if (IsCOM() == FALSE)
        {
          longJp_f = (DashAccBtn_f() || st_.pstMyCh_->LJump_f);
        }
        else
        {
          //適当に歩数で１／２
          if (IsCtrl() && (st_.pstMyCh_->NextAuto.AutoType != dbatCPUJumpBall))
          {
            longJp_f = ((st_.pstMyCh_->Step_c % 2) == 0);
          }
        }


        s32 jpx = longJp_f
          ? RankSpeed(rkLDSJPX)
          : pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashJpX);

        st_.pstMyCh_->Zahyou.dX = (dXSign * jpx);
        st_.pstMyCh_->Zahyou.dZ = 0;

        //斜めジャンプ
        if ((IsCOM() == FALSE) && (IsCtrl()))//捜査権チェックしないとダッシュマンまで斜めジャンプ
        {
          if (MyPad()->IsJumpMv(dxU))//斜めジャンプ入力
          {
            dZSign = +1;
          }
          else if (MyPad()->IsJumpMv(dxD))//斜めジャンプ入力
          {
            dZSign = -1;
          }
        }

        st_.pstMyCh_->Zahyou.dX = dXSign * jpx;
        st_.pstMyCh_->Zahyou.dZ = dZSign * pCommon_->GetNWkJpZ();

      }
      else
      {
        //斜めジャンプ（リターン中は斜めジャンプきかないように）
        if ((IsCOM() == FALSE)
          && (IsFreeMotion(TRUE) || st_.pstMyCh_->FreeAct_f))
        {
          //if ((st_.posNo_ != (s32)dbpoO2) && (st_.posNo_ != (s32)dbpoO3))
          //{
          //  if (MyPad()->IsCrs2(base::cU)) { dZSign = +1; }
          //  else if (MyPad()->IsCrs2(base::cD)) { dZSign = -1; }
          //}

          //if (st_.posNo_ != (s32)dbpoO4)
          //{
          //  if (MyPad()->IsCrs2(base::cL)) { dXSign = -1; }
          //  else if (MyPad()->IsCrs2(base::cR)) { dXSign = +1; }
          //}
          if ((st_.posNo_ != (s32)dbpoO2) && MyPad()->IsJumpMv(dxU))//斜めジャンプ入力
          {
            dZSign = +1;
          }
          else if ((st_.posNo_ != (s32)dbpoO3) && MyPad()->IsJumpMv(dxD))//斜めジャンプ入力
          {
            dZSign = -1;
          }

          if (((st_.posNo_ != (s32)dbpoO4) || (st_.mysideNo_ != SIDE1)) && MyPad()->IsJumpMv(dxL))//斜めジャンプ入力
          {
            dXSign = -1;
          }
          else if (((st_.posNo_ != (s32)dbpoO4) || (st_.mysideNo_ != SIDE0)) && MyPad()->IsJumpMv(dxR))//斜めジャンプ入力
          {
            dXSign = +1;
          }
        }
        else if (st_.pstMyCh_->Auto.AutoType == dbatReturn)
        {
          if (st_.pstMyCh_->Auto.AMuki == maL)
          {
            dXSign = -1;
          }
          else if (st_.pstMyCh_->Auto.AMuki == maR)
          {
            dXSign = +1;
          }

          if (st_.pstMyCh_->Auto.AMukiZ == mzaF)
          {
            dZSign = -1;
          }
          else if (st_.pstMyCh_->Auto.AMukiZ == mzaB)
          {
            dZSign = +1;
          }
        }

        //斜めジャンプ
        if ((dXSign != 0) && (dZSign != 0))
        {
          st_.pstMyCh_->Zahyou.dX = dXSign * pCommon_->GetNWkJpX();
          st_.pstMyCh_->Zahyou.dZ = dZSign * pCommon_->GetNWkJpZ();
        }
        else
        {
          st_.pstMyCh_->Zahyou.dX = dXSign * pCommon_->GetWkJpX();
          st_.pstMyCh_->Zahyou.dZ = dZSign * pCommon_->GetWkJpZ();
        }

        if ((st_.posNo_ == (s32)dbpoO2) || (st_.posNo_ == (s32)dbpoO3))
        {
          st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(st_.pstMyCh_->Zahyou.dZ, JPGAIYAPER);
        }
      }

      ////パスタゲがジャンプしたら操作権をカバーマンに移す
      //if ((st_.pmgMyTm_->st_.pstMyTm_->CtrlNo == pmgSG_->stBa_.PaTgPNo)
      //  && (pmgSG_->stBa_.Motion == bmPass)
      //  && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
      //  && (pmgSG_->stBa_.PaTgPNo <= (s32)dbpoI3))
      //{
      //  //★イマイチな気がする//TeamObj内でいろいろ片付けた方がよい
      //  st_.pmgMyTm_->SetCtrl(st_.pmgMyTm_->st_.pstMyTm_->CvrNo);
      //  st_.pmgMyTm_->st_.pmgMyCh_[st_.pmgMyTm_->st_.pstMyTm_->CtrlNo)->st_.pstMyCh_->Nomove_f = TRUE;
      //  //新しいカバーマン
      //  st_.pmgMyTm_->SeekCover(st_.pmgMyTm_->st_.pstMyTm_->CvrNo, pmgSG_->stBa_.PichPNo, pmgSG_->stBa_.PaTgPNo, TRUE);
      //}
    }
  }
  //現モーション終了
  void TChCount::MotionEnd()
  {
    switch (st_.pstMyCh_->Motion.Mtype)
    {
      //ジャンプしゃがみ
    case dbmtJCr:
      Jumping();//ジャンプ
      break;

      //転がり
    case dbmtRoF:
    case dbmtRoB:
      //転がりカウンタで終了を決める
      break;

      //ダウンダメージ
    case dbmtDnHF:
      pCommon_->SetMtype(dbmtDnF);
      break;
      //ダウンダメージ
    case dbmtDnHB:
      pCommon_->SetMtype(dbmtDnB);
      break;

      //屈み
    case dbmtKG:
      if (Kagami_f())
      {
        st_.pstMyCh_->Kagami_c--;
        pCommon_->SetMtypeReset(dbmtKG);
      }
      else
      {
        pCommon_->SetMtype(dbmtSt);
      }
      break;


      //シュート（ジャンプシュートはどのみち落下まで何も出来ない）
    case dbmtSh:
      //終了カウンタ回す
      if (lib_num::AprTo0(&st_.pstMyCh_->ShEdWait_c))
      {
        pCommon_->SetMtypeReset(dbmtSt);
      }
      break;


      //パス
    case dbmtPa:
    {
      //ボールダッシュマン
      if (pCommon_->IsBMDashOK()
        && (st_.pmgMyTm_->st_.pstMyTm_->DashmanNoBM == st_.posNo_))//予約状態でもある
      {
        pCommon_->SetMtype(dbmtDs);
      }
      else
      {
        pCommon_->SetMtypeReset(dbmtSt);
      }
    }
    break;

    //キャッチ
    case dbmtCa:
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
      {
        pCommon_->SetMtype(dbmtDs);
      }
      else
      {
        pCommon_->SetMtype(dbmtSt);
      }
      
      break;

      //キャッチモーション
    case dbmtCM:
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
      {
        pCommon_->SetMtype(dbmtDs);
      }
      else
      {
        pCommon_->SetMtype(dbmtSt);
      }
      break;

      //その他
    default:
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
      {
        pCommon_->SetMtype(dbmtJDn);
      }
      else
      {
        pCommon_->SetMtypeReset(dbmtSt);
      }
      break;
    }
  }
  //次のフレーム
  void TChCount::NextFrame()
  {
    //ラスコマ
    if (IsLastKoma())//(NowKoma(bmc_Last_f != 0)
    {
      MotionEnd();
    }
    else//次コマ
    {
      ++st_.pstMyCh_->Anime.FrameNo;
      pCommon_->SetFrameData(FALSE);
    }
  }
  //モーションを進める
  void TChCount::IncMotion(BOOL goto_f)//goto_f:強制的に進める
  {
    //アニメーションカウンタ増加
    ++st_.pstMyCh_->Anime.Ani_c;

    //フレーム終了の時間がきましたもしくは強制次フレーム
    if ((st_.pstMyCh_->Anime.Ani_c >= NowKoma(bmc_DefFrm))
      || (goto_f))
    {

      switch (NowKoma(bmc_loopSt))
      {
        //ループ終了
      case (s32)bmlp_Ed:

        //カウンタ
        --st_.pstMyCh_->Anime.Loop_c;

        //ループ終了
        if (st_.pstMyCh_->Anime.Loop_c == 0)
        {
          NextFrame();
        }
        else//ループ中
        {
          //ループ始点まで戻る
          st_.pstMyCh_->Anime.FrameNo = st_.pstMyCh_->Anime.LoopStNo;
          pCommon_->SetFrameData(TRUE);

          if (st_.pstMyCh_->Anime.Loop_c <= NGNUM)
          {
            st_.pstMyCh_->Anime.Loop_c = 0;
            //st_.pstMyCh_->Motion.M_c = 0;
            //攻撃モーションで無い無限ループはM_cを初期化してみる
            //ジャンプ必殺のタイミングをM_cで取るのでやっぱり初期化しない
            //空中以外は初期化してみる
            //下の同コマループはいいかな
          }
        }
        break;
        //同コマループ
      case (s32)bmlp_StEd:
        //カウンタ
        --st_.pstMyCh_->Anime.Loop_c;

        //ループ終了
        if (st_.pstMyCh_->Anime.Loop_c == 0)
        {
          NextFrame();
        }
        else if (st_.pstMyCh_->Anime.Loop_c <= NGNUM)
        {
          st_.pstMyCh_->Anime.Loop_c = 0;
        }
        break;

        //非ループ
      default:
        NextFrame();
        break;
      }
    }
  }

  void TChCount::LongKeepInc()
  {
    const s32 LONGKEEPTIME = 300;
    if (IsBall())
    {
      if (lib_num::AprToDest(&st_.pstMyCh_->LongKeep_c, LONGKEEPTIME))
      {
        st_.pstMyCh_->LongKeep_c = 0;
        //pCommon_->SetSerifu(srfLongKeep);
      }
    }
  }

  void TChCount::PassWaitInc()
  {
    const s32 PASSWAITTIME = 60;

    st_.pstMyCh_->PassWait_c++;
    if (st_.posNo_ == (s32)dbpoO4)
    {
      if (st_.pstMyCh_->PassWait_c == PASSWAITTIME * 3)
      {
        //pCommon_->SetSerifu(srfPassWait);
      }
    }
    else
    {
      if (st_.pstMyCh_->PassWait_c == PASSWAITTIME)
      {
        //pCommon_->SetSerifu(srfPassWait);
      }
    }

  }

  //モーションカウンタ増加
  void TChCount::UpDate()
  {
    //    const s32 SHTAGCOUNTMAX = 60;

    //ボールとの距離とっておく
    pCommon_->GetBallDist();

    //モーションカウンタ増やす
    ++st_.pstMyCh_->Motion.M_c;
    //強奪しましたカウンタ減らす
    //    lib_num::AprTo0(&st_.pstMyCh_->Steal_c);
    //ファンブル終了カウンタ減らす
    lib_num::AprTo0(&st_.pstMyCh_->Fmbl_c);
    //ミラーシュート受付カウンタ減らす
    if (lib_num::AprTo0(&st_.pstMyCh_->MirShotLim_c))
    {
      st_.pstMyCh_->MirShot_c = 0;
    }

    //キャッチダメージ顔
    lib_num::AprTo0(&st_.pstMyCh_->CaDamAfter_c);
    

    //バイオリズムカウンタを増やす
    ++st_.pstMyCh_->Biorhythm_c;

    //シュートにかかわるカウンタ系処理

    //ボールとキャラが重なっていた過去を消すフラグ
    //くっつき系などの不具合対応　15/06/14
    BOOL overlapCancel_f = FALSE;

    if (st_.pstBa_->Motion == bmShoot)
    {//シュート中は加算＆減算
        //連続ヒットカウント：減算　０で再ヒット
        st_.pstMyCh_->hitMTime_cd = st_.pstMyCh_->hitMTime_cd - 1;
        if (st_.pstMyCh_->hitMTime_cd < 0) st_.pstMyCh_->hitMTime_cd = 0;

        //1Fr前はダウンで、今はダウンじゃない場合、球とキャラの重なりフラグを消す
        if ((st_.pstMyCh_->ctMotionPast == dbmtDRv) && (st_.pstMyCh_->Motion.Mtype != dbmtDRv)) overlapCancel_f = TRUE;

    }
    else
    {//シュート外であれば０（初期化にもなる）
        st_.pstMyCh_->hitMTime_cd = 0;
        st_.pstMyCh_->combo_c = 0;
        st_.pstMyCh_->combo_f = FALSE;
        st_.pstMyCh_->comboMax = 0;
        overlapCancel_f = TRUE;
    }

    //現状のモーションタイプを１Ｆｒ前用データに移す
    st_.pstMyCh_->ctMotionPast = st_.pstMyCh_->Motion.Mtype;

    //ボールとキャラが重なっていた過去を消す
    if (overlapCancel_f == TRUE)
    {
        st_.pstMyCh_->stk1StNow_f = FALSE;
        st_.pstMyCh_->stk2StNow_f = FALSE;
        st_.pstMyCh_->stk1StPst_f = FALSE;
        st_.pstMyCh_->stk2StPst_f = FALSE;
    }


    //ダウンから起き上がったら無敵を消す・・・なぜか標的にならないバグを回避
    if (st_.pstMyCh_->Motion.Mtype == dbmtDRv) 
        st_.pstMyCh_->Motion.SubMFlags(dbmfMTK);


    //if (MyPad() != NULL)
    //{
    //  if (MyPad()->IsBtn2(base::bB))
    //  {
    //    ++st_.pstMyCh_->Block_c;
    //  }
    //  else
    //  {
    //    st_.pstMyCh_->Block_c = 0;
    //  }
    //}
    //else
    //{
    //  st_.pstMyCh_->Block_c = 0;
    //}

    ////ナイスキャッチ＆アリウープカウンター減少（シュートモーション以外）
    //if ((st_.pstMyCh_->Motion.Mtype != dbmtSh) && (st_.pstMyCh_->Motion.Mtype != dbmtJSh))
    //{
    //  //lib_num::AprTo0(&st_.pstMyCh_->NiceCounter_c);
    //  lib_num::AprTo0(&st_.pstMyCh_->Alleyoop_c);
    //}

    lib_num::AprTo0(&st_.pstMyCh_->DashmanPaOK_c);

    //空中に居ますカウンタ

    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
    {
      ++st_.pstMyCh_->Air_c;
      //if ((IsBall() == FALSE) || (st_.pstMyCh_->Motion.IsMFlags(dbmfAct) == FALSE))
      //{
      //  ++st_.pstMyCh_->ShAir_c;
      //}
      if ((st_.pstMyCh_->Motion.Mtype != dbmtJSh)
        && (st_.pstMyCh_->Motion.Mtype != dbmtJCr))
      {
        ++st_.pstMyCh_->ShAir_c;
      }
    }
    //else//地上ならアリウープはない
    //{
    //  st_.pstMyCh_->Alleyoop_c = 0;
    //}
    //泥ステップ
    if (st_.pstMyCh_->Motion.Mtype == dbmtDs)
    {
      ++st_.pstMyCh_->MadStep_c;
    }

    //シュートタゲ点滅カウンタ(天使の時は点滅しない)
    if (ShTag_f() && (st_.pstMyCh_->Motion.Mtype != dbmtANG))
    {
      ++st_.pstMyCh_->ShTg_c;
      //lib_num::IncLoop(st_.pstMyCh_->ShTg_c, 0, SHTAGCOUNTMAX);//カウンタループ
    }
    else if (PaTag_f() && (st_.pstMyCh_->Motion.Mtype != dbmtANG))
    {
      if (st_.pstMyCh_->ShTg_c < 20) st_.pstMyCh_->ShTg_c = 20;
      ++st_.pstMyCh_->ShTg_c;//st_.pstMyCh_->ShTg_c = 10;
      //lib_num::IncLoop(st_.pstMyCh_->ShTg_c, 0, SHTAGCOUNTMAX);//カウンタループ
    }
    else
    {
      st_.pstMyCh_->ShTg_c = NGNUM;//点滅なし
    }

    //次キャッチまでの時間のカウント
    lib_num::AprTo0(&st_.pstMyCh_->CatchW_c);


    //各モーションごとの処理
    switch (st_.pstMyCh_->Motion.Mtype)
    {
      //立ち
    case dbmtSt:
      //パス待ち
      if (PassWait_f())
      {
        pCommon_->SetMtype(dbmtPW);//パス待ち
      }
      else
      {
        if (IsBall())//ボール持ち時は息しない
        {
          st_.pstMyCh_->Breath_c = 0;
        }
        else if (lib_num::UpToR(&st_.pstMyCh_->Breath_c, RankSpeed(rkIkiItv)))
        {
          st_.pstMyCh_->Breath_c = 0;
          pCommon_->SetMtype(dbmtBreath);
        }
      }
      //進める
      IncMotion(FALSE);

      //長持ち
      LongKeepInc();

      break;

      //パス待ち
    case dbmtPW:
      //パス待ちじゃなくなったとき
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfPW)
        && (PassWait_f() == FALSE))
      {
        pCommon_->SetMtype(dbmtSt);
      }
      //進める
      IncMotion(FALSE);

      PassWaitInc();//パス待ち
      break;

      //歩き
    case dbmtWk:
    case dbmtPWWk:
      //歩きクリア
      IncTutoAct_c(sta_WalkAct);

      if (st_.pstMyCh_->Motion.IsMFlags(dbmfPW))
      {
        if (PassWait_f() == FALSE) { pCommon_->SetMtypeReset(dbmtWk); }
      }
      else
      {
        if (PassWait_f() == TRUE) { pCommon_->SetMtypeReset(dbmtPWWk); }
      }
      //進める
      IncMotion(FALSE);


      if (PassWait_f())
      {
        PassWaitInc();//パス待ち
      }
      else
      {
        LongKeepInc();//長持ち
      }
      break;


      //ダッシュ
    case dbmtDs:
    case dbmtPWDs:

      //ダッシュクリア
      IncTutoAct_c(sta_DashAct);

      if (st_.pstMyCh_->Motion.IsMFlags(dbmfPW))
      {
        if (PassWait_f() == FALSE) { pCommon_->SetMtypeReset(dbmtDs); }
      }
      else
      {
        if (PassWait_f() == TRUE) { pCommon_->SetMtypeReset(dbmtPWDs); }
      }


      //進める
      IncMotion(FALSE);
      break;

      //しゃがみ
    case dbmtCr:
      if (st_.pstMyCh_->JpCr_c == 0)
      {
        //進める
        IncMotion(FALSE);
      }
      break;


      //ジャンプ
    case dbmtJUp:
      //進める
      IncMotion(FALSE);

      //下降
      if (st_.pstMyCh_->Zahyou.dY < 0)
      {
        pCommon_->SetMtype(dbmtJDn);
      }

      break;

      //ふっとび
    case dbmtFlF:
    case dbmtFlB:
      //進める
      IncMotion(FALSE);
      break;

      //ダウンダメージ
    case dbmtDnHF:
    case dbmtDnHB:
      //進める
      IncMotion(FALSE);
      //ダウンカウンタすすめる
      if (lib_num::UpToR(&st_.pstMyCh_->Down_c, RankHP(rkRevFrm)))
      {
        //pCommon_->SetMtype(dbmtDRv);//起き上がりはしない
      }

      break;

      //ダウン
    case dbmtDnF:
    case dbmtDnB:
      //進める
      IncMotion(FALSE);

      //ダウンカウンタすすめる
      if (lib_num::UpToR(&st_.pstMyCh_->Down_c, RankHP(rkRevFrm)))
      {
        pCommon_->SetMtype(dbmtDRv);
      }

      break;

      //転がり
    case dbmtRoF:
    case dbmtRoB:
      if (lib_num::AprTo0(&st_.pstMyCh_->Roll_c))
      {
        //死亡処理
        if ((st_.pstMyCh_->HP <= 0) && (st_.pmgEnTm_->IsAllDead() == FALSE))
        {
          st_.pstMyCh_->ANGEL_f = TRUE;
          st_.pmgMyTm_->CheckChangePos();//★★
          //pCommon_->SetMtype(dbmtANG);

        }
        else
        {
          if (st_.pstMyCh_->Motion.Mtype == dbmtRoF)
          {
            pCommon_->SetMtype(dbmtDnB);
          }
          else
          {
            pCommon_->SetMtype(dbmtDnF);
          }
          st_.pstMyCh_->Zahyou.dY = 0;
        }
      }

      //進める
      IncMotion(FALSE);

      break;

      //スリップ中
    case dbmtSl:
      //スリップ中は進めない
      //if (st_.pstMyCh_->Zahyou.Fric_c == 0) { IncMotion(FALSE); }
      if (st_.pstMyCh_->Zahyou.dX == 0) { IncMotion(FALSE); }
      //IncMotion(FALSE);
      break;

      //キャッチング中
    case dbmtCM:
    case dbmtFB:
      //スリップ中は進めない
      if (st_.pstMyCh_->Zahyou.Fric_c == 0) { IncMotion(FALSE); }
      break;

      //シュート
    case dbmtSh:
    case dbmtJSh:
      if (lib_num::AprTo0(&st_.pstMyCh_->ShStWait_c))
      {
        //進める
        IncMotion(FALSE);
      }
      break;

      //パス
    case dbmtPa:
    case dbmtJPa:
      ////トスパス解除
      //if (IsSelfCtrl() && (pCommon_->MyPad()->IsPass2() == FALSE))
      //{
      //  st_.pstMyCh_->QuickPass_f = FALSE;
      //}
      //指が離れたらクイックパス
      if (IsSelfCtrl() && (pCommon_->MyPad()->IsPass2() == FALSE))
      {
        st_.pstMyCh_->TossPass_f = FALSE;
      }

      if (lib_num::AprTo0(&st_.pstMyCh_->PaStWait_c))
      {
        //進める
        IncMotion(FALSE);
      }


      break;

      //キャッチ
    case dbmtCa:
    case dbmtJCa:
      if (st_.pstMyCh_->Catch_c != NGNUM)
      {
        //進める
        //IncMotion(IncCatchFrm());
        if (IncCatchFrm())
        {
          NextFrame();
        }
      }
      else
      {
        //進める
        IncMotion(FALSE);
      }
      break;

      //天使
    case dbmtANG:
      //if (st_.pstMyCh_->Ueda_c > 0)
      //{
      //  if ((lib_num::UpToR(&st_.pstMyCh_->Holy_c, pmgEO_->mgDt_.dtSet_.GetDt(setetRespawnTime))
      //    || (st_.pmgMyTm_->st_.pstMyTm_->RType != rtPLAY)))//時間切れなどで試合終了したら即復活
      //  {
      //    pCommon_->RespawnInit();//復活
      //  }
      //}
      //else
      //{
      //  st_.pstMyCh_->Live_f = FALSE;
      //}
      st_.pstMyCh_->Live_f = FALSE;
      break;
    default:
      //進める
      IncMotion(FALSE);
      break;
    }
  }

}
