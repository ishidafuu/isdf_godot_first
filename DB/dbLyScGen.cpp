#include "dbLyScGen.h"
#include "dbSnScenario.h"
namespace db
{
  //コンストラクタ
  TLyScGen::TLyScGen(TSnScenario* pScene) :TUILayer(pScene, pScene->frame_)
  {
    scgrp_ = pScene->scgrp_;
    pScene_ = pScene;
  }

  TLyScGen::~TLyScGen()
  {
  }
  void TLyScGen::ResetBtn()
  {
    DoResetBtn();
  }
}
