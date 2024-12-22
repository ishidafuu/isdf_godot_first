#include "dbMgEtcGrp.h"


namespace db
{
  //コンストラクタ
  TMgEtcGrp::TMgEtcGrp(base::TMgOrder* pmgOrder, mid::TBmpObj* pbmo, const char *TileFilename)
    : TGrTile(pmgOrder, pbmo, TileFilename)
  {

  }

  TMgEtcGrp::~TMgEtcGrp()
  {

  }
  //ＨＰグラフ描画
  void TMgEtcGrp::GraphDrawHP(s32 tX, s32 tY, s32 dt, s32 Mdt, BOOL light_f)
  {
    const s32 HPOFSY = -4;

    //enObjRectType rt = (light_f)
    //  ? obmt_HPBar2
    //  : obmt_HPBar;

    TRect barrect = GetTileRect(obmt_HPBar2);

    //最大値未満
    if (dt < Mdt)
    {
      barrect.width_ = ((dt * barrect.width_) / Mdt);//現在HPの幅
      if ((dt > 0) && (barrect.width_ < 1)) barrect.width_ = 1;
    }
    
    //描画情報
    mid::TStDraw dst;
    dst.Init();
    dst.depth_ = base::DPT_3BUTTON;

    OrderDrawTile(obmt_HPBarBase,
      tX,
      tY + HPOFSY, &dst);

    dst.depth_ += 100;

    OrderDrawRect(barrect,
      tX + 1,
      tY + HPOFSY, &dst);

  }
  void TMgEtcGrp::DrawStep(s32 posx, s32 posy, s32 st0, s32 st1, s32 st2, s32 st3, s32 st4, s32 st5, s32 st6, s32 vj)
  {
    //描画情報
    mid::TStDraw dst;
    dst.Init();
    dst.depth_ = base::DPT_3BUTTON;
    const s32 DX = 3;
    const s32 BX = 5;
    const s32 BY = 3;
    OrderDrawTile(obmt_StepBase, posx, posy, &dst);
    dst.depth_ = base::DPT_3BUTTON + 100;
    if (vj != NGNUM) OrderDrawTile(obmt_StepVJD + vj, posx, BY + posy - 3, &dst);
    dst.depth_ += 5;
    if (st0 != NGNUM) OrderDrawTile(obmt_StepLightD + st0, posx + BX + (DX * 0), BY + posy, &dst);
    dst.depth_ += 5;
    if (st1 != NGNUM) OrderDrawTile(obmt_StepLightD + st1, posx + BX + (DX * 1), BY + posy, &dst);
    dst.depth_ += 5;
    if (st2 != NGNUM) OrderDrawTile(obmt_StepLightD + st2, posx + BX + (DX * 2), BY + posy, &dst);
    dst.depth_ += 5;
    if (st3 != NGNUM) OrderDrawTile(obmt_StepLightD + st3, posx + BX + (DX * 3), BY + posy, &dst);
    dst.depth_ -= 5;
    if (st4 != NGNUM) OrderDrawTile(obmt_StepLightD + st4, posx + BX + (DX * 4), BY + posy, &dst);
    dst.depth_ -= 5;
    if (st5 != NGNUM) OrderDrawTile(obmt_StepLightD + st5, posx + BX + (DX * 5), BY + posy, &dst);
    dst.depth_ -= 5;
    if (st6 != NGNUM) OrderDrawTile(obmt_StepLightD + st6, posx + BX + (DX * 6), BY + posy, &dst);
  }
}
