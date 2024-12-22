#pragma once
#include "dbScene.h"
#include "dbLyList_Tutorial.h"
//=========================================================================
// 
// チュートリアルシーンクラス
//
//=========================================================================
namespace db
{
  class TSnTutorial : public TScene
  {
  public:
    explicit TSnTutorial(TGame* pGame, TMgUI* pMgUI, s32 myScene);
    virtual ~TSnTutorial(void);

    TUIFrame* frame_;
    TLyTuGrp* tuGrp_;
    TLyTuBar* lyTuBar_;

    TLyTuTest*		lyTuTest_;		// これはテスト

    TLyTuTmSelect*	lyTuTmSelect_;

    TLyTuHome*		lyTuHome_;
    TLyTuHmMami*	lyTuHmMami_;

    TLyTuShop*		lyTuShop_;
    TLyTuGatya*		lyTuGatya_;

    TLyTuCenter* lyTuCenter_;
    TLyTuCDock* lyTuCDock_;

    TLyTuProfile*	lyTuProfile_;

    inline void SetState(s32 s){ state_ = s; }

  protected:
    s32 state_;	// チュートリアルステート
    //BOOL setlayer_f_;


    //継承
    void DoSetLayer(void);
    void DoFreeLayer(void);
    void DoReady(void);
    void DoUpdate(void);
    void DoDraw(void);
    void DoGotoTitleFilter();

  private:
    void Init(void);
    void Opening(void);
    void TeamSelect(void);
    void GameSousa(void);
    void HomeSousa(void);
    void GatyaSousa(void);
    void Gousei(void);
    void EndTuto(void);
  };
}
