#include "dbBaCommon.h"
#include "dbGame.h"
#include <set>
namespace db
{


  //共通処理////////////////////////////////

  //コンストラクタ
  TBaCommon::TBaCommon(TGame* pGame) : TBaGen(pGame)
  {
  }
  TBaCommon::~TBaCommon(void)
  {
  }


  //動き停止
  void TBaCommon::ZeroXYZ()
  {
    st_.pstBa_->Zahyou.dX = 0;
    st_.pstBa_->Zahyou.dY = 0;
    st_.pstBa_->Zahyou.dZ = 0;
  }

  //モーション切り替え
  void TBaCommon::SetMtype(enBallMotion tMtype)
  {
    st_.pstSh_->Init();

    //カウンタとかの初期化
    st_.pstBa_->HoldTNo = NGNUM;
    st_.pstBa_->HoldPNo = NGNUM;
    st_.pstBa_->RbwTNo = NGNUM;

    //着地計算まだよ
    st_.pstBa_->LandSet_f = FALSE;
    st_.pstBa_->LowBound_f = FALSE;
    st_.pstBa_->JumpBall = jbNormal;
    st_.pstBa_->ShootFirstBound_f = FALSE;

    switch (tMtype)
    {
    case bmFree:
      for (s32 i = 0; i < DBSIDE; ++i)
      {
        //バウンド以外から直接ＦＲＥＥになったときは探索
        if (st_.pstBa_->Motion != bmBound)
        {
          st_.pmgTm_[i]->SeekCtrl(TRUE);
        }
      }
      break;
    case bmHold:

      st_.pstBa_->Start_f = TRUE;
      switch (st_.pstBa_->Motion)
      {
      case bmShoot:
      case bmFree:
      case bmBound:
      case bmPass:
        break;
      }
      break;
    case bmShoot:

      break;
    case bmPass:

      break;
    case bmBound:

      //ワンバウンド目
      if (st_.pstBa_->Motion != tMtype)
      {
        //着地地点
        st_.pstBa_->Motion = bmBound;//バウンドにしておかないといけない
        GetBoundLandPos((pmgEO_->mgDt_.dtSet_.GetDtGrv(setBoundGrv)));

        //バウンド回数リセット
        st_.pstBa_->Bound_c = 0;

        //こうすると近い人に強引にカーソルが行く
        for (s32 i = 0; i < DBSIDE; ++i)
        {
          st_.pmgTm_[i]->SeekCtrl(TRUE, TRUE);
        }
      }

      st_.pstBa_->counter_f = FALSE;
      break;
    case bmReferee:
      for (s32 i = 0; i < DBSIDE; ++i)
      {
        st_.pmgTm_[i]->SeekCtrl(TRUE);
      }

      st_.pstBa_->counter_f = FALSE;
      break;
    }

#ifdef __K_DEBUG_SHIAI__
    // シュート状態でなくなった時
    // かかったフレーム数書き込み
    if (tMtype != bmShoot)
    {
      kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
      if (pDs && pDs->IsAutoShootMode())
      {
        // シュートが終わった時に自動シュートモードであれば準備モードに戻す
        s32 step = pDs->GetAutoShootStep();
        if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_SHOOT)
        {
          pDs->SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_SETUP);
        }
      }
      kdebug::DebugSystem::GetInstance()->AddShootDebugLogFrameCnt();
    }
