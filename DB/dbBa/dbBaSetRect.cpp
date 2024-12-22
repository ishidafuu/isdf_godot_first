#include "dbBaSetRect.h"
#include "dbGame.h"

namespace db
{
  //判定矩形作成////////////////////////////////
  
  //コンストラクタ
  TBaSetRect::TBaSetRect(TGame* pGame, TBaCommon* pCommon)
  : TBaHaveCommon(pGame, pCommon)
  {
  }
  
  TBaSetRect::~TBaSetRect()
  {
  }
  
  void TBaSetRect::SetOS()
  {
    //きょだいなシュートは判定２倍★特殊処理
    
    //判定
    s32 tW = 0;
    s32 tH = 0;
    s32 tD = 0;
    s32 revZ = 0;
    
    switch (st_.pstBa_->Motion)
    {
        //case bmFree:
        //  tW = pmgEO_->mgDt_.dtSet_.GetDtBall(setFAtariWidht);
        //  tH = pmgEO_->mgDt_.dtSet_.GetDtBall(setFAtariHeight);
        //  tD = pmgEO_->mgDt_.dtSet_.GetDtBall(setFAtariDepth);
        //  break;
      case bmShoot:
        switch (st_.pstBa_->Zahyou.MukiZ)
      {
        case mzF:
          tW = pmgEO_->mgDt_.dtSet_.GetDtBall(setZAtariWidht);
          tH = pmgEO_->mgDt_.dtSet_.GetDtBall(setZAtariHeight);
          tD = pmgEO_->mgDt_.dtSet_.GetDtBall(setZAtariDepth);
          revZ = +((tD / 2) * XYMAG);
          break;
        case mzN:
          tW = pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariWidht);
          tH = pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariHeight);
          tD = pmgEO_->mgDt_.dtSet_.GetDtBall(setAtariDepth);
          break;
        case mzB:
          tW = pmgEO_->mgDt_.dtSet_.GetDtBall(setZAtariWidht);
          tH = pmgEO_->mgDt_.dtSet_.GetDtBall(setZAtariHeight);
          tD = pmgEO_->mgDt_.dtSet_.GetDtBall(setZAtariDepth);
          revZ = -((tD / 2) * XYMAG);
          break;
      }
        
        break;
      default:
        tW = pmgEO_->mgDt_.dtSet_.GetDtBall(setFAtariWidht2);
        tH = pmgEO_->mgDt_.dtSet_.GetDtBall(setFAtariHeight2);
        tD = pmgEO_->mgDt_.dtSet_.GetDtBall(setFAtariDepth2);
        break;
    }
    
    //きょだいなシュートは判定２倍★
    //新シュートでも設定必要
    //if (st_.pstBa_->ShSt.HSNo == hsKYO)
    //{
    //  tW *= 2;
    //  tH *= 2;
    //}
    
    
    //ディレイ
    for (s32 i = 0; i < (BUFPOSMAX - 1); ++i)
    {
      s32 i2 = (BUFPOSMAX - 1 - i);
      st_.pstBa_->BufX[i2] = st_.pstBa_->BufX[i2 - 1];
      st_.pstBa_->BufY[i2] = st_.pstBa_->BufY[i2 - 1];
      st_.pstBa_->BufZ[i2] = st_.pstBa_->BufZ[i2 - 1];
      st_.pstBa_->BufSpin_c[i2] = st_.pstBa_->BufSpin_c[i2 - 1];
    }
    st_.pstBa_->BufX[0] = st_.pstBa_->Zahyou.X;
    st_.pstBa_->BufY[0] = st_.pstBa_->Zahyou.Y;
    st_.pstBa_->BufZ[0] = st_.pstBa_->Zahyou.Z + revZ;
    st_.pstBa_->BufSpin_c[0] = st_.pstSh_->cmn.spin_c;
    
    //着地直前はラグ無し
    s32 lag = 0;
    
    
    st_.pstBa_->Atari2 = st_.pstBa_->Atari;
    st_.pstBa_->AtariZ2 = st_.pstBa_->AtariZ;
    st_.pstBa_->AtariD2 = st_.pstBa_->AtariD;
    
    //当たり判定
    st_.pstBa_->Atari.x_ = (st_.pstBa_->BufX[lag] / XYMAG) - (tW / 2);
    st_.pstBa_->Atari.y_ = (st_.pstBa_->BufY[lag] / XYMAG);
    st_.pstBa_->Atari.width_ = tW;
    st_.pstBa_->Atari.height_ = tH;
    st_.pstBa_->AtariZ = st_.pstBa_->BufZ[lag];
    st_.pstBa_->AtariD = tD;
    
    //中点判定
    st_.pstBa_->Atari2X = (st_.pstBa_->BufX[0] + st_.pstBa_->BufX[1]) / 2;
    st_.pstBa_->Atari2Y = (st_.pstBa_->BufY[0] + st_.pstBa_->BufY[1]) / 2;
    st_.pstBa_->Atari2Z = (st_.pstBa_->BufZ[0] + st_.pstBa_->BufZ[1]) / 2;
    st_.pstBa_->Atari2.x_ = (st_.pstBa_->Atari2.x_ + st_.pstBa_->Atari.x_) / 2;
    st_.pstBa_->Atari2.y_ = (st_.pstBa_->Atari2.y_ + st_.pstBa_->Atari.y_) / 2;
    st_.pstBa_->Atari2.width_ = st_.pstBa_->Atari.width_;
    st_.pstBa_->Atari2.height_ = st_.pstBa_->Atari.height_;
    st_.pstBa_->AtariZ2 = (st_.pstBa_->AtariZ2 + st_.pstBa_->AtariZ) / 2;
    st_.pstBa_->AtariD2 = (st_.pstBa_->AtariD2 + st_.pstBa_->AtariD) / 2;
    
    //味方用当たり判定
    st_.pstBa_->AtariFr = st_.pstBa_->Atari;
    st_.pstBa_->AtariZFr = st_.pstBa_->AtariZ;
    st_.pstBa_->AtariDFr = st_.pstBa_->AtariD;
    switch (st_.pstBa_->Motion)
    {
      case bmPass:
        if (st_.pstBa_->Zahyou.dX > 0)//右進行
        {
          st_.pstBa_->AtariFr.x_ -= st_.pstBa_->AtariFr.width_;
        }
        else
        {
          st_.pstBa_->AtariFr.x_ += st_.pstBa_->AtariFr.width_;
        }
        
        if (st_.pstBa_->Zahyou.dZ > 0)//上進行
        {
          st_.pstBa_->AtariZFr -= st_.pstBa_->AtariDFr;
        }
        else
        {
          st_.pstBa_->AtariZFr += st_.pstBa_->AtariDFr;
        }
        
        break;
      case bmShoot:
        switch (st_.pstBa_->Zahyou.MukiZ)
      {
        case mzF:
        case mzB:
          st_.pstBa_->AtariFr.width_ *= 2;
          break;
        case mzN:
          if (st_.pstBa_->Zahyou.dZ > 0)//上進行
          {
            st_.pstBa_->AtariZFr -= st_.pstBa_->AtariDFr;
          }
          else
          {
            st_.pstBa_->AtariZFr += st_.pstBa_->AtariDFr;
          }
          //st_.pstBa_->AtariDFr *= 2;
          break;
      }
        break;
      default:
        break;
    }
    
    st_.pstBa_->HitTNo = NGNUM;
    st_.pstBa_->HitPNo = NGNUM;
  }
  //更新
  void TBaSetRect::UpDate()
  {
    SetOS();
  }
  
  
}
