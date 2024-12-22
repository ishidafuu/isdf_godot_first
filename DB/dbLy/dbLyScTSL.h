#pragma once
#include "dbLyScGen.h"
#include "dbUGLiTmFilter.h"
#include "dbUGLiShSetSelectAll.h"
#include "dbUGLiShSetSelect.h"
#include "dbUGLiMember.h"
#include "dbUGTitle.h"
#include "dbUGBtnCourt.h"
namespace db
{

  struct TTSLSt
  {
    s64 id_map_;
    s32 enemyno_;
	s64 id_kantoku_en_;
    s32 mid_team_en_;

    s32 mgknum_;
	s64 id_kantoku_my_;
    s64 id_team_;
    s32 mid_team_;
    s64 id_shset_;
    s64 id_shset_first_;

    BOOL shsetmem_f_;

    //BOOL homecourt_f_;

    enTeamFilType filtype_;
    enTeamSrtType srttype_;

    s32 cursorDispTimer_;
    BOOL isCheckStamina_;
    BOOL isMGKBtn_;
    s64 id_prev_team_;
    s64 mid_prev_team_;

    //BOOL enablehome_f_;

    BOOL turnbar_f_;
    void Init()
    {
      Zeromem(this);
      id_team_ = NGNUM;
    }
  };


  //スクリプト管理////////////////////////////////
  class TLyScTSL :public TLyScGen
  {
  public:
    //コンストラクタ
    explicit TLyScTSL(TSnScenario* pScene);
    virtual ~TLyScTSL(void);
    void RetrunHedder();
  protected:
    void MvPage_TSL();
    void MvPage_TmFilter();
    void MvPage_Nige();
    void MvPage_MGK();
    void MvPage_ShSetSelect();
    void MvPage_GotoVS();
    void LoadLookPosTeam();
    void LoadTeamEnemy(s64 id_kantoku, s32 mid_team);

    void Post_Play();
    void Post_MGK();
    void Post_SwitchShSet();
    void Connect_Play();
    void Connect_MGK();
    void Connect_SwitchShSet();
    //オーバーロード
    void DoLoad();
    void DoResetBtn();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);

    void UpdateCheckStamina(void);	// スタミナチェック更新
    void UpdateCursor(BOOL draw_f);		// スタミナ回復先導カーソル更新
    void SetEtcSS();
    void LoadTeamTexture(s32 sideno, s32 mid_team);
    void RefreshTeamTexture();

    LONGVECTOR shsetvec_;
    SORTTEAMVECTOR teamvec_;
    TTSLSt stTSL_;
    //TUGBtnCourt ugbtn_court_;
    TUGDrTeam ugdrTeam_;
    TUGLiShSetSelectAll uglistShSetSelectAll_;
    TUGLiShSetSelect uglistShSetSelect_;
    TUGLiMember uglistMember_;//メンバーリスト
    TUGLiTmFilter uglistTmFilter_;
    TUGTeam ugencard_;//敵カード
    TUGTitle ugtitle_en_;
    TUGTitle ugtitle_me_;
    TUIObj* filter_;
    TUIObj* pCursor_;
  };

}
