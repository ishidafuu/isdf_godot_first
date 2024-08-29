#pragma once
#include "dbInclude.h"
#include "dbMgStGame.h"
#include "dbMgGameObj.h"
#include "dbMgEngineObj.h"

namespace db
{
  //試合管理////////////////////////////////
  class TGame : public base::TGameBase
  {
  protected:
    //オーバーロード
    void DoReady();//初期化
    void DoUpdate();//更新
    void DoDraw();//描画
    
  public:
    //エンジンオブジェクトポインタ（オーバーライド）
    TMgEngineObj* pmgEO_;
    
    //ゲーム内ステータス管理
    TMgStGame mgSG_;
    
    //ゲーム内オブジェ管理クラス
    TMgGameObj mgGO_;
    
    //コンストラクタ
    explicit TGame(TMgEngineObj* pmgEO);
    virtual ~TGame(void);
    
    //レフェリーの持つ次シーン
    enSceneName GetNextScene();
    BOOL IsWin();
    BOOL IsGameSet();
    BOOL IsTimeOver();
    //チュート用結果取得
    s32 GetTutoAct();
    //プレビュー用結果取得
    BOOL IsPrevReturn();
    //チュート用UGTutoからの受信
    void SetTutoAct(s32 tutoact);
    void  SetTutoGameAct();

    //各シーンごとのゲームオブジェの準備
    void Ready_Title();
    void Ready_Shiai();
    void Ready_Prev(BOOL needLoad_f, s64 mid_team, s64 id_team, s32 char0, s32 char1, s32 char2, s32 char3, s32 char4);
    void Ready_Tuto(BOOL retro_f);
    void Ready_VS();

    //復活
    void Retry();
    //切断
    void Dissconnect();

    //
    void TexRerease();
    BOOL IsDissconnect();
  };
  
}
