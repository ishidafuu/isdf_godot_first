#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{

  struct TUGShCountSt
  {
    s32 s0num_;
    s32 s1num_;
    s32 bcount_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //セリフ
  class TUGShCount : public TUGGen
  {
  protected:
    INTVECTOR itemslen_;//各ラインパーツの文字列長さ
    TUIObj* killcount_;//カウント
    TUIObj* ballcount_;//カウント

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGShCountSt stShCount_;
    //コンストラクタ
    explicit TUGShCount(TUIGenLayer* player);
    virtual ~TUGShCount(void);
    void MakeGroup(TUIObj* parent);
    void Refresh();
  };
 
}