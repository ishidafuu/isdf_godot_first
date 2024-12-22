#pragma once
#include "dbObGameGen.h"

namespace db
{
  class TMgTeam;
  class TMgBall;
  class TMgReferee;
  class TMgChar;
  class TMgPad;
  struct TStReferee;//レフェリーステータス
  struct TStBall;//ボールステータス
  struct TStTeam;//チームステータス
  struct TStChar;//キャラステータス
  
  struct TStMgTeam
  {
    s32 mysideNo_;//サイド番号
    s32 ensideNo_;//敵サイド番号
	s64 id_kantoku_;
    s32 mid_team_;//チームID
    s64 id_team_;//チームID
    s32 id_m_shset_;
    s32 rarity_;
    s32 comNo_;
    s32 fomNo_;
    BOOL droped_f_;
    
    //マネージャーポインタ
    TMgTeam* pmgMyTm_;//自チーム
    TMgTeam* pmgEnTm_;//敵チーム
    TMgBall* pmgBa_;//ボール
    TMgReferee* pmgRf_;//レフェリー
    TMgChar* pmgMyCh_[DBMEMBER_ALL];//ポジション
    TMgPad* pSidePad_;
    //ステータスポインタ
    TStReferee* pstRf_;//レフェリーステータス
    TStBall* pstBa_;//ボールステータス
    TStTeam* pstMyTm_;//チームステータス
    TStTeam* pstEnTm_;//チームステータス
    TStChar* pstMyCh_[DBMEMBER_ALL];//キャラステータス
    
    //TMgBall* st_.pmgBa_;//ボール
    //TMgTeam* st_.pmgEnTm_;//敵チーム
    //TMgReferee* st_.pmgRf_;//レフェリー
    //s32 st_.ensideNo_;
    void Init()
    {
      Zeromem(this);
    }
  };
  
  
  //チーム管理およびチーム内プレーヤーオブジェクトの管理////////////////////////////////
  class TMgTeam : public TGameObjGen
  {
  protected:
    
    //サイド位置XDF
    s32 SideDist(s32 side);
    //ラインYDF
    s32 LineDist();
    

    //試合中の名前
    void RedrawShiaiName(s32 drawcno);
    //効果音
    void SESet(enSEID SENo);
    //効果音
    void SysSESet(enSYSID SysNo);
    //効果音
    void SESetInt(s32 SENo);
    //ポジショニング
    void SetPosLine();
    //ダッシュマンストック
    void SetDashmanStock();
    //デフォ敵中心位置
    void SetDefEnCXZ();
    //敵内野の重心
    void GetCOG();
    //はみ出チェック
    void CheckEnC();
    //必殺名表記
    //void HSNameDraw();
    //ボールの位置
    void TeamBallPos();
    void TeamBallPos2();
    
    //操作権キャラ
    void NowCtrlNo();
    //ポジショニング予定位置計算ポジショニングカウンタ減少
    void GetEnC_c();
    
    
    //デバッグ用
    void COMDebugSwitch();
    //ＣＯＭダッシュマン作戦
    void COMDashman();
    //ＣＯＭダッシュマン人数再確認
    BOOL COMDashmanNumRecheck();
    //COM思考サイコロふり直し★★
    void COMSwitch();
    //COMかうんた操作
    void COMCounter();
    
    
    //丸秘作戦
    //BOOL MaruhiStart();
    
    //ポジションカウンタ
    void SetPosition();
    //ダッシュマン指令
    void OrderDashman();
   
    //自チームのフォーメーション番号
    s32 GetMyFomationNo();
    
    //劣勢判断
    BOOL IsRessei();
    
    s32 ChanceTag();
    void ChanceCheck();

    //セミオート
    void SemiSwitch();
    //セミオートキャッチ
    void SemiCatch();
    //セミオートシュート
    void SemiShot();
    //セミオートAI
    void SemiAI();

    //シュートチェンジ
    void Debug_ChangeShot();

    //描画
    void HPDraw(s32 drawcno);
    void FaceDraw(s32 drawcno);
    void StepDraw(s32 drawcno, s32 posno);
    //void NameDraw();
  public:
    
