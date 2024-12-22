#include "dbLyTiGen.h"
#include "dbSnTitle.h"
//#include <memory>
namespace db
{
  //コンストラクタ
  TLyTiGen::TLyTiGen(TSnTitle* pScene) :TUILayer(pScene, pScene->frame_)
  {
    //std::shared_ptr<int> asdf;
    tigrp_ = pScene->tiGrp_;
    pScene_ = pScene;
  }

  TLyTiGen::~TLyTiGen()
  {
  }
  void TLyTiGen::DoLoad()
  {
  }

}
