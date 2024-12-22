#include "dbMgChar.h"
#include "dbGame.h"

namespace db
{
  //プレーヤー動作オブジェ管理および描画////////////////////////////////
  
  //コンストラクタ
  TMgChar::TMgChar(TGame* pGame, s32 sideNo, s32 charNo)
  : TChGen(pGame, sideNo, charNo),
  chCommon_(pGame, sideNo, charNo),
  chCount_(pGame, &chCommon_, sideNo, charNo),
  chMove_(pGame, &chCommon_, sideNo, charNo),
  chAction_(pGame, &chCommon_, sideNo, charNo),
  chRemove_(pGame, &chCommon_, sideNo, charNo),
  chSetRect_(pGame, &chCommon_, sideNo, charNo),
  chDraw_(pGame, &chCommon_, sideNo, charNo)
  {
    
  }
  
  
  TMgChar::~TMgChar()
  {
    
  }
  
  //参照設定
  void TMgChar::DoRefSet()
  {
    //各所時オブジェの参照設定
    chCommon_.RefSet();
    chCount_.RefSet();
    chMove_.RefSet();
    chAction_.RefSet();
    chRemove_.RefSet();
    chSetRect_.RefSet();
    chDraw_.RefSet();
  }
  
  //直前準備
  void TMgChar::DoReady()
  {
    //各所時オブジェの直前準備
    chCommon_.Ready();
    chCount_.Ready();
    chMove_.Ready();
    chAction_.Ready();
    chRemove_.Ready();
    chSetRect_.Ready();
    chDraw_.Ready();

    //ここで初期化
    chCommon_.StInit(FALSE);


    //デモ
    if (pmgSG_->stRf_.Demo_f)
    {
      st_.pstMyCh_->Zahyou.X = 0 - 9600;
      st_.pstMyCh_->Zahyou.Z = DBCRT_BL;
    }
    else
    {
      switch (st_.posNo_)
      {
        case 0:
          st_.pstMyCh_->Zahyou.X = POS0_X;
          st_.pstMyCh_->Zahyou.Z = POS0_Z;
          break;
        case 1:
          st_.pstMyCh_->Zahyou.X = POS1_X;
          st_.pstMyCh_->Zahyou.Z = POS1_Z;
          break;
        case 2:
          st_.pstMyCh_->Zahyou.X = POS2_X;
          st_.pstMyCh_->Zahyou.Z = POS2_Z;
          break;
        case 3:
          st_.pstMyCh_->Zahyou.X = POSPOST_X;
          st_.pstMyCh_->Zahyou.Z = POSPOST_Z;
          break;
        case 4:
          st_.pstMyCh_->Zahyou.X = POS4_X;
          st_.pstMyCh_->Zahyou.Z = CRPOS_B_Z;
          break;
        case 5:
          st_.pstMyCh_->Zahyou.X = POS5_X;
          st_.pstMyCh_->Zahyou.Z = CRPOS_F_Z;
          break;
        case 6:
          st_.pstMyCh_->Zahyou.X = POS6_X;
          st_.pstMyCh_->Zahyou.Z = POS6_Z;
          break;
        case 7:
          st_.pstMyCh_->Live_f = FALSE;//８人目
          break;
      }
    }
    
    st_.pstMyCh_->Zahyou.Muki = (IsInfield())
    ? mR
    : mL;
    
    switch (st_.posNo_)
    {
      case (s32)dbpoO2: st_.pstMyCh_->Zahyou.MukiZ = mzF; break;
      case (s32)dbpoO3: st_.pstMyCh_->Zahyou.MukiZ = mzB; break;
      default: st_.pstMyCh_->Zahyou.MukiZ = mzF; break;
    }
    
    if (st_.mysideNo_ == 1)
    {
      st_.pstMyCh_->Zahyou.X = (DBCRT_W)-st_.pstMyCh_->Zahyou.X;
      st_.pstMyCh_->Zahyou.Muki = chCommon_.RevMuki(st_.pstMyCh_->Zahyou.Muki);
    }
    
    chCommon_.SetMtypeReset(dbmtSt);
    
  }
  
  void TMgChar::DoCount()
  {
    if (st_.pstMyCh_->Live_f == FALSE) return;
    chCount_.UpDate();
  }
  void TMgChar::DoInput()
  {
    if (st_.pstMyCh_->Live_f == FALSE) return;
    
    chAction_.UpDate();
  }
  void TMgChar::DoMove()
  {
    if (st_.pstMyCh_->Live_f == FALSE) return;
    chMove_.UpDate();
  }
  void TMgChar::DoRemove()
  {
    if (st_.pstMyCh_->Live_f == FALSE) return;
    chRemove_.UpDate();
  }
  void TMgChar::DoSetRect()
  {
    if (st_.pstMyCh_->Live_f == FALSE) return;
    chSetRect_.UpDate();
  }
  void TMgChar::DoDraw()
  {
    if (st_.pstMyCh_->Live_f == FALSE) return;
    chDraw_.Update();
  }
  
  //ステータス初期化
  void TMgChar::ResultMotion(enDBMotionType tMtype)
  {
    chCommon_.SetMtypeReset(tMtype);
    st_.pstMyCh_->Zahyou.Y = 0;
  }
  
}
