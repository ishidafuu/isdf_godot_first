#include "dbStSnShiai.h"

namespace db
{



  //Etcステート******************************************************
  TStSnShiai::TStSnShiai()
  {
    Init();
  }

  TStSnShiai::~TStSnShiai()
  {
  }
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
  void TStSnShiai::SetGenShiai(s64 id_kantoku_0, s64 id_kantoku_1, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, BOOL isCOM_0, BOOL isCOM_1, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 shiaiSec, s32 ctrl)
#else
  void TStSnShiai::SetGenShiai(s64 id_kantoku_0, s64 id_kantoku_1, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, BOOL isCOM_0, BOOL isCOM_1, s32 mid_court, s32 mid_bgm, s32 shiaiSec, s32 ctrl)
#endif
  {
    Init();
	id_kantoku_[SIDE0] = id_kantoku_0;
	id_kantoku_[SIDE1] = id_kantoku_1;
    id_team_[SIDE0] = id_team_0;
    id_team_[SIDE1] = id_team_1;
    mid_team_[SIDE0] = mid_team_0;
    mid_team_[SIDE1] = mid_team_1;
	isCOM_[SIDE0] = isCOM_0;
	isCOM_[SIDE1] = isCOM_1;
    mid_court_ = mid_court;
    mid_bgm_ = mid_bgm;
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	mid_bgm_ready_ = mid_bgm_ready;
	mid_bgm_win_ = mid_bgm_win;
	mid_bgm_lose_ = mid_bgm_lose;
#endif
    shiaiSec_ = 240;
    shiaiSec_ = shiaiSec;
    tokkouMag_ = 100;
    ctrl_ = ctrl;
  }

#ifndef __K_MAKE_HIDE_EVENT_SOUND__
  void TStSnShiai::SetVSHum(s32 myside, s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_my, s64 id_team_en, s32 mid_team_my, s32 mid_team_en, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 ctrl, s32 shiaiSec, BOOL fps30_f, BOOL wifi_f)
#else
  void TStSnShiai::SetVSHum(s32 myside, s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_my, s64 id_team_en, s32 mid_team_my, s32 mid_team_en, s32 mid_court, s32 mid_bgm, s32 ctrl, s32 shiaiSec, BOOL fps30_f, BOOL wifi_f)
#endif
  {
	s32 id_kantoku_0 = (myside == SIDE0)
		? id_kantoku_my
		: id_kantoku_en;
	s32 id_kantoku_1 = (myside == SIDE1)
		? id_kantoku_my
		: id_kantoku_en;
	s32 id_team_0 = (myside == SIDE0)
      ? id_team_my
      : id_team_en;
    s32 id_team_1 = (myside == SIDE1)
      ? id_team_my
      : id_team_en;
    s32 mid_team_0 = (myside == SIDE0)
      ? mid_team_my
      : mid_team_en;
    s32 mid_team_1 = (myside == SIDE1)
      ? mid_team_my
      : mid_team_en;

#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	SetGenShiai(id_kantoku_0, id_kantoku_1, id_team_0, id_team_1, mid_team_0, mid_team_1, FALSE, FALSE, mid_court, mid_bgm, mid_bgm_ready, mid_bgm_win, mid_bgm_lose, shiaiSec, ctrl);
#else
	SetGenShiai(id_kantoku_0, id_kantoku_1, id_team_0, id_team_1, mid_team_0, mid_team_1, FALSE, FALSE, mid_court, mid_bgm, shiaiSec, ctrl);
#endif
    myside_ = myside;
    shiaitype_ = (wifi_f)
      ? st_VSWiFi
      : st_VSBT;
    fps30_f_ = fps30_f;
  }


#ifndef __K_MAKE_HIDE_EVENT_SOUND__
  void TStSnShiai::SetTour(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper)
  {
	  SetGenShiai(id_kantoku_my, id_kantoku_en, id_team_0, id_team_1, mid_team_0, mid_team_1, FALSE, FALSE, mid_court, mid_bgm, mid_bgm_ready, mid_bgm_win, mid_bgm_lose, shiaiSec, ctrl);
#else
  void TStSnShiai::SetTour(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, s32 mid_court, s32 mid_bgm, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper)
  {
	  SetGenShiai(id_kantoku_my, id_kantoku_en, id_team_0, id_team_1, mid_team_0, mid_team_1, FALSE, FALSE, mid_court, mid_bgm, shiaiSec, ctrl);
#endif
    caper_ = caper;
    ncaper_ = ncaper;
    shiaitype_ = st_Tour;
    //通信関係クリア
    mid::midInitConnect();
  }

#ifndef __K_MAKE_HIDE_EVENT_SOUND__
  void TStSnShiai::SetShiai(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s32 mid_team_0, s32 mid_team_1, BOOL isCOM_0, BOOL isCOM_1, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 tokkouMag, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper, BOOL tuto_f)
  {
	  SetGenShiai(id_kantoku_my, NGNUM, id_team_0, 0, mid_team_0, mid_team_1, isCOM_0, isCOM_1, mid_court, mid_bgm, mid_bgm_ready, mid_bgm_win, mid_bgm_lose, shiaiSec, ctrl);
#else
  void TStSnShiai::SetShiai(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s32 mid_team_0, s32 mid_team_1, BOOL isCOM_0, BOOL isCOM_1, s32 mid_court, s32 mid_bgm, s32 tokkouMag, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper, BOOL tuto_f)
  {
	SetGenShiai(id_kantoku_my, NGNUM, id_team_0, 0, mid_team_0, mid_team_1, isCOM_0, isCOM_1, mid_court, mid_bgm, shiaiSec, ctrl);
#endif
    caper_ = caper;
    ncaper_ = ncaper;
    tuto_f_ = tuto_f;
    shiaitype_ = st_Normal;
    tokkouMag_ = tokkouMag;
    //通信関係クリア
    mid::midInitConnect();
  }

#ifndef __K_MAKE_HIDE_EVENT_SOUND__
  void TStSnShiai::SetFirstTuto(s32 id_kantoku, s32 mid_team, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 ctrl)
  {
	  const s32 ENEMY_TEAM_MID_ID = 251;		// チュートリアルの対戦相手チーム(251:チュートリアーズ)
	  SetGenShiai(id_kantoku, NGNUM, 0, 0, mid_team, ENEMY_TEAM_MID_ID, FALSE, TRUE, mid_court, mid_bgm, mid_bgm_ready, mid_bgm_win, mid_bgm_lose, 999999, ctrl);
#else
  void TStSnShiai::SetFirstTuto(s32 id_kantoku, s32 mid_team, s32 mid_court, s32 mid_bgm, s32 ctrl)
  {
    const s32 ENEMY_TEAM_MID_ID = 251;		// チュートリアルの対戦相手チーム(251:チュートリアーズ)
	SetGenShiai(id_kantoku, NGNUM, 0, 0, mid_team, ENEMY_TEAM_MID_ID, FALSE, TRUE, mid_court, mid_bgm, 999999, ctrl);
#endif
    tuto_f_ = TRUE;
    //通信関係クリア
    mid::midInitConnect();
  }




  void TStSnShiai::SetPrev(s32 id_kantoku, s64 id_team, s32 mid_team, s32 id_shot_prev, s32 ctrl)
  {
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	SetGenShiai(id_kantoku, NGNUM, id_team, id_team, mid_team, mid_team, FALSE, TRUE, 0, 0, 0, 0, 0, 9999999, ctrl);
#else
    SetGenShiai(id_kantoku, NGNUM, id_team, id_team, mid_team, mid_team, FALSE, TRUE, 0, 0, 9999999, ctrl);
#endif
    id_shot_prev_ = id_shot_prev;
    shiaitype_ = st_Prev;
    //通信関係クリア
    mid::midInitConnect();
  }

  void TStSnShiai::SetTutoToNormal()
  {
    tuto_f_ = FALSE;
  }

#ifndef __K_MAKE_HIDE_EVENT_SOUND__
  void TStSnShiai::SetTimeAttack(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 tokkouMag, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper)
  {
	  SetGenShiai(id_kantoku_my, id_kantoku_en, id_team_0, id_team_1, mid_team_0, mid_team_1, FALSE, TRUE, mid_court, mid_bgm, mid_bgm_ready, mid_bgm_win, mid_bgm_lose, shiaiSec, ctrl);
#else
  void TStSnShiai::SetTimeAttack(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, s32 mid_court, s32 mid_bgm, s32 tokkouMag, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper)
  {
	  SetGenShiai(id_kantoku_my, id_kantoku_en, id_team_0, id_team_1, mid_team_0, mid_team_1, FALSE, TRUE, mid_court, mid_bgm, shiaiSec, ctrl);
#endif
	  caper_ = caper;
	  ncaper_ = ncaper;
	  shiaitype_ = st_TimeAttack;
	  tokkouMag_ = tokkouMag;
	  //通信関係クリア
	  mid::midInitConnect();
  }
}