    TStMgTeam st_;
    
    //コンストラクタ
    explicit TMgTeam(TGame* pGame, s32 sideNo);
    virtual ~TMgTeam();
    
    //初期化
    void RefSet();
    //パッドのセット
    void SetSidePad();
    //ステータス初期化
    void Ready(BOOL tDemo_f);//チーム設定をシーンから貰うか
    //試合中の名前
    void SetShiaiName();
    //状態取得
    s32 GetBallmanNo();//ボールを持っているキャラ
    s32 GetLiveNum();
    BOOL IsLastOne();//一人だけ残ってる
    BOOL IsAllDead();//全員ＨＰ０
    BOOL IsAllNoLive();//全員天使
    BOOL IsAllNoShTg();//全員ねらえる状態でない
    BOOL IsAllStop();//全員停止
    BOOL IsNoFly();//全員停止
    s32 DeadNum();//死亡人数
    s32 InfNum();//内野人数
    s32 PinchNum();//瀕死人数
    BOOL IsGohome(); //帰宅済みかどうか
    
    BOOL IsCOM();//COM
    BOOL IsMAN();//サイド操作()
    base::TPad* SidePad();//サイド操作用パッド
    
    
    //自チームのダッシュマン目標Ｘ★
    s32 GenGetAtcLineX(BOOL jp_f, BOOL dsmn_f);
    s32 GetAtcLineX(BOOL jp_f, BOOL dsmn_f);
    BOOL IsNoJpDashman();
    
    BOOL IsBallCrtMyPos(s32 pos, BOOL wide_f);
    
    
    //com★
    s32 GetCom(enCom com);
    s32 GetComAct(s32 comactNo, enDtComAct cma, BOOL ressei_f, s32 id);
    void ReaderChange();
    
    BOOL IsWaitOver();
    //ダッシュマンパスタイミング
    BOOL IsDMPaItvTime(BOOL infsetter_f);
    //ダッシュマン発動タイミング
    BOOL IsDMDelayTime();
    
    BOOL IsChancePass();
    
    //操作キャラを求める
    void GetCtrl();
    //最前衛に居る内野
    s32 GetFrontInfPNo();
    //近くにいる外野
    s32 GetNearGaiyaPNo();
    
    //最下位オーダーの内野
    s32 GetBottomInfPNo();
    
    //パスタイプセット
    void SetPassType(s32 patype);
    //シュートタイプセット
    void SetQShootType(s32 shtype);
    void SetPShootType(s32 shtype);
    
    //操作権予約
    void SetCtrlReserve(s32 tCRNo);
    //操作権（playerobjから直接呼び出すのはどうだろうか）
    void SetCtrlBallGet(s32 tCNo);
    void SetCtrl(s32 tCNo);
    //現状から操作権最適キャラを決める
    s32 SeekCtrl(BOOL set_f);
    //現状から操作権最適キャラを決める
    s32 SeekCtrl(BOOL set_f, BOOL landpos_f);
    //次期カバーマンを設定する
    void SeekCover(s32 tHoldPNo,s32 tPichPNo, s32 tPaTgPNo, BOOL Cvr2_f);
    void SeekGetter();
    //ポジションリセット予約する
    //void SetPosReset();
    
    //ふぉめちぇんじ
    void PopupFomChange();
    
    //各内野ポジションの現在行くべき位置
    void GetFomposOfInf(s32& getx, s32& getz, s32 posNo);
    
    //交代するかどうか
    BOOL IsIOChange();
    //ポジション入れ替え確認
    void CheckChangePos();

    //天使
    void SetAngel(s32 posNo);
    
    //サドンデス状態
    void SuddunDeath();
    
    //ヒットストップ★
    //void ResetHitStop();
    //動作
    void TeamMove();

#ifdef __K_DEBUG_SHIAI__
	//シュートチェンジ
	void Debug_SetShot(void);
#endif	// #ifdef __K_DEBUG_SHIAI__

    //描画
    void Draw();
  };
  
}
