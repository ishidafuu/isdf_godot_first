#pragma once
#include "dbPoGen.h"


namespace db
{
  class TPostVerCheck : public TPoGen
  {
  protected:
  public:
    explicit TPostVerCheck(TMgEngineObj* pmgEO);
    virtual ~TPostVerCheck(void);
    
    s32 GetClMasterVer();
    s32 GetSvMasterVer();
    s32 GetBannerVer();
    s32 GetEventVer();
    s64 GetSvTime();
    s32 GetConnect();
    
    void Ready_vercheck();
    //void Ready_kiyaku();
  };
}
