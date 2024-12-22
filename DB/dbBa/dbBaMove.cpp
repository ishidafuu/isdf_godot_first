#include "dbBaMove.h"
#include "dbGame.h"

namespace db
{
  //移動////////////////////////////////

  //コンストラクタ
  TBaMove::TBaMove(TGame* pGame, TBaCommon* pCommon)
    : TBaHaveCommon(pGame, pCommon)
  {
  }

  TBaMove::~TBaMove()
  {
  }

  //タイマー
  void TBaMove::TimerCrtPosition()
  {
    BOOL RevTimerCrtSide_f = FALSE;
    s32 tX;
    //タイマー用
    if (st_.pstBa_->LandX <= DBCRT_CL)
    {
      st_.pstBa_->TimerCrtSide = 0;
      tX = st_.pstBa_->LandX;
    }
    else
    {
      st_.pstBa_->TimerCrtSide = 1;
      tX = DBCRT_CL - (st_.pstBa_->LandX - DBCRT_CL);//左コートとして扱う
    }

    if (st_.pstBa_->LandZ <= DBCRT_FL)
    {
      st_.pstBa_->TimerCrtPos = (s32)dbpoO3;
      RevTimerCrtSide_f = TRUE;
    }
    else if (st_.pstBa_->LandZ >= DBCRT_BL)
    {
      st_.pstBa_->TimerCrtPos = (s32)dbpoO2;
      RevTimerCrtSide_f = TRUE;
    }
    else
    {
      if (tX < (DBCRT_SL + ((st_.pstBa_->LandZ - DBCRT_FL) * 10 / DBCRT_SLR)))
      {
        st_.pstBa_->TimerCrtPos = (s32)dbpoO4;
        RevTimerCrtSide_f = TRUE;
      }
      else
      {
        st_.pstBa_->TimerCrtPos = (s32)dbpoI0;
      }
    }

    if (RevTimerCrtSide_f)
    {
      st_.pstBa_->TimerCrtSide = (st_.pstBa_->TimerCrtSide == 0)
        ? 1
        : 0;
    };

    //タイマーリセット
    pCommon_->ResetTimer(st_.pstBa_->TimerCrtSide, FALSE);
  }
  //壁減速
  void TBaMove::WallAbs(BOOL ZWall_f)
  {
    //必殺シュートおよび、エンドライン壁へのシュートは弾まない
    BOOL smallBound_f = ((st_.pstBa_->Motion == bmShoot)
      && ((ZWall_f == FALSE) || (st_.pstSh_->idata.sp_f)));

    s32 dxper = (ZWall_f)
      ? +pmgEO_->mgDt_.dtSet_.GetDtBall(setBaZWallAbsdX)
      : -pmgEO_->mgDt_.dtSet_.GetDtBall(setBaZWallAbsdX);

    s32 dzper = (ZWall_f)
      ? -pmgEO_->mgDt_.dtSet_.GetDtBall(setBaXWallAbsdZ)
      : +pmgEO_->mgDt_.dtSet_.GetDtBall(setBaXWallAbsdZ);

    st_.pstBa_->Zahyou.dX = lib_num::Percent(st_.pstBa_->Zahyou.dX, dxper);
    st_.pstBa_->Zahyou.dZ = lib_num::Percent(st_.pstBa_->Zahyou.dZ, dzper);
  }
  //タゲが居ないかどうか
  BOOL TBaMove::IsCheckNoneTag()
  {
    return (st_.pstBa_->ShTgPNo == NGNUM);
  }
  //距離再検索
  TShotDt TBaMove::GetTagDist(enGetTagType tagtype, BOOL Return_f)
  {
    TShotDt ShDt;

    TZahyouRec tgZahyou = (Return_f)
      ? st_.pmgTm_[st_.pstBa_->PichTNo]->st_.pmgMyCh_[st_.pstBa_->PichPNo]->st_.pstMyCh_->Zahyou
      : st_.pmgTm_[st_.pstBa_->ShTgTNo]->st_.pmgMyCh_[st_.pstBa_->ShTgPNo]->st_.pstMyCh_->Zahyou;

    ShDt.tdx = abs(tgZahyou.X - st_.pstBa_->Zahyou.X);
    ShDt.tdz = abs(tgZahyou.Z - st_.pstBa_->Zahyou.Z);
    ShDt.tdy = 0;
    const s32 TAG16 = 16 * XYMAG;
    s32 tgy = tgZahyou.Y;
    switch (tagtype)
    {
    case gtTgY:

      ShDt.tdy = abs(tgy - st_.pstBa_->Zahyou.Y);
      break;
    case gtTg16:
      tgy += TAG16;
      ShDt.tdy = abs(tgy - st_.pstBa_->Zahyou.Y);
      break;
    case gtBase:
      tgy = 0;
      ShDt.tdy = abs(tgy - st_.pstBa_->Zahyou.Y);
      break;
    default:
      ShDt.tdy = 0;
      break;
    }

    //符号
    ShDt.tdxs = (tgZahyou.X >= st_.pstBa_->Zahyou.X)
      ? +1
      : -1;
    ShDt.tdys = (tgy >= st_.pstBa_->Zahyou.Y)
      ? 0//上昇しないように
      : -1;
    ShDt.tdzs = (tgZahyou.Z >= st_.pstBa_->Zahyou.Z)
      ? +1
      : -1;

    return ShDt;
  }
  //シュートの角度
  void TBaMove::SAngle(TShotDt& ShDt, s32 spd)
  {
    SAngle(ShDt, spd, FALSE);//でふぉは鋭角禁止
  }
  //シュートの角度
  void TBaMove::SAngle(TShotDt& ShDt, s32 spd, BOOL acuteOK_f)
  {
    if (acuteOK_f)
    {
      //近すぎ補正のみ
      if (ShDt.tdx <= XYMAG) { ShDt.tdx = XYMAG; }
      if (ShDt.tdz <= XYMAG) { ShDt.tdz = XYMAG; }
    }
    else
    {
      //45度以上鋭角のシュートにはならないようにする
      if (st_.pstBa_->Zahyou.MukiZ == mzN)
      {
        //近すぎ補正
        if (ShDt.tdx <= XYMAG) { ShDt.tdx = XYMAG; }
        if (abs(ShDt.tdx) < abs(ShDt.tdy)) { ShDt.tdx = ShDt.tdy; }
      }
      else
      {
        //近すぎ補正
        if (ShDt.tdz <= XYMAG) { ShDt.tdz = XYMAG; }
      }
    }

    f32 d = lib_num::Hypot(ShDt.tdx, ShDt.tdy);//斜辺の長さ
    f32 j = lib_num::Hypot((s32)d, ShDt.tdz);

    if (j != 0)
    {
      f32 eZ = ((ShDt.tdz * spd) / j);
      f32 eX = ((ShDt.tdx * spd) / j);
      f32 eY = ((ShDt.tdy * spd) / j);
      ShDt.tdx = (s32)(eX);
      ShDt.tdy = (s32)(eY);
      ShDt.tdz = (s32)(eZ);
    }
    else
    {
      ShDt.tdx = spd;
      ShDt.tdy = 0;
      ShDt.tdz = 0;
    }
  }
  //ねらえる状況か
  BOOL TBaMove::IsTgOKMotion(s32 tgTNo, s32 tgNo)
  {
    if (tgTNo == NGNUM) return FALSE;

    BOOL res_f = FALSE;

    TMgChar* ptag_ = st_.pmgTm_[tgTNo]->st_.pmgMyCh_[tgNo];

    if ((ptag_->st_.pstMyCh_->Dead_f == FALSE)
      && (ptag_->st_.pstMyCh_->HP > 0)
      && (ptag_->st_.pstMyCh_->Motion.IsMFlags(dbmfANG) == FALSE))
    {
      if (ptag_->st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
      {
        res_f = (ptag_->st_.pstMyCh_->Zahyou.Y <= (pmgEO_->mgDt_.dtSet_.GetDtShot(setFlyTagHeight) * XYMAG));
      }
      else
      {
        res_f = TRUE;
      }
    }
    return res_f;
  }

  //壁減速
  void TBaMove::SetShootBound()
  {
    //横軸(絶対値)
    f32 hp = lib_num::Hypot(st_.pstBa_->Zahyou.dX, st_.pstBa_->Zahyou.dZ);

    if (hp == 0)
    {
      st_.pstBa_->Zahyou.dY = -st_.pstBa_->Zahyou.dY;
      return;
    }

    //縦軸（絶対値　でもそのまま使う）
    s32 tdY = abs(st_.pstBa_->Zahyou.dY);

    //角度比率dY/dX
    //最低dY
    if (((tdY / hp) * XYMAG) < pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundMindYpdX))
    {
      tdY = lib_num::Percent((s32)hp, pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundMindYpdX));
    }

