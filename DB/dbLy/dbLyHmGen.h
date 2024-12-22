#pragma once
#include "dbInclude.h"
#include "dbUILayer.h"
#include "dbLyHmGrp.h"
namespace db
{
  class TSnHome;
  class TUIFrame;

  class TLyHmGen :public TUILayer
  {
  protected:
    TLyHmGrp* hmgrp_;
    TSnHome* pScene_;
  public:
    //コンストラクタ
    explicit TLyHmGen(TSnHome* pScene);
    virtual ~TLyHmGen(void);
  };
 
}
