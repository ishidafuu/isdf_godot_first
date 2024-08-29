#pragma once
#include "dbInclude.h"
#include "dbUILayer.h"
//#include "dbUGDrTeam.h"
#include "dbUGInput.h"
#include "dbUGRadio.h"
#include "dbUGLabel.h"
#include "dbUGTeam.h"
#include "dbUGFilter.h"
#include "dbUGButton.h"
#include "dbUGHud.h"
#include "dbLyTiGrp.h"

namespace db
{
  class TSnTitle;

  class TLyTiGen :public TUILayer
  {
  protected:
    TLyTiGrp* tigrp_;
    TSnTitle* pScene_;
  public:
    //コンストラクタ
    explicit TLyTiGen(TSnTitle* pScene);
    virtual ~TLyTiGen(void);
    //オーバーロード
    void DoLoad();
  };
 
}
