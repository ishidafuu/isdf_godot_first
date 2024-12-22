#include "dbPoGetMaster.h"

namespace db
{
  TPostGetMaster::TPostGetMaster(TMgEngineObj* pmgEO) :TPoGen(pmgEO)
  {
  }

  TPostGetMaster::~TPostGetMaster(void)
  {
  }

  ////送信パック初期化
  //void TPostGetMaster::Ready_clmaster()
  //{
  //  //一応バージョン番号送りつける
  //  sendmap_.longmap_["clmver"] = mid::midGetClMasterVer();
  //  sendmap_.longmap_["scheck"] = enSessionCheckType_Reset;
  //  rcvmap_.longmap_["error"] = 0;
  //  
  //  //Zip取得
  //  StartConnect_GetZip("co_clmaster", "clmver");
  //}

  //void TPostGetMaster::Ready_svmaster()
  //{
  //  //一応バージョン番号送りつける
  //  sendmap_.longmap_["svmver"] = mid::midGetSvMasterVer();
  //  sendmap_.longmap_["scheck"] = enSessionCheckType_Reset;
  //  rcvmap_.longmap_["error"] = 0;
  //  
  //  //Zip取得
  //  StartConnect_GetZip("co_svmaster", "svmver");

  //}


  
}
