#include "dbPoVerCheck.h"

namespace db
{
  TPostVerCheck::TPostVerCheck(TMgEngineObj* pmgEO):TPoGen(pmgEO)
  {
  }

  TPostVerCheck::~TPostVerCheck(void)
  {
  }
  
  
  //データ取得
  s32 TPostVerCheck::GetClMasterVer()
  {
    return rcvmap_.longmap_["clmver"];
  }
  s32 TPostVerCheck::GetSvMasterVer()
  {
    return rcvmap_.longmap_["svmver"];
  }
  s64 TPostVerCheck::GetSvTime()
  {
    return rcvmap_.longmap_["svtime"];
  }
  s32 TPostVerCheck::GetBannerVer()
  {
    return rcvmap_.longmap_["bnver"];
  }
  s32 TPostVerCheck::GetEventVer()
  {
    return rcvmap_.longmap_["evver"];
  }
  s32 TPostVerCheck::GetConnect()
  {
    s32 res = 0;
    
    s32 kirisute = 100;
    //10200から10202のバージョンアップに関しては審査の判断が出来なくなるので例外的に従来通り
    if (rcvmap_.longmap_["cver"] == 10200)
    {
      kirisute = 1;
    }
    
    s32 rcvcver = (rcvmap_.longmap_["cver"] / kirisute);
    s32 myver = (mid::midGetClVersion() / kirisute);
    if (myver > rcvcver)//アップル審査
    {
      res = 1;
    }
    else if (myver < rcvcver)//itunesにとばす
    {
      res = 2;
    }

    return res;
  }

  //送信パック初期化
  void TPostVerCheck::Ready_vercheck()
  {
    ClearMap();
    //こっちのクライアントバージョンをおくってやる
    sendmap_.longmap_["cver"] = mid::midGetClVersion();
    sendmap_.longmap_["scheck"] = enSessionCheckType_Reset;
    if (mid::midIsIOS())
    {
        sendmap_.longmap_["os"] = 1;
    }
    else if (mid::midIsADR())
    {
        sendmap_.longmap_["os"] = 2;
    }
    
    rcvmap_.longmap_["error"] = 0;
    rcvmap_.longmap_["cver"] = 0;
    rcvmap_.longmap_["clmver"] = 0;
    rcvmap_.longmap_["svmver"] = 0;
    rcvmap_.longmap_["bnver"] = 0;
    rcvmap_.longmap_["evver"] = 0;


    //rcvmap_.longmap_["connect"] = 0;
    StartConnect("lo_vercheck");
  }


  ////送信パック初期化
  //void TPostVerCheck::Ready_kiyaku()
  //{
  //  ClearMap();
  //  sendmap_.longmap_["dummy"] = 0;
  //  StartConnect("lo_kiyaku");
  //}

  
  
  //BOOL TPostVerCheck::DoRespons()
  //{
  //  BOOL res = FALSE;
  //  s32 code = rcvmap_.longmap_["code"];
  //  switch (code) {
  //    case 0:
  //      res = TRUE;//OK
  //      break;
  //    case 1:
  //      //メンテナンス
  //      mid::midAlert("サーバーメンテナンス", "サーバーがメンテナンス中です。メンテナンス終了までお待ちください。");
  //    case 2:
  //      //クライアントバージョン違い
  //      mid::midAlert2("バージョンエラー", "クライアントのバージョンが違います。最新バージョンにアップデートして下さい。", mid::midJumpiTunes);
  //      break;
  //    default:
  //      //基本的に来てはいけない
  //      mid::midAlert("通信処理エラー", "サーバーの処理に失敗しました。");
  //      break;
  //  }
  //  
  //  return res;
  //}
}
