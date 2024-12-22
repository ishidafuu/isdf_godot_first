#pragma once
#include "dbLyHmGen.h"
#include "dbUGSpMami.h"
#include "dbUGButton.h"
#include "dbUGLiMenu.h"
#include "dbUGLiGiftBox.h"
#include "dbUGLogboIcon.h"

namespace db
{


  struct TMamiSt
  {
    s32 giftpageNo_;
    s32 endpage_;
    BOOL checked_f_;

	BOOL logbo_f_;
	s64 nextitemType_;
	s64 nextitemID_;
	s64 nextitemNum_;
  s32 mid_logbosozai_;
    void Init()
    {
      Zeromem(this);
    }
  };


  class TLyHmMami :public TLyHmGen
  {
  public:
    //コンストラクタ
    explicit TLyHmMami(TSnHome* pScene);
    virtual ~TLyHmMami(void);
  protected:
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);


    void DrawList(TUGLiGen* drawlist);
    void ResetObj();
    void SwitchGiftPageBtn();
    void MvPage_Visit();

	void MvPage_Logbo();
	void MvPage_LogboSp();
	void MvPage_LogboSpYokoku();
	void MvPage_LogboResult();

  void ResetSozaiVec();
  void MvPage_LogboShot();
  void MvPage_LogboSpShot();
  void MvPage_LogboSpShotYokoku();
	void MvPage_LogboShotResult();

    void MvPage_Menu();
    void MvPage_GiftBox();
    void MvPage_GiftBoxResult();
    void MvPage_UpGrade();
    void MvPage_Out();

    //通信
	void Post_Logbo();

    void Post_CheckGiftBox();
    void Post_OpenGiftBox();

	void Connect_Logbo();

    void Connect_CheckGiftBox();
    void Connect_OpenGiftBox();

    void SetEndPage();
    void ActListMenu();
    TUGSpMami ugsp_mami_;
    TUGLiMenu uglistMenu_;
    TUGLiGiftBox uglistGiftBox_;
    LONGVECTOR openidvec_;
    s32 mid_logbosozai_;
    INTVECTOR spsyouhinvec_;
    SOZAIDTVECTOR prevsozaivec_;
    INTVECTOR sozaivec_add_;
    INTVECTOR syouhinvec_add_;

    std::vector<TUGLiGen*> listvec_;
    TMamiSt stMami_;

	TUGLogboIcon* uglogbo_;

	s32 subPage_;
	BOOL isSpShot_;	// Bランクシュート配布日かどうか
	BOOL isCancel_;	// シュートいらない場合

	/* 2015/06/16 Koike Added Bランクシュート配布の場合のメッセージ */
	TUGTitle ugtitle_spshot_;
  };
 
}
