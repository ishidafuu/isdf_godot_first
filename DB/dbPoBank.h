#pragma once
#include "dbPoGen.h"


namespace db
{
  class TPostBank : public TPoGen
  {
  protected:
  public:
    explicit TPostBank(TMgEngineObj* pmgEO);
    virtual ~TPostBank(void);
    
    void Ready_create(s64 mid_supply, s32 techno);
    void Ready_close(const char* tid);
    void Ready_cancel(const char* tid);
  };
}
