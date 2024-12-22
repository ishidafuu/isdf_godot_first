#pragma once
#include "dbPoGen.h"


namespace db
{
  enum enPostDebug
  {
    enPostDebug_AddItem,//アイテム追加
    enPostDebug_ZeroItem,//アイテム全削除
    enPostDebug_AddCredit,//所持金追加
    enPostDebug_ZeroCredit,//所持金削除
    enPostDebug_DecStamina,//スタミナダウン
    enPostDebug_ZeroShot,//シュート全消し
    enPostDebug_AddShot,//シュート追加
    enPostDebug_ResetLogbo,//ログボリセット
    enPostDebug_AddTeam,//チーム追加
	enPostDebug_TeamLvMax,//チームLvMax
	enPostDebug_TeamLvMin,//チームLvMin
	enPostDebug_TeamChLvMax, //チーム内全キャラLvMax
	enPostDebug_TeamChLvMin, //チーム内全キャラLvMin
    enPostDebug_DelDock,//ドック削除
    enPostDebug_AddGiftBox,//贈り物ボックス
    enPostDebug_ResetNaBox,//なつかしボックスリセット
    enPostDebug_AddHaikei,//背景追加
    enPostDebug_AddBGM,//BGM追加
    enPostDebug_AddMap,//マップ追加
    enPostDebug_DelHaikeiBGM,//背景BGM削除
    enPostDebug_ResetTuto,//チュートリアルリセット
    enPostDebug_ResetTutoSub,//チュートリアルリセットサブ
    enPostDebug_DelKantoku,//監督削除
    enPostDebugEND,
  };

  class TPostDebug : public TPoGen
  {
  protected:
    enPostDebug posttype_;
  public:
    explicit TPostDebug(TMgEngineObj* pmgEO);
    virtual ~TPostDebug(void);
    void Ready(enPostDebug debug);
  };
}
