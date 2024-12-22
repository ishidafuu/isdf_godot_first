#pragma once
#include "dbPoGen.h"


namespace db
{

  enum enPostScenario
  {
    enPostScenario_ready,
    enPostScenario_play,
    enPostScenario_retry,
    enPostScenario_retrystar,
    enPostScenario_retire,
    enPostScenario_result,
    enPostScenario_tutoresult,
    enPostScenarioEND,
  };

  class TPostScenario : public TPoGen
  {
  protected:
    enPostScenario posttype_;
  public:
    explicit TPostScenario(TMgEngineObj* pmgEO);
    virtual ~TPostScenario(void);
    
    void Ready_ready(s64 id_m_maptip);
    void Ready_play(s64 id_team);
    void Ready_retry();
    void Ready_retrystar();
    void Ready_retire(s32 ctrl);
    void Ready_result(s64 vsresult, s64 kill, s64 death, s64 vstime, s64 maxdam, s64 totaldam, s32 ctrl);
  };
}
