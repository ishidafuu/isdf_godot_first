#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"

namespace db
{
 
  struct TUGTextSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGRText :public TUGGen
  {
  protected:

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGTextSt stText_;
    std::vector<TUIObj*> text_;

    //コンストラクタ
    explicit TUGRText(TUIGenLayer* player);
    virtual ~TUGRText(void);

    void MakeGroup(TUIObj* parent,s32 linenum);
    void SetData(const STRVECTOR& textvec);
  };
 
}
