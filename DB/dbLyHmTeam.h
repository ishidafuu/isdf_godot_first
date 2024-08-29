#pragma once
#include "dbLyHmGen.h"
#include "dbUGCaBall.h"
#include "dbUGMsgBox.h"
#include "dbUGChKantoku.h"
#include "dbUGChSensyu.h"
namespace db
{
  class TLyHmTeam :public TLyHmGen
  {
  protected:
    //TUIObj* field_;
    TUIObj* banner_;
    TUIObj* bcolors_;
    TUIObj* boombox_;
    TUIObj* camera_;
    TUIObj* films_;
    TUIObj* cardside_;
	TUIObj* cards_;
    TUIObj* tapes_;
    TUIObj* ball_;

    TUGChKantoku ugkantoku_;
    TUGChSensyu ugcap_;
    std::vector<TUGChSensyu*> ugmem_;
    TUGCaBall ugcaball_;

	s64 teamMID_; 
	s32 teamLv_;
	s32 teamRare_;
	TUGRName	ugname_;
	TUIObj*  uiteammid_;

	TUGChSensyu* ballMan1_;
	TUGChSensyu* ballMan2_;
	TUGChSensyu* tapeMan_;

    TUGMsgBox ugmsg_;
    TUGButton ugbln_bgm_;
    TUGButton ugbln_select_;
    TUGButton ugbln_haikei_;
    void ClearRecords();

	void MvPage_Tuto(void);
    void MvPage_Menu();
    void MvPage_InMenu();
    void MvPage_Idle();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyHmTeam(TSnHome* pScene);
    virtual ~TLyHmTeam(void);
    void Idle();
  };
 
}
