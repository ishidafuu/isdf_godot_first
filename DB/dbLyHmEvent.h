#pragma once
#include "dbLyHmGen.h"
#include "dbUGLiEvent.h"
#include "dbUGBtnShiai.h"
#include "dbUGBtnVS.h"
#include "dbUGLock.h"
#include "dbUGRMessage.h"
#include "dbUGBtnEvent.h"
#include "dbUGTitle.h"

namespace db
{
  class TLyHmEvent :public TLyHmGen
  {
  public:
    //コンストラクタ
    explicit TLyHmEvent(TSnHome* pScene);
    virtual ~TLyHmEvent(void);
    void Idle();

	void SetupEvent(s32 eventNo);

  protected:
	  TUGLiEvent uglist_event_;//イベントリスト
	  LONGVECTOR shsetvec_;
	  std::vector<TUGLock*> uglock_;

	  // オススメイベント
	  TUGTitle ugtitle_ev_;
	  TUGBtnEvent ugevbtn_;
	  TUGRMessage ugrmsg_ev_;


	  BOOL IsSubEventOpen();

	  void MvPage_Init(void);
	  void MvPage_Tuto(void);
	  void MvPage_EventNone(void);
	  void MvPage_Main();
	  void MvPage_InEvent();
	  void MvPage_Idle();

	  void Connect_Omakase();
	  void Connect_Event();
	  void Post_Omakase();

	  void Post_Event();
	  //オーバーロード
	  void DoLoad();
	  void DoReady();
	  void DoUpdate(BOOL draw_f);
	  void DoConnect(BOOL rqok_f);
  };

}
