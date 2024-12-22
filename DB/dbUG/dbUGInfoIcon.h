#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
  class TUILayer;

  struct TUGInfoIconSt
  {
    BOOL Info_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGInfoIcon : public TUGGen
  {
  public:
    explicit TUGInfoIcon(TUIGenLayer* player);
    virtual ~TUGInfoIcon(void);
    void MakeGroup(TUIObj* parent);
    void Refresh();
  protected:
    void DoUpdate(BOOL draw_f);
    TUGInfoIconSt stInfoIcon_;
    TUIObj* icon_;
  };
 
}
