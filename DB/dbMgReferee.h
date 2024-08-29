#pragma once
#include "dbObGameGen.h"

namespace db
{
  
  const s32 JPBALLANIF = 6;
  
  //試合開始終了などを管理するレフェリークラス////////////////////////////////
  class TMgReferee : public TGameObjGen
  {
  protected:
    //審判
    void RefereeDraw();
    //試合時間
    void PlayTimeDraw();
    //審判モーション
    void IncMotion();

    void Mv_Shiai();
    void Mv_GameEnd();
    void Mv_Result();
    void Mv_GoHome();
    void Mv_Pause();
  public:
    
    BOOL IsLimitTime();
    BOOL IsSuddunDeath();
    BOOL IsShiai();//操作可能か
    BOOL IsAngeling();//天使になるか
    BOOL IsPrev();//プレビューか
    BOOL IsTour();//疑似対人か

    //BOOL IsTuto();//チュートリアルか
    //BOOL IsFirstTuto();//最初のチュートリアルか;
    BOOL IsNoOverLine();//オーバラインなし
    BOOL IsNoDamage();//自軍ダメージ無し
    BOOL IsOKDodge();//避けＯＫ
    BOOL IsOKCatch();//キャッチＯＫ
    BOOL IsOKWalk();//あるきＯＫ
    BOOL IsOKDash();//ダッシュＯＫ
    BOOL IsOKJump();//ジャンプＯＫ
    BOOL IsOKShot();//シュートＯＫ
    BOOL IsOKPass();//パスＯＫ
    BOOL IsCapOnlyCtrl();//キャプテンのみ操作可能
    BOOL GetTutoActBtn();
    BOOL IsReturnNoSide();
    BOOL IsReturnMySide();
    BOOL IsReturnEnSide();
    BOOL IsCOMMove();//ＣＯＭ動く
    BOOL IsFinTuto();
    
    void SetTutoActBtn(enSendTutoAct tutono);//アクションボタンが押された
    void SetTutoNG(enSendTutoAct tutono, s32 ngno);//チュートリアルＮＧ
    void SetTutoOK(enSendTutoAct tutono);//チュートリアルＯＫ
    void SetPrevReturn();//プレビュー最低1回当たった

    enSendTutoAct GetTutoNo();//チュートリアル番号
    void IncTutoAct_c(enSendTutoAct tutono);//アクションカウンタ増加
    
    //コンストラクタ
    explicit TMgReferee(TGame* pGame);
    virtual ~TMgReferee();
    
    //参照設定
    void RefSet();
    //ステータス初期化
    void Ready(BOOL demo_f, BOOL tour_f);
    void ReadyPrev();
    void ReadyTuto(BOOL first_f, BOOL retro_f);
    void Retry();
    
    //ジングルから開始まで
    void JglCount();
    void SetSYS(enSYSID sys);
    void SetSE(enSEID se);
    void SetMotion(enRfMotion tMotion);
    void SetMotion(enRfMotion tMotion, enMukiType tMuki);
    
    void RefereeMsg(std::string msg);
    

    //チュートリアル用処理
    void Mv_TutoRcv();

    //決着処理
    void ShiaiMove();
    //描画
    void Draw();
  };
  
  
  
}
