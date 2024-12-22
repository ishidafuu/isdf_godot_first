#pragma once
#include "dbInclude.h"

namespace db
{
  
  //オブジェテクスチャ
  class TMgEtcGrp : public base::TGrTile
  {
  public:
    //コンストラクタ
    explicit TMgEtcGrp(base::TMgOrder* pmgOrder,
                       mid::TBmpObj* pbmo,
                       const char *TileFilename);
    virtual ~TMgEtcGrp();
    
    //ＨＰグラフ描画
    void GraphDrawHP(s32 tX, s32 tY, s32 dt, s32 Mdt, BOOL light_f);
    void DrawStep(s32 posx, s32 posy, s32 st0, s32 st1, s32 st2, s32 st3, s32 st4, s32 st5, s32 st6, s32 vj);
  };
  
}
