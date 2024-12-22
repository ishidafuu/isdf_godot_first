#pragma once
#include "dbPoGen.h"

namespace db
{
  class TPostTuto : public TPoGen
  {
  protected:
  public:
    explicit TPostTuto(TMgEngineObj* pmgEO);
    virtual ~TPostTuto(void);
    
    void Ready_SetBit(enTutoBitType tutobit);
    void Ready_logbo();
    void Ready_TmSelect(s32 mid_team);
    void Ready_Gacha(s32 mid_gacha);
    void Ready_Gousei(INTVECTOR* midspsyouhinvec);
    void Ready_ScReady(s64 id_m_maptip);
  };
}
