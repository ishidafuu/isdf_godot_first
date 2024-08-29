#pragma once
#include "dbInclude.h"
#include "dbUILayer.h"
#include "dbUGDrTeam.h"
#include "dbUGDrShot.h"
#include "dbUGInput.h"
#include "dbUGRadio.h"
#include "dbUGLabel.h"
#include "dbUGTeam.h"
#include "dbUGButton.h"
#include "dbUGBtnSupply.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"
#include "dbUGChSensyu.h"
#include "dbUGScEnemy.h"
#include "dbUGScPanel.h"
#include "dbUGScVSTeam.h"
#include "dbLyScGrp.h"

namespace db
{
  class TSnScenario;

  class TLyScGen :public TUILayer
  {
  protected:
    TLyScGrp* scgrp_;
    TSnScenario* pScene_;
    //オーバーロード
    virtual void DoResetBtn(){}
  public:
    //コンストラクタ
    explicit TLyScGen(TSnScenario* pScene);
    virtual ~TLyScGen(void);
    void ResetBtn();
  };
 
}
