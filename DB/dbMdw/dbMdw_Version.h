#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdw_Version
  {
    mdw_version_pk,//バージョン管理ＩＤ
    mdw_version_svmasterVer,//サーバーマスターバージョン番号
    mdw_version_clmasterVer,//クライアントマスターバージョン番号
    mdw_version_bannerVer,//バナーバージョン番号
    mdw_version_eventVer,//イベントバージョン番号
    mdw_version_circleID,//公式サークルＩＤ
  };
  
  const std::string W_VERSIONCLM[] = 
  {
    "pk",//バージョン管理ＩＤ
    "svmasterVer",//サーバーマスターバージョン番号
    "clmasterVer",//クライアントマスターバージョン番号
    "bannerVer",//バナーバージョン番号
    "eventVer",//イベントバージョン番号
    "circleID",//公式サークルＩＤ
  };
  
  class TMdw_Version: public TModelGen
  {
  protected:

  public:
    explicit TMdw_Version();
    virtual ~TMdw_Version();
    s64 GetLineDt(s64 line, enmdw_Version dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdw_Version dtNo);
    s64 GetPKDt(s64 pk, enmdw_Version dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdw_Version dtNo);
  };
}
