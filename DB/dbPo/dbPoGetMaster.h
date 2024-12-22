#pragma once
#include "dbPoGen.h"


namespace db
{

  class TPostGetMaster : public TPoGen
  {
  protected:
  public:

    explicit TPostGetMaster(TMgEngineObj* pmgEO);
    virtual ~TPostGetMaster(void);
    //
    //void Ready_clmaster();
    //void Ready_svmaster();
  };
}
