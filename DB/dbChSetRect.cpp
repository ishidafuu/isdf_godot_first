#include "dbChAction.h"
#include "dbGame.h"

namespace db
{
  //プレーヤーオブジェはやることが多く関数が増えすぎるため
  //おおまかな動作ごとにクラスにする
  
  //判定矩形作成////////////////////////////////
  
  //コンストラクタ
  TChSetRect::TChSetRect(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo)
  : TChHaveCommon(pGame, pCommon, sideNo, charNo)
  {
  }
  
  TChSetRect::~TChSetRect()
  {
  }
  void TChSetRect::SetOS()
  {
    //TFrameData NowFrameAni = pmgEO_->mgDt_.FrameDt[(s32)st_.pstMyCh_->Motion.MNo, st_.pstMyCh_->Anime.FrameNo];
    
    s32 kx = 0;
    s32 ky = 0;
    s32 kw = 8;
    s32 kh = (st_.pstMyCh_->Motion.IsMFlags(dbmfDg))
      ? 8
      : 24;
    
    ////立ちと同じに
    //if ((NowKoma(bmc_kuraiW) == 0) || (NowKoma(bmc_kuraiH) == 0))
    //{
    //  kx = pmgEO_->mgDt_.dtBM_.GetDt(0, 0, bmc_kuraiX);
    //  ky = pmgEO_->mgDt_.dtBM_.GetDt(0, 0, bmc_kuraiY);
    //  kw = pmgEO_->mgDt_.dtBM_.GetDt(0, 0, bmc_kuraiW);
    //  kh = pmgEO_->mgDt_.dtBM_.GetDt(0, 0, bmc_kuraiH);
    //  
    //}
    //else
    //{
    //  kx = NowKoma(bmc_kuraiX);
    //  ky = NowKoma(bmc_kuraiY);
    //  kw = NowKoma(bmc_kuraiW);
    //  kh = NowKoma(bmc_kuraiH);
    //}


    ////左向きの場合Rectの左右を逆にしないといけない
    //st_.pstMyCh_->Kurai.x_ = (st_.pstMyCh_->Zahyou.Muki == mR)
    //  ? (st_.pstMyCh_->Zahyou.X / XYMAG) + kx
    //  : (st_.pstMyCh_->Zahyou.X / XYMAG) - kx - kw;
    //左向きの場合Rectの左右を逆にしないといけない
    st_.pstMyCh_->Kurai.x_ = (st_.pstMyCh_->Zahyou.Muki == mR)
    ? (st_.pstMyCh_->Zahyou.X / XYMAG) + kx - kw
    : (st_.pstMyCh_->Zahyou.X / XYMAG) - kx - kw;
    st_.pstMyCh_->Kurai.y_ = (st_.pstMyCh_->Zahyou.Y / XYMAG) + ky;
    st_.pstMyCh_->Kurai.width_ = kw * 2;
    st_.pstMyCh_->Kurai.height_ = kh;
  }
  
  
  //更新
  void TChSetRect::UpDate()
  {
    SetOS();
  }
  
  
}

