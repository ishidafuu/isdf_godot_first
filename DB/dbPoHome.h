#pragma once
#include "dbPoGen.h"

namespace db
{
  class TPostHome : public TPoGen
  {
  protected:
  public:
    explicit TPostHome(TMgEngineObj* pmgEO);
    virtual ~TPostHome(void);
    
    void Ready_home();
    //void Ready_logbo();
    void Ready_logbo(BOOL cancel_f, s32 mid_logbosozai, INTVECTOR* mid_spsyouhinvec);
    void Ready_ggz();
    void Ready_ggzquick();

    //void Ready_frcode(std::string frcode);
    //void Ready_frcodedecide(std::string frcode);
    void Ready_cpcode(s64 id_m_cpcode, std::string cpcode);
    void Ready_checkdraw(BOOL checkmami_f, BOOL checkmaruka_f);
    void Ready_checkgiftbox();
    void Ready_opengiftbox(LONGVECTOR* openidvec);

    void Ready_getitemlog(s32 page);
    void Ready_useitemlog(s32 page);
    void Ready_buystarlog(s32 page);
    void Ready_usestarlog(s32 page, BOOL bstar_f, BOOL fstar_f);

    void Ready_name(std::string name);
    void Ready_prof(s32 line, std::string prof);
    void Ready_avatar(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 clothecol);

    void Ready_ranker(s32 mid_ranking, BOOL result_f);
    void Ready_rankerpage(s32 mid_ranking, BOOL result_f, s32 page);
    void Ready_myrank(s32 mid_ranking, BOOL result_f);

	void Ready_training(s32 mid_supply, s64 mid_team, s32 posNo);
	void Ready_sellteam(s64 mid_team);
  };
}
