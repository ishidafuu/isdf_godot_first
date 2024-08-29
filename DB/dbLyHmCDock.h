#pragma once
#include "dbLyHmGen.h"
#include "dbUGSpGousei.h"
#include "dbUGDock.h"
#include "dbUGDrShot.h"
#include "dbUGLiShFilter.h"

namespace db
{
  struct TCDockSt
  {
    s64 id_dock_;
    s64 id_shot_base_;
    //s64 id_shot_add_;
    s64 id_shot_look_;
    s64 id_shot_prev_;
    s64 id_shot_get_;
    s32 backpage_prev_;
    s32 backpage_filter_;
		BOOL have_random_;

    void Init()
    {
      Zeromem(this);
      id_shot_base_ = NGNUM;
      //id_shot_add_ = NGNUM;
      id_shot_look_ = NGNUM;
			have_random_ = TRUE;
    }
  };


  class TLyHmCDock :public TLyHmGen
  {
  public:
    TCDockSt stCDock_;
    LONGVECTOR id_shot_mixvec_;

    //コンストラクタ
    explicit TLyHmCDock(TSnHome* pScene);
    virtual ~TLyHmCDock(void);

    void ReadyDock(TUGDock* ugdock, s32 dockno);
  protected:
    TUGSpGousei uggousei_;
    TUGDock* ugdock_;//ポインタ
    TUGDrShot ugdrShot_;
    TUGLiShFilter uglistShFilter_;//シュートフィルター
    TUGShot ugshot_;

		TUGDrShot ugdrMixPrevShot_;
		TUGLiShFilter uglistMixPrevShFilter_;//シュートフィルター
		TUGShot ugmixprevshot_;

    void ResetObj();
    void MvPage_Visit();
    void MvPage_Base();
    void MvPage_Add();
    void MvPage_Add2();
    void MvPage_OK();
    void MvPage_Get();
    void MvPage_Bara();
    void MvPage_ShFilter();
    void MvPage_Prev();
    void MvPage_GetPrev();
    void MvPage_Out();
    /* 2015/06/02 Koike Added */
    void MvPage_MixPrev();
		void MyPage_CustumPrevIn();
		void MyPage_CustumPrev();
		void MyPage_CustumPrevDrum();
		void MyPage_CustomPrevOK();
    
    void Post_Gousei();
    void Post_OKGousei();
    void Post_NGGousei();
    void Connect_Gousei();
    void Connect_OKGousei();
    void Connect_NGGousei();

    void DoChangePage(s32 pageno);
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
    void ResetPrevBtn();

		/* 2015/06/18 Koike Added */
		void MixMyShot();
		void MixMyShot_CustumPrev();
		void SelectMyShot();
		BOOL IsUndoShot();
		void SetActionBallIcon(s32 iconno);
  };
}
