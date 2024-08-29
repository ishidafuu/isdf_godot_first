#include "dbLyTuGen.h"
#include "dbSnTutorial.h"
//=========================================================================
// 
// チュートリアルシーンクラス
//
//=========================================================================
namespace db
{
  //コンストラクタ
  TLyTuGen::TLyTuGen(TSnTutorial* pScene)
    :TUILayer(pScene, pScene->frame_)
  {
    pScene_ = pScene;
    tugrp_ = pScene->tuGrp_;
  }

  TLyTuGen::~TLyTuGen()
  {
  }

}
