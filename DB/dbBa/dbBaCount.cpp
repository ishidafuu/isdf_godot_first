#include "dbBaCount.h"
#include "dbGame.h"

namespace db
{
  
  
  //移動////////////////////////////////
  
  //コンストラクタ
  TBaCount::TBaCount(TGame* pGame, TBaCommon* pCommon)
  : TBaHaveCommon(pGame, pCommon)
  {
    
  }
  
  TBaCount::~TBaCount()
  {
    
  }
  
  //回転カウンタ
  void TBaCount::Spining()
  {
    const s32 SPINMAX = 8 * XYMAG;
    st_.pstSh_->cmn.spin_c += st_.pstSh_->idata.spin;
    
    if (st_.pstSh_->cmn.spin_c >= SPINMAX)
    {
      st_.pstSh_->cmn.spin_c -= SPINMAX;
    }
  }
  
  
  //カメラ移動★
  void TBaCount::CameraPosMove()
  {
    //プレビューの時はカメラ移動なし
    if (pmgGO_->pmgRf_->IsPrev()) return;

    const s32 MOVEDIST = (16 * XYMAG);//16以上の距離から動かし始める
    
    const s32 LOOKBALLDIST = (mid::midGetDotL_Hf() - 48) * XYMAG;//ボールが見える範囲
    const s32 CAMLMAX = (mid::midGetDotL_Hf() * XYMAG) - MOVEDIST;//左限界
    const s32 CAMRMAX = ((GAME_W - mid::midGetDotL_Hf()) * XYMAG) + MOVEDIST;//右限界
    const s32 HOLD_CAMLMAX = CAMLMAX + (32 * XYMAG);
    const s32 HOLD_CAMRMAX = CAMRMAX - (32 * XYMAG);
    
    s32 ballX = st_.pstBa_->Zahyou.X;
    
    BOOL hispd_f = FALSE;
    BOOL norefball_f = FALSE;
    //BOOL sh_f = (st_.pstBa_->Motion == bmShoot);
    
    //カメラ移動先
    s32 toX = ballX;//とりあえずボール
    
    
    //試合終了
    if (pmgSG_->stRf_.DBSSt == dbssResult)
    {
      switch (pmgSG_->stRf_.WinSide)
      {
        case 0: toX = CAMLMAX + MOVEDIST; break;
        case 1: toX = CAMRMAX - MOVEDIST; break;
        default: toX = (GAME_WHf * XYMAG); break;
      }
      
      norefball_f = TRUE;
    }
    else if ((st_.pstBa_->Motion == bmHold) && (st_.pstBa_->HoldPNo < DBMEMBER_INF))//内野が持ってる
    {
      //pmgSG_->stCh_[st_.pstBa_->HoldTNo][st_.pstBa_->HoldPNo]
      
      TStChar* pst = &pmgSG_->stCh_[st_.pstBa_->HoldTNo][st_.pstBa_->HoldPNo];
      
      if (st_.pstBa_->HoldTNo == 0)
      {
        if (pst->Motion.IsMFlags(dbmfDs) && (pst->Zahyou.DsMuki == mR))
        {
          toX = CAMRMAX;
        }
        else if (pst->Zahyou.Muki == mR)
        {
          toX = CAMRMAX;
          //hispd_f = TRUE;
        }
        else
        {
          toX = HOLD_CAMLMAX;
        }
      }
      else
      {
        if (pst->Motion.IsMFlags(dbmfDs) && (pst->Zahyou.DsMuki == mL))
        {
          toX = CAMLMAX;
        }
        else if (pst->Zahyou.Muki == mL)
        {
          toX = CAMLMAX;
          //hispd_f = TRUE;
        }
        else
        {
          toX = HOLD_CAMRMAX;
        }
      }
    }
    else if ((st_.pstBa_->Motion == bmShoot) //内野→内野シュート
             && (st_.pstBa_->PichPNo < DBMEMBER_INF)
             && (st_.pstBa_->ShTgPNo < DBMEMBER_INF))
    {
      if (st_.pstBa_->ShTgTNo == 0)
      {
        toX = CAMLMAX;
      }
      else if (st_.pstBa_->ShTgTNo == 1)
      {
        toX = CAMRMAX;
      }
      //hispd_f = TRUE;
    }
    else if (st_.pstBa_->Motion == bmPass) //内野→外野パス
    {
      if ((st_.pstBa_->PichPNo < DBMEMBER_INF) && (st_.pstBa_->PaTgPNo >= DBMEMBER_INF))//内野→外野
      {
        if (st_.pstBa_->PaTgTNo == 0)
        {
          toX = CAMRMAX;
        }
        else if (st_.pstBa_->PaTgTNo == 1)
        {
          toX = CAMLMAX;
        }
        
        //hispd_f = TRUE;
      }
      else if ((st_.pstBa_->PichPNo >= DBMEMBER_INF) && (st_.pstBa_->PaTgPNo < DBMEMBER_INF))//外野→内野
      {
        if (st_.pstBa_->PaTgTNo == 0)
        {
          toX = CAMLMAX;
        }
        else if (st_.pstBa_->PaTgTNo == 1)
        {
          toX = CAMRMAX;
        }
        //hispd_f = TRUE;//外野→内野パスの時はとりあえず高速
      }
      else
      {
        if (st_.pstBa_->PaTgTNo == 0)
        {
          toX = CAMRMAX;
        }
        else
        {
          toX = CAMLMAX;
        }
      }
    }
    
    
    
    s32 camdist;
    
    //ボールを無視しない場合
    if (norefball_f == FALSE)
    {
      //ボールが見える範囲内
      toX = lib_num::Between(toX, (ballX - LOOKBALLDIST), (ballX + LOOKBALLDIST));
      //コート範囲以内
      toX = lib_num::Between(toX, CAMLMAX, CAMRMAX);
      
      //高速移動しなければいけないぐらい離れている
      
      if (abs(st_.pstBa_->CameraPosX - ballX) > (mid::midGetDotL_Hf() * XYMAG))
        //if ((ballX < (st_.pstBa_->CameraPosX - (mid::midGetDotL_Hf() * XYMAG)))
        //  || (ballX > st_.pstBa_->CameraPosX + (mid::midGetDotL_Hf() * XYMAG)))
      {
        hispd_f = TRUE;
      }
      
      //あそびぶぶん
      camdist = MOVEDIST;
    }
    else
    {
      camdist = 0;
    }
    
    const s32 CAMSPDLO = 100;
    const s32 CAMSPDHI = 200;//シュートの時は速く動く
    
    
    //カメラスピード
    s32 camspd = (hispd_f)
    ? CAMSPDHI
    : CAMSPDLO;
    
    //2ドットずつの移動なのでがたつかないように1ドットずつをループさせる
    
    if ((st_.pstBa_->CameraPosX + camdist) < toX)
    {
      st_.pstBa_->CameraPosX += camspd;
    }
    else if ((st_.pstBa_->CameraPosX - camdist) > toX)
    {
      st_.pstBa_->CameraPosX -= camspd;
    }
    else
    {
      //break;
    }

    //ここでグラフィックマネージャーに渡しておく
    pCommon_->SetCamPos();
    //pmgEO_->mgCamera_.SetCamPos(st_.pstBa_->CameraPosX / XYMAG, 0);
  }
  
  
  //更新★カメラ位置もここでやってみる
  void TBaCount::UpDate()
  {
    //カメラ移動
    CameraPosMove();
    
    //状態ごとの処理
    switch (st_.pstBa_->Motion)
    {
      case bmHold:
        pCommon_->SetNmlBall(FALSE);
        if (st_.pstBa_->HoldPNo)
        {
          
          enDBMotionType mt = pmgSG_->stCh_[st_.pstBa_->HoldTNo][st_.pstBa_->HoldPNo].Motion.Mtype;
          if ((mt == dbmtCM)
              || (mt == dbmtJCM)
              || (mt == dbmtFB)
              || (mt == dbmtJFB))
          {
            Spining();
          }
        }
        break;
      case bmFree:
      case bmReferee:
        pCommon_->SetNmlBall(FALSE);
        break;
      case bmBound:
        pCommon_->SetNmlBall(FALSE);
        Spining();
        break;
      case bmPass:
        pCommon_->SetNmlBall(FALSE);
        Spining();
        //新シュートに必要であれば描き直す
        ++st_.pstSh_->cmn.timeTotal_c;//必殺シュートカウンタ
        lib_num::AprTo0(&st_.pstBa_->NoHit_c);
        lib_num::AprTo0(&st_.pstBa_->PaSelfHit_c);
        if (st_.pstBa_->PaETA_c != NGNUM)
        {
          lib_num::AprTo0(&st_.pstBa_->PaETA_c);
        }
        break;
      case bmShoot:
        Spining();
        lib_num::AprTo0(&st_.pstBa_->NoHit_c);
        lib_num::AprTo0(&st_.pstBa_->PaSelfHit_c);

        //新シュートにかきなおす

        ++st_.pstSh_->cmn.timeTotal_c;//必殺シュートカウンタ

        break;
    }
  }
  
  
  
}
