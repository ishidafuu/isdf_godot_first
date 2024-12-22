#pragma once
#include "dbMgStGame.h"

#include "dbMgChar.h"
#include "dbMgBall.h"
#include "dbMgReferee.h"
#include "dbMgTeam.h"
#include "dbMgEtc.h"
#include "dbMgCommon.h"
#include "dbMgPad.h"

namespace db
{
  
  enum enFNoShiai
  {
    fnsGameTimeName = 80,
    fnsGameTime,
    fnsBallTimeName,
    fnsBallTime,
    fnsShotName0,
    fnsShotName1,
    fnsPauseMsg,
    fnsContinue,
    fnsOption,
    fnsQuit,
    fnsName0,
    fnsName1,
    fnsName2,
    fnsName3,
    fnsName4,
    fnsName5,
    fnsName6,
    fnsName7,
    fns_END
  };
  
  enum enGOType
  {
    goTitle,
    goShiai,
    goShiaiTour,
    goPrev,
    goTutoFirst,
    goTutoRetro,
    goTutoSmart,
	goTutoAuto,
  };
  
  
  //ドッジ試合内での全オブジェ（チーム、プレーヤー、ボール）を管理////////////////////////////////
  class TMgGameObj
  {
  protected:
    s32 objmslinit_c;
    enGOType gotype_;
    
    
    //試合
    void ShiaiObjMove_Cap();
    //試合
    void ShiaiObjMove();
    //チュート終了待機
    void TutoEndMove();
    //試合終了
    void ResultObjMove();
    //デモ帰宅
    void GameSetObjMove();
    
    //クイックセーブ
    s32 QSvSlot;
    
    //乱数初期化
    //void InitGameRnd(BOOL tZero_f);
    //管理しているオブジェの初期化
    void RefSet();
    
    

    void Ready_Shiai(BOOL demo_f, BOOL tour_f);
    void Ready_Prev();
    void Ready_Tuto(BOOL first_f, enGOType type);
    void Ready2_TutoJissen();

    
    void Update_Shiai();
    void Update_Prev();
    void Update_Tuto();
    
    void Draw_Shiai();
    void Draw_Prev();
    void Draw_Tuto();
  public:
    //ゲーム
    TGame* pGame_;
    
    //エンジンオブジェ管理クラスポインタ
    TMgEngineObj* pmgEO_;
    //ステータス管理クラスポインタ
    TMgStGame* pmgSG_;
    
    //オブジェ実体
    TMgChar* pmgCh_[DBSIDE][DBMEMBER_ALL];//プレーヤーオブジェ（ポジションと添え字が一致）
    TMgReferee* pmgRf_;//レフェリー
    TMgTeam* pmgTm_[DBSIDE];//チームオブジェ
    TMgBall* pmgBa_;//ボールオブジェ
    TMgEtc* pmgEtc_[DBETCOBJ];//小物オブジェ
    TMgCommon* pmgCommon_;//共通オブジェ
    TMgPad* pmgPad_[DBSIDE];//パッドオブジェ

    
    //コンストラクタ
    explicit TMgGameObj(TGame* pGame);
    virtual ~TMgGameObj();
    
    
    //管理しているオブジェの試合直前初期化
    void Ready(enGOType gotype);
    //リトライ
    void Retry();
    //切断
    void Dissconnect();
    //更新
    void Update();
    //描画
    void Draw();
    
    
  };
}
