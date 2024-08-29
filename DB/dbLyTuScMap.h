#pragma once
#include "dbLyScGen.h"

namespace db
{

  struct TTuMapSt
  {
    s32 nowmapNo_;
    s32 left_;
    s32 top_;
    s32 bottom_;
    s32 right_;
    s32 walk_c_;
    BOOL paused_f_;

    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGSerifu;
  class TUGFinger;

  class TLyTuScMap :public TLyScGen
  {
  protected:
    PANELVECTOR ugpanelvec_;//各マップマス（ふえるたび確保）
    TUGChKantoku ugkantoku_;
	TUGChCaptain ugcap_;
    TUIObj* map_;
    TUIObj* camera_;
	TTuMapSt stMap_;
	s32 selectMapNo_;

	TUGSerifu* pUGSerifu_;
	TUGFinger* pUGFinger_;
	TUGFinger* pUGFinger2_;	// もう一つ必要になってしまったので用意
	STRVECTOR* pSerifuVec_;
	TUGScPanel* pPanel_;
	s32 tuTxtNo_;

	void SetMsg(s32 msgNo);
	void SetMsg(const char* text0, const char* text1, const char* text2);

    void ClearPanelVec();
    void OpenMap(s32 startpos);
    void OpenSelf(TUGScPanel* parent);
    void OpenChildStart(s64 posNo);
    void OpenChild(s64 posNo);
    void OpenChild(s32 posx, s32 posy);
    void OpenChildlen(TUGScPanel* parent);
    void Pause(BOOL pause_f);

	void MvPage_Text(void);
	void MvPage_TutoStage(void);
	void MvPage_TutoCamMove(void);
	void MvPage_TutoSelect(void);

    void MvPage_Visit();
    void MvPage_SelectPanel();
    void MvPage_Enemy();

    //オーバーロード
    void DoLoad();
    void DoResetBtn();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyTuScMap(TSnScenario* pScene);
    virtual ~TLyTuScMap(void);

    void SetScrollSize();
    void Return();
  };

}
