#pragma once
#include "dbPoGen.h"

namespace db
{
  class TPostHmShiai : public TPoGen
  {
  protected:

  public:
    explicit TPostHmShiai(TMgEngineObj* pmgEO);
    virtual ~TPostHmShiai(void);
    
    void Ready_vshumcheck();
    void Ready_vshumready(s64 id_kantoku_en, s32 wifi_f, s32 roomtype);
    void Ready_vshumplay(s64 id_team_en, s64 id_team_my);
    void Ready_vshumretire(s32 errcode, s32 ng_c, s32 mybamotion, s32 enbamotion, s32 ctrl);
    void Ready_vshumresult(s64 vsresult, s64 kill, s64 death, s64 vstime, s64 maxdam, s64 totaldam, s32 errcode, s32 ctrl);

    void Ready_tourready(s64 mid_event);
    void Ready_tourplay(s64 id_team);
    void Ready_tourretire();
    void Ready_tourresult(s64 vsresult, s64 kill, s64 death, s64 vstime, s64 maxdam, s64 totaldam, s32 ctrl);

    void Ready_tmatready(s64 mid_event);
    void Ready_tmatplay(s64 id_team);
    void Ready_tmatretire();
    void Ready_tmatresult(s64 vsresult, s64 kill, s64 death, s64 vstime, s64 maxdam, s64 totaldam, s32 ctrl);


    void Ready_refreshevent();
  };
}
