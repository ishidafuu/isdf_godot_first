#include "dbPoGen.h"
#include "dbMgEngineObj.h"

namespace db
{
  TPoGen::TPoGen(TMgEngineObj* pmgEO)
  {
    pmgEO_ = pmgEO;
    retry_f_ = FALSE;
  }

  TPoGen::~TPoGen(void)
  {
  }
  void TPoGen::ClearMap()
  {
    sendmap_.Clear();
    rcvmap_.Clear();
    rcvbfpack_.clear();
    
  }

  void TPoGen::GenConnect(std::string url)
  {
    //ＰＯＳＴの前にログを削除
    pmgEO_->mgCoU_.CleanLog();

    if (mid::midIsPack())
    {
      mid::midURLConnection_Post_Pack(&url, &sendmap_, &rcvbfpack_);
    }
    else
    {
      mid::midURLConnection_Post(&url, &sendmap_, &rcvmap_, &pmgEO_->mgCoU_.models_);
    }

    //最後にPOSTしたTPoGenポインタをいれとく//アクティブなモデルはとりあえずユーザーモデルで固定(delフラグも返ってきたら消える)
    pmgEO_->mgPo_.SetActivePost(this, &pmgEO_->mgCoU_);
  }
  //接続開始(ユーザー)
  void TPoGen::StartConnect(std::string url)
  {
    lasturl_ = url;
    retry_f_ = FALSE;

    GenConnect(url);
  }

  //接続開始(ユーザー)課金関係などリトライして二重に購入してしまわないように
  void TPoGen::StartConnect_NoneRetry(std::string url)
  {
    lasturl_ = url;
    retry_f_ = TRUE;

    GenConnect(url);
  }

  ////接続開始(Zip取得)
  //void TPoGen::StartConnect_GetZip(std::string url, std::string verkey, s64 ver)
  //{
  //  mid::midURLConnection_GetZip(&url, &verkey, ver, &sendmap_);
  //  //最後にPOSTしたTPoGenポインタをいれとく
  //  pmgEO_->mgPo_.SetActivePost(this, &pmgEO_->mgCoU_);
  //}
  //リトライ
  void TPoGen::RetryConnect()
  {
    retry_f_ = TRUE;
    GenConnect(lasturl_);
  }

  s32 TPoGen::GetCode()
  {
    return rcvmap_.longmap_["code"];
  }
  std::string TPoGen::GetMsg()
  {
    return rcvmap_.strmap_["msg"];
  }
  s64 TPoGen::GetValue(const char* key)
  {
    return rcvmap_.longmap_[key];
  }
  std::string TPoGen::GetValueStr(const char* key)
  {
    return rcvmap_.strmap_[key];
  }

  BOOL TPoGen::IsCodeZero()
  {
    return (GetCode() == 0);
  }
  BOOL TPoGen::IsNeedRetry()
  {
    return (retry_f_ == FALSE);
  }
  BOOL TPoGen::IsGotoTitle()
  {
    return (GetCode() >= 1000);//エラーコード1000以上がタイトルに戻る
  }
}
