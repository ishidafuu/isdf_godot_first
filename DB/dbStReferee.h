#pragma once
#include "dbStGen.h"

namespace db
{
  //レフェリーステータス************************************
  enum enRfShiaiPhase
  {
    dbssReady,
    dbssShiai,
    dbssGameEnd,
    dbssResult,
    dbssGohome,
    //dbssPause,
    //dbssStep,
  };
  
  enum enRfMotion
  {
    dbrfStand,
    dbrfJumpBallWait,
    dbrfOvLine,
    dbrfJumpBall,
    dbrfLongWhistle,
    dbrfShortWhistle,
    dbrfIOChange,
    dbrfPrevReset,
    dbrfTutoReset,
  };
  
  //レフェリーステータス
  struct TStReferee
  {
    //BOOL TutoEnd_f;
    BOOL Demo_f;
    BOOL DisCnct_f;
    BOOL GameSet_f;
    BOOL JglSE_f;
    BOOL NoSE_f;
    BOOL Option_f;
    BOOL PauseStepNext_f;
    BOOL PauseStep_f;
    BOOL Pause_f;
    BOOL Prev_f;//シュートプレビューモード
    BOOL Suddun_f;
    BOOL TimeUp_f;
    BOOL Tour_f;//疑似対人
    BOOL TutoGameAct_f;//実戦開始
    BOOL TutoSend_f;//チュートアクション発生（レフェリー開始時初期化）
    BOOL actbtn_f;//キャッチ、ドッジボタンが押された（ＯＫかＮＧが送られるまで保存）
    BOOL prevreturn_f;//チュートアクション発生（レフェリー開始時初期化）
    enMukiType Muki;
    enRfMotion Motion;
    enRfShiaiPhase DBSSt;
    enSceneName NextScene;
    s32 Ani_c;
    s32 DrawLimitTime;
    s32 JglTime;
    s32 Jgl_c;
    s32 LimitTime;
    s32 M_c;//一応今のところアニメーションカウンタ的な物も兼ねる
    s32 PauseCsrPos;
    s32 Slow_c;
    s32 TutoNo;//現状のチュートアクション番号（とりあえず値はそのまま）
    s32 TutoRcv;//受信したチュートアクション番号
    s32 TutoSend;//チュートアクション番号(レフェリー開始時初期化、ここに数値を入れておいてpGame処理後拾うようにする)
    s32 Whistle_c;//ホイッスル
    s32 WinSide;
    s32 Win_c;
    s32 act_c;
    u32 PlayTime_c;//成績用試合時間
    
	BOOL isTimeAttackMode_;	// タイムアタックモード

    void Init()
    {
      Zeromem(this);
    }
    
  };
  
}
