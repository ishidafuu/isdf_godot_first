#pragma once
#include "dbLyShGen.h"
#include "dbUGShResult.h"

namespace db
{
  class TLyShResult :public TLyShGen
  {
  protected:
    TUIObj* filter_;
    //TUGButton ugbtn_back_;
    TUGShResult ugresult_;

	s64 mid_team_drop_;

    void SwitchPostResult();
    void MvPage_Result();
    void MvPage_Out();

    void Post_Result();
    void Post_VSHumResult();
    void Post_TourResult();
    void Post_TutoResult(void);
	void Post_TimeAttackResult(void);

    void Connect_Result();
    void Connect_VSHumResult();
    void Connect_TourResult();
    void Connect_FirstTutoResult(void);
	void Connect_TimeAttackResult(void);
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyShResult(TSnShiai* pScene);
    virtual ~TLyShResult(void);

  };

}