    f32 j = lib_num::Hypot(tdY, (s32)hp);
    if (j == 0) j = hp;
    s32 jSp = (pmgEO_->mgDt_.dtRank_.GetRankDtLevel(rkBwSpWall, st_.pstSh_->idata.rank));
    f32 hpSp = ((hp * jSp) / j);

    st_.pstBa_->Zahyou.dY = (s32)((tdY * jSp) / j);
    st_.pstBa_->Zahyou.dX = (s32)((st_.pstBa_->Zahyou.dX * hpSp) / hp);
    st_.pstBa_->Zahyou.dZ = (s32)((st_.pstBa_->Zahyou.dZ * hpSp) / hp);
  }

  //座標移動
  void TBaMove::Moving()
  {

    //ヒットストップ★
    //if (st_.pstBa_->HitStop_c != 0) return;

    st_.pstBa_->CrvX = 0;
    st_.pstBa_->CrvY = 0;
    st_.pstBa_->CrvZ = 0;
    //st_.pstBa_->ShSt.Stop_f = FALSE;

    BOOL stop_f = FALSE;

    switch (st_.pstBa_->Motion)
    {
    case bmFree:
      pCommon_->ZeroXYZ();
      break;
    case bmHold:
      break;
    case bmPass:
      st_.pstBa_->Zahyou.dY -= st_.pstBa_->PaGrv;
      break;
    case bmBound:
      //st_.pstBa_->Zahyou.dY -= FCDiv(pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundGrv));
      st_.pstBa_->Zahyou.dY -= (pmgEO_->mgDt_.dtSet_.GetDtGrv(setBoundGrv));
      break;
    case bmReferee:
      stop_f = TRUE;
      //st_.pstBa_->ShSt.Stop_f = TRUE;
      //pCommon_->ZeroXYZ();//ストップ無くしてこれでよいか
      break;
    case bmShoot:

      //特殊軌道処理
      pbaShot_->UpDate();
      break;
    }

    if (st_.pstBa_->Motion != bmShoot)
    {
      //速すぎ防止
      st_.pstBa_->Zahyou.dX = lib_num::Between(st_.pstBa_->Zahyou.dX, -MAXSPD, MAXSPD);
      st_.pstBa_->Zahyou.dY = lib_num::Between(st_.pstBa_->Zahyou.dY, -MAXSPD, MAXSPD);
      st_.pstBa_->Zahyou.dZ = lib_num::Between(st_.pstBa_->Zahyou.dZ, -MAXSPD, MAXSPD);

      //止まりフラグたってたら動かさない
      if (stop_f == FALSE)
      {
        st_.pstBa_->Zahyou.X = st_.pstBa_->Zahyou.X + st_.pstBa_->Zahyou.dX + st_.pstBa_->CrvX;
        st_.pstBa_->Zahyou.Y = st_.pstBa_->Zahyou.Y + st_.pstBa_->Zahyou.dY + st_.pstBa_->CrvY;
        st_.pstBa_->Zahyou.Z = st_.pstBa_->Zahyou.Z + st_.pstBa_->Zahyou.dZ + st_.pstBa_->CrvZ;
      }
      //画像データの初期化
      const s32 ANM_NUM = 4;
      switch (st_.pstBa_->Motion)
      {
      case bmFree:
      case bmHold:
        st_.pstSh_->cmn.spdtNrmBallNum = 1;
        break;
      default:// bmPass,bmBound,bmReferee
        st_.pstSh_->cmn.spdtNrmBall_c = st_.pstSh_->cmn.spdtNrmBall_c + 1;
        if (st_.pstSh_->cmn.spdtNrmBall_c >= ANM_NUM)
        {
          st_.pstSh_->cmn.spdtNrmBallNum = st_.pstSh_->cmn.spdtNrmBallNum + 1;
          if (st_.pstSh_->cmn.spdtNrmBallNum >= 4) st_.pstSh_->cmn.spdtNrmBallNum = 0;
          st_.pstSh_->cmn.spdtNrmBall_c = 0;
        }
        break;
      }

      //外部読み込み用データ
      //画像番号：現在のパターン番号を参照して読み込む
      st_.pstSh_->cmn.spdtGrhNum = st_.pstSh_->cmn.spdtNrmBallNum;
      st_.pstSh_->cmn.spdtRevVer_f = FALSE;
      st_.pstSh_->cmn.spdtRevHor_f = FALSE;
      st_.pstSh_->cmn.spdtRevClr_f = FALSE;
      st_.pstSh_->cmn.spdtVnsBal_f = FALSE;//球消しフラグ
      st_.pstSh_->cmn.spdtVnsSdw_f = FALSE;//影消し
    }

    //床
    if (st_.pstBa_->Motion != bmHold)
    {
      if ((st_.pstBa_->Motion == bmShoot) && (st_.pstSh_->cmn.shotFinish_f))
      {
        pCommon_->SetMtype(bmBound);
        pCommon_->ZeroXYZ();
      }

      if (st_.pstBa_->Zahyou.Y < 0)
      {
        st_.pstBa_->Zahyou.Y = 0;
        st_.pstBa_->LandSet_f = FALSE;
        st_.pstBa_->JumpBall = jbNormal;
        if (st_.pstSh_->IsShotEffFlag(sef_changeGround_f) == FALSE)
        {
          //弾みすぎ防止
          const s32 LOWBOUNDDIV = 3;

          if (st_.pstBa_->Zahyou.dY < -pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundMaxdY))
          {
            st_.pstBa_->Zahyou.dY = -pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundMaxdY);
          }


          if (st_.pstBa_->Motion == bmShoot)
          {
            SetShootBound();
            pCommon_->SetMtype(bmBound);
            st_.pstBa_->ShootFirstBound_f = TRUE;//シュート着地１回目のバウンド

            //バウンド率
            st_.pstBa_->Zahyou.dY = lib_num::Percent(st_.pstBa_->Zahyou.dY, pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundDefPer));
          }
          else if (st_.pstBa_->Zahyou.dY < -pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundStopdY))
          {
            if (st_.pstBa_->Zahyou.dY < -pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundMaxdY))
            {
              st_.pstBa_->Zahyou.dY = -pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundMaxdY);
            }

            st_.pstBa_->Zahyou.dY = -(st_.pstBa_->Zahyou.dY + pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundAbsdY));
            st_.pstBa_->Zahyou.dX = lib_num::Percent(st_.pstBa_->Zahyou.dX, pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundAbsdX));
            st_.pstBa_->Zahyou.dZ = lib_num::Percent(st_.pstBa_->Zahyou.dZ, pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundAbsdZ));

            if (st_.pstBa_->LowBound_f)
            {
              st_.pstBa_->Zahyou.dY /= LOWBOUNDDIV;
              st_.pstBa_->LowBound_f = FALSE;
            }
            else
            {
              st_.pstBa_->Zahyou.dY = lib_num::Percent(st_.pstBa_->Zahyou.dY, pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundDefPer));
            }

            if (st_.pstBa_->Motion != bmBound)
            {
              pCommon_->SetMtype(bmBound);
            }
            else
            {
              //バウンド回数
              ++st_.pstBa_->Bound_c;
              st_.pstBa_->ShootFirstBound_f = FALSE;
            }

          }
          else
          {
            pCommon_->ZeroXYZ();
            if (st_.pstBa_->Motion != bmFree)
            {
              pCommon_->SetMtype(bmFree);
              //内野にボールが着いた時点で、
              //ボール権が移動
              //タイマーもリセット
              TimerCrtPosition();
            }
          }

          pCommon_->SESet(seBound);
        }

      }

      //強引な位置補正
      if ((st_.pstBa_->Zahyou.X > DBCRT_W)
        || (st_.pstBa_->Zahyou.X < 0))
      {
        st_.pstBa_->Zahyou.X = (st_.pstBa_->Zahyou.X > DBCRT_W)
          ? DBCRT_W
          : 0;
#ifdef __K_DEBUG_SHIAI__
        // X壁にあたった情報を一応ログに追記しておく
        kdebug::DebugSystem::GetInstance()->AddShootDebugLogError(kdebug::DEBUG_LOG_ERROR::DEBUG_ERROR_HIT_X_WALL);
#endif	// #ifdef __K_DEBUG_SHIAI__
        //Ｘ壁
        WallAbs(FALSE);
        if (st_.pstBa_->Zahyou.dY > 0)
        {
          st_.pstBa_->Zahyou.dY = 0;
        }
        pCommon_->SetMtype(bmBound);

        //避け成功
        st_.pmgRf_->SetTutoOK(sta_DodgeAct);
      }

      if ((st_.pstBa_->Zahyou.Z > DBCRT_WAL)
        || (st_.pstBa_->Zahyou.Z < 0))
      {
        st_.pstBa_->Zahyou.Z = (st_.pstBa_->Zahyou.Z > DBCRT_WAL)
          ? DBCRT_WAL
          : 0;
#ifdef __K_DEBUG_SHIAI__
        // Z壁にあたった情報を一応ログに追記しておく
        kdebug::DebugSystem::GetInstance()->AddShootDebugLogError(kdebug::DEBUG_LOG_ERROR::DEBUG_ERROR_HIT_Z_WALL);
#endif	// #ifdef __K_DEBUG_SHIAI__
        //Ｚ壁
        WallAbs(TRUE);
        pCommon_->SetMtype(bmBound);

        //避け成功
        st_.pmgRf_->SetTutoOK(sta_DodgeAct);
      }
    }

    //落下地点予測
    switch (st_.pstBa_->Motion)
    {
    case bmBound:
      //バウンドはモーション代わったときに即する（次の操作権を得るため）
      pCommon_->GetBoundLandPos((pmgEO_->mgDt_.dtSet_.GetDtGrv(setBoundGrv)));
      break;
    case bmShoot:
      pCommon_->GetShootLandPos();
      break;
    case bmPass:
      pCommon_->GetBoundLandPos(st_.pstBa_->PaGrv);
      break;
    default:
      //バウンド以外は現在位置を入れとく
      st_.pstBa_->Land_f = TRUE;
      st_.pstBa_->LandX = st_.pstBa_->Zahyou.X;
      st_.pstBa_->LandZ = st_.pstBa_->Zahyou.Z;
      break;
    }

    //        //座標保存しとく
    //        mid::midSetDbgNum(18, st_.pstBa_->Zahyou.X);
    //        mid::midSetDbgNum(19, st_.pstBa_->Zahyou.Z);

  }

  void TBaMove::CrtPosition()
  {
    BOOL RevTimerCrtSide_f = FALSE;
    s32 tX;

    //タイマー用
    if (st_.pstBa_->LandX <= DBCRT_CL)
    {
      st_.pstBa_->TimerCrtSide = 0;
      tX = st_.pstBa_->LandX;
    }
    else
    {
      st_.pstBa_->TimerCrtSide = 1;
      tX = DBCRT_CL - (st_.pstBa_->LandX - DBCRT_CL);//左コートとして扱う
    }

    if (st_.pstBa_->LandZ <= (DBCRT_FL))
    {
      st_.pstBa_->TimerCrtPos = (s32)dbpoO3;
      RevTimerCrtSide_f = TRUE;
    }
    else if (st_.pstBa_->LandZ >= (DBCRT_BL))
    {
      st_.pstBa_->TimerCrtPos = (s32)dbpoO2;
      RevTimerCrtSide_f = TRUE;
    }
    else
    {
      if (tX < (DBCRT_SL + ((st_.pstBa_->LandZ - DBCRT_FL) * 10 / DBCRT_SLR)))
      {
        st_.pstBa_->TimerCrtPos = (s32)dbpoO4;
        RevTimerCrtSide_f = TRUE;
      }
      else
      {
        st_.pstBa_->TimerCrtPos = (s32)dbpoI0;
      }
    }

    if (RevTimerCrtSide_f)
    {
      st_.pstBa_->TimerCrtSide = (st_.pstBa_->TimerCrtSide == 0)
        ? 1
        : 0;
    }
  }

  void TBaMove::DecTimer()//★
  {
    //プレビューではカウントしない
    if (pmgGO_->pmgRf_->IsPrev()) return;
    if (pmgEO_->stShiai_.IsTuto()) return;//チュートリアル
    if (st_.pstBa_->TimerSide == NGNUM) return;
    if (st_.pmgTm_[st_.pstBa_->TimerSide]->st_.pstMyTm_->CtrlNo == NGNUM) return;
    if (pmgSG_->stRf_.Demo_f) return;//デモは減らさない
    if (pmgSG_->stRf_.GameSet_f) return;//試合終了したら減らさない
    if (pmgGO_->pmgRf_->IsFinTuto()) return;
    //if (pmgSG_->stRf_.TutoEnd_f) return;//試合終了したら減らさない
#ifdef __K_DEBUG_SHIAI__
    // ここにリターンでボールの持ち時間リミットなし
    return;
#endif	// #ifdef __K_DEBUG_SHIAI__


    //シュートまでの時間用
    ++st_.pstBa_->Timer_c2;

    if (st_.pstBa_->Timer_c == 0) return;//無制限

    const s32 COMSHLIM = 30;//残り３０フレで強制シュート
    if (st_.pstBa_->Timer_c == COMSHLIM)
    {
      st_.pmgTm_[st_.pstBa_->TimerSide]->st_.pstMyTm_->COMOverTime_f = TRUE;
    }

    //st_.pstBa_->DrawTime = 0;

    //ボールタイムオーバー
    if (lib_num::AprTo0(&st_.pstBa_->Timer_c))
    {
      if (st_.pstBa_->TimerSide == 0)
      {
        st_.pmgRf_->SetMotion(dbrfJumpBallWait, mR);
      }
      else
      {
        st_.pmgRf_->SetMotion(dbrfJumpBallWait, mL);
      }
      //タイマーリセット
      pCommon_->ResetTimer(NGNUM, FALSE);

      st_.pmgRf_->RefereeMsg("オーバータイム");

    }
  }


  //ボール操作不能のフォロー
  void TBaMove::DecTimer3()//★
  {
    if (pmgSG_->stRf_.GameSet_f) return;//試合終了したら減らさない
    if (pmgGO_->pmgRf_->IsFinTuto()) return;
    //if (pmgSG_->stRf_.TutoEnd_f) return;//試合終了したら減らさない

    const s32 ONESECFRAME = 60;
    switch (st_.pstBa_->Motion)
    {

    case bmHold://持った瞬間にカウンタリセット
      st_.pstBa_->Timer_c3 = (11 * ONESECFRAME);
      break;
    default:
      //ボールタイムオーバー
      if (lib_num::AprTo0(&st_.pstBa_->Timer_c3))
      {
#ifdef __K_DEBUG_SHIAI__
        // 自動シュートモードであればボールが制御不能になったのをログに残しておく
        // ボールの状態変化で必殺シュート終了を見ているのでそれよりも前にログを書き出す
        //if (kdebug::DebugSystem::GetInstance()->IsAutoShootMode())
        {
          kdebug::DebugSystem::GetInstance()->AddShootDebugLogError(kdebug::DEBUG_LOG_ERROR::DEBUG_ERROR_REFEREE);
        }
#endif	// #ifdef __K_DEBUG_SHIAI__
        if (pmgGO_->pmgRf_->IsPrev())
        {
          pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev(FALSE);
        }
        else
        {
          if (st_.pstBa_->TimerSide == 0)
          {
            st_.pmgRf_->SetMotion(dbrfJumpBallWait, mR);
          }
          else if (st_.pstBa_->TimerSide == 1)
          {
            st_.pmgRf_->SetMotion(dbrfJumpBallWait, mL);
          }
          else
          {
            st_.pmgRf_->SetMotion(dbrfJumpBallWait, mN);
          }
        }

        //タイマーリセット
        pCommon_->ResetTimer(NGNUM, FALSE);
      }
      break;

    }

  }

  void TBaMove::DecTimerPrev()
  {
    if (pmgGO_->pmgRf_->IsPrev() == FALSE) return;

    switch (st_.pstBa_->Motion)
    {
    case bmHold://持った瞬間にカウンタリセット
      if (st_.pstBa_->HoldTNo == SIDE0)
      {
        st_.pstBa_->TimerAutoReturn_c = (30);
      }
      else
      {
        if (lib_num::AprTo0(&st_.pstBa_->TimerAutoReturn_c))
        {
          pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev(FALSE);
        }
      }
      break;
    case bmFree:
      if (lib_num::AprTo0(&st_.pstBa_->TimerAutoReturn_c))
      {
        pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev(FALSE);
      }
      break;
    }
  }
  void TBaMove::DecTimerTuto()
  {
    if (pmgEO_->stShiai_.IsTuto() == FALSE) return;
    if (pmgGO_->pmgRf_->IsFinTuto()) return;
    //if (pmgSG_->stRf_.TutoEnd_f) return;//試合終了したら減らさない
    //if (pmgGO_->pmgRf_->IsReturnNoSide())
    //{
    //  if (st_.pstBa_->Motion != bmReferee)
    //  {
    //    //最初だけ
    //    pmgGO_->pmgBa_->baCommon_.ResetRefPos(NGNUM, jbJpball, FALSE);
    //    pmgSG_->stRf_.Motion = dbrfStand;
    //  }
    //}
    //else
    if (pmgGO_->pmgRf_->IsReturnEnSide())
    {
      switch (st_.pstBa_->Motion)
      {
      case bmHold://持った瞬間にカウンタリセット
        if (st_.pstBa_->HoldTNo == SIDE1)
        {
          st_.pstBa_->TimerAutoReturn_c = (30);
        }
        else
        {
          if (lib_num::AprTo0(&st_.pstBa_->TimerAutoReturn_c))
          {
            pmgGO_->pmgBa_->baCommon_.ResetRefPos_Tuto(FALSE, SIDE1);
          }
        }
        break;
      case bmFree:
        if (lib_num::AprTo0(&st_.pstBa_->TimerAutoReturn_c))
        {
          pmgGO_->pmgBa_->baCommon_.ResetRefPos_Tuto(FALSE, SIDE1);
        }
        break;
      }
    }
    else if (pmgGO_->pmgRf_->IsReturnMySide())
    {
      switch (st_.pstBa_->Motion)
      {
      case bmHold://持った瞬間にカウンタリセット
        if (st_.pstBa_->HoldTNo == SIDE0)
        {
          st_.pstBa_->TimerAutoReturn_c = (30);
        }
        else
        {
          if (lib_num::AprTo0(&st_.pstBa_->TimerAutoReturn_c))
          {
            pmgGO_->pmgBa_->baCommon_.ResetRefPos_Tuto(FALSE, SIDE0);
          }
        }
        break;
      case bmFree:
        if (lib_num::AprTo0(&st_.pstBa_->TimerAutoReturn_c))
        {
          pmgGO_->pmgBa_->baCommon_.ResetRefPos_Tuto(FALSE, SIDE0);
        }
        break;
      }
    }

  }
  //更新
  void TBaMove::UpDate()
  {
    Moving();
    CrtPosition();
    DecTimer();
    DecTimer3();
    DecTimerPrev();
    DecTimerTuto();
  }


}
