#pragma once
#include "dbObGameGen.h"

namespace db
{
  class TMgTeam;
  class TMgBall;
  class TMgReferee;
  struct TStReferee;
  struct TStBall;
  struct TStTeam;
  
  struct TStBaGen
  {
    //マネージャーポインタ
    TMgTeam* pmgTm_[DBSIDE];//自チーム
    TMgBall* pmgBa_;//ボール
    TMgReferee* pmgRf_;//レフェリー
    
    //ステータスポインタ
    TStReferee* pstRf_;//レフェリーステータス
    TStBall* pstBa_;//ボールステータス
    TStShot* pstSh_;//シュートステータス
    TStTeam* pstTm_[DBSIDE];//チームステータス


    
    void Init()
    {
      Zeromem(this);
    }
  };
  
  //ボールオブジェクト基礎////////////////////////////////
  class TBaGen : public TGameObjGen
  {
  protected:
    virtual void DoRefSet();
  public:
    TStBaGen st_;
    
    //コンストラクタ
    explicit TBaGen(TGame* pGame);
    virtual ~TBaGen(void);
    
    //初期化
    void RefSet();
  };
  
}
