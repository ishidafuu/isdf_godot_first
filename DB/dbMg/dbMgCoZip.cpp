#include "dbMgCoZip.h"

namespace db
{

  BOOL cdn_f_ = FALSE;
  TMgCoZip::TMgCoZip(std::string co_url, std::string verkey)
  {
    CO_URL = "co_" + co_url;
    CO_URL_CDN = co_url;
    VERKEY = verkey;
  }

  TMgCoZip::~TMgCoZip(void)
  {

  }
  

  void TMgCoZip::StartConnect(s64 ver, BOOL cdn_f)
  {
    cdn_f_ = cdn_f;
    if (cdn_f)
    {
      mid::midURLConnection_GetZipCDN(&CO_URL_CDN, &VERKEY, ver);
    }
    else
    {
      MAPPACK sendpack;
      sendpack.longmap_["scheck"] = enSessionCheckType_Reset;
      sendpack.longmap_["clver"] = mid::midGetClVersion();
      if (mid::midIsIOS())
      {
          sendpack.longmap_["os"] = 1;
      }
      else if (mid::midIsADR())
      {
          sendpack.longmap_["os"] = 2;
      }
      rcvbfpack_ = "";
      if (mid::midIsPack())
      {
        mid::midURLConnection_GetZip_Pack(&CO_URL, &VERKEY, ver, &sendpack, &rcvbfpack_);
      }
      else
      { 
        mid::midURLConnection_GetZip(&CO_URL, &VERKEY, ver, &sendpack);
      }
    }
  }

  enRequestState TMgCoZip::Connecting()
  {
    enRequestState res = rqNone;

    //通信中も抜ける
    if (mid::midGetRequestState() == mid::rqConnecting)
    {
      res = rqConnecting;
    }
    else if (mid::midGetRequestState() == mid::rqOK)//通信成功
    {
      mid::midLog("TMgCoZip::Connecting()rqOK\n");
      
      //パックの場合はここで解凍
      if ((cdn_f_ == FALSE) && mid::midIsPack())
      {
        res = (mid::midURLSaveZip(&rcvbfpack_))
        ? rqOK
        : rqNG;
      }
      else
      {
        res = rqOK;//このタイミングでしかＯＫはでない
      }
      mid::midSetRequestStateNone();//空に戻す
    }
    else if (mid::midGetRequestState() == mid::rqNG)//通信失敗
    {
      mid::midLog("TMgCoZip::Connecting()rqNG\n");
      res = rqNG;//このタイミングでしかＮＧはでない
      mid::midSetRequestStateNone();//空に戻す
    }

    return res;
  }

}
