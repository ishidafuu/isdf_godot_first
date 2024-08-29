#pragma once
#include "dbLyHmGen.h"
#include "dbUGSpMami.h"
#include "dbUGButton.h"
#include "dbUGLiMenu.h"
#include "dbUGLiGiftBox.h"
#include "dbUGLiTmFilter.h"
#include "dbUGLiShSetSelectAll.h"
#include "dbUGLiShSetSelect.h"
#include "dbUGLiMember.h"
#include "dbUGBtnShiai.h"
#include "dbUGScVSTeam.h"
#include "dbUGDrTeam.h"
#include "dbUGPlateProf.h"
#include "dbUGRMessage.h"
#include "dbUGVS.h"
#include "dbUGTitle.h"
#include "dbUGLiVSHumReward.h"
namespace db
{
  struct TVSSt
  {
    BOOL btsv_f_;
    BOOL ready_f_en_;
    BOOL ready_f_my_;
    BOOL wifi_f_;
    enTeamFilType filtype_;
    enTeamSrtType srttype_;
    s32 discn_c_;
    s32 mid_bgm_en_;
    s32 mid_court_en_;
    s32 roomNo_;
    s32 roomtype_;
    s32 send_c_;
    s32 wait_c_;
    s64 id_kantoku_en_;
    s64 id_team_dr_;
    s64 id_team_en_;
    s64 id_team_my_;
    s64 mid_team_dr_;
    u64 roomid_;
    s64 id_shset_;
    s64 id_shset_first_;
    
    s32 senditv_;
    BOOL shsetmem_f_;
	s32 rewardRcNo_;

	s32 cursorDispTimer_;
	BOOL isNeedStamina_;
	BOOL isMGKBtn_;
	s32 mgknum_;

    void Init()
    {
      Zeromem(this);
    }
    void InitVS()
    {
      ready_f_my_ = FALSE;
      ready_f_en_ = FALSE;
      id_team_my_ = 0;
      id_team_en_ = 0;
      id_kantoku_en_ = 0;
      wifi_f_ = FALSE;
      roomtype_ = 0;
	  rewardRcNo_ = NGNUM;
    }
  };


  class TLyHmShVSHum :public TLyHmGen
  {
  protected:
    SORTTEAMVECTOR teamvec_;
    TUGBtnShiai ugshbtn_bt_;//通信
    TUGBtnShiai ugshbtn_wifi_;//通信
    TUGDrTeam ugdrTeam_;

	TUGBtnShiai ugbtn_bt_host_;		// ホスト
	TUGBtnShiai ugbtn_bt_client_;		// クライアント

	TUGBtnShiai ugbtn_wifi_free_;		// 野良部屋
	TUGBtnShiai ugbtn_wifi_select_;	// 指定部屋
      
#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
	TUGTitle ugtitle_reward_;
	TUGLiVSHumReward ugLiReward_;
	std::vector<TUGLiVSHumReward*> ugLiRewardInfo_;
#endif
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
	  BOOL isStaminaCheck_;
	  TUIObj* pCursor_;
#endif
	BOOL isSearchStart_;


	TUGLiMenu uglistBTRoom_;
    TUGLiMenu uglistWifiRoom_;
    TUGLiTmFilter uglistTmFilter_;
    TUGPlateProf ugprof_;
    TUGRMessage ugrmsg_bt_;
    TUGRMessage ugrmsg_wifi_;
    TUGTeam ugTeam_;//決定済みカード
    TUGTitle ugtitle_en_;
    TUGTitle ugtitle_me_;
    TUGVS ugvs_;//VSマーク
    TUIObj* waitTxt_;	// おまちくださいメッセージ
    std::vector<TUGLiGen*> listvec_;
    std::vector<TUGScVSTeam*> ugvsteam_;//チーム
    
    TUGLiShSetSelectAll uglistShSetSelectAll_;
    TUGLiShSetSelect uglistShSetSelect_;
    TUGLiMember uglistMember_;//メンバーリスト
    LONGVECTOR shsetvec_;

    void DrawList(TUGLiGen* drawlist);
    void ResetObj();

    void SetEtcSS();
    void RefreshEnemyTex();
    void LoadLookPosTeam();
    void Backto_Menu();

    void MvPage_Visit();
    void MvPage_Menu();
    void MvPage_WifiRoom();
#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
	void MvPage_WifiRoomReward();
#endif
	void MvPage_WifiReady(void);
	void MvPage_WifiReadyError(void);
    void MvPage_WifiRoomName();
    void MvPage_WifiSearch();
    void MvPage_BTRoom();
#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
	void MvPage_BTRoomReward();
#endif
	void MvPage_BTReady(void);
	void MvPage_BTReadyError(void);
    void MvPage_BTSearch();
    void MvPage_VSHumReady();
    void MvPage_Match();
    void MvPage_TmSelect();

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
	void MvPage_MGK();
#endif

    void MvPage_ShSetSelect();

    void DecideMyTeam();

    void MvPage_TmFilter();
    void MvPage_GotoVS();
    void MvPage_VS();
    void MvPage_Out();
    void MvPage_GotoShiai();
    void MvPage_Error();
    void MvPage_GKError();

    void MvPage_BT_Help(void);
    void MvPage_Wifi_Help(void);

    void SendKantokuData();

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
	BOOL CheckMyTmStamina(void);
	void CheckSelectTmStamina(void);
	void UpdateCursor(BOOL draw_f);
#endif

    void RcvData();

    //通信
    void Post_SwitchShSet();
    void Post_VSHumReady();
    void Post_VSHumPlay();
	void Post_WifiReady();
	void Post_BTReady();

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
	void Post_MGK();
#endif

    void Connect_SwitchShSet();
    void Connect_VSHumReady();
    void Connect_VSHumPlay();
	void Connect_WifiReady();
	void Connect_BTReady();

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
	void Connect_MGK();
#endif

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);

  public:
    TVSSt stVSHum_;
    //コンストラクタ
    explicit TLyHmShVSHum(TSnHome* pScene);
    virtual ~TLyHmShVSHum(void);

    BOOL IsGotoVS();
    void SetVS();
    BOOL Is30FPS();
  };

}
