#pragma once
#include "dbLyHmGen.h"
#include "dbUGSpMami.h"
#include "dbUGButton.h"
#include "dbUGLiMenu.h"
#include "dbUGLiGiftBox.h"
#include "dbUGBtnEvent.h"
#include "dbUGScVSTeam.h"
#include "dbUGDrTeam.h"
#include "dbUGPlateProf.h"
#include "dbUGRMessage.h"
#include "dbUGLiRanking.h"
#include "dbUGLiRanker.h"
#include "dbUGLiTmFilter.h"
#include "dbUGVS.h"
#include "dbUGTitle.h"
#include "dbUGLiShSetSelectAll.h"
#include "dbUGLiShSetSelect.h"
#include "dbUGLiMember.h"
#include "dbUGReward.h"
#include "dbUGLiRkReward.h"
#include "dbUGBgmList.h"
#include "dbUGTokkouInfo.h"
#include "dbUGHaikeiList.h"

namespace db
{

  struct TEventSt
  {
    s64 id_team_dr_;
    s64 mid_team_dr_;

    s64 id_kantoku_en_;
    s64 id_team_en_;
    s64 mid_team_en_;
    s32 mid_court_en_;
    s32 mid_bgm_en_;

    s64 id_shset_;
    s64 id_shset_first_;
    BOOL shsetmem_f_;
    s64 id_team_my_;
    s32 mid_ranking_;
    s32 rankingpage_;

    enTeamFilType filtype_;
    enTeamSrtType srttype_;

    s32 mgknum_;

    s32 cursorDispTimer_;
    BOOL isCheckStamina_;
    BOOL isMGKBtn_;

    BOOL turnbar_f_;
    s32 helpType_;
    s32 helpNo_;
    void Init()
    {
      Zeromem(this);
    }
    void InitEvent()
    {
      id_team_my_ = 0;
      id_team_en_ = 0;
      id_kantoku_en_ = 0;
    }
  };

  class TLyHmEvEvent :public TLyHmGen
  {
  public:
    //コンストラクタ
    explicit TLyHmEvEvent(TSnHome* pScene);
    virtual ~TLyHmEvEvent(void);

    BOOL IsGotoVS();
    void SetVS();
  protected:
    TUGBtnEvent ugevbtn_;
    TUGPlateProf ugprof_;
    TUGRMessage ugrmsg_ev_;
    TUGLiRanking uglistRankMenu_;
    TUGLiRanker uglistRanker_;
    TUGLiRanker uglistMyRank_;
    TUGLiTmFilter uglistTmFilter_;
    TUGDrTeam ugdrTeam_;
    TUGTeam ugTeam_;//決定済みカード
    std::vector<TUGScVSTeam*> ugvsteam_;//チーム
    TUGTeam ugencard_;//敵カード
    TUGVS ugvs_;//VSマーク
    TUGTitle ugtitle_me_;
    TUGTitle ugtitle_en_;
    TUIObj* pCursor_;
    std::vector<TUGLiGen*> listvec_;
    SORTTEAMVECTOR teamvec_;
    TEventSt stEvent_;

    TUGLiShSetSelectAll uglistShSetSelectAll_;
    TUGLiShSetSelect uglistShSetSelect_;
    TUGLiMember uglistMember_;//メンバーリスト
    LONGVECTOR shsetvec_;

	// 報酬表示用
	s64 mid_Ranking_;
	s32 prevPage_;
	s64 nowRank_;
	s64 nowRewardRank_;
	s64 nextRewardRank_;
	TUGReward ugNowReward_;
	TUGReward ugNextReward_;
	TUGReward ugRkReward_;	// 実際フェードとタイトルバーしか使わないからバカらしいけど時間がないのでとりあえず
	TUGLiRkReward ugLiRkReward_;

	// シュートプレビュー用
	s64 mid_sozai_;
	INTVECTOR syouhinVec_;
	SOZAIDTVECTOR prevSozaiVec_;

	TUGTokkouInfo ugTokkouInfo_;
	TUGBgmList ugTokkouBGM_;
	TUGHaikeiList ugTokkouHaikei_;

    void DrawList(TUGLiGen* drawlist);
    void ResetObj();

    void SetEtcSS();
    void LoadTeamEnemy(s64 id_kantoku, s32 mid_team);
    void UpdateCheckStamina(void);	// スタミナチェック更新
    void UpdateCursor(BOOL draw_f);		// スタミナ回復先導カーソル更新
    void SwitchRankerPageBtn();
    //void LoadLookPosTeam();
    void MvPage_Visit();
    void MvPage_Menu();
    void MvPage_Help();
    void MvPage_Ranker();
    void MvPage_MyRank();
    void MvPage_TourReady();
    void MvPage_Match();
    void MvPage_TmSelect();
    void MvPage_ShSetSelect();
    void MvPage_TmFilter();
    void MvPage_MGK();
    void MvPage_Nige();
    void MvPage_GotoVS();
    void MvPage_VS();
	void MvPage_TimeAttackReady(void);
	void MvPage_TimeAttackMatch(void);
	void MvPage_TimeAttackNige(void);

	void MvPage_Reward(void);
	void MvPage_NowReward(void);
	void MvPage_NextReward(void);
	void MvPage_SelectReward(void);
	void MvPage_PreviewReward(void);

	void MvPage_TokkouInfo(void);
	void MvPage_TokkouBGM(void);
	void MvPage_TokkouHaikei(void);

    void MvPage_Out();
    void MvPage_GotoShiai();
    void MvPage_Error();

    //通信
    void Post_TourReady();
    void Post_TourPlay();
    void Post_TourRetire();
    void Post_Ranker();
    void Post_RankerPage();
    void Post_MyRank();
    void Post_MGK();
	void Post_SwitchShSet();
	void Post_TimeAttackReady(void);
	void Post_TimeAttackPlay(void);
	void Post_TimeAttackRetire(void);
  void Post_RewardRanker();

    void Connect_TourReady();
    void Connect_TourPlay();
    void Connect_TourRetire();
    void Connect_Ranker();
    void Connect_RankerPage();
    void Connect_MyRank();
    void Connect_MGK();
    void Connect_SwitchShSet();
	void Connect_TimeAttackReady(void);
	void Connect_TimeAttackPlay(void);
	void Connect_TimeAttackRetire(void);
  void Connect_RewardRanker();

  //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
    void LoadTeamTexture(s32 sideno, s32 mid_team);
    void RefreshTeamTexture();
  };
 
}
