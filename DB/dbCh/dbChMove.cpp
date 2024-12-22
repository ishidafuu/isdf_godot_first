#include "dbChMove.h"
#include "dbGame.h"

namespace db
{
  //移動////////////////////////////////

  //コンストラクタ
  TChMove::TChMove(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo)
    : TChHaveCommon(pGame, pCommon, sideNo, charNo)
  {
  }

  TChMove::~TChMove()
  {
  }

  //よけ矩形チェック
  BOOL TChMove::IsDgBoxCheck(BOOL jpdg_f)
  {
    TRect dg_rect;
    TRect ball_rect;
    BOOL res = FALSE;

    const s32 BW = pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariWidht)  * XYMAG;
    const s32 BH = pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariHeight) * XYMAG;
    const s32 BD = pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariDepth)  * XYMAG;
    const s32 BWhf = BW / 2;
    const s32 BHhf = BH / 2;
    const s32 BDhf = BD / 2;

    //ジャンプ避け
    if (jpdg_f)
    {
      s32 DGTIME = 10;//このフレーム進んだときに当たっているかどうか
      s32 www = ((pmgEO_->mgDt_.dtSet_.GetDtInfield(setJpYokeJudgeWidth) * XYMAG) / 2);
      s32 ddd = ((pmgEO_->mgDt_.dtSet_.GetDtInfield(setJpYokeJudgeDepth) * XYMAG) / 2);

      s32 DgrNo = 0;

      //横シュート
      if (abs(pmgSG_->stBa_.Zahyou.dX) > abs(pmgSG_->stBa_.Zahyou.dZ))
      {
        //ボールより右側
        DgrNo = (st_.pstMyCh_->Zahyou.X > pmgSG_->stBa_.Zahyou.X)
          ? 0//左
          : 1;//右
      }
      //縦シュート
      else
      {
        //ボールより奥
        DgrNo = (st_.pstMyCh_->Zahyou.Z > pmgSG_->stBa_.Zahyou.Z)
          ? 2 //手前
          : 3;//右
      }

      switch (DgrNo)
      {
      case 0: //左
        dg_rect.Bounds(
          st_.pstMyCh_->Zahyou.X - (www),
          st_.pstMyCh_->Zahyou.Z - (ddd),
          www,
          ddd * 2);
        break;
      case 1: //右
        dg_rect.Bounds(
          st_.pstMyCh_->Zahyou.X,
          st_.pstMyCh_->Zahyou.Z - (ddd),
          www,
          ddd * 2);
        break;
      case 2: //手前
        dg_rect.Bounds(
          st_.pstMyCh_->Zahyou.X - (ddd),
          st_.pstMyCh_->Zahyou.Z - (www),
          ddd,
          www * 2);
        break;
      default://奥
        dg_rect.Bounds(
          st_.pstMyCh_->Zahyou.X - (ddd),
          st_.pstMyCh_->Zahyou.Z,
          ddd,
          www * 2);
        break;
      }

      s32 fbx = pmgSG_->stBa_.Zahyou.X + (pmgSG_->stBa_.Zahyou.dX * DGTIME);
      s32 fby = pmgSG_->stBa_.Zahyou.Y + (pmgSG_->stBa_.Zahyou.dY * DGTIME);
      s32 fbz = pmgSG_->stBa_.Zahyou.Z + (pmgSG_->stBa_.Zahyou.dZ * DGTIME);

      //ボールの判定
      ball_rect.Bounds((fbx - BWhf), (fbz - BDhf), BW, BD);

      //現状高さ抜きで当たっているかどうか
      BOOL whhit_f = (st_.pstMyCh_->Kurai.IsPile(pmgSG_->stBa_.Atari));

      BOOL highpile_f = (fby < (pmgEO_->mgDt_.dtSet_.GetDtInfield(setNoJpHeight) * XYMAG));

      res = (dg_rect.IsPile(ball_rect) && highpile_f && (whhit_f == FALSE));
    }
    else//しゃがみ避け
    {
      const s32 CATCHJUDGEWIDTH = 32;
      s32 www = ((pmgEO_->mgDt_.dtSet_.GetDtInfield(setCrYokeJudgeWidth) * XYMAG) / 2);
      s32 ddd = ((pmgEO_->mgDt_.dtSet_.GetDtInfield(setCrYokeJudgeDepth)  * XYMAG) / 2);

      dg_rect.Bounds(
        st_.pstMyCh_->Zahyou.X - (www),
        st_.pstMyCh_->Zahyou.Z - (ddd),
        www * 2,
        ddd * 2);


      //ボールの判定
      ball_rect.Bounds((pmgSG_->stBa_.Zahyou.X - BWhf), (pmgSG_->stBa_.Zahyou.Z - BDhf), BW, BD);

      res = dg_rect.IsPile(ball_rect);
    }

    return res;
  }



  //よけ種類
  enTDGSL TChMove::CheckDS(BOOL tCOMCa_f)//def = FALSE
  {
    const s32 DCTYPE_NONE = 0;
    const s32 DCTYPE_CA = 1;
    const s32 DCTYPE_DG = 2;

    const s32 DCTYPE_JP = 2;
    const s32 DCTYPE_CR = 3;
    const s32 DCTYPE_SW = 4;

    enTDGSL ds = dgsNone;

    //敵のシュート
    if ((pmgSG_->stBa_.Motion == bmShoot)
      && (pmgSG_->stBa_.ShTgTNo == st_.mysideNo_)
      && (IsInfield()))
    {

      //戻り中の避け
      if (st_.pstMyCh_->NextAuto.AutoType == dbatReturn)
      {
        //しゃがみ避け指示が出ている場合のみ
        if ((st_.pmgMyTm_->st_.pstMyTm_->COMRnd.RndAct[st_.posNo_].DgType == dgCr)
          || (st_.pmgMyTm_->st_.pstMyTm_->COMRnd.RndAct[st_.posNo_].DgType == dgBest))
        {
          ds = (IsDgBoxCheck(FALSE))
            ? dgsCr
            : dgsNone;
        }
        else if (st_.pmgMyTm_->st_.pstMyTm_->COMRnd.RndAct[st_.posNo_].DgType == dgJp)//ジャンプ避け
        {
          ds = (IsDgBoxCheck(TRUE))
            ? dgsJp
            : dgsNone;
        }
      }
      else if (st_.pstMyCh_->NextAuto.AutoType == dbatCPUHelp)//ヘルプ
      {
        ds = dgsCa;//かならずキャッチ
      }
      else
      {
        s32 CheckType;

        if (tCOMCa_f)
        {
          CheckType = DCTYPE_CA;
        }
        else
        {
          CheckType = DCTYPE_DG;
        }


        BOOL JpNeed_f = TRUE;
        BOOL shMuki_f = (
          ((pmgSG_->stBa_.Zahyou.dX <= 0) && (st_.pstMyCh_->Zahyou.Muki == mR))
          || ((pmgSG_->stBa_.Zahyou.dX >= 0) && (st_.pstMyCh_->Zahyou.Muki == mL))
          || ((pmgSG_->stBa_.Zahyou.dZ <= 0) && (st_.pstMyCh_->Zahyou.MukiZ == mzB))
          || ((pmgSG_->stBa_.Zahyou.dZ >= 0) && (st_.pstMyCh_->Zahyou.MukiZ == mzF))
          );

        if (CheckType == DCTYPE_CA)
        {
          ds = dgsCa;
        }
        else if (CheckType == DCTYPE_DG)
        {

          ////しゃがみ不可
          //BOOL crnghs_f = ((pmgSG_->stBa_.ShSt.HSNo != hsNML)
          //                 && pmgEO_->mgDt_.dtShot_.GetDtBOOL((s32)pmgSG_->stBa_.ShSt.HSNo,shdtCOMCrDgNG));
          //
          //BOOL crngns_f = ((pmgSG_->stBa_.ShSt.HSNo == hsNML) && pmgSG_->stSh_.idata.jsh_f);
          //BOOL crng_f = (crnghs_f || crngns_f);
          //
          ////ジャンプ不可
          //BOOL jpng_f = ((pmgSG_->stBa_.ShSt.HSNo != hsNML)
          //               && pmgEO_->mgDt_.dtShot_.GetDtBOOL((s32)pmgSG_->stBa_.ShSt.HSNo,shdtCOMJpDgNG));
          //
          ////移動避け不可
          //BOOL swng_f = (((pmgSG_->stBa_.ShSt.HSNo != hsNML)
          //                && pmgEO_->mgDt_.dtShot_.GetDtBOOL((s32)pmgSG_->stBa_.ShSt.HSNo,shdtCOMSwDgNG)));

          switch (st_.pmgMyTm_->st_.pstMyTm_->COMRnd.RndAct[st_.posNo_].DgType)
          {
          case dgCr:
            ////しゃがみ→キャッチ
            //if (crng_f && st_.pmgMyTm_->st_.pstMyTm_->COMRnd.DgToCa_f)
            //{
            //  ds = dgsCa;
            //}
            //else
          {
            ds = (IsDgBoxCheck(FALSE))
              ? dgsCr
              : dgsNone;
          }
          break;
          case dgJp:
            ////ジャンプ→キャッチ
            //if (jpng_f && st_.pmgMyTm_->st_.pstMyTm_->COMRnd.DgToCa_f)
            //{
            //  ds = dgsCa;
            //}
            //else
          {
            ds = (IsDgBoxCheck(TRUE))
              ? dgsJp
              : dgsNone;
          }
          break;
          case dgSw:
            ////移動→キャッチ
            //if (swng_f && st_.pmgMyTm_->st_.pstMyTm_->COMRnd.DgToCa_f)
            //{
            //  ds = dgsCa;
            //}
            //else
          {
            ds = dgsSw;
          }
          break;
          default://シュート対応
            if (pmgSG_->stSh_.idata.jsh_f)
            {
              ds = dgsSw;//最優先スウェー（無条件）

              ////スウェー避け不可のシュート→ジャンプに（範囲に入ったら）
              //if ((ds == dgsSw) && swng_f && (IsDgBoxCheck(TRUE))) ds = dgsJp;
              ////ジャンプ避け不可のシュート→しゃがみに
              //if ((ds == dgsJp) && jpng_f) ds = dgsCr;
              ////しゃがみ避け不可のシュート→キャッチに
              //if ((ds == dgsCr) && crng_f) ds = dgsCa;
            }
            else
            {

              //最優先しゃがみ（範囲に入ったら）
              ds = (IsDgBoxCheck(FALSE))
                ? dgsCr
                : dgsNone;
              //
              ////しゃがみ避け不可のシュート→無条件即スウェーに
              //if (crng_f) ds = dgsSw;
              ////スウェー避け不可のシュート→ジャンプに
              //if ((ds == dgsSw) && swng_f) ds = dgsJp;
              ////ジャンプ避け不可のシュート→キャッチに
              //if ((ds == dgsJp) && jpng_f) ds = dgsCa;
            }
            break;
          }

        }
      }
    }

    //よけ開始時間未満
    if ((ds == dgsSw) || (ds == dgsCr))
    {
      if (pmgSG_->stSh_.cmn.timeTotal_c < st_.pmgMyTm_->st_.pstMyTm_->COMRnd.RndAct[st_.posNo_].DgTime)
      {
        ds = dgsNone;
      }
    }

    return ds;
  }

  //カバージャンプするか（到着の高さをチェックするようにする★
  BOOL TChMove::IsCheckPaCvJp()
  {

    const s32 CVJPY = 24;
    //味方パス
    if ((pmgSG_->stBa_.Motion == bmPass) && ((pmgSG_->stBa_.Zahyou.Y / XYMAG) > CVJPY))
    {
      TRect Dgr;

      Dgr.Bounds(
        st_.pstMyCh_->Zahyou.X - (pmgEO_->mgDt_.dtSet_.GetDtInfield(setShCvJpJudgeWidth) * XYMAG / 2),
        st_.pstMyCh_->Zahyou.Z - (pmgEO_->mgDt_.dtSet_.GetDtInfield(setShCvJpJudgeDepth) * XYMAG / 2),
        pmgEO_->mgDt_.dtSet_.GetDtInfield(setShCvJpJudgeWidth) * XYMAG,
        pmgEO_->mgDt_.dtSet_.GetDtInfield(setShCvJpJudgeDepth) * XYMAG);

      TRect Blr;

      Blr.Bounds(
        pmgSG_->stBa_.Zahyou.X - (pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariWidht) * XYMAG / 2),
        pmgSG_->stBa_.Zahyou.Z - (pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariDepth) * XYMAG / 2),
        pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariWidht) * XYMAG,
        pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariDepth) * XYMAG);

      //枠の中に入りました
      return Dgr.IsPile(Blr);
    }
    else
    {
      return FALSE;
    }
  }

  //カバージャンプするか（到着の高さをチェックするようにする★
  BOOL TChMove::IsCheckShCvJp()
  {


    TRect Dgr;

    Dgr.Bounds(
      st_.pstMyCh_->Zahyou.X - (pmgEO_->mgDt_.dtSet_.GetDtInfield(setShCvJpJudgeWidth) * XYMAG / 2),
      st_.pstMyCh_->Zahyou.Z - (pmgEO_->mgDt_.dtSet_.GetDtInfield(setShCvJpJudgeDepth) * XYMAG / 2),
      pmgEO_->mgDt_.dtSet_.GetDtInfield(setShCvJpJudgeWidth) * XYMAG,
      pmgEO_->mgDt_.dtSet_.GetDtInfield(setShCvJpJudgeDepth) * XYMAG);

    TRect Blr;

    Blr.Bounds(
      pmgSG_->stBa_.Zahyou.X - (pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariWidht) * XYMAG / 2),
      pmgSG_->stBa_.Zahyou.Z - (pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariDepth) * XYMAG / 2),
      pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariWidht) * XYMAG,
      pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariDepth) * XYMAG);

    //枠の中に入りました
    return Dgr.IsPile(Blr);

  }

  //次オートモーション設定(dbatFreeにもどる条件)
  void TChMove::InitAutoTypeMuki()
  {
    const s32 STOPDIST = 5 * XYMAG;

    //次オートモーションの初期化
    st_.pstMyCh_->NextAuto.AutoType = st_.pstMyCh_->Auto.AutoType;//デフォは引き継ぎ

    BOOL MukiReset_f = TRUE;

    //外野オーバーラインoffココで倒すでOKか？//ジャンプでon
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
    {
      st_.pstMyCh_->GaiyaOvL_f = TRUE;
    }

    // BOOL teamball_f;
    switch (st_.pstMyCh_->Auto.AutoType)
    {
    case dbatFree:
      st_.pstMyCh_->AShCv_c = 0;
      break;

    case dbatAlleyoop:
      st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      break;

    case dbatReturn:
    case dbatKgReturn:
      //一応ココではフリーにしてチェックしてダメだったらまたreturnにする
      st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      break;

    case dbatGet:
      //ボールがフリーでなくなったら
      //パスカットはどうするか
      //if (IsBall()
      //  || (pmgSG_->stBa_.Motion == bmFree)
      //  || (pmgSG_->stBa_.Motion == bmBound)
      //  || (pmgSG_->stBa_.Motion == bmHold))
      //{
      st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      //}
      break;

    case dbatPassCvr:
      //ボールがフリーでなくなったら
      //パスカットはどうするか
      if (IsCtrl()
        || IsBall()
        || (pmgSG_->stBa_.Motion != bmPass))
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }
      break;

    case dbatDfPos:
      st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      break;

    case dbatPassCach:
      //ボールがパスボールではなくなったら
      if (pmgSG_->stBa_.Motion != bmPass)
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }
      break;

    case dbatPassJp:
      //ボールがパスボールではなくなったら
      if (pmgSG_->stBa_.Motion != bmPass)
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }
      break;

    case dbatDg:
    case dbatDgSw:
    case dbatDgJp:
    case dbatCPUCa:
      st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      MukiReset_f = FALSE;
      break;

    case dbatCPUShPa:
      //COMじゃなくなったら
      if (IsSelfCtrl())
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
        st_.pstMyCh_->Auto.AutoType = dbatFree;//現状もフリーだったことにする（ダッシュがキレてしまうため）
      }
      //敵サイドになったら
      else if (IsShiai() == FALSE)
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }
      else if (st_.pmgEnTm_->st_.pstMyTm_->ofside_f)
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }
      if (IsCtrl() == FALSE)//if (IsBall() == FALSE)//ボール手放しても走り続けるためには操作権あるあいだ
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }
      else if ((pmgSG_->stBa_.Motion == bmShoot)//シュートになったら
        || (pmgSG_->stBa_.Motion == bmBound)//バウンドになったら
        || (pmgSG_->stBa_.Motion == bmFree))//停止状態になったら
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }
      else if ((pmgSG_->stBa_.Motion == bmHold)//持ちになったら
        && (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)//味方がもったら
        && (pmgSG_->stBa_.HoldPNo != st_.posNo_))//じぶんではない
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }
      MukiReset_f = FALSE;
      break;

    case dbatShCv:
      //ボールがシュートボールではなくなったら
      if (pmgSG_->stBa_.Motion != bmShoot)
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }
      break;

    case dbatCPUJumpBall:
      //毎フレ更新
      st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      break;

    case dbatCPUPaCut:
      //毎フレ更新
      st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      break;

    case dbatCPUHelp:
      //毎フレ更新
      st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      break;

    case dbatDashman://★ダッシュマン
      if (IsCtrl())
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }

      //着地とスリップでダッシュマン解除
      if ((st_.pstMyCh_->Motion.Mtype == dbmtCr)
        || (st_.pstMyCh_->Motion.Mtype == dbmtSl))
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      }

      MukiReset_f = FALSE;
      break;
    default:
      st_.pstMyCh_->NextAuto.AutoType = dbatFree;
      break;
    }


    if (MukiReset_f)
    {
      st_.pstMyCh_->NextAuto.AMuki = maN;
      st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
      st_.pstMyCh_->NextAuto.ADash_f = FALSE;
    }
  }

  //オート戻りラインチェック
  BOOL TChMove::IsReturnLine(enCrtLine lineType, s32 tMax)
  {
    BOOL res_f = FALSE;
    switch (lineType)
    {
    case crtLLine:
      if (GetLeftCrtX() < tMax)
      {
        st_.pstMyCh_->NextAuto.AMuki = maR;
        if ((GetLeftCrtX() < (tMax - DASHDIST))
          || IsInfield())
        {
          st_.pstMyCh_->NextAuto.ADash_f = TRUE;
        }
        else
        {
          st_.pstMyCh_->NextAuto.ADash_f = FALSE;
        }
        res_f = TRUE;
      }
      break;
    case crtRLine:
      if (GetLeftCrtX() > tMax)
      {
        st_.pstMyCh_->NextAuto.AMuki = maL;
        if ((GetLeftCrtX() > (tMax + DASHDIST))
          || IsInfield())
        {
          st_.pstMyCh_->NextAuto.ADash_f = TRUE;
        }
        else
        {
          st_.pstMyCh_->NextAuto.ADash_f = FALSE;
        }
        res_f = TRUE;
      }
      break;
    case crtFLine:
      if (st_.pstMyCh_->Zahyou.Z < tMax)
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
        st_.pstMyCh_->NextAuto.ADash_f = FALSE;
        res_f = TRUE;
      }
      break;
    case crtBLine:
      if (st_.pstMyCh_->Zahyou.Z > tMax)
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
        st_.pstMyCh_->NextAuto.ADash_f = FALSE;
        res_f = TRUE;
      }
      break;
    }

    if (res_f)
    {
      st_.pstMyCh_->NextAuto.AutoType = dbatReturn;
    }

    return res_f;
  }
  //反転
  void TChMove::SetAMuki(s32 RevNo)
  {
    if (st_.pstMyCh_->NextAuto.AutoType == dbatReturn)
    {
      if (st_.mysideNo_ == RevNo)
      {
        if (st_.pstMyCh_->NextAuto.AMuki == maL)
        {
          st_.pstMyCh_->NextAuto.AMuki = maR;
        }
        else if (st_.pstMyCh_->NextAuto.AMuki == maR)
        {
          st_.pstMyCh_->NextAuto.AMuki = maL;
        }
      }
    }
  }

  //オート戻り
  void TChMove::CheckAutoReturn()
  {
    //空中以外
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)) return;

    //if (st_.pstMyCh_->Motion.IsMFlags(dbmfAct)) return;

    //他の任務があるときはそちらを優先
    if ((st_.pstMyCh_->NextAuto.AutoType != dbatFree)
      && (st_.pstMyCh_->NextAuto.AutoType != dbatAlleyoop)
      && (st_.pstMyCh_->NextAuto.AutoType != dbatReturn)
      && (st_.pstMyCh_->NextAuto.AutoType != dbatKgReturn)
      && (st_.pstMyCh_->NextAuto.AutoType != dbatDashman)
      && (st_.pstMyCh_->NextAuto.AutoType != dbatCPUShPa)
      ) return;


    ////自分にパスが出ている★★//敵コート入り放題になる
    //if ((pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
    //  && (pmgSG_->stBa_.PaTgPNo == st_.posNo_)) return;


    BOOL tGOvL_f = FALSE;


    if ((pCommon_->IsInCourt() == FALSE) || (Kagami_f() == FALSE))
    {
      st_.pstMyCh_->NextAuto.ADashStoped_f = FALSE;
    }

    const s32 CRPOSZWIDE = 1 * XYMAG;
    const s32 CRPOSXWIDE = 2 * XYMAG;
    const s32 KGTGWIDHT = 12 * XYMAG;
    const s32 KGTGDEPTH = 4 * XYMAG;


    BOOL lov_f = FALSE;
    BOOL rov_f = FALSE;
    BOOL fov_f = FALSE;
    BOOL bov_f = FALSE;

    switch (st_.posNo_)
    {
    case (s32)dbpoO2:

      rov_f = IsReturnLine(crtRLine, DBCRT_CLI);//tGOvL_f |=
      lov_f = IsReturnLine(crtLLine, CRPOS_B_XL);
      fov_f = IsReturnLine(crtFLine, CRPOS_B_Z - CRPOSZWIDE);//tGOvL_f |=
      bov_f = IsReturnLine(crtBLine, CRPOS_B_Z + CRPOSZWIDE);
      if (((lov_f == FALSE) && fov_f) || rov_f)
      {
        tGOvL_f = TRUE;
      }
      SetAMuki(0);

      break;
    case (s32)dbpoO3:

      rov_f = IsReturnLine(crtRLine, DBCRT_CLI);//tGOvL_f |=
      lov_f = IsReturnLine(crtLLine, CRPOS_F_XL);
      fov_f = IsReturnLine(crtFLine, CRPOS_F_Z - CRPOSZWIDE);
      bov_f = IsReturnLine(crtBLine, CRPOS_F_Z + CRPOSZWIDE);//tGOvL_f |=
      if (((lov_f == FALSE) && bov_f) || rov_f)
      {
        tGOvL_f = TRUE;
      }
      SetAMuki(0);

      break;
    case (s32)dbpoO4:
      //４番外野はぜったいオーバラインならないでＯＫでは
      //tGOvL_f = IsReturnLine(crtRLine, (GetSLX() - CRPOS_S_X + CRPOSXWIDE));
      rov_f = IsReturnLine(crtRLine, (GetSLX() - CRPOS_S_X + CRPOSXWIDE));
      lov_f = IsReturnLine(crtLLine, (GetSLX() - CRPOS_S_X - CRPOSXWIDE));//tGOvL_f |=
      fov_f = IsReturnLine(crtFLine, DBCRT_FL);
      bov_f = IsReturnLine(crtBLine, DBCRT_BL);
      if (rov_f && (fov_f == FALSE) && (bov_f == FALSE))
      {
        tGOvL_f = TRUE;
      }
      SetAMuki(0);
      break;
    default:
      if (Kagami_f() == FALSE)
      {
        //かがみ予約ではないとき
        IsReturnLine(crtRLine, DBCRT_CLI);
        IsReturnLine(crtLLine, GetSLX());
        IsReturnLine(crtFLine, DBCRT_FL);
        IsReturnLine(crtBLine, DBCRT_BL);
        SetAMuki(1);
      }
      else if (
        (st_.pstMyCh_->KagamiRev_c >= RankHP(rkBreathRev))//もどれるじかんを超えたとき
        || (st_.pmgMyTm_->st_.pstMyTm_->BallCrtSideL == st_.mysideNo_)
        )
      {
        //かがみ予約ではないとき
        IsReturnLine(crtRLine, DBCRT_CLI);
        IsReturnLine(crtLLine, GetSLX());
        IsReturnLine(crtFLine, DBCRT_FL);
        IsReturnLine(crtBLine, DBCRT_BL);
        SetAMuki(1);
      }
      else//かがみ予約の時
      {

        s32 KgNo = NGNUM;
        s32 KgX = 0;
        s32 KgZ = 0;
        s32 KgW = 0;
        s32 KgD = 0;

        //初回
        if (st_.pstMyCh_->KagamiRev_c == 0)
        {
          //ボールの位置状態によって戻り位置を変える
          switch (pmgSG_->stBa_.Motion)
          {
          case bmFree:
          case bmBound:
            if (st_.pmgMyTm_->st_.pstMyTm_->BallCrtSideL != st_.mysideNo_)
            {
              KgNo = st_.pmgMyTm_->st_.pstMyTm_->BallCrtSideL;
            }
            break;
          case bmHold:
            if (pmgSG_->stBa_.HoldTNo != st_.mysideNo_)
            {
              KgNo = pmgSG_->stBa_.HoldPNo;
            }
            break;
          case bmPass:
            if (pmgSG_->stBa_.PichTNo != st_.mysideNo_)
            {
              KgNo = pmgSG_->stBa_.PaTgPNo;//PichPNo;
            }
            break;
          }

          st_.pstMyCh_->KagamiRevKgNo = KgNo;

          st_.pstMyCh_->NextAuto.ADashStoped_f = FALSE;
        }
        else
        {
          KgNo = st_.pstMyCh_->KagamiRevKgNo;
        }


        IsReturnLine(crtRLine, (DBCRT_CLXL + KGTGWIDHT + KgX + KgW));
        IsReturnLine(crtLLine, (DBCRT_CLXL - KGTGWIDHT + KgX - KgW));
        IsReturnLine(crtFLine, (DBCRT_CLZ - KGTGDEPTH + KgZ - KgD));
        IsReturnLine(crtBLine, (DBCRT_CLZ + KGTGDEPTH + KgZ + KgD));
        SetAMuki(1);

        //もう戻りではない
        if (st_.pstMyCh_->NextAuto.AutoType != dbatReturn)
        {
          st_.pstMyCh_->KagamiRev_c = RankHP(rkBreathRev);
        }

        //カウンタを減らしても良い状態か
        BOOL revMotion_f = (
          (st_.pstMyCh_->Motion.Mtype == dbmtDs)
          || (st_.pstMyCh_->Motion.Mtype == dbmtWk)
          || (st_.pstMyCh_->Motion.Mtype == dbmtSt));

        //コート内に入ったらかがみまでのカウンタだけ中央によれる

        if (pCommon_->IsInCourt() && revMotion_f)
        {
          ++st_.pstMyCh_->KagamiRev_c;
        }
      }
      break;
    }

    if (st_.pstMyCh_->GaiyaOvL_f
      && ((st_.pstMyCh_->NextAuto.AutoType != dbatReturn) || (tGOvL_f == FALSE)))
    {
      st_.pstMyCh_->GaiyaOvL_f = FALSE;
    }

  }

  //タイトルデモ用帰宅
  void TChMove::CheckGohome()
  {
    const s32 HOMEZ = (8 * XYMAG);

    st_.pstMyCh_->NextAuto.AutoType = dbatGohome;

    st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
      ? maL
      : maR;

    if (st_.pstMyCh_->Zahyou.Z < (DBCRT_BL - HOMEZ))
    {
      st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
    }
    else if (st_.pstMyCh_->Zahyou.Z > (DBCRT_BL + HOMEZ))
    {
      st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
    }
  }

  //パスカバー
  void TChMove::CheckPassCover()
  {
    //かがみ予約でないこと
    if (Kagami_f()) return;

    //空中でない
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)) return;

    //ボールがパス状態である
    if (pmgSG_->stBa_.Motion != bmPass) return;

    //他の任務があるときはそちらを優先
    if ((st_.pstMyCh_->NextAuto.AutoType == dbatFree)
      || (st_.pstMyCh_->NextAuto.AutoType == dbatDfPos)
      || (st_.pstMyCh_->NextAuto.AutoType == dbatPassCvr))
    {

      const s32 STOPDIST = 5 * XYMAG;
      const s32 STOPDIST2 = 32 * XYMAG;
      const s32 STOPDIST3 = 3 * XYMAG;

      const s32 STOPDISTZ = 5 * XYMAG;
      const s32 STOPDISTZ2 = 16 * XYMAG;
      const s32 STOPDISTZ3 = 3 * XYMAG;
      const s32 CY = 48 * XYMAG;

      s32 tDist;
      s32 tDistZ;

      //ラインをまたぐとき
      if (pmgSG_->stBa_.LandLine_f)
      {
        tDist = STOPDIST3;
        tDistZ = STOPDISTZ3;
      }
      //パスタゲに操作権が委譲される前か後か
      else if ((pmgSG_->stBa_.PaTgPNo == st_.pmgMyTm_->st_.pstMyTm_->CtrlNo)
        || (st_.pmgMyTm_->st_.pstMyTm_->CvrNo == st_.pmgMyTm_->st_.pstMyTm_->CtrlNo))
      {
        tDist = STOPDIST;
        tDistZ = STOPDISTZ;
      }
      else
      {
        tDist = STOPDIST2;
        tDistZ = STOPDISTZ2;
      }
      s32 adsf = st_.posNo_;

      if ((pmgSG_->stBa_.LandXLine + tDist) < st_.pstMyCh_->Zahyou.X)
      {
        st_.pstMyCh_->NextAuto.AMuki = maL;
      }
      else if (st_.pstMyCh_->Zahyou.X < (pmgSG_->stBa_.LandXLine - tDist))
      {
        st_.pstMyCh_->NextAuto.AMuki = maR;
      }

      if ((pmgSG_->stBa_.LandZLine + tDistZ) < st_.pstMyCh_->Zahyou.Z)
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
      }
      else if (st_.pstMyCh_->Zahyou.Z < (pmgSG_->stBa_.LandZLine - tDistZ))
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
      }

      //ダッシュ予約
      if ((st_.pstMyCh_->BallLandLineDist > DASHDIST)
        && (abs(st_.pstMyCh_->Zahyou.Z - pmgSG_->stBa_.LandZLine) < DASHDISTZ))//Ｚ軸が離れすぎているときはダッシュしない
      {
        st_.pstMyCh_->NextAuto.ADash_f = TRUE;
      }
      else
      {
        st_.pstMyCh_->NextAuto.ADash_f = FALSE;
      }

      //操作権が移っていて、ラインをまたぐ予定で、軌道に入っているとき
      if ((st_.pstMyCh_->NextAuto.AMuki == maN)
        && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN)
        && (pmgSG_->stBa_.PaTgPNo == st_.pmgMyTm_->st_.pstMyTm_->CtrlNo)
        && pmgSG_->stBa_.LandLine_f
        && (pmgSG_->stBa_.Zahyou.Y > CY)
        && IsCheckPaCvJp())
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatPassJp;
      }
      else
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatPassCvr;
      }
    }
  }

  //当たらなかったシュートのカバー
  void TChMove::CheckShootCover()
  {
    //ぶーめらん特殊処理


    //空中でない
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)) return;

    //他の任務があるときはそちらを優先
    if ((st_.pstMyCh_->NextAuto.AutoType != dbatFree)
      && (st_.pstMyCh_->NextAuto.AutoType != dbatShCv)) return;


    //シュート
    if (pmgSG_->stBa_.Motion == bmShoot)
    {
      if (pmgSG_->stBa_.PichTNo != st_.mysideNo_) return;

      //味方シュートのみ
      if (pmgSG_->stBa_.PichPNo == st_.posNo_) return;


      if (IsInfield())
      {
        return;

        ////内野での味方内野シュート
        //if (pmgSG_->stBa_.PichPNo < DBMEMBER_INF)
        //{
        //  //ぶーめらんシュート以外、もしくはぶーめらんの進み状態であれば抜ける
        //  if ((pmgSG_->stBa_.ShSt.HSNo != hsBMR)
        //      || (pmgSG_->stBa_.ShSt.BmrStepXZ == 0))
        //  {
        //    return;
        //  }
        //}
      }
      else
      {
        //外野は一応全部追う

        ////追わなくてイイシュートは追わない
        //if (pmgSG_->stBa_.ShSt.HSNo != hsNML)
        //{
        //  if (pmgEO_->mgDt_.dtShot_.GetDtBOOL(pmgSG_->stBa_.ShSt.HSNo, shdtGyShCvNG))
        //  {
        //    return;
        //  }
        //}
      }
    }
    else if (pmgSG_->stBa_.Motion == bmBound)//バウンド
    {
      if (pmgSG_->stBa_.ShootFirstBound_f == FALSE)
      {
        return;
      }
    }
    else
    {
      return;
    }

    ShCoverMove();

  }
  //シュートの斜線上に入る★★
  void TChMove::ShCoverMove()
  {
    TZahyouRec* tag = &(st_.pstMyCh_->Zahyou);
    TZahyouRec* ball = &(pmgSG_->stBa_.Zahyou);
    s32 tagLCX = GetLeftCrtX();

    s32 rtimeX = 0;
    s32 rtimeZ = 0;
    s32 rtimeY = 0;

    BOOL OvTag_f = FALSE;
    BOOL ZShoot_f = (ball->MukiZ != mzN);

    BOOL CvBack_f = FALSE;
    BOOL CvFront_f = FALSE;
    BOOL CvLeft_f = FALSE;
    BOOL CvRight_f = FALSE;

    if (ZShoot_f == FALSE)//横ラインシュート
    {
      if (ball->dX > 0)//右進行
      {
        OvTag_f = (ball->X > tag->X);//的を既に超えた
      }
      else
      {
        OvTag_f = (ball->X < tag->X);//的を既に超えた
      }
    }
    else//縦ラインシュート
    {
      if (ball->dZ > 0)//奥進行
      {
        OvTag_f = (ball->Z > tag->Z);
      }
      else
      {
        OvTag_f = (ball->Z < tag->Z);
      }
    }

    s32 DISTLR = (ZShoot_f)
      ? 4 * XYMAG
      : 4 * XYMAG;
    s32 DISTFB = (ZShoot_f)
      ? 4 * XYMAG
      : 4 * XYMAG;


    //まだタゲを超えていない
    if (OvTag_f == FALSE)
    {
      if (ZShoot_f == FALSE)//横ラインシュート
      {
        st_.pstMyCh_->NextAuto.AMuki = maN;
      }
      else//縦ラインシュート
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
      }

      st_.pstMyCh_->NextAuto.ADash_f = FALSE;


      s32 spdX = (ball->dX != 0)
        ? ball->dX
        : 1;
      //ボールが的の奥を通るか手前を通るか
      s32 distX = abs(ball->X - tag->X);
      rtimeX = (s32)(distX / abs(spdX));

      //手前を通る（奥に避ける）
      CvFront_f = ((ball->Z + (ball->dZ * rtimeX)) <= (tag->Z - DISTFB));
      CvBack_f = ((ball->Z + (ball->dZ * rtimeX)) > (tag->Z + DISTFB));

      s32 spdZ = (ball->dZ != 0)
        ? ball->dZ
        : 1;
      //ボールが的の右を通るか左を通るか
      s32 distZ = abs(ball->Z - tag->Z);
      rtimeZ = (s32)(distZ / abs(spdZ));

      //右を通る（右に動く）
      CvRight_f = ((ball->X + (ball->dX * rtimeZ)) > (tag->X + DISTLR));
      CvLeft_f = ((ball->X + (ball->dX * rtimeZ)) <= (tag->X - DISTLR));


      const s32 STOPDIST = 4 * XYMAG;
      const s32 STOPDISTX = 8 * XYMAG;
      const s32 MVNEEDDISTX = 24 * XYMAG;

      st_.pstMyCh_->NextAuto.AMuki = maN;
      st_.pstMyCh_->NextAuto.AMukiZ = mzaN;

      if ((st_.posNo_ == dbpoO2) || (st_.posNo_ == dbpoO3))
      {
        BOOL zcome_f = ((st_.posNo_ == dbpoO2) && (ball->dZ > XYMAG))
          || ((st_.posNo_ == dbpoO3) && (ball->dZ < -XYMAG));

        //追うべきボールか
        BOOL mvneed_f = (ZShoot_f //縦シュート
          || zcome_f //近づいてくるボール
          || (abs(ball->Z - tag->Z) < MVNEEDDISTX)//Ｚ軸的に近い
          );

        if (mvneed_f)
        {
          //コート超えチェック
          if ((tagLCX < (DBCRT_CL - STOPDIST))
            && (tagLCX > pCommon_->GetSLX2(tag->Z, FALSE) + STOPDIST))
          {
            if (CvLeft_f)
            {
              st_.pstMyCh_->NextAuto.AMuki = maL;
            }
            else if (CvRight_f)
            {
              st_.pstMyCh_->NextAuto.AMuki = maR;
            }
          }
        }
      }
      else
      {

        //コート超えチェック
        if ((tag->Z < (DBCRT_BL - STOPDIST)) && (tag->Z >(DBCRT_FL + STOPDIST)))
        {
          if (CvBack_f)
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
          }
          else if (CvFront_f)
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
          }
        }

      }


      st_.pstMyCh_->NextAuto.AutoType = dbatShCv;

      rtimeY = (ZShoot_f)
        ? rtimeZ
        : rtimeX;

      const s32 CVJPY = 24 * XYMAG;
      const s32 CVJPYMAX = 72 * XYMAG;

      //到着時ボールＹ
      s32 reachbY = 0;

      //ファーストバウンドの時
      if ((pmgSG_->stBa_.Motion == bmBound) &&
        pmgSG_->stBa_.ShootFirstBound_f)
      {
        reachbY = ball->Y
          + (ball->dY * rtimeY)
          - (pmgEO_->mgDt_.dtSet_.GetDtGrv(setBoundGrv) * rtimeY * rtimeY / 2);
      }
      else
      {
        reachbY = (ball->dY > 0)//上昇（バウンドシュートをジャンプしてしまうのを防ぐ
          ? ball->Y
          : ball->Y + (ball->dY * rtimeY);
      }


      //if ((st_.pstMyCh_->NextAuto.AMukiZ == mzaN)//ＸＺは位置まで来てる必要がある
      //  && (st_.pstMyCh_->NextAuto.AMuki == maN)
      if ((reachbY > CVJPY)
        && (reachbY < CVJPYMAX)
        && IsCheckShCvJp())
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatPassJp;
      }

      //止まってから次動くまでのラグ
      if (st_.pstMyCh_->AShCv_c > 0)
      {
        lib_num::AprTo0(&st_.pstMyCh_->AShCv_c);
        //歩き移動
        st_.pstMyCh_->NextAuto.ADash_f = FALSE;
        st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
        st_.pstMyCh_->NextAuto.AMuki = maN;
      }
      else
      {
        //動かなかった
        if ((st_.pstMyCh_->NextAuto.AMukiZ == mzaN)
          && (st_.pstMyCh_->NextAuto.AMuki == maN))
        {
          const s32 CVRNEXT = 30;
          st_.pstMyCh_->AShCv_c = CVRNEXT;
        }
      }
    }

  }



  //アリウープ
  void TChMove::CheckAlleyoopMove()
  {
    //ボールがパス状態である
    if (pmgSG_->stBa_.Motion != bmPass) return;

    //味方パスのみ
    if (pmgSG_->stBa_.PaTgTNo != st_.mysideNo_) return;

    //自分へのパスのみ
    if (pmgSG_->stBa_.PaTgPNo != st_.posNo_) return;

    //アリウープパスである
    if (pmgSG_->stBa_.PaAlPa_f == FALSE) return;

    //他の任務があるときはそちらを優先
    if ((st_.pstMyCh_->NextAuto.AutoType == dbatFree)
      || (st_.pstMyCh_->NextAuto.AutoType == dbatDfPos))
    {

      st_.pstMyCh_->NextAuto.AutoType = dbatAlleyoop;

      st_.pstMyCh_->NextAuto.AMuki = maN;

      //とりあえずポストマンの位置
      const s32 TAGX = POSPOST_X;
      const s32 DISTX = 4 * XYMAG;
      if (GetLeftCrtX() > (TAGX + DISTX))
      {
        if (st_.mysideNo_ == 0)
        {
          st_.pstMyCh_->NextAuto.AMuki = maL;
        }
        else if (st_.mysideNo_ == 1)
        {
          st_.pstMyCh_->NextAuto.AMuki = maR;
        }
      }
      else if (GetLeftCrtX() < (TAGX))
      {
        if (st_.mysideNo_ == 0)
        {
          st_.pstMyCh_->NextAuto.AMuki = maR;
        }
        else if (st_.mysideNo_ == 1)
        {
          st_.pstMyCh_->NextAuto.AMuki = maL;
        }
      }

    }
  }


  //パスキャッチジャンプ
  void TChMove::CheckPassCatchJp()
  {
    //ボールがパス状態である
    if (pmgSG_->stBa_.Motion != bmPass) return;

    //味方パスのみ
    if (pmgSG_->stBa_.PaTgTNo != st_.mysideNo_) return;

    //自分へのパスのみ
    if (pmgSG_->stBa_.PaTgPNo != st_.posNo_) return;

    //ジャンプキャッチが必要な場合のみ
    if (pmgSG_->stBa_.PaJPa_f == FALSE) return;

    //ジャンプ可能状態のみ
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfJpOK) == FALSE) return;

    //他の任務があるときはそちらを優先
    if ((st_.pstMyCh_->NextAuto.AutoType == dbatFree)
      || (st_.pstMyCh_->NextAuto.AutoType == dbatDfPos))
    {
      //ジャンプのタイミング
      if (pmgSG_->stBa_.PaETA_c <= PACAJPTIME)
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatPassJp;
      }
    }
    else if (st_.pstMyCh_->NextAuto.AutoType == dbatAlleyoop)
    {
      //ジャンプのタイミング（トスパスキャッチのときは停止まできていたら）
      if ((st_.pstMyCh_->NextAuto.AMuki == maN)
        && (pmgSG_->stBa_.PaETA_c <= PACAJPTIME))
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatPassJp;
      }
    }
  }



  //向かう先
  void TChMove::SetTagPos(s32 tgX, s32 tgZ)
  {

    const s32 STOPDIST = 4 * XYMAG;
    const s32 STOPDISTX = 8 * XYMAG;

    //歩き移動
    st_.pstMyCh_->NextAuto.ADash_f = FALSE;

    //コート超えチェック
    tgZ = lib_num::Between(tgZ, (DBCRT_BL - STOPDIST), (DBCRT_FL + STOPDIST));
    if (st_.mysideNo_ == 0)
    {
      tgX = lib_num::Between(tgX, (DBCRT_CL - STOPDISTX), (pCommon_->GetSLX2(tgZ, FALSE) + STOPDISTX));
    }
    else
    {
      tgX = lib_num::Between(tgX, (DBCRT_CL + STOPDISTX), (pCommon_->GetSLX2(tgZ, TRUE) - STOPDISTX));
    }

    if ((tgX + STOPDISTX) < st_.pstMyCh_->Zahyou.X)
    {
      st_.pstMyCh_->NextAuto.AMuki = maL;
    }
    else if (st_.pstMyCh_->Zahyou.X < (tgX - STOPDISTX))
    {
      st_.pstMyCh_->NextAuto.AMuki = maR;
    }
    else
    {
      st_.pstMyCh_->NextAuto.AMuki = maN;
    }

    if ((tgZ + STOPDIST) < st_.pstMyCh_->Zahyou.Z)
    {
      st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
    }
    else if (st_.pstMyCh_->Zahyou.Z < (tgZ - STOPDIST))
    {
      st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
    }
    else
    {
      st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
    }

    ////動かなかった
    //if ((st_.pstMyCh_->NextAuto.AMukiZ == mzaN) && (st_.pstMyCh_->NextAuto.AMuki == maN))
    //{
    //  const s32 HELPNEXT = 30;
    //  st_.pmgMyTm_->st_.pstMyTm_->stCOM.nextmv_c = HELPNEXT;
    //}
  }

  //パスカット★★
  void TChMove::CheckPassCut()
  {
    ////パスカット可能の作戦が出ているとき
    //if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.PassCutOK_f == FALSE) return;

    //ボールがパス状態である
    if (pmgSG_->stBa_.Motion != bmPass) return;

    //敵パスのみ
    if (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_) return;

    //内野のみ
    if (IsInfield() == FALSE) return;

    //空中でない
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)) return;


    //他の任務があるときはそちらを優先
    if ((st_.pstMyCh_->NextAuto.AutoType != dbatFree)
      && (st_.pstMyCh_->NextAuto.AutoType != dbatDfPos)) return;


    if ((pmgSG_->stBa_.PaTgPNo < DBMEMBER_INF)
      && (pmgSG_->stBa_.PichPNo < DBMEMBER_INF)) return;

    BOOL PaCut_f = FALSE;

    //パスカット条件
    switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPassCut])
    {
    case 0://クイックパス
      PaCut_f = (pmgSG_->stBa_.PaJPa_f == FALSE);
      break;
    case 1://すべてのパス
      PaCut_f = TRUE;
      break;
    case 2://しない
      PaCut_f = FALSE;
      break;
    default://対面＆クイック
      PaCut_f = ((pmgSG_->stBa_.PaLine != plNml) && (pmgSG_->stBa_.PaJPa_f == FALSE));
      break;
    }


    //外野もしくは内野のパスタゲかつ味方外
    if (PaCut_f)
    {
      st_.pstMyCh_->NextAuto.AutoType = dbatCPUPaCut;

      //とりあえず向かう先（パスタゲ）
      //s32 patgX = st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->st_.pstMyCh_->Zahyou.X;
      //s32 patgZ = st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->st_.pstMyCh_->Zahyou.Z;
      s32 patgX = pmgSG_->stBa_.LandX;
      s32 patgZ = pmgSG_->stBa_.LandZ;
      s32 tgX = patgX;
      s32 tgZ = patgZ;

      const s32 TAGLINEDISTX = 8 * XYMAG;
      const s32 TAGLINEDISTZ = 4 * XYMAG;

      BOOL RevX_f = FALSE;
      BOOL RevZ_f = FALSE;

      //パスタゲ
      switch (pmgSG_->stBa_.PaTgPNo)
      {
      case dbpoO2:
        tgZ = DBCRT_BL - TAGLINEDISTZ;
        RevX_f = TRUE;
        break;
      case dbpoO3:
        tgZ = DBCRT_FL + TAGLINEDISTZ;
        RevX_f = TRUE;
        break;
      case dbpoO4:
        if (st_.mysideNo_ == 0)
        {
          tgX = pCommon_->GetSLX2(tgZ, FALSE) + TAGLINEDISTX;
        }
        else
        {
          tgX = pCommon_->GetSLX2(tgZ, TRUE) - TAGLINEDISTX;
        }
        RevZ_f = TRUE;
        break;
      default://内野
        if (st_.mysideNo_ == 0)
        {
          tgX = DBCRT_CL - TAGLINEDISTX;
        }
        else
        {
          tgX = DBCRT_CL + TAGLINEDISTX;
        }
        RevZ_f = TRUE;
        break;
      }

      //Ｘ位置補正(23番外野へのパス)
      if (RevX_f)
      {
        if (abs(pmgSG_->stBa_.Zahyou.dZ) > 0)
        {
          s32 distZ = (tgZ - patgZ);
          s32 xpz = (pmgSG_->stBa_.Zahyou.dX * XYMAG) / pmgSG_->stBa_.Zahyou.dZ;
          tgX += lib_num::Percent(distZ, xpz);
        }
      }
      else if (RevZ_f)
      {
        if (abs(pmgSG_->stBa_.Zahyou.dX) > 0)
        {
          s32 distX = (tgX - patgX);
          s32 zpx = (pmgSG_->stBa_.Zahyou.dZ * XYMAG) / pmgSG_->stBa_.Zahyou.dX;
          tgZ += lib_num::Percent(distX, zpx);
        }
      }

      //タゲ位置セット
      SetTagPos(tgX, tgZ);

      //軌道に入ってる
      if ((st_.pstMyCh_->NextAuto.AMukiZ == mzaN)
        && (st_.pstMyCh_->NextAuto.AMuki == maN))
      {
        s32 jpcount = 0;

        if (abs(pmgSG_->stBa_.Zahyou.dX) > abs(pmgSG_->stBa_.Zahyou.dZ))
        {
          s32 pichX = st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.PichPNo]->st_.pstMyCh_->Zahyou.X;
          jpcount = (pmgSG_->stBa_.PaETA * (pichX - tgX)) / (pichX - patgX);
        }
        else
        {
          s32 pichZ = st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.PichPNo]->st_.pstMyCh_->Zahyou.Z;
          jpcount = (pmgSG_->stBa_.PaETA * (pichZ - tgZ)) / (pichZ - patgZ);
        }
        jpcount = jpcount - (pmgSG_->stBa_.PaETA - pmgSG_->stBa_.PaETA_c);

        //ジャンプのタイミング（最適のタイミングのみ）
        if (jpcount == PACAJPTIME)
        {
          st_.pstMyCh_->NextAuto.AutoType = dbatPassJp;
        }
      }

    }

  }



  //味方ヘルプ★★
  void TChMove::CheckHelp()
  {
    //ヘルプ可能の作戦が出ているとき
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.help_f == FALSE) return;

    //ボールがパス、シュート、敵持ち状態
    if ((pmgSG_->stBa_.Motion != bmShoot)
      && (pmgSG_->stBa_.Motion != bmHold)
      ) return;

    //敵ボール
    if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_) return;
    if (pmgSG_->stBa_.PichTNo == st_.mysideNo_) return;

    //内野のみ
    if (IsInfield() == FALSE) return;

    //空中でない
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)) return;

    //他の任務があるときはそちらを優先
    if ((st_.pstMyCh_->NextAuto.AutoType != dbatFree)
      && (st_.pstMyCh_->NextAuto.AutoType != dbatDfPos)) return;


    //敵番号
    s32 enNo = NGNUM;
    switch (pmgSG_->stBa_.Motion)
    {
    case bmHold: enNo = pmgSG_->stBa_.HoldPNo; break;
    case bmPass: enNo = pmgSG_->stBa_.PaTgPNo; break;
    case bmShoot: enNo = pmgSG_->stBa_.PichPNo; break;
    default:
      enNo = st_.pmgEnTm_->st_.pstMyTm_->CtrlNo;
      break;
    }
    //嫡が見つからなかった
    if (enNo == NGNUM) return;

    s32 pinchNo = NGNUM;
    //ヘルプ対象がいるか
    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      if (i == st_.posNo_) continue;
      //コート内にいる
      if (st_.pmgMyTm_->st_.pmgMyCh_[i]->chCommon_.IsInCourt() == FALSE) continue;
      //接地してる
      if (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Y != 0) continue;
      //生きてる
      if (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f) continue;
      //ダウンかかがみ
      if ((st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfDn) == FALSE)
        && (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfKG) == FALSE)) continue;

      BOOL help_f = FALSE;
      const s32 PINCHHP = 16;
      //ヘルプ条件
      switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comHelp])
      {
      case 0://対象が瀕死なら
        help_f = (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->HP < PINCHHP);
        break;
      case 1://対象がいれば
        help_f = TRUE;
        break;
      case 2://しない
        help_f = FALSE;
        break;
      default://外野攻撃なら
        help_f = (enNo >= DBMEMBER_INF);
        break;
      }

      if (help_f)
      {
        BOOL break_f = FALSE;

        //ヘルプ対象
        switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comHelpTag])
        {
        case 0://上位オーダー
          if (i < st_.posNo_)
          {
            pinchNo = i;
            break_f = TRUE;//上位から探ってるから
          }
          break;
        case 1://誰でも
          pinchNo = i;
          break_f = TRUE;//上位から探ってるから
          break;
        case 2://ボス
          if (i == st_.pmgMyTm_->st_.pstMyTm_->GetRdPos())
          {
            pinchNo = i;
            break_f = TRUE;
          }
          break;
        default://上位ラインナップ
          if (pinchNo == NGNUM)
          {
            if (st_.pmgMyTm_->st_.pstMyTm_->PosSt.CharofP[i] < st_.pmgMyTm_->st_.pstMyTm_->PosSt.CharofP[st_.posNo_])
            {
              pinchNo = i;
            }
          }
          else
          {
            if (st_.pmgMyTm_->st_.pstMyTm_->PosSt.CharofP[i] < st_.pmgMyTm_->st_.pstMyTm_->PosSt.CharofP[pinchNo])
            {
              pinchNo = i;
            }
          }
          break;
        }

        if (break_f) break;
      }
    }

    //いない
    if (pinchNo == NGNUM) return;

    st_.pstMyCh_->NextAuto.AutoType = dbatCPUHelp;

    //とりあえず向かう先（ヘルプ対象）
    s32 hptgX = st_.pmgMyTm_->st_.pmgMyCh_[pinchNo]->st_.pstMyCh_->Zahyou.X;
    s32 hptgZ = st_.pmgMyTm_->st_.pmgMyCh_[pinchNo]->st_.pstMyCh_->Zahyou.Z;//st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->st_.pstMyCh_->Zahyou.Z;
    s32 enX = st_.pmgEnTm_->st_.pmgMyCh_[enNo]->st_.pstMyCh_->Zahyou.X;
    s32 enZ = st_.pmgEnTm_->st_.pmgMyCh_[enNo]->st_.pstMyCh_->Zahyou.Z;


    //ヘルプタゲと敵との角度
    s32 distX = (enX - hptgX);
    s32 distZ = (enZ - hptgZ);
    s32 base = abs(distX * distZ);

    s32 tgX = hptgX;
    s32 tgZ = hptgZ;

    const s32 TAGLDIST = 16 * XYMAG;

    if ((hptgX + TAGLDIST) < pmgSG_->stBa_.Zahyou.X)
    {
      tgX += TAGLDIST;
    }
    else if ((hptgX - TAGLDIST) > pmgSG_->stBa_.Zahyou.X)
    {
      tgX -= TAGLDIST;
    }

    if ((hptgZ + TAGLDIST) < pmgSG_->stBa_.Zahyou.Z)
    {
      tgZ += TAGLDIST;
    }
    else if ((hptgZ - TAGLDIST) > pmgSG_->stBa_.Zahyou.Z)
    {
      tgZ -= TAGLDIST;
    }


    //if (base > 0)
    //{
    //  const s32 TAGLDIST = 16 * XYMAG;

    //  tgX += ((TAGLDIST * distX) / base);
    //  tgZ += ((TAGLDIST * distZ) / base);
    //}

    //タゲ位置セット
    SetTagPos(tgX, tgZ);

  }

  //味方ガード★★
  void TChMove::CheckGuard()
  {

    //ヘルプ可能の作戦が出ているとき
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.help_f == FALSE) return;

    //ボールがパス、シュート、敵持ち状態
    if ((pmgSG_->stBa_.Motion != bmShoot)
      && (pmgSG_->stBa_.Motion != bmHold)
      //&& (pmgSG_->stBa_.Motion != bmPass)
      ) return;

    //敵ボール
    if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_) return;
    if (pmgSG_->stBa_.PichTNo == st_.mysideNo_) return;

    //内野のみ
    if (IsInfield() == FALSE) return;

    //空中でない
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)) return;

    //他の任務があるときはそちらを優先(ヘルプの方が優先)
    if ((st_.pstMyCh_->NextAuto.AutoType != dbatFree)
      && (st_.pstMyCh_->NextAuto.AutoType != dbatDfPos)) return;

    //敵番号
    s32 enNo = NGNUM;
    switch (pmgSG_->stBa_.Motion)
    {
    case bmHold: enNo = pmgSG_->stBa_.HoldPNo; break;
    case bmPass: enNo = pmgSG_->stBa_.PaTgPNo; break;
    case bmShoot: enNo = pmgSG_->stBa_.PichPNo; break;
    default:
      enNo = st_.pmgEnTm_->st_.pstMyTm_->CtrlNo;
      break;
    }
    //嫡が見つからなかった
    if (enNo == NGNUM) return;

    s32 gdNum = 0;
    s32 gdXadd = 0;
    s32 gdZadd = 0;

    //ガード対象がいるか
    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      if (i == st_.posNo_) continue;
      //コート内にいる
      if (st_.pmgMyTm_->st_.pmgMyCh_[i]->chCommon_.IsInCourt() == FALSE) continue;
      //接地してる
      if (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Y != 0) continue;
      //生きてる
      if (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f) continue;



      BOOL gdngpos_f = FALSE;
      //敵のポジション位置からある程度距離ある人のみ守る
      switch (enNo)
      {
      case dbpoO2:
        gdngpos_f = (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z > DBCRT_CLZ);
        break;
      case dbpoO3:
        gdngpos_f = (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z < DBCRT_CLZ);
        break;
      case dbpoO4:
        gdngpos_f = (st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX() < DBCRT_CLXL);
        break;
      default://内野
        gdngpos_f = (st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX() > DBCRT_CLXL);
        break;
      }

      if (gdngpos_f) continue;


      //ガード条件
      BOOL guard_f = FALSE;//条件
      BOOL guard_f2 = FALSE;//対象
      BOOL guard_f3 = FALSE;//対コール

      const s32 PINCHHP = 16;

      //ヘルプ条件
      switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comGuard])
      {
      case 0://対象が瀕死なら
        guard_f = (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->HP < PINCHHP);
        break;
      case 1://対象がいれば
        guard_f = TRUE;
        break;
      case 2://しない
        guard_f = FALSE;
        break;
      default://外野攻撃なら
        guard_f = (enNo >= DBMEMBER_INF);
        break;
      }

      //ヘルプ対象
      switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comGuardTag])
      {
      case 0://上位オーダー
        guard_f2 = (i < st_.posNo_);
        break;
      case 1://誰でも
        guard_f2 = TRUE;
        break;
      case 2://ボス
        guard_f2 = (i == st_.pmgMyTm_->st_.pstMyTm_->GetRdPos());
        break;
      default://上位ラインナップ
        guard_f2 = (st_.pmgMyTm_->st_.pstMyTm_->PosSt.CharofP[i] < st_.pmgMyTm_->st_.pstMyTm_->PosSt.CharofP[st_.posNo_]);
        break;
      }


      ////コールに対するガード
      //if (st_.pmgMyTm_->st_.pstMyTm_->COMCall.CallingGd_f[i])
      //{
      //  s32 cgsw = st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaOdPaTag];//★適切な条件
      //  
      //  switch (cgsw)
      //  {
      //    case 0://オーダーが上（自分の方が大きい）
      //      guard_f3 = (st_.posNo_ > i);
      //      break;
      //    case 1://学年が上（自分の方が小さい）
      //      guard_f3 = (st_.pmgMyTm_->st_.pmgMyCh_[st_.posNo_]->MyPDt(clv_Gakunen)
      //                  <= st_.pmgMyTm_->st_.pmgMyCh_[i]->MyPDt(clv_Gakunen));
      //      break;
      //    case 2://ボスなら
      //      guard_f3 = (i == st_.pmgMyTm_->st_.pstMyTm_->GetRdPos());
      //      break;
      //    default://ラインナップが上（自分の方が大きい）
      //      guard_f3 = (st_.pmgMyTm_->st_.pstMyTm_->PosSt.CharofP[st_.posNo_]
      //                  > st_.pmgMyTm_->st_.pstMyTm_->PosSt.CharofP[i]);
      //      break;
      //  }
      //}

      if ((guard_f && guard_f2) || guard_f3)
      {
        //ガードする座標
        gdXadd += st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X;
        gdZadd += st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z;
        ++gdNum;
      }

    }

    //いない
    if (gdNum == 0) return;

    st_.pstMyCh_->NextAuto.AutoType = dbatCPUHelp;//扱いはヘルプ

    //とりあえず向かう先（ガード対象）
    s32 gdtgX = (gdXadd / gdNum);//重心
    s32 gdtgZ = (gdZadd / gdNum);
    s32 enX = st_.pmgEnTm_->st_.pmgMyCh_[enNo]->st_.pstMyCh_->Zahyou.X;
    s32 enZ = st_.pmgEnTm_->st_.pmgMyCh_[enNo]->st_.pstMyCh_->Zahyou.Z;

    s32 tgX = gdtgX;
    s32 tgZ = gdtgZ;


    //敵のポジション
    switch (enNo)
    {
    case dbpoO2:
      tgZ = ((gdtgZ + DBCRT_BL) / 2);
      tgX = ((enX + gdtgX) / 2);
      break;
    case dbpoO3:
      tgZ = ((gdtgZ + DBCRT_FL) / 2);
      tgX = ((enX + gdtgX) / 2);
      break;
    case dbpoO4:
      tgZ = ((enZ + gdtgZ) / 2);
      tgX = ((gdtgX + pCommon_->GetSLX2(tgZ, (st_.mysideNo_ == 1))) / 2);
      break;
    default://内野
      tgZ = ((enZ + gdtgZ) / 2);
      tgX = ((gdtgX + DBCRT_CL) / 2);
      break;
    }

    //タゲ位置セット
    SetTagPos(tgX, tgZ);

  }

  void TChMove::CheckCOMHelpGuard(BOOL ctrl_f)
  {



    //COM一定の動きする状態
    if (st_.pstMyCh_->stCOM.COMMv_c > 0)
    {

      //別の任務が入ったらCOMMv_cを空に
      if ((st_.pstMyCh_->NextAuto.AutoType != dbatFree)
        && (st_.pstMyCh_->NextAuto.AutoType != dbatDfPos))
      {
        st_.pstMyCh_->stCOM.COMMv_c = 0;
        return;
      }
      //もうガードとキャッチ不要
      else if (st_.pstMyCh_->Auto.AutoType == dbatCPUHelp)//前フレがヘルプ
      {
        if ((pmgSG_->stBa_.Motion != bmShoot)
          && (pmgSG_->stBa_.Motion != bmHold))
        {
          st_.pstMyCh_->stCOM.COMMv_c = 0;
          return;
        }
      }
      else if (st_.pstMyCh_->Auto.AutoType == dbatCPUCa)//前フレがキャッチ
      {
        if (pmgSG_->stBa_.Motion != bmShoot)
        {
          st_.pstMyCh_->stCOM.COMMv_c = 0;
          return;
        }
      }
      else if (st_.pstMyCh_->Auto.AutoType == dbatGet)//前フレがボール拾い
      {
        if (pmgSG_->stBa_.Motion != bmBound)
        {
          st_.pstMyCh_->stCOM.COMMv_c = 0;
          return;
        }
      }
      else if (ctrl_f == FALSE)//操作権用でなければヘルプガードはしない
      {
        if (st_.pstMyCh_->Auto.AutoType == dbatCPUHelp)
        {
          st_.pstMyCh_->stCOM.COMMv_c = 0;
          return;
        }
      }


      //不要でない場合は継続
      s32 STOPDIST = 4 * XYMAG;//コート超えチェック
      s32 STOPDISTX = 10 * XYMAG;

      if ((st_.pstMyCh_->Zahyou.Z > (DBCRT_BL - STOPDIST))
        || (st_.pstMyCh_->Zahyou.Z < (DBCRT_FL + STOPDIST)))
      {
        st_.pstMyCh_->stCOM.COMMvAMukiZ = mzaN;
      }

      if (st_.mysideNo_ == 0)
      {
        if ((st_.pstMyCh_->Zahyou.X >(DBCRT_CL - STOPDISTX))
          || (st_.pstMyCh_->Zahyou.X < (pCommon_->GetSLX2(st_.pstMyCh_->Zahyou.Z, FALSE) + STOPDISTX)))
        {
          st_.pstMyCh_->stCOM.COMMvAMuki = maN;
        }
      }
      else
      {
        if ((st_.pstMyCh_->Zahyou.X < (DBCRT_CL + STOPDISTX))
          || (st_.pstMyCh_->Zahyou.X >(pCommon_->GetSLX2(st_.pstMyCh_->Zahyou.Z, TRUE) - STOPDISTX)))
        {
          st_.pstMyCh_->stCOM.COMMvAMuki = maN;
        }
      }

      st_.pstMyCh_->NextAuto.AutoType = st_.pstMyCh_->Auto.AutoType;
      //歩き移動
      st_.pstMyCh_->NextAuto.AMukiZ = st_.pstMyCh_->stCOM.COMMvAMukiZ;
      st_.pstMyCh_->NextAuto.AMuki = st_.pstMyCh_->stCOM.COMMvAMuki;



      if (lib_num::AprTo0(&st_.pstMyCh_->stCOM.COMMv_c))
      {
        //キャッチで動きがあったとき
        if ((st_.pstMyCh_->NextAuto.AutoType == dbatCPUCa)
          && ((st_.pstMyCh_->stCOM.COMMvAMuki != maN) || (st_.pstMyCh_->stCOM.COMMvAMukiZ != mzaN)))
        {

          //ボールと逆向きを向いていなければ
          BOOL gyakumuki_f = ((st_.pstMyCh_->Zahyou.Muki == mR) && (pmgSG_->stBa_.Zahyou.dX > 0))
            || ((st_.pstMyCh_->Zahyou.Muki == mL) && (pmgSG_->stBa_.Zahyou.dX < 0))
            || ((st_.pstMyCh_->Zahyou.MukiZ == mzB) && (pmgSG_->stBa_.Zahyou.dZ > 0))
            || ((st_.pstMyCh_->Zahyou.MukiZ == mzF) && (pmgSG_->stBa_.Zahyou.dZ < 0));

          if (gyakumuki_f == FALSE)
          {
            //止める
            st_.pstMyCh_->stCOM.COMMv_c = 30;
            st_.pstMyCh_->stCOM.COMMvAMukiZ = mzaN;
            st_.pstMyCh_->stCOM.COMMvAMuki = maN;
          }
        }
      }
    }
    else
    {
      if (ctrl_f)
      {
        CheckHelp();
        CheckGuard();
        CheckDodgeCPU();
      }
      else
      {
        CheckDodge();
      }

      if (st_.pstMyCh_->NextAuto.AutoType == dbatCPUHelp)
      {
        if ((st_.pstMyCh_->NextAuto.AMuki == maN)
          && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN))
        {
          st_.pstMyCh_->stCOM.COMMv_c = 30;
        }
        else
        {
          st_.pstMyCh_->stCOM.COMMv_c = 0;
        }

        st_.pstMyCh_->stCOM.COMMvAMuki = st_.pstMyCh_->NextAuto.AMuki;
        st_.pstMyCh_->stCOM.COMMvAMukiZ = st_.pstMyCh_->NextAuto.AMukiZ;
      }
      else if (st_.pstMyCh_->NextAuto.AutoType == dbatCPUCa)
      {
        if ((st_.pstMyCh_->NextAuto.AMuki == maN)
          && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN))
        {
          st_.pstMyCh_->stCOM.COMMv_c = 30;
        }
        else
        {
          st_.pstMyCh_->stCOM.COMMv_c = 15;
        }

        st_.pstMyCh_->stCOM.COMMvAMuki = st_.pstMyCh_->NextAuto.AMuki;
        st_.pstMyCh_->stCOM.COMMvAMukiZ = st_.pstMyCh_->NextAuto.AMukiZ;
      }
      else if (st_.pstMyCh_->NextAuto.AutoType == dbatDgSw)
      {
        st_.pstMyCh_->stCOM.COMMv_c = 30;
        st_.pstMyCh_->stCOM.COMMvAMuki = st_.pstMyCh_->NextAuto.AMuki;
        st_.pstMyCh_->stCOM.COMMvAMukiZ = st_.pstMyCh_->NextAuto.AMukiZ;
      }

    }
  }



  //パスキャッチモーション
  void TChMove::CheckPassCatchMotion()
  {

    //ボールがパス状態である
    if (pmgSG_->stBa_.Motion != bmPass) return;

    //味方パスのみ
    if (pmgSG_->stBa_.PaTgTNo != st_.mysideNo_) return;

    //自分へのパスのみ
    if (pmgSG_->stBa_.PaTgPNo != st_.posNo_) return;

    //到着時間が設定されてない
    if (pmgSG_->stBa_.PaETA_c == NGNUM) return;

    //パス届くまで８フレ以内でキャッチモーション
    const s32 PCMTIMING = 8;
    if (pmgSG_->stBa_.PaETA_c > PCMTIMING) return;

    //他の任務があるときはそちらを優先
    if ((st_.pstMyCh_->NextAuto.AutoType == dbatFree)
      || (st_.pstMyCh_->NextAuto.AutoType == dbatPassJp)
      || (st_.pstMyCh_->NextAuto.AutoType == dbatDfPos)
      //|| (st_.pstMyCh_->NextAuto.AutoType == dbatPassCach)
      )
    {
      //モーションの変更はココでやっていいのか？
      if (pmgSG_->stBa_.PaJPa_f)
      {
        if ((st_.pstMyCh_->Motion.Mtype == dbmtJUp)
          || (st_.pstMyCh_->Motion.Mtype == dbmtJDn))
        {
          st_.pstMyCh_->NextAuto.AutoType = dbatPassCach;
        }
      }
      else
      {
        if (st_.pstMyCh_->Motion.Mtype == dbmtSt)
        {
          st_.pstMyCh_->NextAuto.AutoType = dbatPassCach;
        }
      }
    }
  }



  BOOL TChMove::IsCheckPosX(s32 tPX, s32 dist)
  {
    return ((tPX + dist) < st_.pstMyCh_->Zahyou.X) || (st_.pstMyCh_->Zahyou.X < (tPX - dist));
  }
  BOOL TChMove::IsCheckPosZ(s32 tPZ, s32 dist)
  {
    return ((tPZ + dist) < st_.pstMyCh_->Zahyou.Z) || (st_.pstMyCh_->Zahyou.Z < (tPZ - dist));
  }
  //ポジショニング種類
  void TChMove::CheckMovePosType()
  {
    //ボール無しのみ
    //if (IsBall()) return;

    //カウンター状態以外
    if (st_.pstMyCh_->COMCounter_f && IsBall()) return;

    //操作キャラ以外のみ
    if (IsSelfCtrl())return;

    //自由状態のみ
    if (st_.pstMyCh_->NextAuto.AutoType != dbatFree) return;

    //ジャンプ不可の時もダメ
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfJpOK) == FALSE) return;



    //ボールが味方パス状態以外//パスタゲ移動不可
    if ((pmgSG_->stBa_.Motion == bmPass)
      && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
      && (pmgSG_->stBa_.PaTgPNo == st_.posNo_)) return;


    const s32 STOPDIST_POS2 = 5 * XYMAG;
    const s32 STOPDIST_POS = 2 * XYMAG;


    //シュートの時あまりにも不具合が多いので
    //切ってみる
    //敵のシュートの時は動かない
    if ((pmgSG_->stBa_.Motion == bmShoot)
      && (pmgSG_->stBa_.PichTNo != st_.mysideNo_)) return;

    ////自分がボールを持っている
    //if ((pmgSG_->stBa_.Motion == bmHold)
    //  && (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
    //  && (pmgSG_->stBa_.HoldPNo == st_.posNo_)) return;


    switch (st_.posNo_)
    {
    case (s32)dbpoO2:
      if (IsCheckPosX(st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCX, STOPDIST_POS2))
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatDfPos;
      }
      break;
    case (s32)dbpoO3:
      if (IsCheckPosX(st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCX3, STOPDIST_POS2))
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatDfPos;
      }
      break;
    case (s32)dbpoO4:
      if (IsCheckPosZ(st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCZ, STOPDIST_POS2))
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatDfPos;
      }
      break;
    default:
      //内野ディフェンスポジショニング
      st_.pstMyCh_->NextAuto.AutoType = dbatDfPos;
      break;
    }

  }


  void TChMove::MoveDfPos()
  {

    s32 PW_X = 0;
    s32 PW_Z = 0;

    //ライン
    BOOL IsO2O3_f = ((st_.pmgMyTm_->st_.pstMyTm_->PosMove.InfLine == iltO2)
      || (st_.pmgMyTm_->st_.pstMyTm_->PosMove.InfLine == iltO3));

    st_.pstMyCh_->NextAuto.ADash_f = FALSE;

    //新フォーメーション
    st_.pmgMyTm_->GetFomposOfInf(PW_X, PW_Z, st_.posNo_);


    const s32 STOPDIST = 2 * XYMAG;

    //内野はセンターラインまで寄る際にZ移動もしてしまうのでアバウトにしないとがたつく
    if ((PW_Z + STOPDIST) < st_.pstMyCh_->Zahyou.Z)
    {
      st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
    }
    else if (st_.pstMyCh_->Zahyou.Z < (PW_Z - STOPDIST))
    {
      st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
    }
    else
    {
      st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
    }

    if (GetLeftCrtX() < (PW_X - STOPDIST))
    {
      st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
        ? maR
        : maL;

      //ダッシュ予約
      st_.pstMyCh_->NextAuto.ADash_f = ((GetLeftCrtX() < (PW_X - DASHDIST))
        && (abs(st_.pstMyCh_->Zahyou.Z - PW_Z) < DASHDISTZ));
    }
    else if (GetLeftCrtX() > (PW_X + STOPDIST))
    {
      st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
        ? maL
        : maR;

      //ダッシュ予約
      st_.pstMyCh_->NextAuto.ADash_f = ((GetLeftCrtX() > (PW_X + DASHDIST))
        && (abs(st_.pstMyCh_->Zahyou.Z - PW_Z) < DASHDISTZ));
    }
    else
    {
      st_.pstMyCh_->NextAuto.AMuki = maN;
    }
  }

  //ポジショニング向き
  void TChMove::CheckMovePosMuki_InfDfPos()
  {



    BOOL postman_f = FALSE;

    //自分以外の味方外野がボールを持っている
    if ((pmgSG_->stBa_.Motion == bmHold)
      && (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
      && (pmgSG_->stBa_.HoldPNo >= DBMEMBER_INF))
    {
      postman_f = TRUE;
    }
    else if ((pmgSG_->stBa_.Motion == bmPass)//外野がタゲのパス
      && (pmgSG_->stBa_.PichTNo == st_.mysideNo_)
      && (pmgSG_->stBa_.PaTgPNo >= DBMEMBER_INF))
    {
      postman_f = TRUE;
    }
    else if ((pmgSG_->stBa_.Motion == bmShoot)//外野のシュート
      && (pmgSG_->stBa_.PichTNo == st_.mysideNo_)
      && (pmgSG_->stBa_.PichPNo >= DBMEMBER_INF))
    {
      postman_f = TRUE;
    }


    //ポストマン候補のとき
    postman_f &= (st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman == st_.posNo_);

    //ポストマン
    if (postman_f)
    {
      SetNextAuto_InfPosOf(POSPOST_X, POSPOST_Z);
    }
    else
    {
      MoveDfPos();

    }

  }

  void TChMove::SetNextAuto_InfPosOf(s32 tPX, s32 tPZ)
  {
    const s32 STOPDIST2 = 3 * XYMAG;
    if ((tPX + STOPDIST2) < GetLeftCrtX())
    {
      st_.pstMyCh_->NextAuto.AMuki = maL;
    }
    else if (GetLeftCrtX() < (tPX - STOPDIST2))
    {
      st_.pstMyCh_->NextAuto.AMuki = maR;
    }

    if (st_.mysideNo_ == 1)
    {
      if (st_.pstMyCh_->NextAuto.AMuki == maL)
      {
        st_.pstMyCh_->NextAuto.AMuki = maR;
      }
      else if (st_.pstMyCh_->NextAuto.AMuki == maR)
      {
        st_.pstMyCh_->NextAuto.AMuki = maL;
      }
    }

    if ((tPZ + STOPDIST2) < st_.pstMyCh_->Zahyou.Z)
    {
      st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
    }
    else if (st_.pstMyCh_->Zahyou.Z < (tPZ - STOPDIST2))
    {
      st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
    }
  }

  //TChMove::外野ポジショニング
  void TChMove::CheckMovePosMuki_GaiyaPos()
  {
    const s32 STOPDIST = 4 * XYMAG;

    st_.pstMyCh_->NextAuto.ADash_f = FALSE;

    switch (st_.posNo_)
    {
    case (s32)dbpoO2:
      if (st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCX < st_.pstMyCh_->Zahyou.X)
      {
        st_.pstMyCh_->NextAuto.AMuki = maL;
      }
      else if (st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCX > st_.pstMyCh_->Zahyou.X)
      {
        st_.pstMyCh_->NextAuto.AMuki = maR;
      }

      //離れているときはダッシュ
      st_.pstMyCh_->NextAuto.ADash_f = ((PaTag_f() == FALSE)
        && (abs(st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCX - st_.pstMyCh_->Zahyou.X) > DASHDIST));

      break;
    case (s32)dbpoO3:
      if (st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCX3 < st_.pstMyCh_->Zahyou.X)
      {
        st_.pstMyCh_->NextAuto.AMuki = maL;
      }
      else if (st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCX3 > st_.pstMyCh_->Zahyou.X)
      {
        st_.pstMyCh_->NextAuto.AMuki = maR;
      }

      //離れているときはダッシュ
      st_.pstMyCh_->NextAuto.ADash_f = ((PaTag_f() == FALSE)
        && (abs(st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCX3 - st_.pstMyCh_->Zahyou.X) > DASHDIST));

      break;
    case (s32)dbpoO4:
      if (st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCZ < st_.pstMyCh_->Zahyou.Z)
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
      }
      else if (st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCZ > st_.pstMyCh_->Zahyou.Z)
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
      }
      break;
    }
  }

  //ポジショニング
  void TChMove::CheckMovePos()
  {
    //ポジショニング判定
    CheckMovePosType();

    if (st_.pstMyCh_->NextAuto.AutoType == dbatDfPos)
    {
      if (IsInfield())
      {
        CheckMovePosMuki_InfDfPos();//内野ディフェンスポジション
      }
      else
      {
        CheckMovePosMuki_GaiyaPos();//外野ポジショニング
      }
    }
  }

  //ダッシュマン★
  void TChMove::CheckDashman()
  {

    BOOL mycall_f = st_.pmgMyTm_->st_.pstMyTm_->IsDashmanCall(st_.posNo_);

    //ダッシュマン発動
    if (st_.pstMyCh_->NextAuto.AutoType == dbatDashman)
    {

    }
    else if (st_.pstMyCh_->NextAuto.AutoType != dbatDashman)
    {
      //FreememかPosの時/空中以外
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
      {
        return;
      }

      if (IsInfield() == FALSE) return;

      //他の任務があるときはそちらを優先
      if ((st_.pstMyCh_->NextAuto.AutoType != dbatFree)
        && (st_.pstMyCh_->NextAuto.AutoType != dbatAlleyoop)
        && (st_.pstMyCh_->NextAuto.AutoType != dbatDfPos)
        && (st_.pstMyCh_->NextAuto.AutoType != dbatShCv)
        && (st_.pstMyCh_->NextAuto.AutoType != dbatPassCvr)
        && (st_.pstMyCh_->NextAuto.AutoType != dbatDashman)//←こないけど
        )
      {
        return;
      }

      BOOL bmdm_f = st_.pmgMyTm_->st_.pstMyTm_->IsBMDashmanCall(st_.posNo_);

      //ボールマンダッシュ出来なかった時点でフラグたたむ
      if (bmdm_f)
      {
        if (pCommon_->IsBMDashOK() == FALSE)
        {
          //不成立
          st_.pmgMyTm_->st_.pstMyTm_->DashmanNoBM = NGNUM;
          bmdm_f = FALSE;
        }
      }

      //ダッシュマン指示がでているか//ボールマン→ダッシュマンフラグのときも（このときはダッシュチェック）
      if (bmdm_f
        || st_.pmgMyTm_->st_.pstMyTm_->IsDashmanCall(st_.posNo_))
      {

        st_.pstMyCh_->NextAuto.AutoType = dbatDashman;

        //Ｚ向かう先
        const s32 TAGLINEDIST = 4 * XYMAG;

        //ダッシュマン番号決まってないとき
        if (st_.pstMyCh_->DashmanNo == NGNUM)
        {
          st_.pstMyCh_->DashmanNo = st_.pmgMyTm_->st_.pstMyTm_->DashmanCall_c;

          if (bmdm_f == FALSE)//ボールマンダッシュでなければ
          {
            //st_.pstMyCh_->Dashman_NT_f = TRUE;//ダッシュマンに初めてタイミングでは触ってない

            ++st_.pmgMyTm_->st_.pstMyTm_->COMDt.dmcalledNum;//ＣＯＭ用発動カウンタふやす
          }


          lib_num::IncLoop(&st_.pmgMyTm_->st_.pstMyTm_->DashmanCall_c, 0, 2);
        }

        //セッターは一度は触ってる
        if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterNo == st_.posNo_)
        {
          //st_.pstMyCh_->Dashman_NT_f = FALSE;//考えたらこのフラグ倒すとセッター走る意味がない
          st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterBMRsvGo_f = FALSE;//セッターダッシュフラグをたたむ
        }


        //本来のポジション
        s32 getx = 0;
        s32 getz = 0;
        st_.pmgMyTm_->GetFomposOfInf(getx, getz, st_.posNo_);


        switch (st_.pmgMyTm_->st_.pstMyTm_->DashmanType)
        {
        case dmtUp://Ｌ
          switch (st_.pstMyCh_->DashmanNo)
          {
          case 0: st_.pstMyCh_->DashmanTgZ = (DBCRT_BL - TAGLINEDIST); break;
          case 1: st_.pstMyCh_->DashmanTgZ = (DBCRT_FL + TAGLINEDIST); break;
          case 2: st_.pstMyCh_->DashmanTgZ = (DBCRT_CLZ); break;
          default: st_.pstMyCh_->DashmanTgZ = st_.pstMyCh_->Zahyou.Z; break;
          }
          break;
        case dmtDw://Ｒ
          switch (st_.pstMyCh_->DashmanNo)
          {
          case 0: st_.pstMyCh_->DashmanTgZ = (DBCRT_FL + TAGLINEDIST); break;
          case 1: st_.pstMyCh_->DashmanTgZ = (DBCRT_BL - TAGLINEDIST); break;
          case 2: st_.pstMyCh_->DashmanTgZ = (DBCRT_CLZ); break;
          default: st_.pstMyCh_->DashmanTgZ = st_.pstMyCh_->Zahyou.Z; break;
          }
          break;
        case dmtStr://すとれーと
          st_.pstMyCh_->DashmanTgZ = getz;//st_.pstMyCh_->Zahyou.Z;
          break;
        case dmtAp://センター  ベタ寄りしないようにするか
        {
          s32 tgZ = st_.pmgMyTm_->st_.pmgMyCh_[st_.pmgMyTm_->st_.pstMyTm_->DMCallmanNo]->st_.pstMyCh_->Zahyou.Z;
          s32 distZ = -((tgZ - st_.pstMyCh_->Zahyou.Z) / 2);//1/2の距離に寄る
          st_.pstMyCh_->DashmanTgZ = tgZ + distZ;
        }
        break;
        //case dmtCr://くろす
        //  {
        //    s32 getx = 0;
        //    s32 getz = 0;
        //    st_.pmgMyTm_->GetFomposOfInf(getx, getz, st_.posNo_);
        //    st_.pstMyCh_->DashmanTgZ = (DBCRT_CLZ - getz) + DBCRT_CLZ;
        //    //st_.pstMyCh_->DashmanTgZ = (DBCRT_CLZ - st_.pstMyCh_->Zahyou.Z) + DBCRT_CLZ;
        //  }
        break;
        }

        //上限下限
        st_.pstMyCh_->DashmanTgZ = lib_num::Between(st_.pstMyCh_->DashmanTgZ,
          (DBCRT_FL + TAGLINEDIST),
          (DBCRT_BL - TAGLINEDIST));


        //同時発動もあるのでここでカウンタ増やす★
        ++st_.pmgMyTm_->st_.pstMyTm_->PosMove.DashmanNum;



        s32 dsx = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashX);
        s32 dsz = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashZ);
        s32 dXSign = (st_.mysideNo_ == 0)
          ? +1
          : -1;

        //移動速度をジャンプ速度に
        s32 jvx = dXSign * pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashJpX);
        s32 jvy = pmgEO_->mgDt_.dtSet_.GetDtJump(setDashJump_vY0);

        const s32 CRTIME = 4;//ジャンプ前のしゃがみ

        //移動先
        s32 tgx = (st_.mysideNo_ == 0)
          ? (DBCRT_CL - st_.pmgMyTm_->GenGetAtcLineX(TRUE, TRUE))
          : (DBCRT_CL + st_.pmgMyTm_->GenGetAtcLineX(TRUE, TRUE));

        //ダッシュマンがジャンプラインに到達するまでの距離
        s32 jumpDistX = (st_.pmgMyTm_->GetAtcLineX(TRUE, TRUE) - GetLeftCrtX());

        //ダッシュしている時間
        s32 timedash = abs(jumpDistX / dsx);

        //ダッシュマンがジャンプラインに到達するまでの時間 + ジャンプしゃがみ時間
        s32 timedjl = (timedash + CRTIME);

        //ジャンプしてから着地までの時間
        s32 jplandtime = (2 * jvy) / pmgEO_->mgDt_.dtSet_.GetDtGrv(setGRV_Y);

        const s32 YOYUU = 30;//３０フレぐらい録ってからの余裕がいるか
        //ダッシュマンが着地するまでの時間
        if (st_.pmgMyTm_->IsNoJpDashman() == FALSE)
        {
          st_.pstMyCh_->DashmanPaOK_c = (timedjl + jplandtime) - YOYUU;
        }
        else
        {
          st_.pstMyCh_->DashmanPaOK_c = timedash;
        }


      }
    }


    //ダッシュマン中および今さっきダッシュマンになった
    if (st_.pstMyCh_->NextAuto.AutoType == dbatDashman)
    {
      st_.pstMyCh_->NextAuto.ADash_f = TRUE;
      const s32 TAGLINEDIST = 2 * XYMAG;

      st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
        ? st_.pstMyCh_->NextAuto.AMuki = maR
        : st_.pstMyCh_->NextAuto.AMuki = maL;

      //向かう先へＺ移動
      if (st_.pstMyCh_->Zahyou.Z < (st_.pstMyCh_->DashmanTgZ - TAGLINEDIST))
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
      }
      else if (st_.pstMyCh_->Zahyou.Z > (st_.pstMyCh_->DashmanTgZ + TAGLINEDIST))
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
      }
      else
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
      }
    }

  }

  //自動取りに行く★★★★
  void TChMove::CheckAutoGetter()
  {
    //かがみ予約
    if (Kagami_f()) return;

    //他の任務があるときはそちらを優先
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)) return;

    if (IsInfield())
    {
      if (IsCOM())
      {
        //内野は操作権のある人（ＣＯＭ）
        if (IsCtrl() == FALSE) return;
      }
      else
      {
        if (IsSelfCtrl()) return;
      }

      //適格者以外は追わない
      if (st_.posNo_ != st_.pmgMyTm_->st_.pstMyTm_->GetNo) return;

      if ((st_.pstMyCh_->NextAuto.AutoType != dbatFree)
        && (st_.pstMyCh_->NextAuto.AutoType != dbatDfPos)//ポジショニングのときも
        && (st_.pstMyCh_->NextAuto.AutoType != dbatGet)
        ) return;
    }
    else
    {
      if (st_.pstMyCh_->GaiyaOvL_f) return;//オーバーラインだとダメ

      if ((st_.pstMyCh_->NextAuto.AutoType != dbatFree)
        && (st_.pstMyCh_->NextAuto.AutoType != dbatReturn)//リターンでも大丈夫
        && (st_.pstMyCh_->NextAuto.AutoType != dbatDfPos)//ポジショニングのときも
        && (st_.pstMyCh_->NextAuto.AutoType != dbatGet)
        ) return;
    }

    //ボールが取りに行くべき状態か
    if ((pmgSG_->stBa_.Motion != bmFree)
      && (pmgSG_->stBa_.Motion != bmBound)) return;

    //ファーストバウンドはシュート扱い
    if ((pmgSG_->stBa_.Motion == bmBound)
      && (pmgSG_->stBa_.ShootFirstBound_f)) return;

    //ジャンプボールは追わない
    if ((pmgSG_->stBa_.Motion == bmBound)
      && (pmgSG_->stBa_.JumpBall == jbJpball)) return;

    //自分のポジション
    s32 tMyPos = (IsInfield())
      ? (s32)dbpoI0
      : st_.posNo_;



    const s32 STOPDIST = 6 * XYMAG;//ボールの幅が14なので8だと逆向きの時届かない
    const s32 STOPDISTZ = 4 * XYMAG;

    const s32 NEARX = 12 * XYMAG;
    const s32 NEARZ = 8 * XYMAG;
    const s32 FARX = 32 * XYMAG;
    const s32 FARZ = 16 * XYMAG;

    const s32 HIY = 40 * XYMAG;

    const s32 KEEPMUKI = 10;

    //自分がオートで取りに行くべき場所にボールがあるか
    //空中ではならない（なると浮いたまま止まってしまう）
    if (st_.pmgMyTm_->IsBallCrtMyPos(tMyPos, FALSE))
    {

      BOOL SetNextAX_f = FALSE;
      BOOL SetNextAZ_f = FALSE;

      st_.pstMyCh_->NextAuto.AutoType = dbatGet;
      st_.pstMyCh_->stCOM.COMMv_c = 0;

      //強奪の時はＸ軸合わせてからＺ軸移動して、一瞬相手コートに入るように
      if (IsInfield() == FALSE)//外野
      {
        const s32 STY = (16 * XYMAG);
        BOOL stealY_f = (pmgSG_->stBa_.Zahyou.Y < STY);

        if (tMyPos != (s32)dbpoO4)//23番外野
        {
          SetNextAX_f = TRUE;
          if (((pmgSG_->stBa_.LandX + STOPDIST) < st_.pstMyCh_->Zahyou.X)
            || (st_.pstMyCh_->Zahyou.X < (pmgSG_->stBa_.LandX - STOPDIST)))
          {
            SetNextAZ_f = FALSE;
            st_.pstMyCh_->NextAuto.ADash_f = (st_.pstMyCh_->BallLandDist > DASHDIST);
          }
          else
          {
            SetNextAZ_f = stealY_f;
          }
        }
        else//4番外野
        {
          SetNextAZ_f = TRUE;
          if (((pmgSG_->stBa_.LandZ + STOPDISTZ) < st_.pstMyCh_->Zahyou.Z)
            || (st_.pstMyCh_->Zahyou.Z < (pmgSG_->stBa_.LandZ - STOPDISTZ)))
          {
            SetNextAX_f = FALSE;
          }
          else
          {
            SetNextAX_f = stealY_f;
          }
        }



        if (((pmgSG_->stBa_.LandX + STOPDIST) < st_.pstMyCh_->Zahyou.X)
          && ((st_.mysideNo_ == 1) || SetNextAX_f))
        {
          st_.pstMyCh_->NextAuto.AMuki = maL;
        }
        else if ((st_.pstMyCh_->Zahyou.X < (pmgSG_->stBa_.LandX - STOPDIST))
          && ((st_.mysideNo_ == 0) || SetNextAX_f))
        {
          st_.pstMyCh_->NextAuto.AMuki = maR;
        }
        else
        {
          st_.pstMyCh_->NextAuto.AMuki = maN;
        }

        if (SetNextAZ_f)
        {
          if ((pmgSG_->stBa_.LandZ + STOPDISTZ) < st_.pstMyCh_->Zahyou.Z)
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
          }
          else if (st_.pstMyCh_->Zahyou.Z < (pmgSG_->stBa_.LandZ - STOPDISTZ))
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
          }
          else
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          }
        }
        else
        {
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
        }


        //ダッシュ予約//Ｚ軸が離れすぎているときはダッシュしない
        st_.pstMyCh_->NextAuto.ADash_f = ((st_.pstMyCh_->BallLandDist > DASHDIST)
          && (abs(st_.pstMyCh_->Zahyou.Z - pmgSG_->stBa_.LandZ) < DASHDISTZ))
          ? TRUE
          : FALSE;

      }
      else//COM内野
      {
        SetNextAX_f = TRUE;
        SetNextAZ_f = TRUE;

        BOOL lowball_f = (pmgSG_->stBa_.Zahyou.Y < HIY);

        s32 distx = abs(pmgSG_->stBa_.LandX - st_.pstMyCh_->Zahyou.X);
        s32 distz = abs(pmgSG_->stBa_.LandZ - st_.pstMyCh_->Zahyou.Z);

        //非常に近いとき
        BOOL nearX_f = (distx < NEARX);// && lowball_f);
        BOOL nearZ_f = (distz < NEARZ);// && lowball_f);

        BOOL farX_f = (distx > FARX);
        BOOL farZ_f = (distz > FARZ);

        ////前フレから継続でない時
        //if ((st_.pstMyCh_->Auto.AutoType != dbatGet)
        //  || (st_.pstMyCh_->stCOM.KeepMuki_c == 0)
        //  || nearX_f
        //  || nearZ_f)
        //{
        if (((pmgSG_->stBa_.LandX + STOPDIST) < st_.pstMyCh_->Zahyou.X)
          && ((st_.mysideNo_ == 1) || SetNextAX_f))
        {
          st_.pstMyCh_->NextAuto.AMuki = maL;
        }
        else if ((st_.pstMyCh_->Zahyou.X < (pmgSG_->stBa_.LandX - STOPDIST))
          && ((st_.mysideNo_ == 0) || SetNextAX_f))
        {
          st_.pstMyCh_->NextAuto.AMuki = maR;
        }
        else
        {
          st_.pstMyCh_->NextAuto.AMuki = maN;
        }

        if (SetNextAZ_f)
        {
          if ((pmgSG_->stBa_.LandZ + STOPDISTZ) < st_.pstMyCh_->Zahyou.Z)
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
          }
          else if (st_.pstMyCh_->Zahyou.Z < (pmgSG_->stBa_.LandZ - STOPDISTZ))
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
          }
          else
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          }
        }
        else
        {
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
        }


        //ダッシュ予約//Ｚ軸が離れすぎているときはダッシュしない
        st_.pstMyCh_->NextAuto.ADash_f = ((st_.pstMyCh_->BallLandDist > DASHDIST)
          && (abs(st_.pstMyCh_->Zahyou.Z - pmgSG_->stBa_.LandZ) < DASHDISTZ));


        st_.pstMyCh_->stCOM.KeepMuki_c = KEEPMUKI;
        //}
        //else
        //{
        //  st_.pstMyCh_->NextAuto.AMuki = st_.pstMyCh_->Auto.AMuki;
        //  st_.pstMyCh_->NextAuto.AMukiZ = st_.pstMyCh_->Auto.AMukiZ;
        //  st_.pstMyCh_->NextAuto.ADash_f = st_.pstMyCh_->Auto.ADash_f;

        //  lib_num::AprTo0(&st_.pstMyCh_->stCOM.KeepMuki_c);
        //}
      }

    }



    //強引に取りに行く
    if (IsInfield()
      && (st_.pstMyCh_->NextAuto.AutoType != dbatGet)
      && st_.pmgMyTm_->IsBallCrtMyPos(tMyPos, TRUE))
    {
      BOOL reach_f = FALSE;

      //ボール奪い//微妙なら行く	行く	行かない	とれそうなら//★
      if ((st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaBallTake] == 0)
        || (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaBallTake] == 3))
      {
        //対象がセンターラインかエンドラインか
        BOOL cl_f = FALSE;

        //左コート
        if (pmgSG_->stBa_.Zahyou.X < DBCRT_CL)
        {
          //右進行
          cl_f = (pmgSG_->stBa_.Zahyou.dX > 0);
        }
        else
        {
          //左進行
          cl_f = (pmgSG_->stBa_.Zahyou.dX < 0);
        }

        s32 bdist = (cl_f)
          ? (DBCRT_CL - pmgSG_->stBa_.Zahyou.X)
          : (st_.pmgBa_->baCommon_.GetSLX() - pmgSG_->stBa_.Zahyou.X);

        s32 bout = bdist;
        if (pmgSG_->stBa_.Zahyou.dX != 0)
        {
          bout = abs(bdist / pmgSG_->stBa_.Zahyou.dX);//0除算
        }

        s32 pdist = (cl_f)
          ? (DBCRT_CL - st_.pstMyCh_->Zahyou.X)
          : (GetSLX() - st_.pstMyCh_->Zahyou.X);

        s32 pout = pdist;
        if (pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashX) != 0)
        {
          pout = abs(pdist / pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashX));
        }

        const s32 BIMYOU = 15;
        //微妙なら
        if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaBallTake] == 0)
        {
          reach_f = ((bout + BIMYOU) > pout);
        }
        else//とれそうなら
        {
          reach_f = ((bout - BIMYOU) > pout);
        }
      }


      BOOL stealGo_f = FALSE;

      //ボール奪い//微妙なら行く	行く	行かない	とれそうなら//★
      switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaBallTake])
      {
      case 0://微妙なら行く
        stealGo_f = reach_f;
        break;
      case 1://行く
        stealGo_f = TRUE;
        break;
      case 2://行かない
        stealGo_f = FALSE;
        break;
      default://とれそうならいく
        stealGo_f = reach_f;
        break;
      }

      if (stealGo_f)
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatGet;

        //BOOL lowball_f = (pmgSG_->stBa_.Zahyou.Y < HIY);

        ////非常に近いとき
        //BOOL nearX_f = (((pmgSG_->stBa_.Zahyou.X - STOPDIST) <= st_.pstMyCh_->Zahyou.X)
        //  && (st_.pstMyCh_->Zahyou.X <= (pmgSG_->stBa_.Zahyou.X + STOPDIST)));

        // BOOL nearZ_f = (((pmgSG_->stBa_.Zahyou.Z - STOPDISTZ) <= st_.pstMyCh_->Zahyou.Z)
        //  && (st_.pstMyCh_->Zahyou.Z <= (pmgSG_->stBa_.Zahyou.Z + STOPDISTZ)));


        BOOL lowball_f = (pmgSG_->stBa_.Zahyou.Y < HIY);

        s32 distx = abs(pmgSG_->stBa_.LandXLine - st_.pstMyCh_->Zahyou.X);
        s32 distz = abs(pmgSG_->stBa_.LandZLine - st_.pstMyCh_->Zahyou.Z);

        //非常に近いとき
        BOOL nearX_f = ((distx < NEARX) && lowball_f);
        BOOL nearZ_f = ((distz < NEARZ) && lowball_f);

        BOOL farX_f = (distx > FARX);
        BOOL farZ_f = (distz > FARZ);



        ////前フレから継続でない時
        //if ((st_.pstMyCh_->Auto.AutoType != dbatGet)
        //  || (st_.pstMyCh_->stCOM.KeepMuki_c == 0)
        //  || nearX_f
        //  || nearZ_f)
        //{

        //タゲ位置セット
        SetTagPos(pmgSG_->stBa_.LandXLine, pmgSG_->stBa_.LandZLine);

        //前フレダッシュしていた
        if (st_.pstMyCh_->Auto.ADash_f)
        {
          //ダッシュ予約//Ｚ軸が離れすぎているときはダッシュしない//着地位置になっていたのを修正
          st_.pstMyCh_->NextAuto.ADash_f = ((abs(st_.pstMyCh_->Zahyou.X - pmgSG_->stBa_.LandXLine) < DASHDIST)
            && (abs(st_.pstMyCh_->Zahyou.Z - pmgSG_->stBa_.LandZLine) < DASHDISTZ))
            ? TRUE
            : FALSE;
        }
        else
        {
          st_.pstMyCh_->NextAuto.ADash_f = FALSE;
        }

        st_.pstMyCh_->stCOM.KeepMuki_c = KEEPMUKI;

        //}
        //else
        //{
        //  st_.pstMyCh_->NextAuto.AMuki = st_.pstMyCh_->Auto.AMuki;
        //  st_.pstMyCh_->NextAuto.AMukiZ = st_.pstMyCh_->Auto.AMukiZ;
        //  st_.pstMyCh_->NextAuto.ADash_f = st_.pstMyCh_->Auto.ADash_f;
        //  lib_num::AprTo0(&st_.pstMyCh_->stCOM.KeepMuki_c);
        //}
        //



        ////がたつき防止
        //if ((st_.pstMyCh_->NextAuto.AMuki == maN)
        //  && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN))
        //{
        //  st_.pstMyCh_->stCOM.COMMv_c = 30;
        //}
        //else
        //{
        //  st_.pstMyCh_->stCOM.COMMv_c = 0;
        //}


      }
    }


  }


  //狙いに近いパスタゲポジション取得
  s32 TChMove::GetCPUNearPaTag(s32 enPos)
  {
    s32 res;

    TMgChar* ptg = st_.pmgEnTm_->st_.pmgMyCh_[enPos];

    const s32 ZWID = 12 * XYMAG;
    const s32 XWID = 40 * XYMAG;
    BOOL O4_f = (ptg->GetLeftCrtX() < DBCRT_CLXL);
    BOOL O2_f = (ptg->st_.pstMyCh_->Zahyou.Z > (DBCRT_CLZ + ZWID));
    BOOL O3_f = (ptg->st_.pstMyCh_->Zahyou.Z < (DBCRT_CLZ - ZWID));
    BOOL cnt_f = ((ptg->GetLeftCrtX() < (DBCRT_CLXL + XWID))
      && (ptg->GetLeftCrtX() > (DBCRT_CLXL - XWID)));

    res = (O4_f)
      ? dbpoO4
      : st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman;

    if (cnt_f)
    {
      if (O2_f) res = dbpoO2;
      if (O3_f) res = dbpoO3;
    }
    else if (IsInfield())//内野から直接４番外野には送らない
    {
      res = (pmgGO_->pmgCommon_->GetRand(2) == 0)
        ? dbpoO2
        : dbpoO3;
    }

    return res;
  }
  //指定向きパスタゲポジション取得
  s32 TChMove::GetCPUMukiPaTag(enCOMPassMuki pamuki)
  {
    s32 patg = NGNUM;
    switch (pamuki)
    {
    case cpmTaimen://対面パス
      switch (st_.posNo_)
      {
      case dbpoO2:patg = dbpoO3; break;
      case dbpoO3:patg = dbpoO2; break;
      case dbpoO4:patg = st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman; break;
      default: patg = dbpoO4; break;
      }
      break;
    case cpm342://左回り
      switch (st_.posNo_)
      {
      case dbpoO2:patg = st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman; break;
      case dbpoO3:patg = dbpoO4; break;
      case dbpoO4:patg = dbpoO2; break;
      default: patg = dbpoO3; break;
      }
      break;
    case cpm243://右回り
      switch (st_.posNo_)
      {
      case dbpoO2:patg = dbpoO4; break;
      case dbpoO3:patg = st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman; break;
      case dbpoO4:patg = dbpoO3; break;
      default: patg = dbpoO2; break;
      }
      break;
    case cpmInf://内野
      patg = st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman;
      break;
    }

    //内野から内野向きパスの場合
    if (IsInfield() &&
      (patg == st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman))
    {
      //最後の一人
      if (st_.pmgMyTm_->IsLastOne())
      {
        patg = NGNUM;//パス不可
      }
      else//前にいる人から順に
      {
        s32 tgpno = NGNUM;
        s32 tgst = 0;
        s32 mypos = (st_.pstMyCh_->Zahyou.X + st_.pstMyCh_->Zahyou.Z);
        for (s32 i = 0; i < DBMEMBER_INF; ++i)//後衛→前衛
        {
          s32 i2 = (DBMEMBER_INF - i - 1);//前衛→後衛

          //最後衛,攻撃力高い人,近い人,最前衛
          s32 infpatg = st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaInfPaTag];
          switch (infpatg)
          {
          case 0://最後衛
            if (pCommon_->IsNGPassTag(i2)) continue;
            tgpno = (i2);
            break;
          case 1://攻撃力高い人
          {
            if (pCommon_->IsNGPassTag(i)) continue;
            s32 pwfc = (st_.pmgMyTm_->st_.pmgMyCh_[i]->MyStLv(dbst_Pow));
            //+ st_.pmgMyTm_->st_.pmgMyCh_[i]->MyStLv(dbst_Guts)

            //パワー比較
            if ((tgst == NGNUM) || (tgst < pwfc))
            {
              tgpno = i;
              tgst = pwfc;
            }
          }
          break;
          case 2://近い人
          {
            if (pCommon_->IsNGPassTag(i)) continue;
            s32 tgpos = (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X + st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z);
            s32 dist = abs(mypos - tgpos);
            //距離比較
            if ((tgst == NGNUM) || (tgst > dist))
            {
              tgpno = i;
              tgst = dist;
            }
          }
          break;
          default://最前衛
            if (pCommon_->IsNGPassTag(i)) continue;
            tgpno = i;
            break;
          }

        }

        if ((tgpno == NGNUM) || (tgpno == st_.posNo_))
        {
          patg = NGNUM;//シングルパス不可
        }
        else
        {
          patg = tgpno;
        }
      }
    }

    return patg;
  }

  //チャンスチェック
  BOOL TChMove::CPUChance()
  {
    BOOL res = FALSE;

    //チームでチャンス判断が出てる
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.Chance_f)
    {
      //ノーガードチャンス
      if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.ngChance_f)
      {
        //狙いに近い味方
        s32 patg = GetCPUNearPaTag(st_.pmgMyTm_->st_.pstMyTm_->COMDt.ngTagNo);

        //自分が狙う
        if ((st_.pmgMyTm_->st_.pstMyTm_->COMDt.ngTagNo == st_.pmgMyTm_->st_.pstMyTm_->COMDt.vnTagNo)//近い
          || (patg == NGNUM)//パス出せる人がいない
          || (patg == st_.posNo_) //自分がパスタゲ
          || (IsInfield() && (patg == st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman)))//内野でパスタゲがポストマン（内野）
        {
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = TRUE;//クイック行動
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f = TRUE;//シュート
          res = TRUE;
        }
        else if (st_.pmgMyTm_->IsChancePass())//パス
        {
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = TRUE;//クイック行動
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.pa_f = TRUE;//パス
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = patg;//パスタゲ
          res = TRUE;
        }

        //なおかつ至近距離チャンス
        if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.ngTagNo == st_.pmgMyTm_->st_.pstMyTm_->COMDt.vnTagNo)
        {
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.vnnojp_f = TRUE;
        }
      }
      else if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.vnChance_f)//至近距離チャンス
      {
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = TRUE;//クイック行動
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f = TRUE;//シュート
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.vnnojp_f = TRUE;
        res = TRUE;
      }
    }


    return res;
  }

  //キャッチカウンターチャンスチェック
  BOOL TChMove::CPUCCChance()
  {
    BOOL res = FALSE;

    //カウンターキャッチ状態
    if (st_.pstMyCh_->COMCounter_f)
    {
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = TRUE;//クイック行動
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f = TRUE;//シュート
      res = TRUE;
    }

    return res;
  }

  //ナイスキャッチチャンスチェック
  BOOL TChMove::CPUNCChance()
  {
    BOOL res = FALSE;

    ////チームでナイスキャッチチャンス判断が出てる
    //if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.ncChance_f)
    //{
    //  st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = TRUE;//クイック行動
    //  st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f = TRUE;//シュート
    //  res = TRUE;
    //}

    return res;
  }


  //ＣＯＭダッシュマンチェック
  BOOL TChMove::CPUDashman()
  {

    //ダッシュマン作戦フラグが立ってる
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashman_f)
    {
      //セッターパスが必要
      if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterOK_f == FALSE)
      {
        if ((CPUSetterPass() == FALSE) //無いとは思うけどセッターパスできない
          && (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterNo != st_.posNo_))//自分がセッターでもない
        {
          //ダッシュマン作戦ナシに
          st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashman_f = FALSE;
          st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashmanFmWait_f = FALSE;
        }
      }
    }

    return (st_.pmgMyTm_->st_.pstMyTm_->COMDt.Isdm());
  }
  //ＣＯＭチャンスメイク
  BOOL TChMove::CPUChanceMake()
  {
    //return FALSE;
    ////個人性格によって、チャンスメイクパスしない場合
    //if (pmgGO_->pmgCommon_->GetRandPer(pmgEO_->mgDt_.dtChar_.GetCoopStop(st_.id_)))
    //{
    //  return FALSE;
    //}
    //else
    {
      BOOL chMake_f = FALSE;//チャンスメイクする

      //チャンスメイク
      switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comChMake])
      {
      case 0://劣勢ならする
        chMake_f = st_.pmgMyTm_->st_.pstMyTm_->COMDt.Ressei_f;
        break;
      case 1://チャンスメイクする
        chMake_f = TRUE;
        break;
      case 2://チャンスメイクしない
        chMake_f = FALSE;
        break;
      default://劣勢ならしない
        chMake_f = !(st_.pmgMyTm_->st_.pstMyTm_->COMDt.Ressei_f);
        break;
      }

      //チャンスメイクする場合
      if (chMake_f)
      {

        enCOMPassMuki pamuki = cpmNone;
        s32 patg = 0;

        //チャンスメイクタイプ
        switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comChMakePtn])
        {
        case 0://側面ランダムパス
          pamuki = (pmgGO_->pmgCommon_->GetRand(2) == 0)
            ? cpm342
            : cpm243;
          break;
        case 1://チェイス
          break;
        case 2://左巻き
          pamuki = (st_.mysideNo_ == 0)
            ? cpm342
            : cpm243;
          break;
        case 3://対面
          pamuki = cpmTaimen;
          break;
        }

        //パス向き指定
        if (pamuki != cpmNone)
        {
          //指定向きパスタゲポジション取得
          patg = GetCPUMukiPaTag(pamuki);

        }
        else//向きではなく人指定のチェイスパス
        {
          s32 enpos = 0;
          for (s32 i = 0; i < DBMEMBER_INF; ++i)//上から検索かけていく
          {
            //ねらえない
            if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsShTgOK() == FALSE) continue;
            enpos = i;
            break;
          }
          //狙いに近い人
          patg = GetCPUNearPaTag(enpos);

          //自分がそうだった場合チャンス扱いにしてチャンスメイクやめる
          if ((patg == st_.posNo_) ||
            (IsInfield() && (patg == st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman)))
          {
            //行動決定
            st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = TRUE;//クイック行動
            st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f = TRUE;//シュート

            chMake_f = FALSE;
          }

        }

        if (patg == NGNUM)//タゲが居ない場合
        {
          chMake_f = FALSE;
        }


        if (chMake_f)
        {
          //行動決定
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = patg;//パスタゲ
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.pa_f = TRUE;//パス
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = TRUE;//クイック行動
        }
      }

      return chMake_f;
    }


  }

  //ＣＯＭオーダーパス
  BOOL TChMove::CPUOrderPass()
  {
    s32 odTg = NGNUM;
    s32 comod = st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaOdPaTag];

    switch (comod)
    {
    case 0://上位オーダー順にパス
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)//上から検索かけていく
      {
        //自分が最優先のときは終了
        if (i == st_.posNo_) break;

        if (st_.pmgMyTm_->st_.pmgMyCh_[i]->IsFreeMotion(TRUE))
        {
          odTg = i;
          break;
        }
      }
      break;
      //case 1://同じ学校の先輩
      //{
      //  const s32 GAKUNEN = 3;
      //  s32 mycnt = st_.pmgMyTm_->st_.pmgMyCh_[st_.posNo_]->MyPDt(clv_Cnt);
      //  s32 mygak = st_.pmgMyTm_->st_.pmgMyCh_[st_.posNo_]->MyPDt(clv_Gakunen);
      //  for (s32 g = 1; g < (GAKUNEN + 1); ++g)//低学年→高学年
      //  {
      //    //後輩にはパスしない
      //    if (mygak > g) continue;
      //    
      //    for (s32 i = 0; i < DBMEMBER_ALL; ++i)//上から検索かけていく
      //    {
      //      //学年が違う
      //      if (g != st_.pmgMyTm_->st_.pmgMyCh_[i]->MyPDt(clv_Gakunen)) continue;
      //      
      //      //同じ学校ではない
      //      if (mycnt != st_.pmgMyTm_->st_.pmgMyCh_[i]->MyPDt(clv_Cnt)) continue;
      //      
      //      //自分が最優先のときは終了
      //      if (i == st_.posNo_) break;
      //      
      //      if (st_.pmgMyTm_->st_.pmgMyCh_[i]->IsFreeMotion(TRUE))
      //      {
      //        odTg = i;
      //        break;
      //      }
      //    }
      //  }
      //}
      //  
      //  break;
    case 2://ボスにパス
    {
      s32 bossno = st_.pmgMyTm_->st_.pstMyTm_->GetRdPos();
      if ((st_.pmgMyTm_->st_.pmgMyCh_[bossno]->st_.pstMyCh_->ANGEL_f == FALSE)
        && (st_.pmgMyTm_->st_.pmgMyCh_[bossno]->IsFreeMotion(TRUE)))
      {
        odTg = bossno;//０番の人のポジション
      }
    }
    break;
    default://上位ラインナップにパス
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)//上から検索かけていく
      {
        s32 tg = st_.pmgMyTm_->st_.pstMyTm_->PosSt.PosofC[i];

        //自分が最優先のときは終了
        if (tg == st_.posNo_) break;

        if ((st_.pmgMyTm_->st_.pmgMyCh_[tg]->st_.pstMyCh_->ANGEL_f == FALSE)
          && st_.pmgMyTm_->st_.pmgMyCh_[tg]->IsFreeMotion(TRUE))
        {
          odTg = tg;
          break;
        }
      }
      break;
    }

    //外野→内野のオーダーパスタゲはポストマン経由
    if (odTg != NGNUM)
    {
      if ((IsInfield() == FALSE) && (odTg < DBMEMBER_INF))
      {
        odTg = st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman;
      }
    }


    //最終的にそのキャラにパス出せるかチェック
    if (pCommon_->IsNGPassTag(odTg))
    {
      odTg = NGNUM;//パス不可
    }

    //オーダーパスＯＫ
    if (odTg != NGNUM)
    {
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.pa_f = TRUE;//パス
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = odTg;//オーダーパスタゲ
    }

    return (odTg != NGNUM);

  }


  //ＣＯＭシングルパス
  BOOL TChMove::CPUSinglePass()
  {
    s32 sgPaTg = NGNUM;

    //パスフラグおよびパスカウンタ残っていたら
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].SgOdPa_f
      || ((st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].ShCg == scPass) && IsInfield()))
    {

      enCOMPassMuki pamuki = cpmNone;
      s32 rnd2 = pmgGO_->pmgCommon_->GetRand(2);

      s32 comsh = st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaSgPaTag];
      //シングルパスタイプ//シングルパス先//内野,側面,対面,近接
      switch (comsh)
      {
      case 0:pamuki = cpmInf; break;//内野パス
      case 1://側面パス
        pamuki = (rnd2 == 0)
          ? cpm243
          : cpm342;
        break;
      case 2:pamuki = cpmTaimen; break;//対面パス
      default:break;//近接パス
      }

      //向き指定のシングルパス
      if (pamuki != cpmNone)
      {
        //指定向きパスタゲポジション取得
        sgPaTg = GetCPUMukiPaTag(pamuki);
      }
      else//タゲ指定の近接パス
      {
        s32 enpos = NGNUM;
        s32 tgst = NGNUM;
        s32 comnpt = st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaNrPaTag];

        for (s32 i = 0; i < DBMEMBER_INF; ++i)//上から検索かけていく
        {
          //ねらえない
          if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsShTgOK() == FALSE) continue;


          //近接パス狙い
          switch (comnpt)
          {
          case 0://上位オーダーに近い人
            if (enpos == NGNUM)
            {
              enpos = i;
            }
            break;
          case 1://攻撃力高いに近い人
          {
            s32 pwfc = (st_.pmgEnTm_->st_.pmgMyCh_[i]->MyStLv(dbst_Pow)
              + st_.pmgEnTm_->st_.pmgMyCh_[i]->MyStLv(dbst_Guts));
            //パワー比較
            if ((tgst == NGNUM) || (tgst < pwfc))
            {
              enpos = i;
              tgst = pwfc;
            }
          }
          break;
          case 2://守備が下手に近い人
          {
            s32 def = (st_.pmgEnTm_->st_.pmgMyCh_[i]->MyStLv(dbst_Kire)
              + st_.pmgEnTm_->st_.pmgMyCh_[i]->MyStLv(dbst_Tech));
            //守備比較
            if ((tgst == NGNUM) || (def < tgst))
            {
              enpos = i;
              tgst = def;
            }
          }
          break;

          default://HPすくない人に近い人
          {
            s32 hp = st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->HP;
            //ＨＰ比較
            if ((tgst == NGNUM) || (hp < tgst))
            {
              enpos = i;
              tgst = hp;
            }
          }
          break;

          }
        }

        //狙いに近い人
        sgPaTg = GetCPUNearPaTag(enpos);
      }

      //最終的にそのキャラにパス出せるかチェック
      if (pCommon_->IsNGPassTag(sgPaTg))
      {
        sgPaTg = NGNUM;//シングルパス不可
      }

      //シングルパスＯＫ
      if (sgPaTg != NGNUM)
      {
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.pa_f = TRUE;//パス
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = sgPaTg;//シングルパスタゲ

        st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].SgOdPa_f = FALSE;
        st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].ShCg = scNone;
      }

    }

    return (sgPaTg != NGNUM);

  }


  //ＣＯＭセッターパス
  BOOL TChMove::CPUSetterPass()
  {
    s32 stTg = st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterNo;


    //自分
    if (stTg == st_.posNo_)
    {
      st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterOK_f = TRUE;
      return FALSE;
    }

    //最終的にそのキャラにパス出せるかチェック
    if (stTg != NGNUM)
    {
      //オーダーパスタゲがパス出せるか
      if (st_.pmgMyTm_->st_.pmgMyCh_[stTg]->IsFreeMotion(TRUE) == FALSE)
      {
        stTg = NGNUM;
      }

      //外野→内野のオーダーパスタゲはポストマン経由
      if (stTg != NGNUM)
      {
        if ((IsInfield() == FALSE) && (stTg < DBMEMBER_INF))
        {
          stTg = st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman;
        }
      }
    }



    //セッターパスＯＫ
    if (stTg != NGNUM)
    {

      //  BOOL infpass_f = (IsInfield() && (stTg < DBMEMBER_INF));
      //  //内野間はおかしくなるのでトスパス禁止//ジャンプもダッシュマンでなくなる可能性あるので
      //  if (infpass_f)
      //  {
      //    st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f   = FALSE;
      //    st_.pmgMyTm_->st_.pstMyTm_->stCOM.toss_f = FALSE;
      //  }
      //  else
      //  {
      //    //パスタイプセット
      //    st_.pmgMyTm_->SetPassType(st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPaType]);
      //  }
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.pa_f = TRUE;//パス
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = stTg;//セッターパスタゲ
    }

    return (stTg != NGNUM);

  }


  //ＣＯＭダッシュマン攻撃思考★★
  void TChMove::CheckDMAtcCPU()
  {
    //セッター行動
    BOOL setter_f = ((st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterNo == st_.posNo_)
      && (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterBMRsvGo_f == FALSE));

    if (setter_f)
    {
      BOOL infpass_f = IsInfield();//タゲは必ず内野なのでセッターが内野かどうかだけ

      //内野間はおかしくなるのでトスパス禁止←？
      //ジャンプもダッシュマンでなくなる可能性あるので
      if (infpass_f)
      {
        st_.pmgMyTm_->SetPassType(st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPaTypeInf]);
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f = FALSE;
        //st_.pmgMyTm_->st_.pstMyTm_->stCOM.toss_f = FALSE;
      }
      else
      {
        //パスタイプセット
        st_.pmgMyTm_->SetPassType(st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPaType]);
        //ジャンプはいらない
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f = FALSE;
      }
    }
    else
    {
      //セッターパスが必要
      if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterOK_f == FALSE)
      {

        BOOL infpass_f = (IsInfield()
          && (st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag < DBMEMBER_INF));
        //内野間はおかしくなるのでトスパス禁止//ジャンプもダッシュマンでなくなる可能性あるので
        if (infpass_f)
        {
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f = FALSE;
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.toss_f = FALSE;
        }
        else
        {
          //パスタイプセット
          st_.pmgMyTm_->SetPassType(st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPaType]);
        }
      }
      else
      {
        st_.pmgMyTm_->SetQShootType(st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaQShType]);
        //ダッシュマンなので一応
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.ds_f = TRUE;
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.dspos_f = TRUE;//ダッシュ開始位置無視
      }
    }
  }
  //ＣＯＭ通常攻撃思考
  void TChMove::CheckStdAtcCPU()
  {
    //行動決定
    BOOL comOK_f = FALSE;

    //チャンスチェック
    comOK_f = CPUChance();

    //カウンターキャッチチェック
    if (comOK_f == FALSE)
    {
      comOK_f = CPUCCChance();
    }

    //ナイスキャッチ投げ返しチェック
    if (comOK_f == FALSE)
    {
      comOK_f = CPUNCChance();
    }

    //チャンスメイク
    if (comOK_f == FALSE)
    {
      if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.chpass_c > 0)//カウンタまだのこってる
      {
        comOK_f = CPUChanceMake();

        //チャンスメイク以外はカウンタ０にする
        if (comOK_f == FALSE)
        {
          st_.pmgMyTm_->st_.pstMyTm_->COMDt.chpass_c = 0;
        }
      }
    }

    //基本行動
    if (comOK_f == FALSE)
    {
      BOOL pa_f = FALSE;

      s32 comact = st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaAction];

      switch (comact)
      {
      case 0://クイックシュート
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = TRUE;
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f = TRUE;//シュート
        if ((st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].ShCg == scPass) && IsInfield())
        {
          pa_f = CPUSinglePass();//シュート変更パス用
        }
        break;
      case 1://パワーシュート
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = FALSE;
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f = TRUE;
        if ((st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].ShCg == scPass) && IsInfield())
        {
          pa_f = CPUSinglePass();//シュート変更パス用
        }
        break;
      case 2://オーダーパス（カウンタにかかわらず）
        pa_f = CPUOrderPass();//オーダーパス
        //パスが無理
        if (pa_f == FALSE)
        {
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = FALSE;
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f = TRUE;
        }
        break;
      default://シングルパス
        pa_f = CPUSinglePass();//シングルパス
        //パスが無理
        if (pa_f == FALSE)
        {
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = FALSE;
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f = TRUE;
        }
        break;

      }
    }

    //クイック作戦が出ているとき
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.Quick_f)
    {
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f = TRUE;
    }


    //パスタイプ
    if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag != NGNUM)
    {

      BOOL infpass_f = (IsInfield()
        && (st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag < DBMEMBER_INF)
        );

      //パスタイプ
      s32 patype = (infpass_f)
        ? st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPaTypeInf]
        : st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPaType];

      //パスタイプセット
      st_.pmgMyTm_->SetPassType(patype);

      //内野内野パスはジャンプいらない
      if (infpass_f)
      {
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f = FALSE;
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.toss_f = FALSE;//トスもいらない★
      }

      if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.chpass_c > 0)
      {
        //チャンスメイクカウンタ減少
        lib_num::AprTo0(&st_.pmgMyTm_->st_.pstMyTm_->COMDt.chpass_c);
      }
      else
      {
        ////パスカウンタの減少
        //if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f)//シュート変更パスの場合
        //{
        //  lib_num::AprTo0(&st_.pmgMyTm_->st_.pstMyTm_->COMDt.pass_c);
        //}
        //else//シングルパスの場合
        //{
        //  st_.pmgMyTm_->st_.pstMyTm_->COMDt.pass_c = 0;//１回で終わり
        //}
      }
    }

    //シュートタイプ
    if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f)
    {

      //内野間トスパスの場合
      if (st_.pstMyCh_->COMTossPassGet_f)
      {
        //シュートはクイック
        st_.pmgMyTm_->SetQShootType(st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaQShType]);
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.ds_f = FALSE;//ダッシュは切る
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.dspos_f = TRUE;//ダッシュ開始位置無視
      }
      else
      {
        if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f //クイック
          || (IsInfield() == FALSE)) //外野
        {
          st_.pmgMyTm_->SetQShootType(st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaQShType]);
        }
        else//パワー
        {
          st_.pmgMyTm_->SetPShootType(st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPwShType]);
        }
      }

      //ジャンプ不要指示
      if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.vnnojp_f)
      {
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f = FALSE;
      }

      //カウンターの時はかならずダッシュ
      if (st_.pstMyCh_->COMCounter_f)
      {
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.ds_f = TRUE;
      }
    }

  }

  //行動予定セットＣＯＭダッシュマン
  void TChMove::AMvSet_DMCPU()
  {
    BOOL ds_f = FALSE;

    //内野
    if (IsInfield())
    {
      //コールＯＫの内野
      if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.callOK_f)
      {
        //シュート指示が出ている
        if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashmanShOK_f)
        {
          //ボール持ってたらダッシュ
          ds_f = IsBall();
        }
        else//まだシュート指示が出ていない
        {
          //ダッシュマン
          if (st_.pstMyCh_->DashmanNo != NGNUM)
          {
            if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.DsKeep_f)//パス元ダッシュ継続
            {
              ds_f = TRUE;//ダッシュ
            }
            else
            {
              ds_f = IsBall();//ボール手放したら止まる
            }
          }
          else if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterNo == st_.posNo_)//セッター（ダッシュマンにまだなってない）
          {
            if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterBMRsvGo_f)
            {
              ds_f = TRUE;//ダッシュ
            }
            else
            {
              ds_f = FALSE;//うごかない
            }
          }
          else//セッター以外
          {
            ds_f = FALSE;//うごかない
          }

        }

      }
      else//まだコールＯＫに入ってない
      {
        //うごかない
        ds_f = FALSE;
      }

      if (ds_f)//ダッシュ開始
      {
        st_.pstMyCh_->NextAuto.ADash_f = TRUE;
        st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
          ? maR
          : maL;
        st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
      }
      else//ダッシュ前
      {
        MoveDfPos();//ポジション位置に戻る
      }
    }
    else//外野
    {

      //外野の向き
      switch (st_.posNo_)
      {
      case dbpoO2:
        st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
        break;
      case dbpoO3:
        st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
        break;
      case dbpoO4:
        st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
          ? maL
          : maR;
        break;
      }

    }

  }

  void TChMove::AMvSet_dspos()
  {
    st_.pmgMyTm_->st_.pstMyTm_->stCOM.dspos_f = TRUE;

    //★ダッシュ向かい先
    s32 dsz = st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaDsStyle];


    st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ = st_.pstMyCh_->Zahyou.Z;
    st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ_f = FALSE;

    const s32 BLDRND = 2 * XYMAG;
    switch (dsz)//ダッシュスタイル//タゲから離れる,タゲによる,直進,近い角を目指す
    {
    case 0://タゲから離れる
      if ((st_.pmgEnTm_->st_.pstMyTm_->CtrlNo != NGNUM)
        && (st_.pmgEnTm_->st_.pstMyTm_->CtrlNo < DBMEMBER_INF))
      {
        s32 enz = st_.pmgEnTm_->st_.pmgMyCh_[st_.pmgEnTm_->st_.pstMyTm_->CtrlNo]->st_.pstMyCh_->Zahyou.Z;

        if ((DBCRT_CLZ - BLDRND) > enz)
        {
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ = DBCRT_FL;
        }
        else if ((DBCRT_CLZ + BLDRND) < enz)
        {
          st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ = DBCRT_BL;
        }
        else
        {
          enz = (enz > st_.pstMyCh_->Zahyou.Z)
            ? DBCRT_FL
            : DBCRT_BL;
        }
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ_f = TRUE;

      }
      break;
    case 1://クイック
      if ((st_.pmgEnTm_->st_.pstMyTm_->CtrlNo != NGNUM)
        && (st_.pmgEnTm_->st_.pstMyTm_->CtrlNo < DBMEMBER_INF))
      {
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ = st_.pmgEnTm_->st_.pmgMyCh_[st_.pmgEnTm_->st_.pstMyTm_->CtrlNo]->st_.pstMyCh_->Zahyou.Z;
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ_f = TRUE;
      }
      break;
    case 2://ストレート
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ = st_.pstMyCh_->Zahyou.Z;
      break;
    default://クロス
      if ((DBCRT_CLZ - BLDRND) > st_.pstMyCh_->Zahyou.Z)
      {
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ = DBCRT_FL;
      }
      else if ((DBCRT_CLZ + BLDRND) < st_.pstMyCh_->Zahyou.Z)
      {
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ = DBCRT_BL;
      }
      else
      {
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ = st_.pmgMyTm_->st_.pstMyTm_->COMRnd.DgToCa_f//適当に1/2
          ? DBCRT_FL
          : DBCRT_BL;
      }
      break;
    }

    //Ｚ向かう先
    const s32 TAGLINEDIST = 4 * XYMAG;

    //上限下限
    st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ = lib_num::Between(st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ,
      (DBCRT_FL + TAGLINEDIST),
      (DBCRT_BL - TAGLINEDIST));
  }

  //行動予定セットＣＯＭ通常
  void TChMove::AMvSet_StdCPU()
  {

    //内野
    if (IsInfield())
    {
      if (IsBall() == FALSE)
      {
        //うごかない
        st_.pstMyCh_->NextAuto.ADash_f = FALSE;
        //一応敵向き
        st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
          ? maR
          : maL;
        st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
      }
      else
      {
        //攻撃
        if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f)
        {
          //ダッシュ攻撃
          if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.ds_f)
          {
            //一旦止まる
            if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.waitmove_c > 0)
            {
              st_.pstMyCh_->NextAuto.ADash_f = FALSE;
              st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
              st_.pstMyCh_->NextAuto.AMuki = maN;

              lib_num::AprTo0(&st_.pmgMyTm_->st_.pstMyTm_->stCOM.waitmove_c);
            }
            //ダッシュ開始位置まで移動
            else if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.dspos_f == FALSE)
            {
              //歩いて移動
              st_.pstMyCh_->NextAuto.ADash_f = FALSE;

              s32 comdspos = st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaDsPos];


              //クイック攻撃の場合
              if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f)
              {
                AMvSet_dspos();
              }
              //どこからでもダッシュの場合
              else if (comdspos == 1)
              {
                //既に攻撃ラインを超えている
                if (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(TRUE, FALSE))//ジャンプ開始位置まできた
                {
                  //ダッシュ攻撃フラグをたたむ
                  st_.pmgMyTm_->st_.pstMyTm_->stCOM.ds_f = FALSE;
                }
                else
                {
                  AMvSet_dspos();
                }
              }
              else//ダッシュ開始位置まで移動
              {
                const s32 OKDIST = 4 * XYMAG;

                if (GetLeftCrtX() > (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].DsPos + OKDIST))
                {
                  st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
                    ? maL
                    : maR;
                }
                else if (GetLeftCrtX() < (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].DsPos - OKDIST))
                {
                  st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
                    ? maR
                    : maL;
                }
                else
                {
                  AMvSet_dspos();
                }
              }
            }

            //ダッシュ開始位置まで来た

            if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.dspos_f
              && (st_.pmgMyTm_->st_.pstMyTm_->stCOM.waitmove_c == 0))
            {
              //常に位置をとり続ける
              if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ_f)
              {
                AMvSet_dspos();
              }

              //ダッシュ開始
              st_.pstMyCh_->NextAuto.ADash_f = TRUE;
              st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
                ? maR
                : maL;

              const s32 TAGDIST = 4 * XYMAG;

              if (st_.pstMyCh_->Zahyou.Z < (st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ - TAGDIST))
              {
                st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
              }
              else if (st_.pstMyCh_->Zahyou.Z > (st_.pmgMyTm_->st_.pstMyTm_->stCOM.dsTagZ + TAGDIST))
              {
                st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
              }
              else
              {
                st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
              }



              //ウエイト
              if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].ShCg == scWait)
              {
                //ジャンプ開始位置まできた
                if (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(TRUE, FALSE)
                  || (st_.pstMyCh_->Step_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].DShStep))
                {
                  const s32 WAITTIME = 16;
                  st_.pmgMyTm_->st_.pstMyTm_->stCOM.waitmove_c = (pmgGO_->pmgCommon_->GetRand(WAITTIME) + WAITTIME);
                  st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].ShCg = scNone;

                  st_.pstMyCh_->NextAuto.ADash_f = FALSE;
                  st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
                  st_.pstMyCh_->NextAuto.AMuki = maN;

                }
              }


            }

          }
          else//ダッシュ攻撃ではない
          {
            //うごかない
            st_.pstMyCh_->NextAuto.ADash_f = FALSE;

            //向き
            st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
              ? maR
              : maL;
            st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          }
        }
        else//攻撃ではない
        {
          MoveDfPos();//ポジション位置に戻る
        }
      }
    }
    else//外野
    {
      //外野の向き
      switch (st_.posNo_)
      {
      case dbpoO2:
        st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
        break;
      case dbpoO3:
        st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
        break;
      case dbpoO4:
        st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
          ? maL
          : maR;
        break;
      }
    }

  }

  //ＣＯＭ思考★★
  void TChMove::CheckAtcCPU()
  {
    //ＣＯＭ思考行動決定するかどうか
    BOOL actset_f = FALSE;

    //行動開始
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comatcSt_f)
    {
      if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.Isdm())//ダッシュマン作戦中
      {
        if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashman_f)
        {
          if (IsBall()
            && ((st_.pstMyCh_->NextAuto.AutoType == dbatFree) || (st_.pstMyCh_->NextAuto.AutoType == dbatDfPos))
            )
          {
            actset_f = TRUE;
          }
        }
      }
      else
      {
        if (IsBall())
        {
          //外野がリターン中に行動してしまう
          if (st_.pstMyCh_->NextAuto.AutoType == dbatReturn)
          {

          }
          else if ((st_.pstMyCh_->NextAuto.AutoType == dbatFree)
            || (st_.pstMyCh_->NextAuto.AutoType == dbatDfPos))
          {
            actset_f = TRUE;
          }
          else if ((st_.pmgMyTm_->st_.pstMyTm_->COMDt.dmstop_f)//ＤＭ態勢ウエイトから出た直後||→&&
            || (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comReset_f))//ＣＯＭ思考リセット時
          {
            actset_f = TRUE;
            st_.pmgMyTm_->st_.pstMyTm_->COMDt.dmstop_f = FALSE;
            st_.pmgMyTm_->st_.pstMyTm_->COMDt.comReset_f = FALSE;

          }
        }
      }
    }

    //ＣＯＭ思考行動決定
    if (actset_f)
    {

      //COM行動
      st_.pstMyCh_->NextAuto.AutoType = dbatCPUShPa;

      //向き初期化
      st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
      st_.pstMyCh_->NextAuto.ADash_f = FALSE;
      //とりあえず決められた向き
      if (IsInfield())
      {
        st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
          ? maR
          : maL;
      }
      else
      {
        st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
          ? maL
          : maR;

        //2,3番外野は敵チームの重心を向く
        if (st_.posNo_ != (s32)dbpoO4)
        {
          if ((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.X < st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCX))
          {
            st_.pstMyCh_->NextAuto.AMuki = maR;
          }
          else if ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.X > st_.pmgMyTm_->st_.pstMyTm_->PosMove.EnCX))
          {
            st_.pstMyCh_->NextAuto.AMuki = maL;
          }
        }
      }

      //stCOM初期化
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.Init();
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = NGNUM;
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.getstep = st_.pstMyCh_->Step_c;

      //ダッシュマン作戦可能チェック
      BOOL dashman_f = CPUDashman();


      if (dashman_f)
      {
        CheckDMAtcCPU();//ダッシュマン作戦
      }
      else
      {
        CheckStdAtcCPU();//通常作戦
      }

    }//ＣＯＭ思考開始END

    //ＣＯＭ思考行動中
    if (st_.pstMyCh_->NextAuto.AutoType == dbatCPUShPa)
    {
      //ダッシュマン作戦中
      if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.Isdm())
      {
        AMvSet_DMCPU();
      }
      else//ダッシュマン以外
      {
        AMvSet_StdCPU();
      }
    }
  }


  BOOL TChMove::IsOKDashCatch()
  {
    BOOL res = FALSE;

    //敵内野シュート
    if (pmgSG_->stBa_.PichPNo >= DBMEMBER_INF) return FALSE;
    ////カウンターキャッチの作戦が出ているとき
    //if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.CCaGo_f == FALSE) return FALSE;


    ////カウンターねらえるシュート
    //if ((pmgSG_->stBa_.ShSt.HSNo != hsNML)
    //    && (pmgEO_->mgDt_.dtShot_.GetDtBOOL(pmgSG_->stBa_.ShSt.HSNo,shdtDsCaOK) == FALSE))
    //{
    //  return FALSE;
    //}


    BOOL ccaGo_f = FALSE;

    //カウンターキャッチ対応確率//危険ならしない,する,しない,強い球ならばしない
    switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaCounter])
    {
    case 0://危険ならしない
      ccaGo_f = !(pCommon_->IsDangerBall());
      break;
    case 1://する
      ccaGo_f = TRUE;
      break;
    case 2://しない
      ccaGo_f = FALSE;
      break;
    default://強い球ならばしない
      ccaGo_f = !(pCommon_->IsStrongBall());
      break;
    }

    //ccaGo_f = TRUE;//★テスト


    if (ccaGo_f == FALSE) return FALSE;


    ////打ち出しのＺ角度の大きいボールは追わない
    //if (abs(pmgSG_->stBa_.ShSt.fdX) < abs(pmgSG_->stBa_.ShSt.fdZ)) return FALSE;

    //味方コート上空にあるボールしか追わない
    if (pmgSG_->stBa_.Zahyou.Z > DBCRT_BL) return FALSE;
    if (pmgSG_->stBa_.Zahyou.Z < DBCRT_FL) return FALSE;
    //高すぎるボールは追わない
    const s32 COUNTERY = 32 * XYMAG;
    if (pmgSG_->stBa_.Zahyou.Y > COUNTERY) return FALSE;
    //自分の前に居るボールしか追わない
    BOOL leftsh_f = (pmgSG_->stBa_.PichTNo == 0);
    if (leftsh_f)
    {
      if (st_.pstMyCh_->Zahyou.X < pmgSG_->stBa_.Zahyou.X) return FALSE;
    }
    else
    {
      if (st_.pstMyCh_->Zahyou.X > pmgSG_->stBa_.Zahyou.X) return FALSE;
    }

    BOOL inf_f = FALSE;
    //味方コート上空にあるボールしか追わない
    if (leftsh_f)
    {
      inf_f = (pmgSG_->stBa_.Zahyou.X > DBCRT_CL);
    }
    else
    {
      inf_f = (pmgSG_->stBa_.Zahyou.X < DBCRT_CL);
    }


    if (pmgSG_->stSh_.idata.jsh_f)
    {
      //味方コート上空にあるボールしか追わない
      if (inf_f == FALSE) return FALSE;
    }
    else
    {
      if ((abs(pmgSG_->stBa_.Zahyou.X - DBCRT_CL) > abs(st_.pstMyCh_->Zahyou.X - DBCRT_CL))
        && (inf_f == FALSE))
      {
        return FALSE;
      }
    }


    return TRUE;


  }

  //シュートの斜線上に入る
  void TChMove::GuardMove()
  {
    TZahyouRec* tag = &(st_.pstMyCh_->Zahyou);
    TZahyouRec* ball = &(pmgSG_->stBa_.Zahyou);


    s32 rtime = 0;
    BOOL OvTag_f = FALSE;
    BOOL ZShoot_f = (ball->MukiZ != mzN);

    BOOL CvBack_f = FALSE;
    BOOL CvFront_f = FALSE;
    BOOL CvLeft_f = FALSE;
    BOOL CvRight_f = FALSE;

    if (ZShoot_f == FALSE)//横ラインシュート
    {
      if (ball->dX > 0)//右進行
      {
        OvTag_f = (ball->X > tag->X);//的を既に超えた
      }
      else
      {
        OvTag_f = (ball->X < tag->X);//的を既に超えた
      }
    }
    else//縦ラインシュート
    {
      if (ball->dZ > 0)//奥進行
      {
        OvTag_f = (ball->Z > tag->Z);
      }
      else
      {
        OvTag_f = (ball->Z < tag->Z);
      }
    }

    s32 DISTLR = (ZShoot_f)
      ? 4 * XYMAG
      : 4 * XYMAG;
    s32 DISTFB = (ZShoot_f)
      ? 4 * XYMAG
      : 4 * XYMAG;

    if (OvTag_f == FALSE)
    {
      if (ZShoot_f == FALSE)//横ラインシュート
      {
        if (ball->dX > 0)//右進行
        {
          if (st_.pstMyCh_->NextAuto.AMuki == maR) st_.pstMyCh_->NextAuto.AMuki = maN;
        }
        else
        {
          if (st_.pstMyCh_->NextAuto.AMuki == maL) st_.pstMyCh_->NextAuto.AMuki = maN;
        }
      }
      else//縦ラインシュート
      {
        if (ball->dZ > 0)//奥進行
        {
          if (st_.pstMyCh_->NextAuto.AMukiZ == mzaB) st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
        }
        else
        {
          if (st_.pstMyCh_->NextAuto.AMukiZ == mzaF) st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
        }
      }

      //ガードの移動を残す
      st_.pstMyCh_->NextAuto.ADash_f = FALSE;

      s32 spdX = (ball->dX != 0)
        ? ball->dX
        : 1;
      //ボールが的の奥を通るか手前を通るか
      s32 distX = abs(ball->X - tag->X);
      rtime = (s32)(distX / abs(spdX));

      //手前を通る（奥に避ける）
      CvFront_f = ((ball->Z + (ball->dZ * rtime)) <= (tag->Z - DISTFB));
      CvBack_f = ((ball->Z + (ball->dZ * rtime)) > (tag->Z + DISTFB));

      s32 spdZ = (ball->dZ != 0)
        ? ball->dZ
        : 1;
      //ボールが的の右を通るか左を通るか
      s32 distZ = abs(ball->Z - tag->Z);
      rtime = (s32)(distZ / abs(spdZ));

      //右を通る（右に動く）
      CvRight_f = ((ball->X + (ball->dX * rtime)) > (tag->X + DISTLR));
      CvLeft_f = ((ball->X + (ball->dX * rtime)) <= (tag->X - DISTLR));


      const s32 STOPDIST = 4 * XYMAG;
      const s32 STOPDISTX = 8 * XYMAG;

      if (ZShoot_f)
      {
        if (CvLeft_f)
        {
          if (st_.mysideNo_ == 0)
          {
            if ((pCommon_->GetSLX2(tag->Z, FALSE) + STOPDISTX) < tag->X)//コート超えチェック
            {
              st_.pstMyCh_->NextAuto.AMuki = maL;
            }
          }
          else
          {
            if ((DBCRT_CL + STOPDISTX) < tag->X)//コート超えチェック
            {
              st_.pstMyCh_->NextAuto.AMuki = maL;
            }
          }
        }
        else if (CvRight_f)
        {
          if (st_.mysideNo_ == 0)
          {
            if ((DBCRT_CL - STOPDISTX) > tag->X)//コート超えチェック
            {
              st_.pstMyCh_->NextAuto.AMuki = maR;
            }
          }
          else
          {
            if ((pCommon_->GetSLX2(tag->Z, TRUE) - STOPDISTX) > tag->X)//コート超えチェック
            {
              st_.pstMyCh_->NextAuto.AMuki = maR;
            }
          }
        }
      }
      else
      {
        if (CvBack_f)
        {
          if (tag->Z < (DBCRT_BL - STOPDIST))//コート超えチェック
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
          }
        }
        else if (CvFront_f)
        {
          if (tag->Z > (DBCRT_FL + STOPDIST))//コート超えチェック
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
          }
        }
      }

    }

  }

  void TChMove::SetCOMMv()
  {
    st_.pstMyCh_->stCOM.COMMv_c = 30;
    st_.pstMyCh_->stCOM.COMMvAMuki = st_.pstMyCh_->NextAuto.AMuki;
    st_.pstMyCh_->stCOM.COMMvAMukiZ = st_.pstMyCh_->NextAuto.AMukiZ;
  }

  //スウェー
  void TChMove::SwayDodge()
  {

    TZahyouRec* tag = &(st_.pstMyCh_->Zahyou);
    TZahyouRec* ball = &(pmgSG_->stBa_.Zahyou);

    s32 rtimeX = 0;
    s32 rtimeZ = 0;
    BOOL dgtime_f = FALSE;
    BOOL OvTag_f = FALSE;
    BOOL ZShoot_f = (ball->MukiZ != mzN);

    BOOL CvBack_f = FALSE;
    BOOL CvFront_f = FALSE;
    BOOL CvLeft_f = FALSE;
    BOOL CvRight_f = FALSE;

    if (ZShoot_f == FALSE)//横ラインシュート
    {
      //的を既に超えた
      OvTag_f = (ball->dX > 0)//右進行
        ? (ball->X > tag->X)
        : (ball->X < tag->X);
    }
    else//縦ラインシュート
    {
      OvTag_f = (ball->dZ > 0)//奥進行
        ? (ball->Z > tag->Z)
        : (ball->Z < tag->Z);
    }

    s32 DISTLR = (ZShoot_f)
      ? 0
      : 4 * XYMAG;
    s32 DISTFB = (ZShoot_f)
      ? 4 * XYMAG
      : 0;


    st_.pstMyCh_->NextAuto.AMuki = maN;
    st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
    st_.pstMyCh_->NextAuto.ADash_f = FALSE;

    if (OvTag_f == FALSE)
    {
      s32 spdX = (ball->dX != 0)
        ? ball->dX
        : 1;
      //ボールが的の奥を通るか手前を通るか
      s32 distX = abs(ball->X - tag->X);
      rtimeX = (s32)(distX / abs(spdX));

      //手前を通る（奥に避ける）
      CvBack_f = ((ball->Z + (ball->dZ * rtimeX)) <= (tag->Z - DISTFB));
      CvFront_f = ((ball->Z + (ball->dZ * rtimeX)) > (tag->Z + DISTFB));

      s32 spdZ = (ball->dZ != 0)
        ? ball->dZ
        : 1;
      //ボールが的の右を通るか左を通るか
      s32 distZ = abs(ball->Z - tag->Z);
      rtimeZ = (s32)(distZ / abs(spdZ));

      //右を通る（左に避ける）
      CvLeft_f = ((ball->X + (ball->dX * rtimeZ)) > (tag->X + DISTLR));
      CvRight_f = ((ball->X + (ball->dX * rtimeZ)) <= (tag->X - DISTLR));


      const s32 STOPDIST = 6 * XYMAG;
      const s32 STOPDISTX = 12 * XYMAG;

      //到着まで90f秒以内であれば避け始める
      const s32 DGSTARTTIME = 90;


      if (ZShoot_f)
      {
        dgtime_f = (rtimeZ < DGSTARTTIME);
      }
      else
      {
        dgtime_f = (rtimeX < DGSTARTTIME);
      }

      //近い場合も避ける
      const s32 DGDIST = 32;
      if (abs(ball->X - tag->X) < DGDIST) dgtime_f = TRUE;
      if (abs(ball->Z - tag->Z) < DGDIST) dgtime_f = TRUE;



      if (dgtime_f)
      {
        if (CvLeft_f)
        {
          if (st_.mysideNo_ == 0)
          {
            if ((pCommon_->GetSLX2(tag->Z, FALSE) + STOPDISTX) < tag->X)//コート超えチェック
            {
              st_.pstMyCh_->NextAuto.AMuki = maL;
            }
          }
          else
          {
            if ((DBCRT_CL + STOPDISTX) < tag->X)//コート超えチェック
            {
              st_.pstMyCh_->NextAuto.AMuki = maL;
            }
          }
        }
        else if (CvRight_f)
        {
          if (st_.mysideNo_ == 0)
          {
            if ((DBCRT_CL - STOPDISTX) > tag->X)//コート超えチェック
            {
              st_.pstMyCh_->NextAuto.AMuki = maR;
            }
          }
          else
          {

            if ((pCommon_->GetSLX2(tag->Z, TRUE) - STOPDISTX) > tag->X)//コート超えチェック
            {
              st_.pstMyCh_->NextAuto.AMuki = maR;
            }
          }
        }

        if (ZShoot_f == FALSE)
        {
          if (CvBack_f)
          {
            if (tag->Z < (DBCRT_BL - STOPDIST))//コート超えチェック
            {
              st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
            }
          }
          else if (CvFront_f)
          {
            if (tag->Z > (DBCRT_FL + STOPDIST))//コート超えチェック
            {
              st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
            }
          }
        }

      }
    }

  }
  //よけモーション
  void TChMove::CheckDodge()
  {
    //敵のシュートが来てるときのみ
    if ((pmgSG_->stBa_.Motion == bmShoot)
      && (pmgSG_->stBa_.ShTgTNo == st_.mysideNo_)
      && (IsInfield()))
    {

      //ボールなしのみ
      if (IsBall()) return;

      //自由のみ
      if (IsFreeMotion(FALSE) == FALSE) return;

      //自由のみ
      if ((st_.pstMyCh_->NextAuto.AutoType == dbatFree)
        || (st_.pstMyCh_->NextAuto.AutoType == dbatReturn)
        || (st_.pstMyCh_->NextAuto.AutoType == dbatDfPos)
        || (st_.pstMyCh_->NextAuto.AutoType == dbatCPUHelp))
      {

        BOOL ca_f = FALSE;
        //キャッチ対応確率//ブロック,する,しない,強い球ならばしない
        switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaCatchJdg])
        {
        case 0://ブロック
          ca_f = !(pCommon_->IsDangerBall());
          break;
        case 1://する
          ca_f = TRUE;
          break;
        case 2://しない
          ca_f = FALSE;
          break;
        default://強い球ならばしない
          ca_f = !(pCommon_->IsStrongBall());
          break;
        }


        enTDGSL ds = CheckDS(ca_f);//ＣＯＭ思考で★
        //enTDGSL ds = CheckDS(FALSE);//ＣＯＭ思考で★


        switch (ds)
        {
        case dgsJp:
          st_.pstMyCh_->NextAuto.AutoType = dbatDgJp;
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          st_.pstMyCh_->NextAuto.AMuki = maN;
          st_.pstMyCh_->NextAuto.ADash_f = FALSE;
          break;
        case dgsCr:
          st_.pstMyCh_->NextAuto.AutoType = dbatDg;
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          st_.pstMyCh_->NextAuto.AMuki = maN;
          st_.pstMyCh_->NextAuto.ADash_f = FALSE;
          break;
        case dgsSw:
          st_.pstMyCh_->NextAuto.AutoType = dbatDgSw;
          SwayDodge();
          break;
        case dgsCa:
          st_.pstMyCh_->NextAuto.AutoType = dbatCPUCa;
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          st_.pstMyCh_->NextAuto.AMuki = maN;
          st_.pstMyCh_->NextAuto.ADash_f = FALSE;
          //ダッシュキャッチ狙いはしない
          break;
        }

      }
    }

  }
  //ＣＯＭよけ//ダッシュキャッチ狙い★★
  void TChMove::CheckDodgeCPU()
  {

    //敵のシュートが来てるときのみ
    if ((pmgSG_->stBa_.Motion == bmShoot)
      && (pmgSG_->stBa_.ShTgTNo == st_.mysideNo_)
      && (IsInfield()))
    {

      //ボールなしのみ
      if (IsBall()) return;

      //自由のみ
      if (IsFreeMotion(FALSE) == FALSE) return;

      //自由のみ
      if ((st_.pstMyCh_->NextAuto.AutoType == dbatFree)
        || (st_.pstMyCh_->NextAuto.AutoType == dbatReturn)
        || (st_.pstMyCh_->NextAuto.AutoType == dbatDfPos)
        || (st_.pstMyCh_->NextAuto.AutoType == dbatCPUHelp))
      {

        BOOL ca_f = FALSE;


        //キャッチ対応確率//ブロック,する,しない,強い球ならばしない
        switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaCatchJdg])
        {
        case 0://ブロック
          ca_f = !(pCommon_->IsDangerBall());
          break;
        case 1://する
          ca_f = TRUE;
          break;
        case 2://しない
          ca_f = FALSE;
          break;
        default://強い球ならばしない
          ca_f = !(pCommon_->IsStrongBall());
          break;
        }

        enTDGSL ds = CheckDS(ca_f);//ＣＯＭ思考で★
        //enTDGSL ds = CheckDS(FALSE);//ＣＯＭ思考で★

        BOOL dsca_f = FALSE;
        BOOL gd_f = FALSE;

        switch (ds)
        {
        case dgsJp:
          st_.pstMyCh_->NextAuto.AutoType = dbatDgJp;
          st_.pstMyCh_->NextAuto.AMuki = maN;
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          st_.pstMyCh_->NextAuto.ADash_f = FALSE;
          break;
        case dgsCr:
          st_.pstMyCh_->NextAuto.AutoType = dbatDg;
          st_.pstMyCh_->NextAuto.AMuki = maN;
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          st_.pstMyCh_->NextAuto.ADash_f = FALSE;
          break;
        case dgsSw:
          st_.pstMyCh_->NextAuto.AutoType = dbatDgSw;
          SwayDodge();
          break;
        case dgsCa:
          //ヘルプの時はボールに向かっていく
          if (st_.pstMyCh_->NextAuto.AutoType == dbatCPUHelp)
          {
            gd_f = TRUE;
          }

          st_.pstMyCh_->NextAuto.AutoType = dbatCPUCa;
          if (gd_f == FALSE)
          {
            //ダッシュキャッチ狙い★★
            dsca_f = IsOKDashCatch();
          }

          //ダッシュキャッチ狙い

          if (dsca_f)
          {

            st_.pstMyCh_->NextAuto.ADash_f = TRUE;

            const s32 DISTZ = 8 * XYMAG;

            if (st_.pstMyCh_->Zahyou.Z < (pmgSG_->stBa_.Zahyou.Z - DISTZ))
            {
              st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
            }
            else if (st_.pstMyCh_->Zahyou.Z > (pmgSG_->stBa_.Zahyou.Z + DISTZ))
            {
              st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
            }
            else
            {
              st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
            }

            st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
              ? maR
              : maL;
          }
          else if (gd_f)
          {
            GuardMove();
          }
          else
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
            st_.pstMyCh_->NextAuto.AMuki = maN;
            st_.pstMyCh_->NextAuto.ADash_f = FALSE;
          }

          break;
        }

      }
    }
  }

  //ＣＯＭジャンプボール★
  void TChMove::CheckJumpBallCPU()
  {
    //ボールがジャンプボール状態
    if (pmgSG_->stBa_.JumpBall != jbJpball) return;

    if ((st_.pstMyCh_->NextAuto.AutoType != dbatFree)
      && (st_.pstMyCh_->NextAuto.AutoType != dbatCPUJumpBall)) return;

    if (pmgSG_->stBa_.Motion == bmReferee)
    {
      st_.pstMyCh_->NextAuto.AMuki = maN;
      st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
      st_.pstMyCh_->NextAuto.ADash_f = FALSE;
      st_.pstMyCh_->stCOM.JpBallWait_c = 10;
      st_.pstMyCh_->NextAuto.AutoType = dbatCPUJumpBall;
      return;
    }

    st_.pstMyCh_->NextAuto.AMuki = maN;
    st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
    st_.pstMyCh_->NextAuto.ADash_f = FALSE;

    //まだ投げてない
    if (pmgSG_->stRf_.Motion == dbrfJumpBallWait)
    {
    }
    else if (pmgSG_->stBa_.Zahyou.dY > 0)//(st_.pstMyCh_->stCOM.JpBallWait_c > 0)//上に歩く
    {
      const s32 DIST = (4 * XYMAG);
      const s32 JBZ = (8 * XYMAG);
      const s32 JBX = POSPOST_X;

      st_.pstMyCh_->NextAuto.AMuki = maN;
      st_.pstMyCh_->NextAuto.AMukiZ = mzaN;

      if (st_.pstMyCh_->Zahyou.Z < (pmgSG_->stBa_.LandZ + JBZ))
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
      }
      else if (st_.pstMyCh_->Zahyou.Z > (pmgSG_->stBa_.LandZ + JBZ + DIST))
      {
        st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
      }


      if (GetLeftCrtX() < (JBX - DIST))
      {
        st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
          ? maR
          : maL;
      }
      else if (GetLeftCrtX() > (JBX + DIST))
      {
        st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
          ? maL
          : maR;
      }


      //lib_num::AprTo0(&st_.pstMyCh_->stCOM.JpBallWait_c);
    }
    else
    {
      if (st_.pstMyCh_->stCOM.JpBallStep == 0)
      {
        s32 distCL = (DBCRT_CL - st_.pstMyCh_->Zahyou.X);//センターラインまでの距離
        s32 tx = abs(distCL / pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashX));//センターラインまで走って掛かる時間


        //着いたらダッシュを待つ
        s32 distBP = (pmgSG_->stBa_.Zahyou.Z - st_.pstMyCh_->Zahyou.Z);//ボールとのＺ距離
        s32 tz = (pmgSG_->stBa_.Zahyou.dZ != 0)//ボールが並ぶまでの時間
          ? abs(distBP / pmgSG_->stBa_.Zahyou.dZ)
          : abs(distBP);
        s32 tGrv = pmgEO_->mgDt_.dtSet_.GetDtGrv(setGRV_Y);
        s32 ty = abs(pmgEO_->mgDt_.dtSet_.GetDtJump(setDashJump_vY0) / tGrv);//頂点までの時間

        switch (st_.pstMyCh_->Motion.Mtype)
        {
        case dbmtSt:
        case dbmtWk:

          //ダッシュ開始
          if (tz <= tx)
          {
            st_.pstMyCh_->NextAuto.ADash_f = TRUE;
            st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
              ? maR
              : maL;
          }
          else
          {
            st_.pstMyCh_->NextAuto.ADash_f = FALSE;
            st_.pstMyCh_->NextAuto.AMuki = maN;
            st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          }
          break;
        case dbmtDs:

          tx = abs(distCL / pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashJpX));//センターラインまでジャンプして掛かる時間
          //ジャンプ
          if (ty >= tx)
          {
            st_.pstMyCh_->stCOM.JpBallStep = 1;
          }

          st_.pstMyCh_->NextAuto.ADash_f = TRUE;
          st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
            ? maR
            : maL;
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;

          break;
        }
      }
      else
      {

      }
    }

    st_.pstMyCh_->NextAuto.AutoType = dbatCPUJumpBall;
  }

  //ダッシュ
  void TChMove::DashSet_SetAutoMove()
  {
    if (st_.pstMyCh_->Motion.Mtype == dbmtDs)
    {
      //向き違うとき
      if (((st_.pstMyCh_->NextAuto.AMuki == maL) && (st_.pstMyCh_->Zahyou.DsMuki == mR))
        || ((st_.pstMyCh_->NextAuto.AMuki == maR) && (st_.pstMyCh_->Zahyou.DsMuki == mL)))
      {
        //スリップ
        pCommon_->SetSlip();
      }
    }
    else
    {
      if (st_.pstMyCh_->NextAuto.AMuki == maL)
      {
        pCommon_->MukiSetDash(mL);
        pCommon_->SetMtypeReset(dbmtDs);
      }
      else if (st_.pstMyCh_->NextAuto.AMuki == maR)
      {
        pCommon_->MukiSetDash(mR);
        pCommon_->SetMtypeReset(dbmtDs);
      }
    }
  }

  //ボールの方向を向く
  void TChMove::SetBallMukiX()
  {
    const s32 DEFDISTX = 16 * XYMAG;

    s32 distX = (IsInfield())
      ? (DEFDISTX / 2)
      : DEFDISTX;

    SetBallMukiX(distX);
  }
  void TChMove::SetBallMukiX(s32 distx)
  {
    if (IsBall()) return;

    //自分がタゲでパスが飛んできてるときはそのままの向き自分が投げたときも
    if ((IsInfield() == FALSE)
      && (pmgSG_->stBa_.Motion == bmPass)
      && (pmgSG_->stBa_.PichTNo == st_.mysideNo_)
      && (pmgSG_->stBa_.PichPNo == st_.posNo_))
    {
      return;
    }

    s32 asdf = pmgSG_->stBa_.Zahyou.X;
    s32 asdf2 = st_.pstMyCh_->Zahyou.X;

    if (pmgSG_->stBa_.Zahyou.X < (st_.pstMyCh_->Zahyou.X - distx))
    {
      st_.pstMyCh_->NextAuto.AMuki = maL;
    }
    else if (pmgSG_->stBa_.Zahyou.X > (st_.pstMyCh_->Zahyou.X + distx))
    {
      st_.pstMyCh_->NextAuto.AMuki = maR;
    }
  }

  void TChMove::SetBallMukiZ()
  {

    if (IsBall()) return;

    //自分がタゲでパスが飛んできてるときはそのままの向き
    //自分が投げたときも
    if ((IsInfield() == FALSE) && (pmgSG_->stBa_.Motion == bmPass))
    {
      if ((pmgSG_->stBa_.PichTNo == st_.mysideNo_)
        && (pmgSG_->stBa_.PichPNo == st_.posNo_))
      {
        return;
      }

      //パスの時はパス先を向く
      switch (pmgSG_->stBa_.PaTgPNo)
      {
      case (s32)dbpoO2: st_.pstMyCh_->NextAuto.AMukiZ = mzaB; break;
      case (s32)dbpoO3: st_.pstMyCh_->NextAuto.AMukiZ = mzaF; break;
      default: st_.pstMyCh_->NextAuto.AMukiZ = mzaN; break;
      }
    }
    else
    {
      const s32 DEFDISTZ = 16 * XYMAG;

      s32 distZ = (IsInfield())
        ? DEFDISTZ//(DEFDISTZ / 2)
        : DEFDISTZ;

      if (pmgSG_->stBa_.Zahyou.Z < st_.pstMyCh_->Zahyou.Z)
      {
        if (pmgSG_->stBa_.Zahyou.Z < (st_.pstMyCh_->Zahyou.Z - distZ))
        {
          st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
        }
        else if (pmgSG_->stBa_.Zahyou.Z > (st_.pstMyCh_->Zahyou.Z - (distZ / 2)))
        {
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
        }
        else
        {
          if (st_.pstMyCh_->NextAuto.AMukiZ == mzaB)
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          }
        }
      }
      else
      {
        if (pmgSG_->stBa_.Zahyou.Z > (st_.pstMyCh_->Zahyou.Z + distZ))
        {
          st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
        }
        else if (pmgSG_->stBa_.Zahyou.Z < (st_.pstMyCh_->Zahyou.Z + (distZ / 2)))
        {
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
        }
        else
        {

          if (st_.pstMyCh_->NextAuto.AMukiZ == mzaF)
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          }
        }
      }


      //if (pmgSG_->stBa_.Zahyou.Z < (st_.pstMyCh_->Zahyou.Z - distZ))
      //{
      //  st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
      //}
      //else if (pmgSG_->stBa_.Zahyou.Z > (st_.pstMyCh_->Zahyou.Z + distZ))
      //{
      //  st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
      //}
      //else
      //{
      //  st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
      //}
    }
  }



  void TChMove::GenAMv_Wk(BOOL &MukiSet_f)
  {
    //移動
    if ((st_.pstMyCh_->NextAuto.AMuki != maN)
      || (st_.pstMyCh_->NextAuto.AMukiZ != mzaN))
    {
      switch (st_.pstMyCh_->Motion.Mtype)
      {
      case dbmtDs:
      case dbmtPWDs:
      case dbmtSt:
      case dbmtPW:
        pCommon_->SetMtype(dbmtWk);
        break;
      }
    }
    else//とまる
    {
      pCommon_->SetMtype(dbmtSt);
    }

    MukiSet_f = TRUE;
  }


  //ボールの方を向きながら歩く(停止は別箇所)
  void TChMove::GenAMv_WkLkBl(BOOL &MukiSet_f)
  {
    if (IsBall() == FALSE)
    {
      const s32 MUKIDIST = 16 * XYMAG;

      //ボールの方を向きながら歩く
      if (pmgSG_->stBa_.Zahyou.X < (st_.pstMyCh_->Zahyou.X - MUKIDIST))
      {
        st_.pstMyCh_->Zahyou.Muki = mL;
      }
      else if (pmgSG_->stBa_.Zahyou.X > (st_.pstMyCh_->Zahyou.X + MUKIDIST))
      {
        st_.pstMyCh_->Zahyou.Muki = mR;
      }

      //ボールの方を向きながら歩く
      if (pmgSG_->stBa_.Zahyou.Z < (st_.pstMyCh_->Zahyou.Z - MUKIDIST))
      {
        st_.pstMyCh_->Zahyou.MukiZ = mzF;
      }
      else if (pmgSG_->stBa_.Zahyou.Z > (st_.pstMyCh_->Zahyou.Z + MUKIDIST))
      {
        st_.pstMyCh_->Zahyou.MukiZ = mzB;
      }

    }

    switch (st_.pstMyCh_->Motion.Mtype)
    {
    case dbmtDs:
    case dbmtSt:
    case dbmtPW:
      pCommon_->SetMtype(dbmtWk);
      break;
    }

    MukiSet_f = FALSE;
  }

  void TChMove::GenAMv_Ds(BOOL &MukiSet_f)
  {

    switch (st_.pstMyCh_->Motion.Mtype)
    {
    case dbmtDs:
      DashSet_SetAutoMove();//ダッシュの時はスリップ
      break;
    case dbmtSt:
    case dbmtPW:
    case dbmtWk:
      DashSet_SetAutoMove();
      MukiSet_f = TRUE;
      break;
    }
  }


  BOOL TChMove::AMv_Free()
  {
    BOOL MukiSet_f = FALSE;

    //ダッシュマンの時★//リターンから戻ってきたときはそのまま
    if (IsDashman() && (st_.pstMyCh_->Auto.AutoType != dbatReturn))
    {
      return FALSE;
    }

    switch (st_.posNo_)
    {
    case (s32)dbpoO2:
      st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
      SetBallMukiX();
      break;
    case (s32)dbpoO3:
      st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
      SetBallMukiX();
      break;
    case (s32)dbpoO4:
      st_.pstMyCh_->NextAuto.AMuki = (st_.mysideNo_ == 0)
        ? maL
        : maR;
      SetBallMukiZ();
      break;
    default:
      SetBallMukiX();
      SetBallMukiZ();
      break;
    }

    //モーションごとの処理
    switch (st_.pstMyCh_->Motion.Mtype)
    {
    case dbmtDs:
      pCommon_->SetMtypeReset(dbmtWk);
      break;
    case dbmtSl:
      break;
    case dbmtSt:
    case dbmtWk:
    case dbmtPW:
      st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;

      if ((pCommon_->MukiSetAuto() //向き変えアリ
        || (st_.pstMyCh_->Motion.Mtype == dbmtWk)))
      {
        pCommon_->SetMtypeReset(dbmtSt);
      }
      return FALSE;//おしまい//MukiSet_fを待ってられない
    }

    //自分がタゲでパスが飛んできてるときはそのままの向き
    //４番外野が自分タゲで飛んでくるときに横を向いてしまうのを防ぐため
    if ((pmgSG_->stBa_.Motion == bmPass)
      && (IsInfield() == FALSE)
      && (((pmgSG_->stBa_.PaTgTNo == st_.mysideNo_) && (pmgSG_->stBa_.PaTgPNo == st_.posNo_))
      || ((pmgSG_->stBa_.PichTNo == st_.mysideNo_) && (pmgSG_->stBa_.PichPNo == st_.posNo_))))
    {
      MukiSet_f = FALSE;
    }
    else if (Kagami_f())//かがみ予約
    {
      MukiSet_f = FALSE;
    }
    else
    {
      MukiSet_f = TRUE;
    }
    return MukiSet_f;
  }

  BOOL TChMove::AMv_Alleyoop()
  {
    BOOL MukiSet_f = FALSE;

    if (IsDashman() && (st_.pstMyCh_->Auto.AutoType != dbatReturn))
    {
      return FALSE;
    }

    //移動
    if (st_.pstMyCh_->NextAuto.AMuki != maN)
    {
      switch (st_.pstMyCh_->Motion.Mtype)
      {
      case dbmtDs:
      case dbmtPWDs:
      case dbmtSt:
      case dbmtPW:
        pCommon_->SetMtype(dbmtWk);
        break;
      }
    }
    else//とまる
    {
      pCommon_->SetMtype(dbmtSt);
    }

    MukiSet_f = TRUE;

    return MukiSet_f;
  }

  BOOL TChMove::AMv_Pos()
  {
    BOOL MukiSet_f = FALSE;

    if ((st_.pstMyCh_->NextAuto.AMuki == maN)
      && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN))
    {

      SetBallMukiX();
      SetBallMukiZ();

      st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;


      if ((pCommon_->MukiSetAuto() //向き変えアリ
        || (st_.pstMyCh_->Motion.Mtype == dbmtWk))
        || (st_.pstMyCh_->Motion.Mtype == dbmtPWWk))
      {
        pCommon_->SetMtypeReset(dbmtSt);
      }

      return FALSE;//おしまい
    }
    else
    {
      if (st_.pstMyCh_->NextAuto.ADash_f)// && (st_.pstMyCh_->NextAuto.AMuki != maN))
      {
        GenAMv_Ds(MukiSet_f);

      }
      else
      {
        //ボールの方を向きながら歩く
        GenAMv_WkLkBl(MukiSet_f);
        //if (IsBall() == FALSE)
        //{
        //  const s32 MUKIDIST = 16 * XYMAG;

        //  //ボールの方を向きながら歩く
        //  if (pmgSG_->stBa_.Zahyou.X < (st_.pstMyCh_->Zahyou.X - MUKIDIST))
        //  {
        //    st_.pstMyCh_->Zahyou.Muki = mL;
        //  }
        //  else if (pmgSG_->stBa_.Zahyou.X > (st_.pstMyCh_->Zahyou.X + MUKIDIST))
        //  {
        //    st_.pstMyCh_->Zahyou.Muki = mR;
        //  }
        //}

        //switch (st_.pstMyCh_->Motion.Mtype)
        //{
        //  case dbmtDs:
        //  case dbmtSt:
        //  case dbmtPW:
        //    pCommon_->SetMtype(dbmtWk);
        //    break;
        //}

      }
    }


    return MukiSet_f;
  }

  BOOL TChMove::AMv_GoToBall()
  {
    BOOL MukiSet_f = FALSE;

    //左右
    if (st_.pstMyCh_->NextAuto.AMuki != maN)
    {
      //ダッシュ
      if (st_.pstMyCh_->NextAuto.ADash_f)
      {
        switch (st_.pstMyCh_->Motion.Mtype)
        {
        case dbmtSt:
          pCommon_->SetMtypeReset(dbmtWk);
          MukiSet_f = TRUE;
          break;
        case dbmtWk:
          DashSet_SetAutoMove();
          MukiSet_f = TRUE;
          break;
        case dbmtDs:
          DashSet_SetAutoMove();
          break;
        }
      }
      else
      {

        //ボールの方を向きながら歩く
        GenAMv_WkLkBl(MukiSet_f);

        //switch (st_.pstMyCh_->Motion.Mtype)
        //{
        //  case dbmtDs:
        //    pCommon_->SetMtypeReset(dbmtWk);//★スリップしないようにしてみた
        //    //pCommon_->SetSlip();//pCommon_->SetMtypeReset(dbmtSl);
        //    break;
        //  case dbmtSt:
        //    pCommon_->SetMtypeReset(dbmtWk);
        //    break;
        //}
        //MukiSet_f = TRUE;
      }
    }
    else if (st_.pstMyCh_->NextAuto.AMukiZ != mzaN)//手前奥
    {
      //ボールの方を向きながら歩く
      GenAMv_WkLkBl(MukiSet_f);

      //if (st_.pstMyCh_->Motion.Mtype == dbmtSt)
      //{
      //  pCommon_->SetMtypeReset(dbmtWk);
      //}
      //MukiSet_f = TRUE;
    }
    return MukiSet_f;
  }

  BOOL TChMove::AMv_Return()
  {
    BOOL MukiSet_f = FALSE;

    //まずオーバーライン処理
    if (IsBall()
      && (st_.pmgRf_->IsNoOverLine() == FALSE)
      && (IsInfield() || st_.pstMyCh_->GaiyaOvL_f))
    {

      switch (st_.pstMyCh_->Motion.Mtype)
      {
      case dbmtDs:
        pCommon_->SetSlip();//pCommon_->SetMtype(dbmtSl);
        break;

      case dbmtSt:
      case dbmtWk:
        pCommon_->SetMtype(dbmtOvL);
        break;
        //他はスルーしてそのモーションの終わりを待つ
      }

      ////攻撃時間処理止める
      //pmgSG_->stBa_.TimerSide = NGNUM;
      //pmgSG_->stBa_.OvLine_f = TRUE;//このフラグ中はパスタゲにならない

      st_.pmgMyTm_->st_.pstMyTm_->RsvResetCOM_f = TRUE;
    }
    //オーバーライン処理が終わってから
    else if (st_.pstMyCh_->Motion.Mtype != dbmtOvL)
    {
      //外野以外も入ってる

      if (st_.pstMyCh_->NextAuto.AMuki != maN)
      {
        //ダッシュ
        if (st_.pstMyCh_->Motion.Mtype == dbmtDs)
        {
          //これだと少しオーバーしてしまう
          if (((st_.pstMyCh_->NextAuto.AMuki == maL) && (st_.pstMyCh_->Zahyou.DsMuki == mR))
            || ((st_.pstMyCh_->NextAuto.AMuki == maR) && (st_.pstMyCh_->Zahyou.DsMuki == mL)))
          {
            //戻ってくるのはスリップよりもぴったり止まった方がよさげ
            pCommon_->SetSlip();//pCommon_->SetMtype(dbmtSl);
            //pCommon_->SetMtypeReset(dbmtWk);//★

            if (pCommon_->IsInCourt() && Kagami_f())
            {
              st_.pstMyCh_->NextAuto.ADashStoped_f = TRUE;
            }
          }
        }
        else
        {
          if (st_.pstMyCh_->NextAuto.ADash_f)
          {

            switch (st_.pstMyCh_->Motion.Mtype)
            {
            case dbmtDs:
              if (Kagami_f() && st_.pstMyCh_->NextAuto.ADashStoped_f)
              {
                pCommon_->SetMtype(dbmtSt);
              }
              else
              {
                DashSet_SetAutoMove();
              }
              break;
            case dbmtSt:
            case dbmtWk:
              if (Kagami_f() && st_.pstMyCh_->NextAuto.ADashStoped_f)
              {
                pCommon_->SetMtype(dbmtSt);
              }
              else
              {
                DashSet_SetAutoMove();
                MukiSet_f = TRUE;
              }
              break;
            }

          }
          else
          {
            switch (st_.pstMyCh_->Motion.Mtype)
            {
            case dbmtDs:
              pCommon_->SetSlip();
              break;
            case dbmtSl:
              if (Kagami_f())
              {
                st_.pstMyCh_->NextAuto.ADashStoped_f = TRUE;
              }
              break;
            case dbmtSt:
            case dbmtPW:
              pCommon_->SetMtype(dbmtWk);
              break;
            }
            MukiSet_f = TRUE;
          }
        }

      }
      else if (st_.pstMyCh_->NextAuto.AMukiZ != mzaN)
      {
        switch (st_.pstMyCh_->Motion.Mtype)
        {
        case dbmtSt:
        case dbmtWk:
        case dbmtPW:
          pCommon_->SetMtype(dbmtWk);
          MukiSet_f = TRUE;
          break;
        }
      }
      else
      {
        pCommon_->SetMtype(dbmtSt);
      }
    }

    return MukiSet_f;
  }

  BOOL TChMove::AMv_GoHome()
  {
    BOOL MukiSet_f = FALSE;

    if (st_.pstMyCh_->Motion.Mtype == dbmtDs)
    {
      DashSet_SetAutoMove();
    }
    else if ((st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
      && (st_.pstMyCh_->Motion.IsMFlags(dbmfDam) == FALSE))
    {
      if (st_.pstMyCh_->NextAuto.AMuki == maL)
      {
        pCommon_->MukiSetDash(mL);
      }
      else if (st_.pstMyCh_->NextAuto.AMuki == maR)
      {
        pCommon_->MukiSetDash(mR);
      }
      pCommon_->SetMtype(dbmtDs);
      MukiSet_f = TRUE;
    }

    return MukiSet_f;
  }

  BOOL TChMove::AMv_Get()
  {
    BOOL MukiSet_f = FALSE;

    if ((st_.pstMyCh_->NextAuto.AMuki == maN)
      && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN))
    {

      const s32 NEARDIST = 4 * XYMAG;
      switch (st_.pstMyCh_->Motion.Mtype)
      {
      case dbmtDs:
        //pCommon_->SetSlip();//pCommon_->SetMtype(dbmtSl);
        pCommon_->SetMtypeReset(dbmtWk);//★
        break;
      case dbmtSl:
        break;
      default:
        pCommon_->SetMtype(dbmtSt);
        SetBallMukiX(NEARDIST);
        SetBallMukiZ();
        MukiSet_f = TRUE;
        break;
      }
    }
    else
    {
      MukiSet_f = AMv_GoToBall();
    }

    return MukiSet_f;
  }

  BOOL TChMove::AMv_ShCvr()
  {
    BOOL MukiSet_f = FALSE;

    if ((st_.pstMyCh_->NextAuto.AMuki == maN)
      && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN))
    {
      switch (st_.pstMyCh_->Motion.Mtype)
      {
      case dbmtDs:
        pCommon_->SetMtypeReset(dbmtWk);//★
        break;
      case dbmtSl:
        break;
      default:
        pCommon_->SetMtype(dbmtSt);
        SetBallMukiX();
        SetBallMukiZ();
        MukiSet_f = TRUE;
        break;
      }

    }
    else
    {
      MukiSet_f = AMv_GoToBall();
    }
    const s32 CATCHDIST = 12 * XYMAG;
    const s32 CATCHDISTY = 32 * XYMAG;

    return MukiSet_f;
  }

  BOOL TChMove::AMv_CPUPaCut()
  {
    BOOL MukiSet_f = FALSE;


    if ((st_.pstMyCh_->NextAuto.AMuki == maN)
      && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN))
    {

      SetBallMukiX();
      SetBallMukiZ();

      st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;


      if ((pCommon_->MukiSetAuto() //向き変えアリ
        || (st_.pstMyCh_->Motion.Mtype == dbmtWk))
        || (st_.pstMyCh_->Motion.Mtype == dbmtPWWk))
      {
        pCommon_->SetMtypeReset(dbmtSt);
      }

      return FALSE;//おしまい
    }
    else
    {
      //ボールの方を向きながら歩く
      GenAMv_WkLkBl(MukiSet_f);
    }

    return MukiSet_f;
  }
  BOOL TChMove::AMv_CPUHelp()
  {
    BOOL MukiSet_f = FALSE;


    if ((st_.pstMyCh_->NextAuto.AMuki == maN)
      && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN))
    {

      SetBallMukiX();
      SetBallMukiZ();

      st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;


      if ((pCommon_->MukiSetAuto() //向き変えアリ
        || (st_.pstMyCh_->Motion.Mtype == dbmtWk))
        || (st_.pstMyCh_->Motion.Mtype == dbmtPWWk))
      {
        pCommon_->SetMtypeReset(dbmtSt);
      }

      return FALSE;//おしまい
    }
    else
    {
      //ボールの方を向きながら歩く
      GenAMv_WkLkBl(MukiSet_f);
    }
    return MukiSet_f;
  }
  BOOL TChMove::AMv_DgSw()
  {
    BOOL MukiSet_f = FALSE;


    if ((st_.pstMyCh_->NextAuto.AMuki == maN)
      && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN))
    {

      SetBallMukiX();
      SetBallMukiZ();

      st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;


      if ((pCommon_->MukiSetAuto() //向き変えアリ
        || (st_.pstMyCh_->Motion.Mtype == dbmtWk))
        || (st_.pstMyCh_->Motion.Mtype == dbmtPWWk))
      {
        pCommon_->SetMtypeReset(dbmtSt);
      }

      return FALSE;//おしまい
    }
    else
    {

      //ボールの方を向きながら歩く
      GenAMv_WkLkBl(MukiSet_f);
    }
    return MukiSet_f;
  }
  void TChMove::AMv_PaCatch()
  {
    //モーションの変更はココでやっていいのか？
    if (pmgSG_->stBa_.PaJPa_f)
    {
      if ((st_.pstMyCh_->Motion.Mtype == dbmtJUp)
        || (st_.pstMyCh_->Motion.Mtype == dbmtJDn))
      {
        pCommon_->SetMtype(dbmtJCa);
        pmgSG_->stBa_.PaETA_c = NGNUM;
      }
    }
    else
    {
      if (st_.pstMyCh_->Motion.Mtype == dbmtSt)
      {
        st_.pstMyCh_->NextAuto.AutoType = dbatPassCach;
        pCommon_->SetMtype(dbmtCa);
      }
    }
  }

  //ＣＯＭ思考行動★★
  BOOL TChMove::AMv_CPUShPaDM()
  {
    BOOL MukiSet_f = FALSE;

    //ダッシュマン作戦時間経過内野のみ移動が入る
    if (IsInfield())
    {
      if (st_.pstMyCh_->NextAuto.ADash_f)
      {
        GenAMv_Ds(MukiSet_f);
      }
      else
      {
        GenAMv_Wk(MukiSet_f);
      }
    }
    else//外野
    {
      switch (st_.pstMyCh_->Motion.Mtype)
      {
      case dbmtDs:
        pCommon_->SetSlip();//スリップ
        break;
      case dbmtSl:
        break;
      case dbmtWk:
      case dbmtPW:
        pCommon_->SetMtype(dbmtSt);
        MukiSet_f = TRUE;
        break;
      }
    }


    //ジャンプ
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashman_f)//作戦中（待ち以外）
    {
      //ボール持ち＆ジャンプ予定＆態勢
      BOOL jpok_f = (IsBall()
        && st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f
        && st_.pstMyCh_->Motion.IsMFlags(dbmfJpOK));

      //コールＯＫ
      if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.callOK_f)
      {
        //内野ダッシュしている場合
        if (IsInfield() && st_.pstMyCh_->NextAuto.ADash_f)
        {
          //ジャンプ開始位置まできた
          if (jpok_f && //ボール持ってるときはダッシュマン（NoJp）でも通常のアタックラインでジャンプ
            (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(TRUE, FALSE)))
          {
            pCommon_->JumpSet(FALSE, FALSE, FALSE);
          }

          ////ダッシュ攻撃ライン
          //if (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(FALSE))//ここまで来たらシュートＯＫ
          //{
          //  //st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashmanShOK_f = TRUE;
          //}
        }
        else if (IsInfield() == FALSE)//外野
        {
          //ジャンプ予定
          if (jpok_f)
          {
            //すぐ飛ぶ
            pCommon_->JumpSet(FALSE, FALSE, FALSE);
          }
        }
      }
      else//セッターパス
      {
        if (jpok_f)
        {
          //すぐ飛ぶ
          pCommon_->JumpSet(FALSE, FALSE, FALSE);
        }
      }
    }


    return MukiSet_f;
  }

  BOOL TChMove::AMv_CPUShPaStd()
  {

    BOOL MukiSet_f = FALSE;

    //チームウエイトすんでから
    if (st_.pmgMyTm_->IsWaitOver())
    {
      //内野の動き出し
      if (IsInfield())
      {
        if (IsBall())
        {
          if (st_.pstMyCh_->NextAuto.ADash_f)
          {
            GenAMv_Ds(MukiSet_f);
          }
          else
          {
            if (st_.pstMyCh_->NextAuto.AMuki != maN)
            {
              switch (st_.pstMyCh_->Motion.Mtype)
              {
              case dbmtDs:
              case dbmtSt:
              case dbmtPW:
                pCommon_->SetMtype(dbmtWk);
                MukiSet_f = TRUE;
                break;
              case dbmtWk:
                MukiSet_f = TRUE;
                break;
              }
            }
            else
            {
              switch (st_.pstMyCh_->Motion.Mtype)
              {
              case dbmtDs:
                pCommon_->SetSlip();//スリップ
                break;
              case dbmtSl:
                break;
              case dbmtWk:
              case dbmtPW:
                pCommon_->SetMtype(dbmtSt);
                MukiSet_f = TRUE;
                break;
              }
            }
          }
        }
        else//ボール無し
        {
          switch (st_.pstMyCh_->Motion.Mtype)
          {
          case dbmtDs:
            pCommon_->SetSlip();//スリップ
            break;
          case dbmtSl:
            break;
          case dbmtWk:
          case dbmtPW:
            pCommon_->SetMtype(dbmtSt);
            MukiSet_f = TRUE;
            break;
          }
        }
      }
      else//外野
      {
        switch (st_.pstMyCh_->Motion.Mtype)
        {
        case dbmtDs:
          pCommon_->SetSlip();//スリップ
          break;
        case dbmtSl:
          break;
        case dbmtWk:
        case dbmtPW:
          pCommon_->SetMtype(dbmtSt);
          MukiSet_f = TRUE;
          break;
        }
      }


      //ジャンプ予定＆態勢
      BOOL jpok_f = (IsBall()
        && st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f
        && st_.pstMyCh_->Motion.IsMFlags(dbmfJpOK));

      //ジャンプシュート用ジャンプ
      if (jpok_f)
      {
        //内野ダッシュジャンプ
        if (IsInfield() && st_.pmgMyTm_->st_.pstMyTm_->stCOM.ds_f)//st_.pstMyCh_->NextAuto.ADash_f)//
        {
          if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.dspos_f
            && (st_.pmgMyTm_->st_.pstMyTm_->stCOM.waitmove_c == 0))//ダッシュ開始指定位置条件クリア
          {
            //一歩ＤＪＳ
            if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.onestep_f)
            {
              if (st_.pstMyCh_->Step_c >= 1)
              {
                pCommon_->JumpSet(FALSE, FALSE, FALSE);
              }
            }
            else
            {
              if (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(TRUE, FALSE))//ジャンプ開始位置まできた
              {
                pCommon_->JumpSet(FALSE, FALSE, FALSE);
              }
            }
          }
        }
        else//ダッシュ攻撃ではない
        {
          //すぐ飛ぶ
          pCommon_->JumpSet(FALSE, FALSE, FALSE);
        }
      }
    }
    else//行動時間まで待つ
    {
      //内野部分的に動く
      if (IsInfield()
        && IsBall()
        && st_.pmgMyTm_->st_.pstMyTm_->stCOM.ds_f
        && (st_.pstMyCh_->NextAuto.ADash_f == FALSE)
        && (st_.pstMyCh_->NextAuto.AMuki != maN))
      {
        switch (st_.pstMyCh_->Motion.Mtype)
        {
        case dbmtDs:
        case dbmtSt:
        case dbmtPW:
          pCommon_->SetMtype(dbmtWk);
          MukiSet_f = TRUE;
          break;
        case dbmtWk:
          MukiSet_f = TRUE;
          break;
        }
      }
      else
      {
        switch (st_.pstMyCh_->Motion.Mtype)
        {
        case dbmtDs:
          pCommon_->SetSlip();//スリップ
          break;
        case dbmtSl:
          break;
        case dbmtWk:
        case dbmtPW:
          pCommon_->SetMtype(dbmtSt);
          break;
        }

        if ((IsInfield() == FALSE)
          || st_.pmgMyTm_->st_.pstMyTm_->stCOM.dspos_f)
        {
          MukiSet_f = TRUE;
        }
        else
        {
          MukiSet_f = FALSE;
        }

      }
    }

    return MukiSet_f;
  }
  BOOL TChMove::AMv_CPUShPa()
  {
    BOOL MukiSet_f = FALSE;

    //ダッシュマン
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.Isdm())
    {
      MukiSet_f = AMv_CPUShPaDM();
    }
    else//通常
    {
      MukiSet_f = AMv_CPUShPaStd();
    }

    return MukiSet_f;

  }

  BOOL TChMove::AMv_CPUCa()
  {
    BOOL MukiSet_f = FALSE;

    if (st_.pstMyCh_->NextAuto.ADash_f)
    {
      GenAMv_Ds(MukiSet_f);
    }
    else
    {
      if ((st_.pstMyCh_->NextAuto.AMuki == maN)
        && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN))
      {

        SetBallMukiX();
        SetBallMukiZ();

        st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;


        if ((pCommon_->MukiSetAuto() //向き変えアリ
          || (st_.pstMyCh_->Motion.Mtype == dbmtWk))
          || (st_.pstMyCh_->Motion.Mtype == dbmtPWWk))
        {
          pCommon_->SetMtypeReset(dbmtSt);
        }

        return FALSE;//おしまい
      }
      else
      {
        //ボールの方を向きながら歩く
        GenAMv_WkLkBl(MukiSet_f);
      }

    }
    return MukiSet_f;
  }

  BOOL TChMove::AMv_CPUJumpBall()
  {
    BOOL MukiSet_f = FALSE;

    if (st_.pstMyCh_->stCOM.JpBallStep > 1)//あとは録るだけ
    {
      pCommon_->AutoPickUp();//自動拾い
    }
    else if (st_.pstMyCh_->stCOM.JpBallStep > 0)//ジャンプ
    {
      pCommon_->JumpSet(TRUE, FALSE, FALSE);
      st_.pstMyCh_->stCOM.JpBallStep = 2;//終了
    }
    else
    {
      //待ち
      if ((st_.pstMyCh_->NextAuto.AMuki == maN)
        && (st_.pstMyCh_->NextAuto.AMukiZ == mzaN)
        && (st_.pstMyCh_->NextAuto.ADash_f == FALSE))
      {

        SetBallMukiX();
        SetBallMukiZ();

        st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;


        if ((pCommon_->MukiSetAuto() //向き変えアリ
          || (st_.pstMyCh_->Motion.Mtype == dbmtWk))
          || (st_.pstMyCh_->Motion.Mtype == dbmtPWWk))
        {
          pCommon_->SetMtypeReset(dbmtSt);
        }

        return FALSE;//おしまい

      }
      //ダッシュ
      else if (st_.pstMyCh_->NextAuto.ADash_f
        && (st_.pstMyCh_->NextAuto.AMuki != maN))
      {
        GenAMv_Ds(MukiSet_f);
      }
      else
      {
        switch (st_.pstMyCh_->Motion.Mtype)
        {
        case dbmtDs:
        case dbmtSt:
        case dbmtPW:
          pCommon_->SetMtype(dbmtWk);
          break;
        }

        MukiSet_f = TRUE;
      }
    }

    return MukiSet_f;
  }

  BOOL TChMove::AMv_Dashman()
  {
    BOOL MukiSet_f = FALSE;

    BOOL cometosspa_f = ((pmgSG_->stBa_.Motion == bmPass)
      && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
      && (pmgSG_->stBa_.PaTgPNo == st_.posNo_)
      && (pmgSG_->stBa_.PaJPa_f));

    st_.pstMyCh_->Zahyou.Muki = (st_.mysideNo_ == 0)
      ? mR
      : mL;
    switch (st_.pstMyCh_->Motion.Mtype)
    {
    case dbmtDs:
      //ジャンプするチーム
      if ((st_.pmgMyTm_->IsNoJpDashman() == FALSE)
        || cometosspa_f)//★トスパスなら飛ぶ
      {
        //ダッシュマン目標Ｘ
        if (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(TRUE, TRUE))
        {
          pCommon_->JumpSet(TRUE, FALSE, FALSE);//ジャンプ
        }
      }
      break;
    case dbmtSt:
    case dbmtPW:
    case dbmtWk:
      DashSet_SetAutoMove();
      MukiSet_f = TRUE;
      break;
    }


    return MukiSet_f;
  }

  void TChMove::AMv_MukiSet()
  {
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfMuki))
    {
      //内野からノンタゲでＺ軸にシュートしたとき
      //それを取りに行った外野が上向きでとまるのを直す
      //シュートを味方外野が直で取ったとき
      //コートと逆方向押しっぱなしでそっちを向いてしまうのを直す

      BOOL ResetMuk_f = FALSE;

      if (st_.pstMyCh_->NextAuto.AutoType == dbatFree)
      {
        ResetMuk_f = (pCommon_->MukiSet(TRUE, TRUE, FALSE));
      }
      else
      {
        ResetMuk_f = (pCommon_->MukiSet(FALSE, FALSE, FALSE));
      }

      if (ResetMuk_f && (st_.pstMyCh_->Motion.IsMFlags(dbmfMuki)))
      {
        pCommon_->SetMtypeReset(st_.pstMyCh_->Motion.Mtype);
      }

    }
  }

  //設定された動作を反映
  void TChMove::SetAutoMove()
  {

    if (pmgSG_->stRf_.DBSSt == dbssReady) return;

    //        //逆向きで走ってる/ちゃんと原因を追及する　//オートバックダッシュ
    //        if ((st_.pstMyCh_->Motion.Mtype == dbmtDs)
    //            && (st_.pstMyCh_->Zahyou.DsMuki != st_.pstMyCh_->Zahyou.Muki))
    //        {
    //            //向きロックが入ると、勝手に向きを変えてしまうのは混乱する
    //            if (MukiLockBtn_f() == FALSE)
    //            {
    //                st_.pstMyCh_->Zahyou.Muki = st_.pstMyCh_->Zahyou.DsMuki;
    //            }
    //        }

    BOOL MukiSet_f = FALSE;

    //アクション中はナシよ
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAct) == FALSE)
    {
      switch (st_.pstMyCh_->NextAuto.AutoType)
      {
        //自由
      case dbatFree:
        MukiSet_f = AMv_Free();
        break;

        //アリウープ
      case dbatAlleyoop:
        MukiSet_f = AMv_Alleyoop();
        break;

        //戻ってくる
      case dbatReturn:
      case dbatKgReturn:
        MukiSet_f = AMv_Return();
        break;

        //外にはける
      case dbatGohome:
        MukiSet_f = AMv_GoHome();
        break;

        //ボール取りにいく
      case dbatGet:
      case dbatPassCvr:
        MukiSet_f = AMv_Get();
        break;

        //シュートカバー
      case dbatShCv:
        MukiSet_f = AMv_ShCvr();
        break;

        //ポジショニング
      case dbatDfPos:
        MukiSet_f = AMv_Pos();
        break;

        //パスキャッチモーション
      case dbatPassCach:
        AMv_PaCatch();
        break;

        //ジャンプ//ジャンプ避け
      case dbatPassJp:
        pCommon_->JumpSet(TRUE, FALSE, TRUE);//垂直ジャンプ
        break;

      case dbatDgJp:
        pCommon_->JumpSet(TRUE, FALSE, FALSE);//垂直ジャンプ
        break;

        //COMのシュート予定
      case dbatCPUShPa:
        MukiSet_f = AMv_CPUShPa();
        break;

        //キャッチ待ち
      case dbatCPUCa:
        MukiSet_f = AMv_CPUCa();
        break;

        //移動避け
      case dbatDgSw:
        MukiSet_f = AMv_DgSw();
        break;

        //COMジャンプボール
      case dbatCPUJumpBall:
        MukiSet_f = AMv_CPUJumpBall();
        break;

        //COMパスカット
      case dbatCPUPaCut:
        MukiSet_f = AMv_CPUPaCut();
        break;

        //COMヘルプ
      case dbatCPUHelp:
        MukiSet_f = AMv_CPUHelp();
        break;

        //ダッシュマン
      case dbatDashman:
        MukiSet_f = AMv_Dashman();
        break;
      }
    }

    st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;

    //向きセット
    if (MukiSet_f)
    {
      AMv_MukiSet();
    }
  }

  //ダッシュ入力
  void TChMove::DashInput()
  {
    if (IsSelfCtrl() == FALSE) return;

    BOOL ds_f = FALSE;

    if (MyPad()->IsDashLR(dxL))//ダッシュ入力
    {
      pCommon_->MukiSetDash(mL);
      ds_f = TRUE;
      if (st_.pstMyTm_->smart_f_) st_.pstMyCh_->DsAccIOS_f = TRUE;
    }
    else if (MyPad()->IsDashLR(dxR))
    {
      pCommon_->MukiSetDash(mR);
      ds_f = TRUE;
      if (st_.pstMyTm_->smart_f_) st_.pstMyCh_->DsAccIOS_f = TRUE;
    }



    //スマート入力
    if ((ds_f == FALSE)
      && st_.pstMyTm_->smart_f_
      && MyPad()->IsDashAuto())
    {
      ds_f = TRUE;
      st_.pstMyCh_->DsAccIOS_f = FALSE;
      if (st_.mysideNo_ == SIDE0)
      {
        if (GetLeftCrtX() < (DBCRT_CL - (pmgEO_->mgDt_.dtSet_.GetDtInput(setBackDashX) * XYMAG)))
          pCommon_->MukiSetDash(mR);
        else
          pCommon_->MukiSetDash(mL);
      }
      else
      {
        if (GetLeftCrtX() < (DBCRT_CL - (pmgEO_->mgDt_.dtSet_.GetDtInput(setBackDashX) * XYMAG)))
          pCommon_->MukiSetDash(mL);
        else
          pCommon_->MukiSetDash(mR);
      }
    }

    if (ds_f) pCommon_->SetMtypeReset(dbmtDs);
  }


  //直前のキー
  void TChMove::SetLastKeyLR()
  {
    if (IsSelfCtrl() == FALSE) return;

    if (MyPad()->IsWalk2(dxL))//歩き入力
    {
      st_.pstMyCh_->LastLRKey = maL;
    }
    else if (MyPad()->IsWalk2(dxR))
    {
      st_.pstMyCh_->LastLRKey = maR;
    }
    else if (MyPad()->IsWalk2(dxU) || MyPad()->IsWalk2(dxD))
    {
      st_.pstMyCh_->LastLRKey = maN;
    }

  }

  //キー入力
  void TChMove::MCKeyInput()
  {
    if (IsShiai() == FALSE) return;
    //動けない
    if (st_.pstMyCh_->Nomove_f) return;


    //プレビュー中パス禁止
    BOOL jpbtn_f = MyPad()->IsJump();
    BOOL jpbtnlong_f = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
      ? MyPad()->IsDsJumpLongTouch()//ダッシュ中スワイプジャンプ
      : MyPad()->IsJumpLongTouch();
    BOOL jpbtn2_f = MyPad()->IsJump2();
    BOOL dsbtn_f = MyPad()->IsDash(IsBall());

    BOOL wkbtn_f = MyPad()->IsAnyWalk2();
    BOOL ng_f = FALSE;


    if (st_.pstMyTm_->smart_f_)
    {
      //ジャンプボタンが放されて居ればジャンプ可能に
      if (st_.pstMyCh_->jumpok_f_ == FALSE)
      {
        if (jpbtn2_f == FALSE)
        {
          st_.pstMyCh_->jumpok_f_ = TRUE;
        }
        else
        {
          jpbtnlong_f = FALSE;//キャッチ後ジャンプおしっぱで出てしまうのカット
        }
      }

      //BOOL longjumpng_f = ((pmgSG_->stBa_.Motion == bmShoot)
      //  || (pmgSG_->stBa_.Motion == bmPass)
      //  || ((pmgSG_->stBa_.Motion == bmHold) && pmgSG_->stBa_.shotmotion_f));

      //if (longjumpng_f)
      //{
      //  jpbtnlong_f = FALSE;
      //}
    }

    jpbtn_f |= jpbtnlong_f;


    //チュートリアルのＮＧ行為
    if (pmgEO_->stShiai_.IsTuto() && (st_.mysideNo_ == SIDE0))
    {
      switch (st_.pmgRf_->GetTutoNo())
      {
      case sta_DodgeAct:
        ng_f = TRUE;
        break;
      case sta_CatchAct:
        ng_f = TRUE;
        break;
      case sta_WalkAct:
        dsbtn_f = FALSE;
        jpbtn_f = FALSE;
        jpbtn2_f = FALSE;
        break;
      case sta_DashAct:
        if (jpbtn_f)
        {
          st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_dash_ngjump);//よけおしてる
        }
        jpbtn_f = FALSE;
        jpbtn2_f = FALSE;
        break;
      case sta_JumpAct:
        wkbtn_f = FALSE;
        if (dsbtn_f)
        {
          dsbtn_f = FALSE;
          st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_jump_ngdash);//よけおしてる
        }
        break;
      case sta_ShotAct:
      case sta_GameAct:
      case sta_Fin:
        break;
      default:
        ng_f = TRUE;
        break;
      }
    }

    if (ng_f)
    {
      dsbtn_f = FALSE;
      jpbtn_f = FALSE;
      jpbtn2_f = FALSE;
      wkbtn_f = FALSE;
    }


    switch (st_.pstMyCh_->Motion.Mtype)
    {
    case dbmtSt:
      if (jpbtn_f)//ジャンプ入力
      {
        pCommon_->JumpSet(TRUE, FALSE, FALSE);
      }
      else
      {
        if (dsbtn_f)//ダッシュ入力
        {
          if (IsInfield()) DashInput();
        }
        else if (wkbtn_f)//歩き入力
        {
          if ((IsInfield())
            || ((st_.posNo_ == (s32)dbpoO4)
            && ((MyPad()->IsWalk2(dxU) || MyPad()->IsWalk2(dxD))))
            || (((st_.posNo_ == (s32)dbpoO2) || (st_.posNo_ == (s32)dbpoO3))
            && ((MyPad()->IsWalk2(dxL) || MyPad()->IsWalk2(dxR)))))
          {
            pCommon_->MukiSet(TRUE, FALSE, FALSE);
            pCommon_->SetMtype(dbmtWk);
          }
          else if (pCommon_->MukiSet(TRUE, TRUE, FALSE))
          {
            pCommon_->SetMtype(dbmtSt);
          }

          //最後に押したキー
          SetLastKeyLR();
        }
        else //if (MyPad()->IsBtn2(base::bY))//オート向き
        {
          if (IsBall() == FALSE)//オート向き
          {
            SetBallMukiX();
            SetBallMukiZ();
            st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;
            pCommon_->MukiSetAuto();
            pCommon_->SetMtypeReset(dbmtSt);
          }
        }
      }
      break;

    case dbmtWk:

      if (jpbtn_f)//ジャンプ入力
      {
        pCommon_->JumpSet(TRUE, FALSE, FALSE);
      }
      else if (dsbtn_f)//ダッシュ入力
      {
        if (IsInfield()) DashInput();
      }
      else if ((IsInfield() && (wkbtn_f == FALSE))//歩き入力
        || ((st_.posNo_ == (s32)dbpoO4)
        && ((MyPad()->IsWalk2(dxU) == FALSE)
        && (MyPad()->IsWalk2(dxD) == FALSE)))
        || (((st_.posNo_ == (s32)dbpoO2) || (st_.posNo_ == (s32)dbpoO3))
        && ((MyPad()->IsWalk2(dxL) == FALSE)
        && (MyPad()->IsWalk2(dxR) == FALSE))))
      {
        //外野は立ち止まると画像だけ前後を向く（MukiZは変わらない）
        //★↑そういうのをなくす
        switch (st_.posNo_)
        {
        case (s32)dbpoO2:
        case (s32)dbpoO3:
          pCommon_->MukiSet(TRUE, TRUE, FALSE);
          break;
        case (s32)dbpoO4:
          st_.pstMyCh_->Zahyou.Muki = (st_.mysideNo_ == 0)
            ? mL
            : mR;
          break;
        }
        pCommon_->SetMtype(dbmtSt);
      }
      else
      {
        if (pCommon_->MukiSet(TRUE, FALSE, FALSE))
        {
          pCommon_->SetMtypeReset(dbmtWk);
        }
      }

      //最後に押したキー
      SetLastKeyLR();

      break;


      if (st_.mysideNo_ == SIDE0)
      {
        if (GetLeftCrtX() < (DBCRT_CL - (pmgEO_->mgDt_.dtSet_.GetDtInput(setBackDashX) * XYMAG)))
          pCommon_->MukiSetDash(mR);
        else
          pCommon_->MukiSetDash(mL);
      }
      else
      {
        if (GetLeftCrtX() < (DBCRT_CL - (pmgEO_->mgDt_.dtSet_.GetDtInput(setBackDashX) * XYMAG)))
          pCommon_->MukiSetDash(mL);
        else
          pCommon_->MukiSetDash(mR);
      }



    case dbmtDs:
      if (jpbtn_f)//ジャンプ入力
      {
        pCommon_->JumpSet(TRUE, FALSE, FALSE);
      }
      else if (jpbtn2_f && (GetLeftCrtX() > (DBCRT_CL - (pmgEO_->mgDt_.dtSet_.GetDtInput(setAutoJumpX) * XYMAG))))//オーバーラインしないための自動ジャンプ位置
      {
        pCommon_->JumpSet(TRUE, FALSE, FALSE);
      }
      else
      {
        //スリップ//★向きロック入ってるときは止まらない
        BOOL dsstop_f = FALSE;
        if (st_.pstMyCh_->Zahyou.DsMuki == mL)
        {
          dsstop_f = (MyPad()->IsWalk2(dxR) || MyPad()->IsDashStop(dxR));
        }
        else if (st_.pstMyCh_->Zahyou.DsMuki == mR)
        {
          dsstop_f = (MyPad()->IsWalk2(dxL) || MyPad()->IsDashStop(dxL));
        }

        //BOOL lstop_f = ((st_.pstMyCh_->Zahyou.DsMuki == mL) && (MyPad()->IsWalk2(dxR)));//歩き入力
        //BOOL rstop_f = ((st_.pstMyCh_->Zahyou.DsMuki == mR) && (MyPad()->IsWalk2(dxL)));//歩き入力
        if (dsstop_f)//ダッシュ停止入力
        {
          pCommon_->SetSlip();
        }
        else
        {
          //ダッシュ加速入力
          if (((st_.pstMyCh_->Zahyou.DsMuki == mL) && MyPad()->IsDashAcc(dxL))
            || ((st_.pstMyCh_->Zahyou.DsMuki == mR) && MyPad()->IsDashAcc(dxR)))
          {
            st_.pstMyCh_->DsAccIOS_f = TRUE;
          }
        }
      }
      break;
    case dbmtJUp:
    case dbmtJDn:
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)
        && (st_.pstMyCh_->AirAct_f == FALSE))
      {
        if (wkbtn_f)//歩き入力
        {
          if (IsInfield())
          {
            if (pCommon_->MukiSet(FALSE, FALSE, FALSE))
            {
              pCommon_->SetMtypeReset(dbmtJDn);
            }
          }
          else
          {
            if (pCommon_->MukiSet(TRUE, TRUE, FALSE))
            {
              pCommon_->SetMtypeReset(dbmtJDn);
            }
          }
        }
      }

      //最後に押したキー
      SetLastKeyLR();

      break;
    case dbmtCr:
    case dbmtJCr:

      break;
    case dbmtSl:
      if (jpbtn_f)//ジャンプ入力
      {
        pCommon_->JumpSet(TRUE, FALSE, FALSE);
      }
      break;
    case dbmtCa:
      //オーバーライン奪い
      if (IsBall()
        && wkbtn_f//歩き入力
        && st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
      {
        if (pCommon_->MukiSet(TRUE, FALSE, FALSE))
        {
          pCommon_->PaTagSet();
          pCommon_->ShTagSet(FALSE);
        }
      }
      //else if (dsbtn_f)//キャッチキャンセルダッシュ入力
      //{
      //  if (IsInfield()) DashInput();
      //}
      else if (IsBall() == FALSE)
      {
        pCommon_->CanselJump(FALSE);
      }
      break;
    case dbmtDg:
      //しゃがみからのキャンセルジャンプはダッシュフラグを消す
      pCommon_->CanselJump(IsBall());
      break;
    case dbmtCM:
      break;
    default:
      //判定出るまでもしくは6f以内はジャンプキャンセル可能
      pCommon_->CanselJump(FALSE);
      break;
    }

    //４番外野が後ろを押しているか★
    if (st_.posNo_ == (s32)dbpoO4)
    {
      st_.pstMyCh_->O4BackSh_f =
        (((st_.mysideNo_ == 0) && MyPad()->IsWalk2(dxR))
        || ((st_.mysideNo_ == 1) && MyPad()->IsWalk2(dxL)));
    }
  }

  //操作権キャラ操作
  void TChMove::ManualMove()
  {
    //向きとType初期化
    InitAutoTypeMuki();

    //ポジションから外れているときは戻る
    CheckAutoReturn();

    //取りに行ける場所にあるか
    CheckAutoGetter();

    if (((st_.pstMyCh_->Auto.AutoType == dbatFree)
      && (st_.pstMyCh_->NextAuto.AutoType == dbatFree)))//★ダッシュマン
    {
      if (Kagami_f() == FALSE)
      {
        MCKeyInput();//入力
      }
    }
    else
    {
      //動作セット
      SetAutoMove();
    }

  }

  //ＣＯＭ操作権キャラ操作
  void TChMove::COMMove()
  {
    //向きとType初期化
    InitAutoTypeMuki();

    //ポジションから外れているときは戻る
    CheckAutoReturn();

    if (pmgGO_->pmgRf_->IsPrev())
    {
      //ポジショニング
      CheckMovePos();
    }
    else
    {
      if ((Kagami_f() == FALSE)
        && IsShiai()//これ入れないと試合終了後も飛ぶ
        && (GameEndWait_f() == FALSE)
        && (st_.pstMyCh_->Motion.IsMFlags(dbmfDam) == FALSE))
      {
        //当たらなかったシュートのカバー
        CheckShootCover();

        //アリウープダッシュ
        CheckAlleyoopMove();

        //パスキャッチのジャンプ
        CheckPassCatchJp();

        //みんな死んでたらこれらはやらない
        if (st_.pmgMyTm_->IsAllDead() == FALSE)
        {
          CheckAtcCPU();
          CheckCOMHelpGuard(TRUE);
          CheckJumpBallCPU();
          CheckPassCut();//★
        }

        //取りに行ける場所にあるかCheckAutoReturnよりもあとに持ってこないと∞拾いおとしを繰り返す
        if (PaCvrMan_f())
        {
          CheckPassCover();
        }
        else
        {
          CheckAutoGetter();//取りに行ける場所にあるか
        }
        //ポジショニング
        CheckMovePos();
      }
    }

    //動作セット
    SetAutoMove();

    //MySt.Auto = NextAuto;//SetAutoMove();でやってる
  }

  //オート操作  void TChMove::COMMove()
  void TChMove::AutoMove()
  {

    //向きとType初期化
    InitAutoTypeMuki();

    BOOL returnOnly_f = (st_.pstMyCh_->Dead_f
      || st_.pstMyCh_->Motion.IsMFlags(dbmfDam)
      || st_.pstMyCh_->Motion.IsMFlags(dbmfANG));

    if (returnOnly_f)
    {
      //ポジションから外れているときは戻る
      CheckAutoReturn();
    }
    else if (pmgGO_->pmgRf_->IsPrev())
    {
      //ポジションから外れているときは戻る
      CheckAutoReturn();
      //ポジショニング
      CheckMovePos();
    }
    else
    {

      //ポジションから外れているときは戻る
      CheckAutoReturn();

      if ((Kagami_f() == FALSE)
        && (st_.pstMyCh_->Motion.IsMFlags(dbmfDam) == FALSE))
      {
        //当たらなかったシュートのカバー
        CheckShootCover();

        //アリウープダッシュ
        CheckAlleyoopMove();

        //パスキャッチのジャンプ
        CheckPassCatchJp();

        //操作以外キャラよけ
        //CheckDodge();

        CheckCOMHelpGuard(FALSE);

        //ポジショニング
        CheckMovePos();

        //ダッシュマン
        CheckDashman();
      }

      if (PaCvrMan_f())
      {
        //パスカバー
        CheckPassCover();
      }
      else
      {
        //取りに行ける場所にあるか
        CheckAutoGetter();
      }
    }
    //動作セット
    SetAutoMove();
  }

  //動かさないとき
  void TChMove::NoMove()
  {
    //向きとType初期化
    InitAutoTypeMuki();

    //もどるのだけはやる
    //試合終了時相手コートにいるとfreeになって自コートにワープするのを防ぐため
    CheckAutoReturn();

    //動作セット
    SetAutoMove();
  }

  //デモ帰宅
  void TChMove::GohomeMove()
  {
    //向きとType初期化
    InitAutoTypeMuki();

    CheckGohome();

    SetAutoMove();
  }

  //必殺アシスト★
  void TChMove::GetHSTiming()
  {
    st_.pstMyCh_->HSTiming = hstmNormal;

    if (IsBall()
      && (((st_.pstMyCh_->Motion.Mtype == dbmtJUp))
      || ((st_.pstMyCh_->Motion.Mtype == dbmtJDn))
      || ((st_.pstMyCh_->Motion.Mtype == dbmtDs))
      || (st_.pstMyCh_->Motion.Mtype == dbmtJSh)
      || (st_.pstMyCh_->Motion.Mtype == dbmtSh)))
    {

      if (pCommon_->IsHSTiming())
      {
        st_.pstMyCh_->HSTiming = hstmSpecial;//必殺
      }
    }

  }


  //更新
  void TChMove::UpDate()
  {
    //前フレオート状態だったかを保存
    BOOL lastAuto_f = (st_.pstMyCh_->Auto.AutoType != dbatFree);


    if (GameEndWait_f())
    {
      NoMove();
    }
    else
    {

      if (IsSelfCtrl())
      {
        GetHSTiming();//必殺アシスト
        ManualMove();
      }
      else if (IsCOM() && st_.pmgRf_->IsCOMMove()
        && (IsCtrl() || (IsInfield() && st_.pmgMyTm_->IsLastOne())))
      {
        GetHSTiming();//必殺アシスト
        COMMove();
      }
      else
      {
        AutoMove();//オート
      }
    }

    //タゲ再設定
    //シュートターゲット（アクションに入ったら変更不可）
    if ((Kagami_f() == FALSE)
      && IsBall()
      && (st_.pstMyCh_->Motion.IsMFlags(dbmfAct) == FALSE))
    {
      pCommon_->ShTagSet(FALSE);
      pCommon_->PaTagSet();

      //オート→フリーに変わった瞬間
      if (lastAuto_f
        && (st_.pstMyCh_->Auto.AutoType == dbatFree))
      {
        //最初のタゲを敵の操作キャラに
        //外野の可能性もでるのでなんとかする
        if (pmgSG_->stBa_.ShTgPNo == NGNUM)
        {
          pCommon_->ShTagSet(TRUE);
        }
        st_.pmgEnTm_->SetCtrl(pmgSG_->stBa_.ShTgPNo);
      }
    }


    //戻り回避フラグをたたむ
    if ((st_.pstMyCh_->Auto.AutoType == dbatFree)
      && (st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE))
    {
      st_.pstMyCh_->ECDjp_f = FALSE;
      st_.pstMyCh_->ECDdg_f = FALSE;
    }

  }

}