#endif	// #ifdef __K_DEBUG_SHIAI__


    st_.pstBa_->Motion = tMtype;

  }

  //ボールを持ったとき
  void TBaCommon::SetMTypeHold(s32 tTNo, s32 tPNo)
  {
    //ボールの設定
    ZeroXYZ();
    SetMtype(bmHold);
    st_.pstBa_->HoldTNo = st_.pstBa_->PichTNo = tTNo;
    st_.pstBa_->HoldPNo = st_.pstBa_->PichPNo = tPNo;
    st_.pstBa_->EnemTNo = (tTNo == 0) ? 1 : 0;//敵サイド番号
    st_.pstBa_->OvLine_f = FALSE;
    st_.pstBa_->shotmotion_f = FALSE;

    //両チームの設定
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      //カウンタリセット
      st_.pmgTm_[i]->st_.pstMyTm_->PosMove.EnC_c = 0;
    }
  }

  //バウンド着地点
  void TBaCommon::GetBoundLandPos(s32 tGrv)
  {
    //もう設定済み
    if (st_.pstBa_->LandSet_f) return;

    s32 t1 = (st_.pstBa_->Zahyou.dY / tGrv);//最高点までの時間
    s32 top = (st_.pstBa_->Zahyou.dY > 0)
      ? (st_.pstBa_->Zahyou.dY * st_.pstBa_->Zahyou.dY) / (2 * tGrv)//最高点
      : ((tGrv * (t1 * t1)) / 2);//最高点だったところを逆算
    //最高点から地面までの時間（後にt1+t2で現地点から地面までの時間になる）
    s32 t2 = (s32)sqrt((f32)((2 * (st_.pstBa_->Zahyou.Y + top)) / tGrv));

    //着地点（壁超えしないように）
    st_.pstBa_->LandX = lib_num::Between((st_.pstBa_->Zahyou.X + (st_.pstBa_->Zahyou.dX * (t1 + t2))), 0, DBCRT_W);
    st_.pstBa_->LandZ = lib_num::Between((st_.pstBa_->Zahyou.Z + (st_.pstBa_->Zahyou.dZ * (t1 + t2))), 0, DBCRT_WAL);
    st_.pstBa_->LandZLine = st_.pstBa_->LandZ;
    st_.pstBa_->LandXLine = st_.pstBa_->LandX;
    st_.pstBa_->LandLine_f = FALSE;
    st_.pstBa_->Land_f = TRUE;
    st_.pstBa_->LandSet_f = TRUE;

    const s32 LLDIST = 8 * XYMAG;
    BOOL tLandXLine_f = FALSE;
    const s32 LLDISTZ = 6 * XYMAG;
    BOOL tLandZLine_f = FALSE;


    //Xラインまたぐとき
    if (st_.pstBa_->LandX < (DBCRT_SL + LLDIST))
    {
      st_.pstBa_->LandXLine = (DBCRT_SL + LLDIST);
      tLandXLine_f = TRUE;
    }
    else if (st_.pstBa_->LandX > (DBCRT_SL2 - LLDIST))
    {
      st_.pstBa_->LandXLine = (DBCRT_SL2 - LLDIST);
      tLandXLine_f = TRUE;
    }

    //Zラインまたぐとき
    if (st_.pstBa_->LandZ > DBCRT_BL - LLDISTZ)
    {
      st_.pstBa_->LandZLine = (DBCRT_BL - LLDISTZ);
      tLandZLine_f = TRUE;
    }
    else if (st_.pstBa_->LandZ < DBCRT_FL + LLDISTZ)
    {
      st_.pstBa_->LandZLine = (DBCRT_FL + LLDISTZ);
      tLandZLine_f = TRUE;
    }

    s32 tLandX = st_.pstBa_->LandXLine;
    s32 tLandZ = st_.pstBa_->LandZLine;

    //線までの距離／落下地点(%)
    if ((tLandXLine_f) && (st_.pstBa_->Zahyou.X != tLandX))
    {
      s32 dp2 = abs(st_.pstBa_->Zahyou.X - st_.pstBa_->LandX);
      if (dp2 > 0)
      {
        //ライン際までの長さ/本来の長さ
        s32 dp = (abs(st_.pstBa_->Zahyou.X - st_.pstBa_->LandXLine) * XYMAG) / dp2;
        st_.pstBa_->LandZLine = st_.pstBa_->Zahyou.Z + lib_num::Percent((st_.pstBa_->LandZ - st_.pstBa_->Zahyou.Z), dp);
      }
    }


    if ((tLandZLine_f) && (st_.pstBa_->Zahyou.Z != tLandZ))
    {
      s32 dp2 = abs(st_.pstBa_->Zahyou.Z - st_.pstBa_->LandZ);
      if (dp2 > 0)
      {
        s32 dp = (abs(st_.pstBa_->Zahyou.Z - st_.pstBa_->LandZLine) * XYMAG) / dp2;
        st_.pstBa_->LandXLine = st_.pstBa_->Zahyou.X + lib_num::Percent((st_.pstBa_->LandX - st_.pstBa_->Zahyou.X), dp);
      }
    }

    //線をまたぐ
    st_.pstBa_->LandLine_f = (tLandZLine_f || tLandXLine_f);
  }
  //シュート着地点
  void TBaCommon::GetShootLandPos()
  {

    s32 tdX = st_.pstBa_->Zahyou.dX + st_.pstBa_->CrvX;
    s32 tdY = st_.pstBa_->Zahyou.dY + st_.pstBa_->CrvY;
    s32 tdZ = st_.pstBa_->Zahyou.dZ + st_.pstBa_->CrvZ;
    //if (st_.pstBa_->ShSt.Crvable_f)
    //{
    //  tdX = st_.pstBa_->Zahyou.dX + st_.pstBa_->CrvX;
    //  tdY = st_.pstBa_->Zahyou.dY + st_.pstBa_->CrvY;
    //  tdZ = st_.pstBa_->Zahyou.dZ + st_.pstBa_->CrvZ;
    //}
    //else
    //{
    //  tdX = st_.pstBa_->ShSt.fdX;
    //  tdY = st_.pstBa_->ShSt.fdY;
    //  tdZ = st_.pstBa_->ShSt.fdZ;
    //}

    //下降
    st_.pstBa_->Land_f = (tdY < 0);

    if (st_.pstBa_->Land_f && (tdY != 0))
    {
      //落ちるまでの時間
      s32 tlt = (st_.pstBa_->Zahyou.Y / (-tdY));
      st_.pstBa_->LandX = st_.pstBa_->Zahyou.X + (tdX * tlt);
      st_.pstBa_->LandZ = st_.pstBa_->Zahyou.Z + (tdZ * tlt);
    }
    else
    {
      st_.pstBa_->LandX = st_.pstBa_->Zahyou.X;
      st_.pstBa_->LandZ = st_.pstBa_->Zahyou.Z;
    }
  }

  //ボールタイマー初期化
  void TBaCommon::ResetTimer(s32 tSideNo, BOOL init_f)
  {
    if ((init_f == FALSE) && (st_.pstBa_->TimerSide == tSideNo)) return;

    const s32 ONESECFRAME = 60;

    st_.pstBa_->TimerSide = tSideNo;

    if (st_.pstBa_->TimerSide != NGNUM)
    {
      st_.pstBa_->Timer_c = (10 * ONESECFRAME);
    }

    st_.pstBa_->Timer_c3 = (16 * ONESECFRAME);//16秒
    st_.pstBa_->Timer_c2 = 0;
  }

  void TBaCommon::SetCamPos()
  {
    //ここでグラフィックマネージャーに渡しておく
    pmgEO_->mgCamera_.SetCamPos((st_.pstBa_->CameraPosX / XYMAG) + mid::midGetDotL_Hf(), +mid::midGetDotS());//ゲームの中心をカメラのゼロ点に
  }
  void TBaCommon::ResetCamPos()
  {
    //カメラ位置戻す
    st_.pstBa_->CameraPosX = st_.pstBa_->Zahyou.X;
    SetCamPos();
  }

  //位置リセット
  void TBaCommon::ResetRefPos(s32 tSideNo, enJumpBall tJumpBal, BOOL demo_f)
  {
    s32 tmpCamPos = st_.pstBa_->CameraPosX;
    //ボールステータスの初期化
    st_.pstBa_->Init();
    st_.pstBa_->Zahyou.X = DBCRT_CL;
    if ((tJumpBal == jbJpball) && (demo_f == FALSE))
    {
      ResetCamPos();
    }
    else
    {
      //カメラ位置戻す
      st_.pstBa_->CameraPosX = tmpCamPos;
    }

    if (demo_f == FALSE)
    {
      st_.pstBa_->Zahyou.Z = DBCRT_WAL;
      st_.pstBa_->Zahyou.Y = 1600;

      s32 TOPTIME = (tSideNo == NGNUM)
        ? 50
        : 30;
      //st_.pstBa_->Zahyou.dY = TOPTIME * FCDiv(pmgEO_->mgDt_.dtSet_.GetDtBall(setBoundGrv));
      st_.pstBa_->Zahyou.dY = TOPTIME * (pmgEO_->mgDt_.dtSet_.GetDtGrv(setBoundGrv));
      st_.pstBa_->Zahyou.dZ = ((DBCRT_CLZ - DBCRT_WAL) / (TOPTIME * 2));

      switch (tSideNo)
      {
      case 0:
        st_.pstBa_->Zahyou.dX = -(abs(st_.pstBa_->Zahyou.dZ));
        break;
      case 1:
        st_.pstBa_->Zahyou.dX = +(abs(st_.pstBa_->Zahyou.dZ));
        break;
      default: st_.pstBa_->Zahyou.dX = 0; break;
      }
      SetMtype(bmReferee);//★
      ResetTimer(NGNUM, TRUE);
      //ジャンプボール
      st_.pstBa_->JumpBall = tJumpBal;
    }
    else
    {
      st_.pstBa_->Zahyou.Z = DBCRT_CLZ;
      st_.pstBa_->Zahyou.Y = 18800;
      //st_.pstBa_->Zahyou.Y = 0;
      st_.pstBa_->Zahyou.dX = 0;
      st_.pstBa_->Zahyou.dY = 100;
      st_.pstBa_->Zahyou.dZ = 0;
      SetMtype(bmReferee);//★
      ResetTimer(NGNUM, TRUE);
      st_.pstBa_->Start_f = TRUE;
    }
  }

  //位置リセット
  void TBaCommon::ResetRefPos_Prev(BOOL ready_f)
  {
    //s32 tmpCamPos = st_.pstBa_->CameraPosX;
    //ボールステータスの初期化
    st_.pstBa_->Init();
    //st_.pstBa_->Zahyou.X = st_.pmgTm_[SIDE0]->st_.pmgMyCh_[0]->st_.pstMyCh_->Zahyou.X;

    //カメラ位置戻す
    st_.pstBa_->CameraPosX = DBCRT_CL;
    SetCamPos();
    st_.pstBa_->Zahyou.X = st_.pmgTm_[SIDE0]->st_.pmgMyCh_[0]->st_.pstMyCh_->Zahyou.X;
    st_.pstBa_->Zahyou.Z = st_.pmgTm_[SIDE0]->st_.pmgMyCh_[0]->st_.pstMyCh_->Zahyou.Z;
    st_.pstBa_->Zahyou.Y = st_.pmgTm_[SIDE0]->st_.pmgMyCh_[0]->st_.pstMyCh_->Zahyou.Y;

    ////st_.pstBa_->Zahyou.Y = 0;
    //st_.pstBa_->Zahyou.dX = 0;
    //st_.pstBa_->Zahyou.dY = 100;
    //st_.pstBa_->Zahyou.dZ = 0;
    //SetMtype(bmReferee);//★
    st_.pmgTm_[SIDE0]->st_.pmgMyCh_[0]->chCommon_.BallGet(ready_f, FALSE);
    ResetTimer(NGNUM, TRUE);
    st_.pstBa_->Start_f = TRUE;


    //プレビュー最低1回なげた
    st_.pmgRf_->SetPrevReturn();
  }

  //位置リセットチュートリアル
  void TBaCommon::ResetRefPos_Tuto(BOOL ready_f, s32 side)
  {
    //ボールステータスの初期化
    st_.pstBa_->Init();

    //カメラ位置戻す
    st_.pstBa_->CameraPosX = DBCRT_CL;
    SetCamPos();
    //if (side != NGNUM)
    {
      st_.pstBa_->Zahyou.X = st_.pmgTm_[side]->st_.pmgMyCh_[0]->st_.pstMyCh_->Zahyou.X;
      st_.pstBa_->Zahyou.Z = st_.pmgTm_[side]->st_.pmgMyCh_[0]->st_.pstMyCh_->Zahyou.Z;
      st_.pstBa_->Zahyou.Y = st_.pmgTm_[side]->st_.pmgMyCh_[0]->st_.pstMyCh_->Zahyou.Y;
      st_.pmgTm_[side]->st_.pmgMyCh_[0]->chCommon_.BallGet(ready_f, FALSE);
    }
    ResetTimer(NGNUM, TRUE);
    st_.pstBa_->Start_f = TRUE;

  }

  //位置リセット
  void TBaCommon::RefThrow()
  {
    st_.pstBa_->Motion = bmBound;//こうしないとジャンプボールフラグが倒れる

    //SetMtype(bmBound);
  }

  //逆向き
  enMukiType TBaCommon::RevMuki(enMukiType tMuki)
  {
    if (tMuki == mL)
    {
      return mR;
    }
    else
    {
      return mL;
    }
  }

  //ボールを通常状態にする
  void TBaCommon::SetNmlBall(BOOL InShoot_f)
  {
    st_.pstBa_->AniNo = 0;
    st_.pstBa_->Invs_f = FALSE;
    //if (InShoot_f)
    //{
    //  st_.pstBa_->ShSt.HSNo = hsNML;
    //}
  }

  //効果音
  void TBaCommon::SESet(enSEID SENo)
  {
    if (pmgSG_->stRf_.NoSE_f == FALSE)
    {
      pmgEO_->mgSound_.PlaySE(SENo);//, FALSE);
    }
  }

  //効果音
  void TBaCommon::HomingSESet(enSEID SENo)
  {
    HomingSESet((s32)SENo);
  }
  //効果音
  void TBaCommon::HomingSESet(s32 SENo)
  {
    if (pmgSG_->stRf_.NoSE_f == FALSE)
    {
      pmgEO_->mgSound_.PlayHMNG(SENo);
    }
  }

  //エンドライン
  s32 TBaCommon::GetSLX()
  {
    return (DBCRT_SL + ((st_.pstBa_->Zahyou.Z - DBCRT_FL) * 10 / DBCRT_SLR));
  }

  //はんで／難易度によるダメージ変動
  s32 TBaCommon::HandiDam(s32 dam, s32 sideNo, s32 padNo)
  {
    return dam;
    //s32 per = XYMAG;
    //return lib_num::Percent(dam, per);
  }


  //共通処理をもつ////////////////////////////////

  //コンストラクタ
  TBaHaveCommon::TBaHaveCommon(TGame* pGame, TBaCommon* pCommon)
    : TBaGen(pGame), pCommon_(pCommon)
  {

  }
  TBaHaveCommon::~TBaHaveCommon(void)
  {
  }

    }
