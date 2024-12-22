#include "dbPoHmTeam.h"

namespace db
{
  TPostHmTeam::TPostHmTeam(TMgEngineObj* pmgEO) :TPoGen(pmgEO)
  {
  }

  TPostHmTeam::~TPostHmTeam(void)
  {
  }
  //送信パック初期化
  void TPostHmTeam::Ready_mgk(s64 id_team)
  {
    ClearMap();
    sendmap_.longmap_["id_team"] = id_team;
    StartConnect("hmteam_mgk");
  }
  void TPostHmTeam::Ready_mgkquick(s64 id_team)
  {
    ClearMap();
    sendmap_.longmap_["id_team"] = id_team;
    StartConnect("hmteam_mgkquick");
  }
  void TPostHmTeam::Ready_attachcustom(s64 id_team, LONGVECTOR* id_shot_vec)
  {
    ClearMap();
    sendmap_.longmap_["id_team"] = id_team;
    for (s32 i = 0; i < id_shot_vec->size(); ++i)
    {
      std::string id_shot = "id_shot" + lib_str::IntToStr1b(i, 2);//2keta
      sendmap_.longmap_[id_shot.c_str()] = id_shot_vec->at(i);
    }
    StartConnect("hmteam_attachcustom");
  }
  void TPostHmTeam::Ready_attachshset(s64 id_shset, LONGVECTOR* id_shot_vec)
  {
    ClearMap();
    sendmap_.longmap_["id_shset"] = id_shset;
    for (s32 i = 0; i < id_shot_vec->size(); ++i)
    {
      std::string id_shot = "id_shot" + lib_str::IntToStr1b(i, 2);//2keta
      sendmap_.longmap_[id_shot.c_str()] = id_shot_vec->at(i);
    }
    StartConnect("hmteam_attachshset");
  }
  void TPostHmTeam::Ready_switchshset(LONGVECTOR* id_team_vec, LONGVECTOR* id_shset_vec)
  {
    ClearMap();

    for (s32 i = 0; i < id_team_vec->size(); ++i)
    {
      std::string id_team = "id_team" + lib_str::IntToStr1b(i, 2);//2keta
      std::string id_shset = "id_shset" + lib_str::IntToStr1b(i, 2);//2keta
      sendmap_.longmap_[id_team.c_str()] = id_team_vec->at(i);
      sendmap_.longmap_[id_shset.c_str()] = id_shset_vec->at(i);
    }
    StartConnect("hmteam_switchshset");
  }
  void TPostHmTeam::Ready_switchshset(s64 id_team, s64 id_shset)
  {
    ClearMap();
    sendmap_.longmap_["id_team00"] = id_team;
    sendmap_.longmap_["id_shset00"] = id_shset;
    
    StartConnect("hmteam_switchshset");
  }
  void TPostHmTeam::Ready_setbgm(s64 mid_bgm)
  {
    ClearMap();
    sendmap_.longmap_["mid_bgm"] = mid_bgm;
    StartConnect("hmteam_setbgm");
  }
  void TPostHmTeam::Ready_sethaikei(s64 mid_haikei)
  {
    ClearMap();
    sendmap_.longmap_["mid_haikei"] = mid_haikei;
    StartConnect("hmteam_sethaikei");
  }
  void TPostHmTeam::Ready_okiniteam(s64 id_team)
  {
    ClearMap();
    sendmap_.longmap_["id_team"] = id_team;
    StartConnect("hmteam_okiniteam");
  }
  void TPostHmTeam::Ready_renameshset(s64 id_shset, std::string shsetname)
  {
    ClearMap();
    sendmap_.longmap_["id_shset"] = id_shset;
    sendmap_.strmap_["shsetname"] = shsetname;//文字列
    StartConnect("hmteam_renameshset");
  }
  void TPostHmTeam::Ready_renameshot(s64 id_shot, std::string shname)
  {
    ClearMap();
    sendmap_.longmap_["id_shot"] = id_shot;
    sendmap_.strmap_["shname"] = shname;//文字列
    StartConnect("hmteam_shrename");
  }
  void TPostHmTeam::Ready_shlock(LONGVECTOR* id_shot_vec)
  {
    ClearMap();
    for (s32 i = 0; i < id_shot_vec->size(); ++i)
    {
      std::string id_shot = "id_shot" + lib_str::IntToStr1b(i, 2);//2keta
      sendmap_.longmap_[id_shot.c_str()] = id_shot_vec->at(i);
    }
    StartConnect("hmteam_shlock");
  }

}
