#pragma once
#include "dbInclude.h"
#include "dbUILayer.h"
#include "dbLyTuGrp.h"
//=========================================================================
// 
// チュートリアルシーンクラス
//
//=========================================================================
namespace db
{
  class TSnTutorial;
  class TUIFrame;

  class TLyTuGen :public TUILayer
  {
  protected:
    TLyTuGrp* tugrp_;
    TSnTutorial* pScene_;
  public:
    //コンストラクタ
	  explicit TLyTuGen(TSnTutorial* pScene);
    virtual ~TLyTuGen(void);
  };
 
}
