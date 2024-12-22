#pragma once
#include "dbLyScGen.h"


namespace db
{

  class TLyScEnemy :public TLyScGen
  {
  public:
    //コンストラクタ
    explicit TLyScEnemy(TSnScenario* pScene);
    virtual ~TLyScEnemy(void);

    void RedayEnemy(s64 mid_maptip, s32 lno_maptip);
    void SetCheckHP(bool isSet);
  protected:
    //オーバーロード
    void DoLoad();
    void DoResetBtn();
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);

    void MvPage_Enemy();
    void MvPage_GotoTSL();
    void MvPage_Idle();
    void Post_Ready();
    void Connect_Ready();
    TUIObj* filter_;
    std::vector<TUGScEnemy*> ugenteam_;//チーム
    s64 mid_maptip_;
    bool isCheckHP_;
    TUIObj* pCursor_;
    s32 cursorDispTimer_;
    BOOL enablehome_f_;
  };

}
