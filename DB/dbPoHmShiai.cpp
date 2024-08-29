#include "dbPoHmShiai.h"

namespace db
{
  TPostHmShiai::TPostHmShiai(TMgEngineObj* pmgEO):TPoGen(pmgEO)
  {
  }

  TPostHmShiai::~TPostHmShiai(void)
  {
  }


  //対人
  void TPostHmShiai::Ready_vshumcheck()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;

    StartConnect("hmshiai_vshumcheck");
  }
  void TPostHmShiai::Ready_vshumready(s64 id_kantoku_en, s32 wifi_f, s32 roomtype)
  {
    ClearMap();
    sendmap_.longmap_["id_kantoku_en"] = id_kantoku_en;
    sendmap_.longmap_["wifi_f"] = wifi_f;
    sendmap_.longmap_["roomtype"] = roomtype;
    
    StartConnect("hmshiai_vshumready");
  }
  void TPostHmShiai::Ready_vshumplay(s64 id_team_en, s64 id_team_my)
  {
    ClearMap();
    sendmap_.longmap_["id_team_en"] = id_team_en;
    sendmap_.longmap_["id_team_my"] = id_team_my;
    StartConnect("hmshiai_vshumplay");
  }
  void TPostHmShiai::Ready_vshumretire(s32 errcode, s32 ng_c, s32 mybamotion, s32 enbamotion, s32 ctrl)
  {
    ClearMap();
    sendmap_.longmap_["ng_c"] = ng_c;
    sendmap_.longmap_["mybamotion"] = mybamotion;
    sendmap_.longmap_["enbamotion"] = enbamotion;
    sendmap_.longmap_["errcode"] = errcode;
    sendmap_.longmap_["ctrl"] = ctrl;
    StartConnect("hmshiai_vshumretire");
  }
  void TPostHmShiai::Ready_vshumresult(s64 vsresult, s64 kill, s64 death, s64 vstime, s64 maxdam, s64 totaldam, s32 errcode, s32 ctrl)
  {
    ClearMap();
    sendmap_.longmap_["kill"] = kill;
    sendmap_.longmap_["death"] = death;
    sendmap_.longmap_["vstime"] = vstime;
    sendmap_.longmap_["maxdam"] = maxdam;
    sendmap_.longmap_["totaldam"] = totaldam;
    sendmap_.longmap_["errcode"] = errcode;
    sendmap_.longmap_["ctrl"] = ctrl;
    StartConnect("hmshiai_vshumresult");
  }


  //疑似対人##########


  void TPostHmShiai::Ready_tourready(s64 mid_event)
  {
    ClearMap();
    sendmap_.longmap_["mid_event"] = mid_event;
    StartConnect("hmshiai_tourready");
  }
  void TPostHmShiai::Ready_tourplay(s64 id_team)
  {
    ClearMap();
    sendmap_.longmap_["id_team"] = id_team;
    StartConnect("hmshiai_tourplay");
  }
  void TPostHmShiai::Ready_tourretire()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("hmshiai_tourretire");
  }
  void TPostHmShiai::Ready_tourresult(s64 vsresult, s64 kill, s64 death, s64 vstime, s64 maxdam, s64 totaldam, s32 ctrl)
  {
    ClearMap();
    //sendmap_.longmap_["vsresult"] = vsresult;
    sendmap_.longmap_["kill"] = kill;
    sendmap_.longmap_["death"] = death;
    sendmap_.longmap_["vstime"] = vstime;
    sendmap_.longmap_["maxdam"] = maxdam;
    sendmap_.longmap_["totaldam"] = totaldam;
    sendmap_.longmap_["ctrl"] = ctrl;
    StartConnect("hmshiai_tourresult");
  }

  //タイムアタック##########


  void TPostHmShiai::Ready_tmatready(s64 mid_event)
  {
    ClearMap();
    sendmap_.longmap_["mid_event"] = mid_event;
    StartConnect("hmshiai_tmatready");
  }
  void TPostHmShiai::Ready_tmatplay(s64 id_team)
  {
    ClearMap();
    sendmap_.longmap_["id_team"] = id_team;
    StartConnect("hmshiai_tmatplay");
  }
  void TPostHmShiai::Ready_tmatretire()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("hmshiai_tmatretire");
  }
  void TPostHmShiai::Ready_tmatresult(s64 vsresult, s64 kill, s64 death, s64 vstime, s64 maxdam, s64 totaldam, s32 ctrl)
  {
    ClearMap();
    //sendmap_.longmap_["vsresult"] = vsresult;
    sendmap_.longmap_["kill"] = kill;
    sendmap_.longmap_["death"] = death;
    sendmap_.longmap_["vstime"] = vstime;
    sendmap_.longmap_["maxdam"] = maxdam;
    sendmap_.longmap_["totaldam"] = totaldam;
    sendmap_.longmap_["ctrl"] = ctrl;
    StartConnect("hmshiai_tmatresult");
  }

  void TPostHmShiai::Ready_refreshevent()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("hmshiai_refreshevent");
  }
}
