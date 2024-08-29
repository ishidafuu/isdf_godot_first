#pragma once
#include "dbInclude.h"

namespace db
{
  
  class TMgCoZip
  {
  public:
    explicit TMgCoZip(std::string co_url, std::string verkey);
    virtual ~TMgCoZip(void);
    //チェックアウト
    void StartConnect(s64 ver, BOOL cdn_f);
    enRequestState Connecting();
  protected:
    std::string CO_URL;
    std::string CO_URL_CDN;
    std::string VERKEY;
    std::string rcvbfpack_;
  };
}
