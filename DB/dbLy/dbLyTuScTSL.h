#pragma once
#include "dbLyScGen.h"
#include "dbUGLiTmFilter.h"
#include "dbUGTitle.h"

namespace db
{
  class TUGSerifu;
  class TUGFinger;

  struct TTuTSLSt
  {
    s64 id_map_;
    s32 enemyno_;
	s64 id_kantoku_en_;
    s32 mid_team_en_;

    s32 mgknum_;
	s64 id_kantoku_my_;
    s32 id_team_;
    s32 mid_team_;
    BOOL homecourt_f_;
    enTeamFilType filtype_;
    enTeamSrtType srttype_;

    void Init()
    {
      Zeromem(this);
      id_team_ = NGNUM;
    }
  };


  class TLyTuScTSL :public TLyScGen
  {
  public:
    //コンストラクタ
    explicit TLyTuScTSL(TSnScenario* pScene);
    virtual ~TLyTuScTSL(void);
  protected:
    //INTVECTOR testdt_;
    TTuTSLSt stTSL_;
    TUIObj* filter_;
    TUGDrTeam ugdrTeam_;
    TUGTeam ugencard_;//敵カード
    TUGTitle ugtitle_me_;
    TUGTitle ugtitle_en_;
    TUGLiTmFilter uglistTmFilter_;

    SORTTEAMVECTOR teamvec_;

    s64 id_prev_team_;
    s64 mid_prev_team_;

    TUGSerifu* pUGSerifu_;
    TUGFinger* pUGFinger_;
    TUGFinger* pUGFinger2_;
    TUGFinger* pUGFinger3_;
    STRVECTOR* pSerifuVec_;
    s32 tuTxtNo_;

    void SetMsg(s32 msgNo);
    void SetMsg(const char* text0, const char* text1, const char* text2);

    void MvPage_TutoStamina(void);
    void MvPage_TutoStart(void);

    void MvPage_TSL();
    void MvPage_GotoVS();
    void LoadLookPosTeam();
    void LoadTeamEnemy(s32 mid_team);

    void Post_Play();
    void Connect_Play();
    //オーバーロード
    void DoLoad();
    void DoResetBtn();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
    void SetEtcSS();

    //#define __DEBUG_POSITION_CHECK__
#ifdef __DEBUG_POSITION_CHECK__
    void debug();
#endif


  };

}
