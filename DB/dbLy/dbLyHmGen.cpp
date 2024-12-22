#include "dbLyHmGen.h"
#include "dbSnHome.h"

namespace db
{
  //コンストラクタ
  TLyHmGen::TLyHmGen(TSnHome* pScene)
    :TUILayer(pScene, pScene->frame_)
  {
    pScene_ = pScene;
    hmgrp_ = pScene->hmGrp_;

  }

  TLyHmGen::~TLyHmGen()
  {
  }

}
