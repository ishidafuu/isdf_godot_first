#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{
  class TUGVS : public TUGGen
  {
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    explicit TUGVS(TUIGenLayer* player);
    virtual ~TUGVS(void);
    void MakeGroup(TUIObj* parent);
  };
 
}