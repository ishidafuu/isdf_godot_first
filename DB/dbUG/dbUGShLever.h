#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{
  class TUGShKantoku;

  struct TUGShLeverSt
  {
    s32 btntype_;

    void Init()
    {
      Zeromem(this);
    }
  };

  
  //キャッチ
  class TUGShLever : public TUGGen
  {
  protected:
    TUIObj* bal_;
    void GenMakeGroup(TUIObj* parent);
    void Refresh();
    void DoUpdate(BOOL draw_f);
  public:
    TUGShLeverSt stShLever_;
    //コンストラクタ
    explicit TUGShLever(TUIGenLayer* player);
    virtual ~TUGShLever(void);
    void MakeGroup(TUIObj* parent, BOOL prev_f);
  };
 
}