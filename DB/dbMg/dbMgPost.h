#pragma once
#include "dbInclude.h"
#include "dbPoGetMaster.h"
#include "dbPoVerCheck.h"
#include "dbPoTuto.h"
#include "dbPoHome.h"
#include "dbPoHmTeam.h"
#include "dbPoHmShiai.h"
#include "dbPoHmCenter.h"
#include "dbPoHmShop.h"
#include "dbPoScenario.h"
#include "dbPoBank.h"
#include "dbPoDebug.h"

namespace db
{
  class TMgCoGen;

  class TMgPost
  {
  protected:
    TMgEngineObj* pmgEO_;
  public:
    TPoGen* poActive_;
    TMgCoGen* mgcoActive_;
    std::string* rcvbfpack_;
    TPostGetMaster poGetMaster_;//マスターZIP取得
    TPostVerCheck poVerCheck_;//バージョンチェック
    TPostTuto poTuto_;//チュートリアルフラグ立て
    TPostHome poHome_;//ホーム
    TPostHmTeam poHmTeam_;//ホーム：チーム
    TPostHmShiai poHmShiai_;//ホーム：しあい
    TPostHmCenter poHmCenter_;//ホーム：センター
    TPostHmShop poHmShop_;//ホーム：ショップ
    TPostScenario poScenario_;//シナリオ
    TPostBank poBank_;//スター
    TPostDebug poDebug_;//デバッグ
    explicit TMgPost(TMgEngineObj* pmgEO);
    virtual ~TMgPost(void);
    
    enRequestState Connecting();
    BOOL Unpack();
    void SetActivePost(TPoGen* poActive, TMgCoGen* mgcoActive);
    
    //最後にポストした奴から取る
    s32 GetCode();
    std::string GetMsg();
    s64 GetValue(const char* key);
    std::string GetValueStr(const char* key);
    BOOL IsCodeZero();
    BOOL IsNeedRetry();
    BOOL IsGotoTitle();
    void RetryConnect();
  };
}
