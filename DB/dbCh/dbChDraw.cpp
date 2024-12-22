#include "dbChDraw.h"
#include "dbGame.h"

namespace db
{
  TChDraw::TChDraw(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo)
  : TChHaveCommon(pGame, pCommon, sideNo, charNo)
  {
  }
  
  TChDraw::~TChDraw(void)
  {
  }
  
  void TChDraw::Update()
  {
    Drawing();//キャラ描画処理
  }
  
  //描画
  void TChDraw::Drawing()
  {
    if (st_.pstMyCh_->Live_f == FALSE) return;

    const s32 SHTGFLSITV = 24;
    const s32 SHTGFLSITV_H = SHTGFLSITV / 2;
    
    //カメラ位置
    s32 camposX = pmgEO_->mgCamera_.GetRevCamPosX();
    s32 camposY = pmgEO_->mgCamera_.GetRevCamPosY();
    
    //死亡フラグ立ってたら何も描画しない
    if (st_.pstMyCh_->Dead_f) return;
    
    s32 baseline = (pmgGO_->pmgRf_->IsPrev())
      ? DRAWBASELINE_PREV
      : DRAWBASELINE;

    s32 ShadowX = (st_.pstMyCh_->Zahyou.X / XYMAG) + camposX;
    s32 ShadowY = -(st_.pstMyCh_->Zahyou.Z / XYMAG) + camposY + baseline;


    s32 cellNo = NowKoma(bmc_bmpNo);
    //s32 bx = pmgEO_->mgDt_.dtCell_.GetDt(cellNo, ccBX);
    //s32 by = pmgEO_->mgDt_.dtCell_.GetDt(cellNo, ccBY);
    s32 bmpNo = cellNo;
    if (pmgEO_->mgDt_.dtCell_.GetDt(cellNo, ccRefNo) != 0)
    {
      bmpNo = pmgEO_->mgDt_.dtCell_.GetDt(cellNo, ccRefNo);
    }

    s32 rx = NowKoma(bmc_posX);
    s32 ry = NowKoma(bmc_posY) - (st_.pstMyCh_->Zahyou.Y / XYMAG);
      
    //反転
    BOOL mirror_f_ = (st_.pstMyCh_->Zahyou.Muki == mL);
    //BOOL Mir_f = (NowKomaBOOL(bmc_revX_f) ^ (st_.pstMyCh_->Zahyou.Muki == mL));
    
    if (mirror_f_) rx = -rx;

    s32 posX = ShadowX + rx;
    s32 posY = ShadowY + ry;

    //描画情報
    
    //影
    BOOL nazo_f = FALSE;
    pmgEO_->mgGrp_.pmgChGrp_[st_.mysideNo_][st_.objNo_]->OrderShadow(ShadowX, ShadowY, nazo_f, pmgGO_->pmgRf_->IsPrev());
    
    //キャラ描画情報
    TCharDraw cdst;
    cdst.Init();
    cdst.mirror_f_ = mirror_f_;
    cdst.ang_f_ = st_.pstMyCh_->ANGEL_f;
    cdst.cadam_c_ = (st_.pstMyCh_->CaDamAfter_c / 2);

    s32 basedepth = (pmgGO_->pmgRf_->IsPrev())
      ? base::DPT_5_0FILTER
      : base::DPT_3OBJ;

    //DPT_4OVER
    cdst.depth_ = st_.pstMyCh_->Zahyou.Pri - (st_.pstMyCh_->Zahyou.Z * ZMAG) + basedepth;// +PRVLAYRE_BODY;//ここではやらないCharGrpで


    cdst.cellNo_ = cellNo;
    //上書きパーツ
    if (pmgEO_->mgDt_.dtCell_.GetDt(cellNo, ccOvNo) != 0)
    {
      //描画条件：ボールもしくは常に
      if (pCommon_->IsBall()
        || (pmgEO_->mgDt_.dtCell_.GetDt(cellNo, ccOvJ) != 0))
      {
        cdst.ov_f_ = TRUE;
      }
    }


    s32 shTg_c = (IsShiai())//点滅はインプレーのときのみ
    ? st_.pstMyCh_->ShTg_c
    : NGNUM;

    //タゲフラッシュ
    if ((shTg_c != NGNUM) && (shTg_c > 0) && (st_.pmgRf_->IsCapOnlyCtrl() == FALSE))
    {
      cdst.drawmode_ = mid::drm_ColFil;

      const s32 FLSITV = 20;
      const s32 FLSITVHf = FLSITV / 10;//強く光るように見える
      const s32 FLSBRT = (shTg_c < FLSITV)//最初だけ強いフラッシュ
        ? 24
        : 6;
      s32 count = (shTg_c % FLSITV);//0~19

      s32 col = (count < FLSITVHf)
        ? (count * FLSBRT)
        : ((FLSITV - count) * FLSBRT);
      cdst.argb2_.aaa_ = lib_num::Between255(col);
    }

    //キャラ
    int posno = st_.pstMyTm_->PosSt.CharofP[st_.objNo_];//オブジェはポジションを扱ってるもの
    pmgEO_->mgGrp_.pmgChGrp_[st_.mysideNo_][posno]->OrderDraw(bmpNo, posX, posY, &cdst);

    //カーソル★
    if ((IsCtrl() || IsMAN())
        && IsShiai() && (pmgSG_->stRf_.Demo_f == FALSE))//(AOT.SDt.DBGDt.DBCsl_c  < 15)
    {
      
      

      enObjRectType cr = (pmgEO_->stShiai_.IsTour())
        ? obmt_CursorD_EN
        : obmt_CursorD_COM;
      s32 crNo = NGNUM;
      if (st_.pmgMyTm_->st_.pstMyTm_->SideMaruhi_f)
      {
        
      }
      else if (IsMAN())
      {
        //if (st_.pstMyCh_->stPad.OmksChar_f == FALSE)
        {
          crNo = st_.pstMyCh_->stPad.MANCharPadNo;
        }
      }
      else if (st_.pmgMyTm_->st_.pstMyTm_->MANSide_f)
      {
        if (st_.pmgMyTm_->st_.pstMyTm_->OmksSide_f == FALSE)
        {
          crNo = st_.pmgMyTm_->st_.pstMyTm_->MANSidePadNo;
        }
      }
      
      switch (crNo)
      {
        case 0: cr = obmt_CursorD_1P; break;
        case 1: cr = obmt_CursorD_2P; break;
        //case 2: cr = obmt_CursorD_3P; break;
        //case 3: cr = obmt_CursorD_4P; break;
        //default: cr = obmt_CursorD_COM; break;
      }
      const s32 CSLPOSX = 0;// +12;
      const s32 CSLPOSY = -10 - 32;
      
      //描画情報
      mid::TStDraw dst;
      dst.Init();
      dst.argb_.aaa_ = SDWALP;
      dst.depth_ = basedepth + st_.pstMyCh_->Zahyou.Pri - (st_.pstMyCh_->Zahyou.Z * ZMAG);
      dst.centerOrigin_f_ = TRUE;
      pmgEO_->mgGrp_.pmgEtcGrp_->OrderDrawTile(cr, (s32)posX + CSLPOSX, (s32)posY + CSLPOSY, &dst);
    }
  }
}
