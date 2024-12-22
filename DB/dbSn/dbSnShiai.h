#pragma once
#include "dbScene.h"
#include "dbLyShiai.h"
#include "dbLyShiaiTutoSmart.h"
#include "dbLyShiaiTutoRetro.h"
#include "dbLyShiaiTutoAuto.h"
#include "dbLyShiaiVS.h"
#include "dbLyShPause.h"
#include "dbLyShRetry.h"
#include "dbLyShResult.h"
#include "dbLyShVSError.h"
#include "dbLyShBar.h"
#include "dbLyShRank.h"

namespace db
{

  //タイトル///////////////////////////
  class TSnShiai : public TScene
  {
  public:
    TUIFrame* frame_;
    TLyShiaiGen* lyShiai_;//共通
    TLyShPause* lyShPause_;
    TLyShRetry* lyShRetry_;
    TLyShResult* lyShResult_;
    TLyShVSError* lyShVSError_;
    TLyShBar* lyShBar_;
	TLyShRank* lyShRank_;

    explicit TSnShiai(TGame* pGame, TMgUI* pMgUI, s32 myScene);
    virtual ~TSnShiai(void);

    void GoToMap();
    void GoToHome();
	void GoToEventHome(void);
	void GoToRanking();
  protected:
    //BOOL setlayer_f_;
    //BOOL goto_f_;
    //BOOL backhome_f_;
	enSceneName nextScene_;
	//BOOL tutorial_f_;
	BOOL isRanking_;

	void SetShiaiLayer();
	void SetRankingLayer();

    //継承
    virtual void DoSetLayer();
    virtual void DoFreeLayer();
    virtual void DoReady();
    virtual void DoUpdate();//継承先の更新内容はこれをオーバーロード
    virtual void DoDraw();//継承先の描画内容はこれをオーバーロード
    virtual void DoGotoTitleFilter();
	//void GoToTutorial(void);
  };
}
