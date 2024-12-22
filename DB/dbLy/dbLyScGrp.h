#pragma once
#include "dbInclude.h"
#include "dbUILayer.h"

namespace db
{
  class TSnScenario;

  //画像ローダー////////////////////////////////
  class TLyScGrp :public TUILayer//自動開放読み込みするためTUILayerを継承
  {
  protected:
    //オーバーロード
    void DoLoad();
  public:
    //base::TGrPart* vs_vs_;
    //base::TGrPart* vs_vsset_;
    //base::TGrPart* vs_body_;
    //base::TGrPart* vs_hp_;

    //コンストラクタ
    explicit TLyScGrp(TSnScenario* pScene);
    virtual ~TLyScGrp(void);


  };

}
