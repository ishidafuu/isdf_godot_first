#include "dbPoHome.h"

namespace db
{
  TPostHome::TPostHome(TMgEngineObj* pmgEO):TPoGen(pmgEO)
  {
  }

  TPostHome::~TPostHome(void)
  {
  }
  //送信パック初期化
  void TPostHome::Ready_home()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("hm_home");
  }

  //void TPostHome::Ready_logbo()
  //{
  //  ClearMap();
  //  sendmap_.longmap_["dummy"] = 0;
  //  StartConnect("hm_logbo");
  //}
  void TPostHome::Ready_logbo(BOOL cancel_f, s32 mid_logbosozai, INTVECTOR* mid_spsyouhinvec)
  {
    ClearMap();
    sendmap_.longmap_["logboshot"] = (cancel_f)
      ? 2
      : 1;
    sendmap_.longmap_["mid_logbosozai"] = mid_logbosozai;
    for (s32 i = 0; i < mid_spsyouhinvec->size(); ++i)
    {
      std::string outid = "mid_spsyouhin" + lib_str::IntToStr1b(i, 0);
      sendmap_.longmap_[outid.c_str()] = mid_spsyouhinvec->at(i);
    }
    StartConnect("hm_logbo");
  }
  void TPostHome::Ready_ggz()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("hm_ggz");
  }
  void TPostHome::Ready_ggzquick()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("hm_ggzquick");
  }
  //void TPostHome::Ready_frcode(std::string frcode)
  //{
  //  ClearMap();
  //  sendmap_.strmap_["frcode"] = frcode;//文字列
  //  StartConnect("hm_frcode");
  //}

  //void TPostHome::Ready_frcodedecide(std::string frcode)
  //{
  //  ClearMap();
  //  sendmap_.strmap_["frcode"] = frcode;//文字列
  //  StartConnect("hm_frcodedecide");
  //}
  void TPostHome::Ready_cpcode(s64 id_m_cpcode, std::string cpcode)
  {
    ClearMap();
    sendmap_.longmap_["id_m_cpcode"] = id_m_cpcode;
    sendmap_.strmap_["cpcode"] = cpcode;//文字列
    StartConnect("hm_cpcode");
  }
  void TPostHome::Ready_checkdraw(BOOL checkmami_f, BOOL checkmaruka_f)
  {
    ClearMap();
    sendmap_.longmap_["checkmami_f"] = (checkmami_f) ? 1 : 0;
    sendmap_.longmap_["checkmaruka_f"] = (checkmaruka_f) ? 1 : 0;
    StartConnect("hm_checkdraw");
  }
  void TPostHome::Ready_checkgiftbox()
  {
    ClearMap();
    sendmap_.longmap_["dummy"] = 0;
    StartConnect("hm_checkgiftbox");
  }
  void TPostHome::Ready_opengiftbox(LONGVECTOR* openidvec)
  {
    ClearMap();
    for (s32 i = 0; i < openidvec->size(); ++i)
    {
      std::string openid = "id_giftbox" + lib_str::IntToStr1b(i, 0);
      sendmap_.longmap_[openid.c_str()] = openidvec->at(i);
    }
    StartConnect("hm_opengiftbox");
  }

  void TPostHome::Ready_getitemlog(s32 page)
  {
    ClearMap();
    sendmap_.longmap_["page"] = page;
    StartConnect("hm_getitemlog");
  }
  void TPostHome::Ready_useitemlog(s32 page)
  {
    ClearMap();
    sendmap_.longmap_["page"] = page;
    StartConnect("hm_useitemlog");
  }

  void TPostHome::Ready_buystarlog(s32 page)
  {
    ClearMap();
    sendmap_.longmap_["page"] = page;
    StartConnect("hm_buystarlog");
  }
  void TPostHome::Ready_usestarlog(s32 page, BOOL bstar_f, BOOL fstar_f)
  {
    ClearMap();
    sendmap_.longmap_["page"] = page;
    sendmap_.longmap_["bstar_f"] = (bstar_f) ? 1 : 0;
    sendmap_.longmap_["fstar_f"] = (fstar_f) ? 1 : 0;
    StartConnect("hm_usestarlog");
  }
  void TPostHome::Ready_name(std::string name)
  {
    ClearMap();
    sendmap_.strmap_["name"] = name;//文字列
    StartConnect("hm_name");
  }
  void TPostHome::Ready_prof(s32 line, std::string prof)
  {
    ClearMap();
    sendmap_.longmap_["line"] = line;//文字列
    sendmap_.strmap_["prof"] = prof;//文字列
    StartConnect("hm_prof");
  }
  void TPostHome::Ready_avatar(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 clothecol)
  {
    ClearMap();
    sendmap_.longmap_["hair"] = hair;
    sendmap_.longmap_["face"] = face;
    sendmap_.longmap_["haircol"] = haircol;
    sendmap_.longmap_["skincol"] = skincol;
    sendmap_.longmap_["eyecol"] = eyecol;
    sendmap_.longmap_["acccol"] = acccol;
    sendmap_.longmap_["clothecol"] = clothecol;
    StartConnect("hm_avatar");
  }

  void TPostHome::Ready_ranker(s32 mid_ranking, BOOL result_f)
  {
    ClearMap();
    sendmap_.longmap_["mid_ranking"] = mid_ranking;
    sendmap_.longmap_["result_f"] = (result_f) ? 1 : 0;
    StartConnect("hm_ranker");
  }

  void TPostHome::Ready_rankerpage(s32 mid_ranking, BOOL result_f, s32 page)
  {
    ClearMap();
    sendmap_.longmap_["mid_ranking"] = mid_ranking;
    sendmap_.longmap_["page"] = page;
    sendmap_.longmap_["result_f"] = (result_f) ? 1 : 0;
    StartConnect("hm_rankerpage");
  }


  void TPostHome::Ready_myrank(s32 mid_ranking, BOOL result_f)
  {
    ClearMap();
    sendmap_.longmap_["mid_ranking"] = mid_ranking;
    sendmap_.longmap_["result_f"] = (result_f) ? 1 : 0;
    StartConnect("hm_myrank");
  }


  void TPostHome::Ready_training(s32 mid_supply, s64 mid_team, s32 posNo)
  {
	  ClearMap();
	  sendmap_.longmap_["mid_supply"]	= mid_supply;
	  sendmap_.longmap_["mid_team"]		= mid_team;
	  sendmap_.longmap_["id_charpos"]	= posNo;
	  StartConnect("hm_training");
  }
  void TPostHome::Ready_sellteam(s64 mid_team)
  {
	  ClearMap();
      sendmap_.longmap_["mid_team"] = mid_team;
	  StartConnect("hm_sellteam");
  }
}
