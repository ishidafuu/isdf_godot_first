#pragma once
#include "dbChGen.h"
#include "dbChCommon.h"
#include "dbChCount.h"
#include "dbChAction.h"
#include "dbChMove.h"
#include "dbChRemove.h"
#include "dbChSetRect.h"
#include "dbChDraw.h"

namespace db
{
  //プレーヤー動作オブジェ管理および描画////////////////////////////////
  class TMgChar : public TChGen
  {
  protected:
    //参照設定
    void DoRefSet();
    void DoReady();
  public:
    //各動作クラス
    TChCommon chCommon_;//共通処理
    TChCount chCount_;//モーションカウンタ
    //TChInputMT chManual_;//マニュアル入力
    //TChInputAT chAuto_;//オート入力
    TChMove chMove_;//移動
    TChAction chAction_;//アクション
    TChRemove chRemove_;//位置補正
    TChSetRect chSetRect_;//判定矩形作成
    TChDraw chDraw_;
    
    //コンストラクタ
    explicit TMgChar(TGame* pGame, s32 sideNo, s32 charNo);
    virtual ~TMgChar(void);
   
    //ステータス初期化
    void ResultMotion(enDBMotionType tMtype);
    //外から呼ばれるアクション
    void DoCount();
    void DoInput();
    void DoMove();
    void DoRemove();
    void DoMap();
    void DoEvent();
    void DoSearch();
    void DoSetRect();
    void DoDraw();
  };
}
