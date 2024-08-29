#pragma once
#include "dbScene.h"
#include "dbLyList_Home.h"

namespace db
{
  struct TSnHomeSt
  {
    s32 nextscene_;
    void Init()
    {
      Zeromem(this);
      //mid_team_prev_ = NGNUM;/
    }
  };
  //ホーム///////////////////////////
  class TSnHome : public TScene
  {
  protected:
    //BOOL setlayer_f_;
    //BOOL goto_f_;

    TSnHomeSt stHm_;

    
    //継承
    void DoSetLayer();

    BOOL IsSTuTeam();

    BOOL IsSTuShiai();

    void DoFreeLayer();
    void DoGotoTitleFilter();
    void DoReady();
    void DoUpdate();
    void DoDraw();
  public:
    TUIFrame* frame_;
    TLyHmGrp* hmGrp_;
    TLyHmBar* lyHmBar_;

    TLyHmHome* lyHmHome_;
    TLyHmMaveko* lyHmMaveko_;
    TLyHmMami* lyHmMami_;
	TLyHmHasebe* lyHmHasebe_;
    TLyTechno* lyTechno_;
    TLyProfile* lyProfile_;

    TLyHmShiai* lyHmShiai_;
    TLyHmShVSHum* lyHmShVS_;
	TLySTuShiai* lySTuShiai_;	// サブチュートリアル用

	TLyHmEvent* lyHmEvent_;
	TLyHmEvEvent* lyHmEvEvent_;

    TLyHmTeam*  lyHmTeam_;
    TLyHmTmSelect*  lyHmTmSelect_;
    TLyHmTmHaikei*  lyHmTmHaikei_;
    TLyHmTmBGM*  lyHmTmBGM_;
	TLySTuTeam* lySTuTeam_;	// サブチュートリアル用
	

    TLyHmShop*  lyHmShop_;
    TLyHmShopInside*  lyHmShopInside_;
    TLyHmGachaInside*  lyHmGachaInside_;
    TLyHmHukuInside*  lyHmHukuInside_;
    TLyHmCenter*  lyHmCenter_;
    TLyHmCDock*  lyHmCDock_;
    TLyHmCBBA*  lyHmCBBA_;
    TLyHmCMaruka*  lyHmCMaruka_;
    //TLyHmCTuto*  lyHmCTuto_;

    explicit TSnHome(TGame* pGame, TMgUI* pMgUI, s32 myScene);
    virtual ~TSnHome(void);
    
    void GotoNextScene();
    void SetNextScenario();
    void SetNextTitle();
    void SetNextVSShiai();
  };
}
