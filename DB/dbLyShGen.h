#pragma once
#include "dbInclude.h"
#include "dbUILayer.h"
#include "dbUGButton.h"

namespace db
{
  class TSnShiai;

  class TLyShGen :public TUILayer
  {
  protected:
    //TLyScGrp* scgrp_;
    TSnShiai* pScene_;
  public:
    //コンストラクタ
    explicit TLyShGen(TSnShiai* pScene);
    virtual ~TLyShGen(void);
    //オーバーロード
    void DoLoad();
  };
 
}
