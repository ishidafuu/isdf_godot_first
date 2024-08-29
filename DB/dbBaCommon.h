#pragma once
#include "dbBaGen.h"

namespace db
{

  //共通処理////////////////////////////////
  class TBaCommon : public TBaGen
  {
  protected:

  public:
    //コンストラクタ
    explicit TBaCommon(TGame* pGame);
    virtual ~TBaCommon(void);
    
    //動き停止
    void ZeroXYZ();
    
    //モーション切り替え
    void SetMtype(enBallMotion tMtype);
    
    //ボールを持ったとき
    void SetMTypeHold(s32 tTNo, s32 tPNo);
    
    //バウンド着地点
    void GetBoundLandPos(s32 tGrv);
    //シュート着地点
    void GetShootLandPos();
    
    //ボールタイマー初期化
    void ResetTimer(s32 tSideNo, BOOL init_f);
    
    //位置リセット
    void ResetRefPos(s32 tSideNo, enJumpBall tJumpBal, BOOL demo_f);

    //位置リセットプレビュー
    void ResetRefPos_Prev(BOOL ready_f);

    //位置リセットチュートリアル
    void ResetRefPos_Tuto(BOOL ready_f, s32 side);
    
    //カメラ位置をマネージャに渡す
    void SetCamPos();
    //ボールの位置にカメラの位置をセット
    void ResetCamPos();
    
    //位置リセット
    void RefThrow();
    
    //ボールを通常状態にする
    void SetNmlBall(BOOL InShoot_f);
    
    //逆向き
    enMukiType RevMuki(enMukiType tMuki);
    
    //効果音
    void SESet(enSEID SENo);
    
    //効果音
    void HomingSESet(enSEID SENo);
    //効果音
    void HomingSESet(s32 SENo);
    
    //エンドライン
    s32 GetSLX();
    
    //はんでこうか
    s32 HandiDam(s32 dam, s32 sideNo, s32 padNo);

  };
  
  //共通処理をもつ////////////////////////////////
  class TBaHaveCommon : public TBaGen
  {
  protected:
    TBaCommon* pCommon_;//共通処理
    //コンストラクタ
  public:
    explicit TBaHaveCommon(TGame* pGame, TBaCommon* pCommon);
    virtual ~TBaHaveCommon(void);
  };
}
