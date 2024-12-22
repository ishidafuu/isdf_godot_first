#pragma once
#include "dbBaGen.h"
#include "dbBaCommon.h"
#include "dbBaCount.h"
#include "dbBaMove.h"
#include "dbBaSetRect.h"
#include "dbBaJudge.h"
#include "dbBaDraw.h"

#include "dbBaShot.h"
#include "dbBaShot_Tri.h"
#include "dbBaShot_Acc.h"

namespace db
{
  class TMgTeam;
  class TMgReferee;
  class TMgChar;

  //const s32 LOCKEND = 2;
  //const s32 HSLV = 7;
  //const s32 RHSLV = 8;

  //ボール動作オブジェクト管理および描画////////////////////////////////
  class TMgBall : public TBaGen
  {
  protected:
    virtual void DoRefSet();
  public:
    //各アクション用クラス
    TBaCommon baCommon_;
    TBaCount baCount_;
    TBaMove baMove_;
    TBaSetRect baSetRect_;
    TBaJudge baJudge_;
    TBaDraw baDraw_;

    TBaShot baShot_;
    TBaShot_Tri baShot_Tri_;
    TBaShot_Acc baShot_Acc_;
    
    explicit TMgBall(TGame* pGame);
    virtual ~TMgBall(void);
    
    //参照設定
    void AllRefSet();
    
    //外から呼ばれるアクション
    void DoCount();
    void DoInput();
    void DoMove();
    void DoSetRect();
    void DoJudge();
    void DoResult();
    void DoDraw();
    
  };
  
  
  
}
