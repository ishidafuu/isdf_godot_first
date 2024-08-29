#pragma once
#include "dbInclude.h"


namespace db
{
  class TMgEngineObj;
  
  //Post基礎
  class TPoGen
  {
  protected:
    BOOL retry_f_;//リトライ
    TMgEngineObj* pmgEO_;
    std::string lasturl_;
    void ClearMap();
    void GenConnect(std::string url);
  public:
    MAPPACK sendmap_;
    MAPPACK rcvmap_;
    std::string rcvbfpack_;
    
    explicit TPoGen(TMgEngineObj* pmgEO);
    virtual ~TPoGen(void);
    void StartConnect(std::string url);
    void StartConnect_NoneRetry(std::string url);
    //void StartConnect_GetZip(std::string url, std::string verkey, s64 ver);
    void RetryConnect();
    //enRequestState Connecting();
    s32 GetCode();
    std::string GetMsg();
    s64 GetValue(const char* key);
    std::string GetValueStr(const char* key);
    BOOL IsCodeZero();
    BOOL IsNeedRetry();
    BOOL IsGotoTitle();
  };
}
