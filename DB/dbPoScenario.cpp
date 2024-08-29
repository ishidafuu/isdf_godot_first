#include "dbPoScenario.h"

namespace db
{
  TPostScenario::TPostScenario(TMgEngineObj* pmgEO):TPoGen(pmgEO)
  {
  }

  TPostScenario::~TPostScenario(void)
  {
  }

  //送信パック初期化
  void TPostScenario::Ready_ready(s64 id_m_maptip)
  {
    ClearMap();
    posttype_ = enPostScenario_ready;
    sendmap_.longmap_["id_m_maptip"] = id_m_maptip;
    StartConnect("sc_ready");
  }
  void TPostScenario::Ready_play(s64 id_team)
  {
    ClearMap();
    posttype_ = enPostScenario_play;
    sendmap_.longmap_["id_team"] = id_team;
    StartConnect("sc_play");
  }
  void TPostScenario::Ready_retry()
  {
    ClearMap();
    posttype_ = enPostScenario_retry;
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("sc_retry");
  }
  void TPostScenario::Ready_retrystar()
  {
    ClearMap();
    posttype_ = enPostScenario_retrystar;
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("sc_retrystar");
  }
  void TPostScenario::Ready_retire(s32 ctrl)
  {
    ClearMap();
    posttype_ = enPostScenario_retire;
    sendmap_.longmap_["ctrl"] = ctrl;
    StartConnect("sc_retire");
  }
  void TPostScenario::Ready_result(s64 vsresult, s64 kill, s64 death, s64 vstime, s64 maxdam, s64 totaldam, s32 ctrl)
  {
    ClearMap();
    posttype_ = enPostScenario_result;
    sendmap_.longmap_["vsresult"] = vsresult;
    sendmap_.longmap_["kill"] = kill;
    sendmap_.longmap_["death"] = death;
    sendmap_.longmap_["vstime"] = vstime;
    sendmap_.longmap_["maxdam"] = maxdam;
    sendmap_.longmap_["totaldam"] = totaldam;
    sendmap_.longmap_["ctrl"] = ctrl;
    StartConnect("sc_result");
  }
}
