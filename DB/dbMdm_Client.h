#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Client
  {
    mdm_client_pk,//クライアント状態ＩＤ
    mdm_client_mbgCircleID,//モバゲーサークルＩＤ
    mdm_client_vsbuf,//通信バッファ
    mdm_client_caper,//キャッチ確率
    mdm_client_ncaper,//ナイスキャッチ確率
  };
  
  const std::string M_CLIENTCLM[] = 
  {
    "pk",//クライアント状態ＩＤ
    "mbgCircleID",//モバゲーサークルＩＤ
    "vsbuf",//通信バッファ
    "caper",//キャッチ確率
    "ncaper",//ナイスキャッチ確率
  };
  
  class TMdm_Client: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Client();
    virtual ~TMdm_Client();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Client dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Client dtNo);
    s64 GetPKDt(s64 pk, enmdm_Client dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Client dtNo);
  };
}
