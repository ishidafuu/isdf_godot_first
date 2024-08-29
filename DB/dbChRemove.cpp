#include "dbChAction.h"
#include "dbGame.h"

namespace db
{
  
  //コンストラクタ
  TChRemove::TChRemove(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo)
  : TChHaveCommon(pGame, pCommon, sideNo, charNo)
  {
  }
  
  TChRemove::~TChRemove()
  {
  }
  
  void TChRemove::Remove()
  {
    //もし人持ちとか出てきたらここで処理
    //ボール位置補正
    if (IsBall() == FALSE) return;
    
    const s32 TXREV = 8;
    //const s32 TIPSIZE8 = 8;
    //const s32 CHARSQWIDE = 32;
    
    //アニメーションのポインタセット
    //TFrameData NowFrameAni = pmgEO_->mgDt_.FrameDt[(s32)st_.pstMyCh_->Motion.MNo, st_.pstMyCh_->Anime.FrameNo];
    
    s32 cellNo = NowKoma(bmc_bmpNo);
    s32 bx = pmgEO_->mgDt_.dtCell_.GetDt(cellNo ,ccBX);
    s32 by = pmgEO_->mgDt_.dtCell_.GetDt(cellNo ,ccBY);
    //持たれ位置補正
    s32 tx = NowKoma(bmc_posX) + bx;// -TXREV;
    s32 ty = NowKoma(bmc_posY) + by - TXREV;// +TIPSIZE8;
    //プライオリティ
    s32 tz = (pmgEO_->mgDt_.dtCell_.GetDt(cellNo, ccIZ) == 0)
    ? PRVLAYRE_BALL
    : 0;
    s32 tlx = pmgEO_->mgDt_.dtCell_.GetDt(cellNo, ccIX) + tx;
    s32 tly = pmgEO_->mgDt_.dtCell_.GetDt(cellNo ,ccIY) + ty;
    tlx = (st_.pstMyCh_->Zahyou.Muki == mL)
      ? (-(tlx) * XYMAG)
      : (+(tlx) * XYMAG);
    tly = ((-tly) * XYMAG);
    
    //ボールの補正
    
    //※座標変化量(dxdydz)も補正された分だけ代入した方が壁処理とかのとき楽そう
    //ていうかそうしないとダメぽい
    pmgSG_->stBa_.Zahyou.Muki = st_.pstMyCh_->Zahyou.Muki;
    pmgSG_->stBa_.Zahyou.MukiZ = st_.pstMyCh_->Zahyou.MukiZ;
    pmgSG_->stBa_.Zahyou.X = st_.pstMyCh_->Zahyou.X + tlx;
    pmgSG_->stBa_.Zahyou.Y = st_.pstMyCh_->Zahyou.Y + tly;
    pmgSG_->stBa_.Zahyou.Z = st_.pstMyCh_->Zahyou.Z;
    pmgSG_->stBa_.Zahyou.Pri = st_.pstMyCh_->Zahyou.Pri + tz;

  }
  
  //更新
  void TChRemove::UpDate()
  {
    Remove();
  }
  
}
