#pragma once
#include "dbInclude.h"
namespace db
{
  enum enNSF
  {
    nsf_title = 17,
	nsf_tuto = 40,
    nsf_home = 174,
	nsf_shop = 156,
    nsf_shop_in = 46,
	nsf_shop_gatya = 108,
	nsf_shop_huku = 110,
    nsf_scenario = 207,
    nsf_nabox = 131,
    nsf_gacha = 33,
    nsf_ga_lv1_1 = 43,
    nsf_ga_lv1_2 = 99,
    nsf_ga_lv2_1 = 80,
    nsf_ga_lv2_2 = 49,
    nsf_ga_lv3_1 = 122,
    nsf_ga_lv3_2 = 29,
    nsf_ga_lv4_1 = 31,
    nsf_ga_lv4_2 = 54,
    nsf_jgl_ready = 20,
    nsf_jgl_win = 34,
    nsf_jgl_lose = 35,
	nsf_rank_up = 65,		// ランクアップ
	nsf_rank_down = 68,		// ランクダウン
	nsf_rank_same = 190,		// ランク変わらず
  };

  class TMgEngineObj;

  class TMgNSF
  {
  protected:
    TMgEngineObj* pmgEO_;
    s32 mid_bgm_now_;
    s32 mid_bgm_last_;
    void GenPlayID(s32 mid_bgm);
  public:
    
    explicit TMgNSF(TMgEngineObj* pmgEO);
    virtual ~TMgNSF(void);
    
    void PlayID(s32 mid_bgm);
    void PlayID_GetBGM(s32 mid_bgm);
    void Play(enNSF nsfno);
    void Pause();
    void Restart();
    void PlayLast();
    void StopBGM();
  };
}
