#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{
  class TUGLock : public TUGGen
  {
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    explicit TUGLock(TUIGenLayer* player);
    virtual ~TUGLock(void);
    void MakeGroup(TUIObj* parent);
  };
 
}