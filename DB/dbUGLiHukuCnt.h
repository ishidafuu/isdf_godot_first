#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcHukuCnt.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiHukuCntSt
  {
    s64 mid_Huku_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiHukuCnt :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcHukuCnt*> records_;

    TUGLiHukuCntSt stLiHukuCnt_;
    //コンストラクタ
    explicit TUGLiHukuCnt(TUIGenLayer* player);
    virtual ~TUGLiHukuCnt(void);
    void Refresh(s64 mid_hukugrp);
    s64 GetCommonID();
  };
 
}
