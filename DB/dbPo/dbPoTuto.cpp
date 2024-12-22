#include "dbPoTuto.h"

namespace db
{
  TPostTuto::TPostTuto(TMgEngineObj* pmgEO):TPoGen(pmgEO)
  {
  }

  TPostTuto::~TPostTuto(void)
  {
  }
  void TPostTuto::Ready_SetBit(enTutoBitType tutobit)
  {
    ClearMap();
    sendmap_.longmap_["tutobit"] = (s32)tutobit;
    StartConnect("tuto_setbit");
  }

  void TPostTuto::Ready_logbo()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("tuto_logbo");
  }
  void TPostTuto::Ready_TmSelect(s32 mid_team)
  {
    ClearMap();
    sendmap_.longmap_["mid_team"] = mid_team;
    StartConnect("tuto_tmselect");
  }
  void TPostTuto::Ready_Gacha(s32 mid_gacha)
  {
    ClearMap();
    sendmap_.longmap_["mid_gacha"] = mid_gacha;
    StartConnect("tuto_gacha");
  }
  void TPostTuto::Ready_Gousei(INTVECTOR* midspsyouhinvec)
  {
    ClearMap();
    for (s32 i = 0; i < midspsyouhinvec->size(); ++i)
    {
      std::string outid = "mid_spsyouhin" + lib_str::IntToStr1b(i, 0);
      sendmap_.longmap_[outid.c_str()] = midspsyouhinvec->at(i);
    }
    StartConnect("tuto_gousei");
  }

  //void TPostTuto::Ready_play(s64 id_team)
  //{
  //  ClearMap();
  //  //posttype_ = enPostScenario_play;
  //  sendmap_.longmap_["id_team"] = id_team;
  //  StartConnect("tuto_play");
  //}

  void TPostTuto::Ready_ScReady(s64 id_m_maptip)
  {
    ClearMap();
    sendmap_.longmap_["id_m_maptip"] = id_m_maptip;
    StartConnect("tuto_ready");
  }

}
