#pragma once
#include "dbStSnGen.h"

namespace db
{
  //Etcステート
  class TStSnShiai : TStSnGen
  {
  public:
	  enum enShiaiType
	  {
		  st_Normal,
		  st_Prev,
		  st_VSBT,
		  st_VSWiFi,
		  st_Tour,
		  st_TimeAttack,
	  };

    //コンストラクタ
    explicit TStSnShiai();
    virtual ~TStSnShiai();
    void Init(){ Zeromem(this); };
    BOOL IsNormal(){ return (shiaitype_ == st_Normal); };
    BOOL IsVSHum(){ return ((shiaitype_ == st_VSWiFi) || (shiaitype_ == st_VSBT)); };
    BOOL IsWifi(){ return (shiaitype_ == st_VSWiFi); };
    BOOL IsPrev(){ return (shiaitype_ == st_Prev); };
    BOOL IsTour(){ return (shiaitype_ == st_Tour); };
	BOOL IsTimeAttack(){ return (shiaitype_ == st_TimeAttack); }
	enShiaiType GetShiaiType(void){ return (enShiaiType)shiaitype_; }
    BOOL IsTuto(){ return tuto_f_; };
    BOOL IsFPS30(){ return fps30_f_; };

	s32 GetKantokuID(s32 sideNo){ return id_kantoku_[sideNo]; }
    s32 GetMidTeam(s32 sideNo){ return mid_team_[sideNo]; }
    s32 GetIDTeam(s32 sideNo){ return id_team_[sideNo]; }
	BOOL IsCOM(s32 sideNo) { return isCOM_[sideNo]; }
    s32 GetMidBGM(){ return mid_bgm_; };
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	s32 GetReadyMidBGM(){ return mid_bgm_ready_; }
	s32 GetWinMidBGM(){ return mid_bgm_win_; }
	s32 GetLoseMidBGM(){ return mid_bgm_lose_; }
#endif
    s32 GetMidCourt(){ return mid_court_; };
    s32 GetIDShotPrev(){ return id_shot_prev_; };
    s32 GetCaPer(){ return caper_; };
    s32 GetNCaPer(){ return ncaper_; };
    s32 GetShiaiSec(){ return shiaiSec_; };
    s32 GetMySide(){ return myside_; };
    s32 GetTokkouMag(){ return tokkouMag_; };

#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	void SetVSHum(s32 myside, s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_my, s64 id_team_en, s32 mid_team_my, s32 mid_team_en, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 ctrl, s32 shiaiSec, BOOL fps30_f, BOOL wifi_f);
	void SetTour(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper);
	void SetShiai(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s32 mid_team_0, s32 mid_team_1, BOOL isCOM_0, BOOL isCOM_1, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 tokkouMag, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper, BOOL tuto_f);
	void SetFirstTuto(s32 id_kantoku, s32 mid_team, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 ctrl);
	void SetTimeAttack(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 tokkouMag, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper);
#else
    void SetVSHum(s32 myside, s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_my, s64 id_team_en, s32 mid_team_my, s32 mid_team_en, s32 mid_court, s32 mid_bgm, s32 ctrl, s32 shiaiSec, BOOL fps30_f, BOOL wifi_f);
	void SetTour(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, s32 mid_court, s32 mid_bgm, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper);
	void SetShiai(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s32 mid_team_0, s32 mid_team_1, BOOL isCOM_0, BOOL isCOM_1, s32 mid_court, s32 mid_bgm, s32 tokkouMag, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper, BOOL tuto_f);
    void SetFirstTuto(s32 id_kantoku, s32 mid_team, s32 mid_court, s32 mid_bgm, s32 ctrl);
	void SetTimeAttack(s64 id_kantoku_my, s64 id_kantoku_en, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, s32 mid_court, s32 mid_bgm, s32 tokkouMag, s32 ctrl, s32 shiaiSec, s32 caper, s32 ncaper);
#endif
    void SetPrev(s32 id_kantoku, s64 id_team, s32 mid_team, s32 id_shot_prev, s32 ctrl);
    void SetTutoToNormal();
    s32 GetCtrl(){ return ctrl_; };
    void SetCtrl(s32 ctrl){ ctrl_ = ctrl; };

	void SetDisConnect(BOOL disConnect){ isDisConnect_ = disConnect; }
	BOOL IsDisConnect(void){ return isDisConnect_; }

  protected:
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	  void SetGenShiai(s64 id_kantoku_0, s64 id_kantoku_1, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, BOOL isCOM_0, BOOL isCOM_1, s32 mid_court, s32 mid_bgm, s32 mid_bgm_ready, s32 mid_bgm_win, s32 mid_bgm_lose, s32 shiaiSec, s32 ctrl);
#else
	  void SetGenShiai(s64 id_kantoku_0, s64 id_kantoku_1, s64 id_team_0, s64 id_team_1, s32 mid_team_0, s32 mid_team_1, BOOL isCOM_0, BOOL isCOM_1, s32 mid_court, s32 mid_bgm, s32 shiaiSec, s32 ctrl);//初期化兼ねてる
#endif
    s32 shiaitype_;
	s64 id_kantoku_[DBSIDE];
    s64 id_team_[DBSIDE];
    s32 mid_team_[DBSIDE];
	BOOL isCOM_[DBSIDE];
    s32 mid_court_;
    s32 mid_bgm_;
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	s32 mid_bgm_ready_;
	s32 mid_bgm_win_;
	s32 mid_bgm_lose_;
#endif
    s32 shiaiSec_;
    s32 caper_;//シナリオ敵キャッチ確率
    s32 ncaper_;//シナリオ敵ナイスキャッチ確率
    BOOL fps30_f_;
    BOOL tuto_f_;
    s64 id_shot_prev_;
    s32 myside_;
    s32 tokkouMag_;
    s32 ctrl_;
	BOOL isDisConnect_;
  };

}
