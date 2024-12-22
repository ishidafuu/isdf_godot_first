#pragma once
#include "dbLyHmGen.h"
#include "dbUGLiEvent.h"
#include "dbUGBtnShiai.h"
#include "dbUGBtnVS.h"
#include "dbUGLock.h"
#include "dbUGRMessage.h"

namespace db
{
  class TLyHmShiai :public TLyHmGen
  {
  protected:
    TUGBtnShiai ugshbtn_scenario_;//シナリオ
    TUGBtnVS ugshbtn_vs_;//対戦
	TUGRMessage ugrmsg_scenario_;
	TUGRMessage ugrmsg_vs_;
    LONGVECTOR shsetvec_;
    std::vector<TUGLock*> uglock_;

	void MvPage_Tuto(void);

  BOOL IsSubEventOpen();

  void MvPage_Main();
    void MvPage_GotoScenario();
    void MvPage_InVS();
    void MvPage_Idle();

    void Connect_Omakase();
    void Post_Omakase();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoUpdate(BOOL draw_f);
    void DoConnect(BOOL rqok_f);
  public:
    //コンストラクタ
    explicit TLyHmShiai(TSnHome* pScene);
    virtual ~TLyHmShiai(void);
    void Idle();

	void SetupShiai(s32 shiaiNo);

	void SetScenarioOnly(BOOL isOnly);
  };

}
