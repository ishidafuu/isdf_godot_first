#pragma once
#include "dbChCommon.h"

namespace db
{
  //モーションカウンタ進める////////////////////////////////
  class TChCount : public TChHaveCommon
  {
  protected:
    //キャッチカウンタ増やす＆キャッチ時間を超えているか
    BOOL IncCatchFrm();
    void Jumping();//ジャンプ
    void MotionEnd();//現モーション終了
    void NextFrame();//次のフレーム
    void IncMotion(BOOL goto_f);//モーションを進める
    void LongKeepInc();
    void PassWaitInc();
  public:
    //コンストラクタ
    explicit TChCount(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo);
    virtual ~TChCount(void);
    
    //モーションカウンタ増加
    void UpDate();
  };
}
