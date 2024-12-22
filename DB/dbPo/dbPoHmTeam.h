#pragma once
#include "dbPoGen.h"


namespace db
{
  class TPostHmTeam : public TPoGen
  {
  protected:

  public:
    explicit TPostHmTeam(TMgEngineObj* pmgEO);
    virtual ~TPostHmTeam(void);
    
    void Ready_mgk(s64 id_team);
    void Ready_mgkquick(s64 id_team);
    void Ready_attachcustom(s64 id_team, LONGVECTOR* id_shot_vec);
    void Ready_attachshset(s64 id_shset, LONGVECTOR* id_shot_vec);
    void Ready_switchshset(LONGVECTOR* id_team_vec, LONGVECTOR* id_shset_vec);
    void Ready_switchshset(s64 id_team, s64 id_shset);
    void Ready_setbgm(s64 mid_bgm);
    void Ready_sethaikei(s64 mid_haikei);
    void Ready_okiniteam(s64 id_team);
    void Ready_renameshset(s64 id_shset, std::string shname);
    void Ready_renameshot(s64 id_shot, std::string shname);
    void Ready_shlock(LONGVECTOR* id_shot_vec);
  };
}
