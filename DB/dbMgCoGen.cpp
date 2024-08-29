#include "dbMgCoGen.h"
#include "dbMgEngineObj.h"
namespace db
{

  TMgCoGen::TMgCoGen(TMgEngineObj* pmgEO, std::string co_url)
  {
    pmgEO_ = pmgEO;
    stCO_.Init();
    CO_URL = co_url;
  }

  TMgCoGen::~TMgCoGen(void)
  {

  }

  //一括チェックアウトの時は中味を空に
  void TMgCoGen::ClearModels()
  {
    for (s32 i = 0; i < models_.size(); ++i)
    {
      models_[i]->Clear();
    }
    sendmap_.Clear();
    //sendmap_.longmap_["svmver"] = mid::midGetGenVer();
  }

  //メインチェックアウト
  void TMgCoGen::StartConnect()
  {
    //メインチェックアウトの時は中味を空に
    ClearModels();
    DoInitSendPack();
    rcvbfpack_co_.clear();
    if (mid::midIsPack())
    {
      mid::midURLConnection_CheckOut_Pack(&CO_URL, &sendmap_, &rcvbfpack_co_);
    }
    else
    {
      mid::midURLConnection_CheckOut(&CO_URL, &sendmap_, &models_);
    }

  }

  //追加チェックアウト
  void TMgCoGen::StartConnectSub(std::string co_url_sub)
  {
    DoInitSendPack();
    rcvbfpack_co_.clear();
    if (mid::midIsPack())
    {
      mid::midURLConnection_CheckOut_Pack(&co_url_sub, &sendmap_, &rcvbfpack_co_);
    }
    else
    {
      mid::midURLConnection_CheckOut(&co_url_sub, &sendmap_, &models_);
    }

  }

  enRequestState TMgCoGen::Connecting()
  {
    enRequestState res = rqNone;

    //通信中も抜ける
    if (mid::midGetRequestState() == mid::rqConnecting)
    {
      res = rqConnecting;
    }
    else if (mid::midGetRequestState() == mid::rqOK)//通信成功
    {
      mid::midLog("rqOK\n");
      res = rqOK;//このタイミングでしかＯＫはでない

      //受信したBFPackをMappack,modelsに分配
      if (mid::midIsPack())
      {
        MAPPACK rcvmap;
        if (mid::midBFpackToMappack(rcvbfpack_co_, rcvmap, models_, TRUE) ==FALSE)
        {
          res = rqNG;
        }
        //rcvmap["error"];
      }

      //チェックアウトしたものを保存 マスターはZIP→CSVからの読み込みにするので現状不要
      //SaveModel();
      //ＰＫ行番号マッピングチェックアウトの時はすべて
      MappingPK(FALSE);
      mid::midSetRequestStateNone();//空に戻す
    }
    else if (mid::midGetRequestState() == mid::rqNG)//通信失敗
    {
      mid::midLog("rqNG\n");
      res = rqNG;//このタイミングでしかＮＧはでない
      mid::midSetRequestStateNone();//空に戻す
    }

    return res;
  }


  //PK行番号マップを作成
  void TMgCoGen::MappingPK(BOOL checkupdate_f)
  {
    MODELMAPVECTOR::iterator models_it;
    for (models_it = models_.begin(); models_it != models_.end(); ++models_it)
    {
      (*models_it)->MappingPK(checkupdate_f);
    }
  }

  //削除フラグが立ってるものを削除
  void TMgCoGen::CleanDel()
  {
    MODELMAPVECTOR::iterator models_it;
    for (models_it = models_.begin(); models_it != models_.end(); ++models_it)
    {
      INTVECTOR::iterator delvec_it;
      for (delvec_it = (*models_it)->delvec_.begin(); delvec_it != (*models_it)->delvec_.end(); ++delvec_it)
      {
        //からのベクタは無死
        if ((*models_it)->intvecmap_.size() == 0) break;

        //del_fがあったベクタ（ＰＫが入ってる）の要素
        s64 delpk = (*delvec_it);

        s32 delpos = NGNUM;

        s32 veclen = (*models_it)->intvecmap_["pk"].size();

        for (s32 i3 = 0; i3 < veclen; ++i3)
        {
          if ((*models_it)->intvecmap_["pk"][i3] == delpk)//そのＰＫと一致
          {
            delpos = i3;
            break;
          }
        }

        if (delpos == NGNUM) break;

        {
          INTVECMAP::iterator intvecmap_it;
          //全てのキーに対して
          for (intvecmap_it = (*models_it)->intvecmap_.begin(); intvecmap_it != (*models_it)->intvecmap_.end(); ++intvecmap_it)
          {
            //ベクタ配列
            s32 index = 0;
            INTVECTOR::iterator intvec_it;
            for (intvec_it = (*intvecmap_it).second.begin(); intvec_it != (*intvecmap_it).second.end(); ++intvec_it)
            {
              if (index == delpos)
              {
                (*intvecmap_it).second.erase(intvec_it);//削除
                break;
              }
              else
              {
                ++index;
              }
            }
          }
        }
        {
          STRVECMAP::iterator strvecmap_it;
          //全てのキーに対して
          for (strvecmap_it = (*models_it)->strvecmap_.begin(); strvecmap_it != (*models_it)->strvecmap_.end(); ++strvecmap_it)
          {
            //ベクタ配列
            s32 index = 0;
            STRVECTOR::iterator strvec_it;
            for (strvec_it = (*strvecmap_it).second.begin(); strvec_it != (*strvecmap_it).second.end(); ++strvec_it)
            {
              if (index == delpos)
              {
                (*strvecmap_it).second.erase(strvec_it);//削除
                break;
              }
              else
              {
                ++index;
              }
            }
          }
        }
      }

      //使い終わった削除ベクタはクリア
      (*models_it)->delvec_.clear();
    }
  }

}
