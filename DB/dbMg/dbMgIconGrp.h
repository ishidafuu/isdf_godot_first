#pragma once
#include "dbInclude.h"

namespace db
{
  class TMgEngineObj;
  class TMgGrp;
  class TMgCharGrp;
  
  //モノクロ画像管理クラス
  class TMgIconGrp
  {
  protected:
    //エンジンオブジェ
    TMgEngineObj* pmgEO_;
    
  public:
    //コンストラクタ
    explicit TMgIconGrp(TMgEngineObj* pmgEO);
    virtual ~TMgIconGrp();
    
    //顔アイコン描画予約
    void OrderDrawIcon(s32 id, s32 posX, s32 posY, BOOL mirror_f, BOOL mono_f, s32 depth);
    
    //ゲーム顔アイコン描画予約
    void OrderDrawIconHead(s32 sideno, s32 charno, BOOL mono_f, s32 posX, s32 posY);
  };
  
}
