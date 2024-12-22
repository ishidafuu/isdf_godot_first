#include "dbLyShGen.h"
#include "dbSnShiai.h"
namespace db
{
  //コンストラクタ
  TLyShGen::TLyShGen(TSnShiai* pScene) :TUILayer(pScene, pScene->frame_)
  {
    //scgrp_ = pScene->scgrp_;
    pScene_ = pScene;
  }

  TLyShGen::~TLyShGen()
  {
  }
  void TLyShGen::DoLoad()
  {
  }

}
