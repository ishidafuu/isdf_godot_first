#include "dbBaGen.h"
#include "dbGame.h"

namespace db
{
  
  //コンストラクタ
  TBaGen::TBaGen(TGame* pGame) : TGameObjGen(pGame)
  {
    st_.Init();
    
    RefSet();
    
  }
  
  TBaGen::~TBaGen()
  {
  }
  
  //初期化
  void TBaGen::RefSet()
  {
    //マネージャーポインタ
    st_.pmgTm_[0] = pmgGO_->pmgTm_[0];
    st_.pmgTm_[1] = pmgGO_->pmgTm_[1];
    st_.pmgBa_ = pmgGO_->pmgBa_;
    st_.pmgRf_ = pmgGO_->pmgRf_;
    
    //ステータスポインタ
    st_.pstRf_ = &pmgSG_->stRf_;//レフェリーステータス
    st_.pstBa_ = &pmgSG_->stBa_;//ボールステータス
    st_.pstSh_ = &pmgSG_->stSh_;//シュートステータス
    st_.pstTm_[0] = &pmgSG_->stTm_[0];
    st_.pstTm_[1] = &pmgSG_->stTm_[1];
    
    DoRefSet();
  }
  
  void TBaGen::DoRefSet()
  {
  }
  
}
