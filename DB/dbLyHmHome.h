#pragma once
#include "dbLyHmGen.h"
#include "dbUGButton.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"
#include "dbUGChGirl.h"
#include "dbUGLogboIcon.h"
#include "dbUGInfoIcon.h"
#include "dbUGBanner.h"
#include "dbUGTmBoard.h"

namespace db
{
  const s32 WINDOWNUM = 3;
  class TLyHmHome :public TLyHmGen
  {
  public:
    //コンストラクタ
    explicit TLyHmHome(TSnHome* pScene);
    virtual ~TLyHmHome(void);

	// バナーを押したときのホーム内移動
	void SetupHome(s32 moveNo);

	// まみ、画面外へ
	void SetOutMami(void);

	// 最強チームの表示更新
	void RefreshTopTeam(void);

    void Idle();

    //次のレイヤから操作される
    TUGChKantoku ugkantoku_;
    TUGChCaptain ugcap_;
    TUGChGirl ugmami_;
    TUGChGirl ugmabeko_;
	TUGChGirl ughasebe_;
    TUGLogboIcon uglogbo_;
    TUGInfoIcon uginfo_;
  protected:
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoChangePage(s32 pageno);
    void RefreshHome();
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);

	void MvPage_LoginBonus();
    void MvPage_Walking();
    void MvPage_Selected();
    void MvPage_InMenu();
	void MvPage_OutMami();
    void MvPage_Idle();
    void MvPage_Help();
    void Post_CheckMami();
    void Post_Event();
    void Connect_CheckMami();
    void Connect_Event();
    //TUIObj* field_;
    TUIObj* floorB_;
    TUIObj* floorF_;
    TUIObj* boardL_;
    TUIObj* boardR_;
    //TUIObj* flower_;
    TUIObj* shadow_[WINDOWNUM];
    TUIObj* window_[WINDOWNUM];

    TUGButton ugbln_mami_;
    TUGButton ugbln_mabeko_;
	TUGButton ugbln_hasebe_;

	TUGTmBoard ugTmBoard_;

	TUGBanner ugBanner_;
  BOOL mamichecked_f_;
  };
 
}
