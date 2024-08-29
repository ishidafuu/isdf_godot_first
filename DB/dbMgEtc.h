#pragma once
#include "dbObGameGen.h"

namespace db
{
  
  class TMgEtc : public TGameObjGen
  {
  public:
    //コンストラクタ
    explicit TMgEtc(TGame* pGame, s32 etcNo);
    virtual ~TMgEtc();
    void Update();//移動
    void Draw();
    void SetMType(enEtcObjMotion tMType);
    BOOL SetNewMType(enEtcObjMotion tMType, TZahyouRec tZahyou, s32 tSideNo, s32 tCharNo);
    s32 etcNo_;
    TStEtc* pstMyEtc_;
  protected:
    void UpdateItem();
    void UpdateAngel();
    void DrawAngel(s32 camX, s32 camY, s32 baseline);
    void DrawItem(s32 camX, s32 camY, s32 baseline);
    void SetMtypeItem();
    void SetMtypeItemMv();
    void UpdateItemMv();
    void BrkItemMv(s32 tagpos, s32& nowpos, s32& dpos);
  };
  
}
