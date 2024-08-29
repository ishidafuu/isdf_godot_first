#pragma once
#include "dbLyScGen.h"


namespace db
{
	class TUGSerifu;
	class TUGFinger;

  class TLyTuScEnemy :public TLyScGen
  {
  protected:
    TUIObj* filter_;
    std::vector<TUGScEnemy*> ugenteam_;//チーム
    s64 mid_maptip_;

	TUGSerifu* pUGSerifu_;
	TUGFinger* pUGFinger_;
	TUGFinger* pUGFinger2_;
	TUGFinger* pUGFinger3_;
	STRVECTOR* pSerifuVec_;
	s32 tuTxtNo_;

	void SetMsg(s32 msgNo);
	void SetMsg(const char* text0, const char* text1, const char* text2);

	void MvPage_TutoEnemyTxt(void);
	void MvPage_TutoItemTxt(void);
	void MvPage_TutoBattleBtnTxt(void);
	void MvPage_TutoHPTxt(void);
	void MvPage_TutoNeedHPTxt(void);

    void MvPage_Enemy();
    void MvPage_GotoTSL();
    void MvPage_Idle();

    void Post_Ready();
    void Connect_Ready();

    //オーバーロード
    void DoLoad();
    void DoResetBtn();
	void DoReady(void);
    void DoConnect(BOOL rqok_f);
	void DoChangePage(s32 pageno);
    void DoUpdate(BOOL draw_f);

  public:
    //コンストラクタ
    explicit TLyTuScEnemy(TSnScenario* pScene);
    virtual ~TLyTuScEnemy(void);

    void RedayEnemy(s64 mid_maptip, s32 lno_maptip);
  };
 
}
