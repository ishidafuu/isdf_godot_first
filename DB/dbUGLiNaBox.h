#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNaBox.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiNaBoxSt
  {
    s64 mid_gacha_;
    s64 mid_gachagrp_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGLiNaBox :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNaBox*> records_;
    TUGLiNaBoxSt stLiNaBox_;
    //コンストラクタ
    explicit TUGLiNaBox(TUIGenLayer* player);
    virtual ~TUGLiNaBox(void);
    void Refresh();
    s64 GetCommonID();
  };
 
}
