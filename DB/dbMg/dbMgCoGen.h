#pragma once
#include "dbModelGen.h"

namespace db
{


  struct TMgCoSt
  {

    void Init()
    {
      Zeromem(this);
    }
  };

  class TMgEngineObj;
  
  class TMgCoGen
  {
  protected:
    TMgEngineObj* pmgEO_;

    //クリア
    void ClearModels();
    //送信データセット
    virtual void DoInitSendPack() = 0;
  public:
    std::string CO_URL;
    std::vector<TModelGen*> mdvec_;
    TMgCoSt stCO_;
    MODELMAPVECTOR models_;
    MAPPACK sendmap_;
    //MAPPACK rcvmap_;//使わないけど一応
    std::string rcvbfpack_co_;
    
    explicit TMgCoGen(TMgEngineObj* pmgEO, std::string co_url);
    virtual ~TMgCoGen(void);

    //チェックアウト
    void StartConnect();
    void StartConnectSub(std::string co_url_sub);
    enRequestState Connecting();

    //PK行番号マップを作成
    void MappingPK(BOOL checkupdate_f);
    //BFPackを解凍
    //void Unpack();
    //削除フラグが立ってるものを削除
    void CleanDel();

    ////永続化からの読み込み
    //BOOL LoadModel();
    ////永続化書き込み
    //BOOL SaveModel();
  };
}
