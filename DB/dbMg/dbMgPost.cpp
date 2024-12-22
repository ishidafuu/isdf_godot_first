#include "dbMgPost.h"
#include "dbMgEngineObj.h"
//#include "dbMgCoUser.h"
#include "dbMgCoGen.h"
namespace db
{

  TMgPost::TMgPost(TMgEngineObj* pmgEO) :pmgEO_(pmgEO)
    , poGetMaster_(pmgEO)//ZIP取得
    , poVerCheck_(pmgEO)//バージョンチェック
    , poTuto_(pmgEO)
    , poHome_(pmgEO)//ホーム
    , poHmShiai_(pmgEO)
    , poHmTeam_(pmgEO)
    , poDebug_(pmgEO)
    , poHmShop_(pmgEO)
    , poScenario_(pmgEO)
    , poHmCenter_(pmgEO)
    , poBank_(pmgEO)
  {
    poActive_ = NULL;
  }

  TMgPost::~TMgPost(void)
  {

  }

  //接続中
  enRequestState TMgPost::Connecting()
  {
    enRequestState res = rqNone;
    //通信中も抜ける
    if (mid::midGetRequestState() == mid::rqConnecting)
    {
      res = rqConnecting;
    }
    else if (mid::midGetRequestState() == mid::rqOK)
    {
      //通信成功保存はmidIOS内でしておく
      mid::midLog("TMgPost::ConnectingrqOK\n");
      res = rqOK;//このタイミングでしかＯＫはでない

      if (mid::midIsPack())
      {
        if (Unpack() == FALSE)//解凍
        {
          res = rqNG;
        }
      }

      mgcoActive_->CleanDel();//削除フラグの値削除
      mgcoActive_->MappingPK(TRUE);//更新チェック

      mid::midSetRequestStateNone();//空に戻す
    }
    else if (mid::midGetRequestState() == mid::rqNG)
    {
      //通信失敗
      mid::midLog("TMgPost::ConnectingrqNG\n");
      res = rqNG;//このタイミングでしかＮＧはでない
      mid::midSetRequestStateNone();//空に戻す
    }

    return res;
  }
  BOOL TMgPost::Unpack()
  {
    //受信したBFPackをMappack,modelsに分配
    mid::midLog("midBFpackToMappack\n");
    return mid::midBFpackToMappack(poActive_->rcvbfpack_, poActive_->rcvmap_, mgcoActive_->models_, FALSE);
  }
  void TMgPost::SetActivePost(TPoGen* poActive, TMgCoGen* mgcoActive)
  {
    poActive_ = poActive;
    mgcoActive_ = mgcoActive;
  }
  s32 TMgPost::GetCode()
  {
    return poActive_->GetCode();
  }
  std::string TMgPost::GetMsg()
  {
    return poActive_->GetMsg();
  }
  s64 TMgPost::GetValue(const char* key)
  {
    return poActive_->GetValue(key);
  }
  std::string TMgPost::GetValueStr(const char* key)
  {
    return poActive_->GetValueStr(key);
  }
  BOOL TMgPost::IsCodeZero()
  {
    return poActive_->IsCodeZero();
  }
  BOOL TMgPost::IsNeedRetry()
  {
    return poActive_->IsNeedRetry();
  }
  BOOL TMgPost::IsGotoTitle()
  {
    return poActive_->IsGotoTitle();
  }
  void TMgPost::RetryConnect()
  {
    return poActive_->RetryConnect();
  }
}
