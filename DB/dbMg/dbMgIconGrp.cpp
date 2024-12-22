#include "dbMgIconGrp.h"
#include "dbMgEngineObj.h"

namespace db
{
  //コンストラクタ
  TMgIconGrp::TMgIconGrp(TMgEngineObj* pmgEO)
  : pmgEO_(pmgEO)
  {
    
  }
  
  //開放
  TMgIconGrp::~TMgIconGrp()
  {
  }
  
  
  //顔アイコン描画予約(メニュー用顔髪顎合成描画)
  void TMgIconGrp::OrderDrawIcon(s32 id, s32 posX, s32 posY, BOOL mirror_f, BOOL mono_f, s32 depth)
  {
    s32 kaoX = posX;
    s32 kaoY = posY;// +FACEICON_OFSY;
    
    const s32 ZURARY = -1;
    s32 zuraX = kaoX;
    s32 zuraY = kaoY;
    zuraY += ZURARY;
    
    kaoY -= (FACEH / 2);
    s32 mouseX = kaoX;
    s32 mouseY = kaoY;
    mouseY += FACEH;
    
    s32 lykao = depth + 1;
    s32 lyzura = depth + 2;
    s32 skincol = pmgEO_->mgDt_.dtChar_.GetLooks(clk_SkinCol, id);
    s32 mx = 0;
    s32 my = 0;

    //描画情報
    mid::TStDraw dst;
    dst.Init();
    dst.mirror_f_ = mirror_f;
    dst.centerOrigin_f_ = TRUE;
    dst.depth_ = lykao;
    
    ////あご
    //pmgEO_->mgGrp_.pgr_IconMouse_[skincol]->OrderDrawPtXY(mx, my, mouseX, mouseY, &dst);
    //
    //{
    //  //カラーかお
    //  pmgEO_->mgGrp_.pgr_IconFace_[skincol]->OrderDrawPtX(id, kaoX, kaoY, &dst);
    //  
    //  //カラーづら
    //  dst.depth_ = lyzura;
    //  pmgEO_->mgGrp_.pgr_IconHair_[skincol]->OrderDrawPtX(id, zuraX, zuraY, &dst);
    //}
    //
  }
  
  
  //ゲーム用顔アイコン描画予約
  void TMgIconGrp::OrderDrawIconHead(s32 sideno, s32 charno, BOOL mono_f, s32 posX, s32 posY)
  {
    //描画情報
    mid::TStDraw dst;
    dst.Init();
    dst.mirror_f_ = (sideno != 0);
    dst.centerOrigin_f_ = TRUE;
    dst.depth_ = base::DPT_3BUTTON;
    s32 dX = posX;
    s32 dY = posY;
    pmgEO_->mgGrp_.pgr_IconHead_[sideno][charno]->OrderDrawTile(0, dX, dY, &dst);
  }

}
