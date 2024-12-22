#pragma once
#include "dbObGameGen.h"
#include "dbMgData.h"
#include "dbMgTeam.h"


namespace db
{




  class TMgTeam;
  class TMgBall;
  class TMgReferee;
  struct TStReferee;//レフェリーステータス
  struct TStBall;//ボールステータス
  struct TStTeam;//チームステータス
  struct TStChar;//キャラステータス
  
  struct TStChGen
  {
    s32 mysideNo_;//サイド番号
    s32 ensideNo_;//敵サイド番号
    s32 objNo_;//キャラオブジェクト番号
    s32 posNo_;//ポジション番号
	s64 id_kantoku_;
    s32 id_team_;//チームID
    s64 mid_team_;//チームID
    s32 capid_;//キャプテンＩＤ
    
    //マネージャーポインタ
    TMgTeam* pmgMyTm_;//自チーム
    TMgTeam* pmgEnTm_;//敵チーム
    TMgBall* pmgBa_;//ボール
    TMgReferee* pmgRf_;//レフェリー
    //ステータスポインタ
    TStReferee* pstRf_;//レフェリーステータス
    TStBall* pstBa_;//ボールステータス
    TStTeam* pstMyTm_;//チームステータス
    TStTeam* pstEnTm_;//チームステータス
    TStChar* pstMyCh_;//キャラステータス
    
    void Init()
    {
      Zeromem(this);
    }
  };
  
  
  //プレーヤーオブジェクト基礎////////////////////////////////
  class TChGen : public TGameObjGen
  {
  protected:
    
    BOOL Monitor_f();//デバッグ表示用
    BOOL IsInplay();
    BOOL IsShiai();//操作可能か
    BOOL GameEndWait_f();
    BOOL DashAccBtn_f();//ダッシュ加速ボタン
    BOOL BackDs_f();//バックダッシュ
    BOOL ShTag_f();//シュートタゲかどうか
    BOOL PaTag_f();//パスタゲかどうか
    BOOL PassWait_f();//パス待ちかどうか

    BOOL IsTuto();//チュートリアルか
    BOOL IsNoDamage();//自軍ダメージ無し
    BOOL IsOKDodge();//避けＯＫ
    BOOL IsOKCatch();//キャッチＯＫ
    BOOL IsOKWalk();//あるきＯＫ
    BOOL IsOKDash();//ダッシュＯＫ
    BOOL IsOKJump();//ジャンプＯＫ
    BOOL IsOKShot();//シュートＯＫ
    BOOL IsOKPass();//パスＯＫ
    void SetActBtn(enSendTutoAct tutono);//アクションボタンが押された
    void SetTutoNG(enSendTutoAct tutono, s32 ngno);//チュートリアルＮＧ
    void SetTutoOK(enSendTutoAct tutono);//チュートリアルＯＫ
    void IncTutoAct_c(enSendTutoAct tutono);//アクションカウンタ増加

    virtual void DoRefSet();
    virtual void DoReady();
  public:
    
    TStChGen st_;
    
    //コンストラクタ
    explicit TChGen(TGame* pGame, s32 sideNo, s32 objNo);
    virtual ~TChGen(void);
    
    //参照設定
    void RefSet();
    void Ready();
    
    //状態取得
    BOOL IsHSOK(enCharLv dtNo);//ひっさつＯＫか
    s32 MyStLv(endbStateType dtNo);//自データ
    s32 MyPDt(enCharLv dtNo);//自データ
    std::string MyName();
    std::string MySerifu();
    TMgPad* MyPad();
    
    BOOL IsInfield();//内野
    BOOL Kagami_f();//かがみ待ちかどうか
    BOOL IsCtrl();//操作権
    BOOL IsCOM();//COM
    BOOL IsSelfCtrl();//★専用操作も含めて、自分で操作するキャラかどうか
    BOOL IsMAN();//★専用操作も含めて、自分で操作するキャラかどうか
    BOOL IsCtrlOK();//動かせるキャラかどうか
    BOOL PaCvrMan_f();
    BOOL IsShTgOK();//ねらえる状態かどうか
    BOOL IsCallingOK();//コール出来る状態か
    BOOL IsNoGuard(BOOL adDam_f);//ノーガード状態か
    s32 GetLeftCrtX();//T0の外野とT1の内野は右に居るので反転
    BOOL IsDMOKPos();//ダッシュマンに行ける位置かどうか
    void SetLeftCrtX(s32 value);//T0の外野とT1の内野は右に居るので反転
    void SetLeftCrtX(s32 value, enCrtLine lineType);//T0の外野とT1の内野は右に居るので反転
    
    s32 NowKoma(enBMCommonDt dtNo);
    BOOL NowKomaBOOL(enBMCommonDt dtNo);
    BOOL IsLastKoma();
    BOOL IsDashman();
    
    //エンドライン
    s32 GetSLX();
    //ボール持ちかどうか
    BOOL IsBall();
    //自由状態かどうか
    BOOL IsFreeMotion(BOOL checkAutoType_f);
    
    //ランク
    s32 RankLevel(enRankLevel r);
    s32 RankHP(enRankHP r);
    s32 RankSpeed(enRankSpeed r);
    
  };
  
}
