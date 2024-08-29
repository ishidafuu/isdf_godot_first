#include "dbMgBall.h"
#include "dbGame.h"

namespace db
{
  
  //コンストラクタ
  TMgBall::TMgBall(TGame* pGame)
  : TBaGen(pGame),
  baCommon_(pGame),
  baCount_(pGame, &baCommon_),
  baMove_(pGame, &baCommon_),
  baSetRect_(pGame, &baCommon_),
  baJudge_(pGame, &baCommon_),
  baDraw_(pGame, &baCommon_),
  baShot_(pGame, &baCommon_),
  baShot_Tri_(pGame, &baCommon_),
  baShot_Acc_(pGame, &baCommon_)
  {
    baShot_.pbaShot_Tri_ = &baShot_Tri_;
    baShot_.pbaShot_Acc_ = &baShot_Acc_;

    baMove_.pbaShot_ = &baShot_;
  }
  
  TMgBall::~TMgBall(void)
  {
  }
  
  //参照設定
  void TMgBall::DoRefSet()
  {
    baCommon_.RefSet();
    baCount_.RefSet();
    baMove_.RefSet();
    baSetRect_.RefSet();
    baJudge_.RefSet();
    baDraw_.RefSet();

    baShot_.RefSet();
    baShot_Tri_.RefSet();
    baShot_Acc_.RefSet();
  }
  
  
  
  void TMgBall::DoCount()
  {
    baCount_.UpDate();
  }
  void TMgBall::DoInput()
  {
    
  }
  void TMgBall::DoMove()
  {
    baMove_.UpDate();
  }
  void TMgBall::DoSetRect()
  {
    baSetRect_.UpDate();
  }
  void TMgBall::DoJudge()
  {
    baJudge_.UpDate();
  }
  void TMgBall::DoDraw()
  {
    baDraw_.Update();
  }
  
  
}
