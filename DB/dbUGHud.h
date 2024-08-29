#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"
#include "dbUGMsgBox.h"
#include "dbUGItemView.h"
#include "dbUGPrev.h"
#include "dbUGInput.h"
#include "dbUGBtnTokusyou.h"
#include "dbUGMixPrev.h"

namespace db
{
  class TUGHeadder;
  class TUGFooter;

  enum enUGHudAct
  {
    enUGHudAct_OK,
    enUGHudAct_Cancel,
    enUGHudAct_ItemView,
    enUGHudAct_PrevBack,
		enUGHudAct_PrevMix,
		enUGHudAct_PrevMixClear,
    enUGHudAct_PrevHit,
    enUGHudAct_InputOK,
    enUGHudAct_InputCancel,
		/* 2015/06/19 Koike Added */
		enUGHudAct_PrevHaveMix,
		enUGHudAct_MixPrevUndo,
		enUGHudAct_MixPrevBall0,
		enUGHudAct_MixPrevBall1,
		enUGHudAct_MixPrevBall2,
		enUGHudAct_MixPrevBall3,
		enUGHudAct_MixPrevBall4,
		enUGHudAct_MixPrevBall5,
		enUGHudAct_MixPrevBall6,
		enUGHudAct_MixPrevBall7,
		enUGHudAct_MixPrevSelect,
		enUGHudAct_MixPrevSelectOK,
		enUGHudAct_MixPrevSelectNO,
		enUGHudAct_MixPrevCancel,
  };


  struct TUGHudSt
  {
    s32 ani_c_;
    s32 ani2_c_;
    BOOL ani_f_;
    BOOL msg_f_;
    BOOL itemview_f_;
    BOOL itemlist_f_;
    BOOL prev_f_;
    BOOL input_f_;
    BOOL vswait_f_;
    BOOL count_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGHud :public TUGGen
  {
  protected:
    static const s32 HTNUM = 6;

    void DoUpdate(BOOL draw_f);
    void SetAni(BOOL ani_f, s32 anitype);
    void GenSetItemView();
    
    void GenMessage();
    void SetInputEnd();
  public:
    TUGHudSt stHud_;
    TUGHeadder* ugheadder_;
    TUGFooter* ugfooter_;
    TUGButton ugbtn_cancel_;
    TUGBtnTokusyou ugbtn_tsh_;//特賞法ボタン
    
    TUIObj* panel_;
    TUIObj* ganba_;
    TUIObj* countdown_;
    TUIObj* hart_[HTNUM];

    TUGMsgBox ugmsg_;
    TUGItemView ugitemview_;
    TUGPrev ugprev_;
    TUGInput uginput_;
		/* 2015/06/30 Koike Added */
		TUGMixPrev ugmixprev_;

    //コンストラクタ
    explicit TUGHud(TUIGenLayer* player);
    virtual ~TUGHud(void);
    void MakeGroup(TUIObj* parent);
    void SetHedder(TUGHeadder* ugheadder, TUGFooter* ugfooter);

    void SetModal(BOOL modal_f);

    void SetErrorMessage(s32 code, const char* msg);
    void SetMessage(BOOL cancel_f, BOOL tsh_f, STRVECTOR* strvec);
    void SetMessageTodd(STRVECTOR* strvec, s32 num, BOOL get_f);
    void SetMessageStar(STRVECTOR* strvec, s32 num);
    void SetMessageStarSupply(s32 mid_supply, BOOL quse_f);
    void SetMessageStarGacha(s32 mid_gacha);
    void SetMessageStarNaBox(s32 mid_nabox, BOOL otona_f);
    void SetGKErrorMessage();

    void SetAniStart();
    void SetAniStart_Login();
    void SetAniStart_Auth();
    void SetAniStart_EnWait();
    void SetAniEnd();
    void SetMsgEnd();
		void SetItemView_GetLog(BOOL msgok_f, BOOL leftpos_f, BOOL filter_f);
		void SetItemView_GetLogSingle(s32 logno, BOOL msgok_f, BOOL leftpos_f);
		void SetItemView_GachaCnt(s64 mid_gachacnt);
		void SetItemView_GachaInit();
		void SetItemView_GachaGet(s32 logno);
		void SetItemView_NaBoxCnt(s64 mid_naboxcnt);
		void SetItemView_HukuCnt(s64 mid_hukucnt);
		void SetItemView_Reward(s64 mid_ranking, s32 rank, BOOL msgok_f, BOOL leftpos_f, BOOL filter_f);
		void SetItemView_View(s64 itemType, s64 itemID, s64 itemNum, BOOL msgok_f, BOOL leftpos_f);
		void SetItemView_Shot(SOZAIDTVECTOR sozai, BOOL leftpos_f);
		void SetItemView_SozaiList(SOZAIDTVECTOR sozai, BOOL leftpos_f);
		void SetItemView_Drop(s64 itemType, s64 itemID, s64 itemNum, s32 tokkouNum, s32 tokkouMag, BOOL comp_f);
		void SetItemViewEnd();
		void SetItemViewDraw(BOOL draw_f);
		void SetPrevEnd();
		//void SetPrev(s64 id_team, s64 id_shot, BOOL backbtn_f);
		void SetPrev(const TPrevParam& param);
		void SetPrev_Mixed(LONGVECTOR* id_shot_mixvec);
		void SetPrevChangeShot(const SOZAIDTVECTOR& sozaivec);
		/* 2015/06/19 Koike Added */
		void SetPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec);

		void SetPrevMixSozai(s64 id_sozai, BOOL isRndSet);
		void SetPrevMixShot(s64 id_shot, BOOL isRndSet);

		void SetInput(s32 outw, BOOL alponly_f);
		void SetInput(s32 outw, BOOL alponly_f, INTVECTOR defmcvec);
		void SetVSWait_Search();
		void SetVSWait_EnWait();
		void SetVSWaitEnd();

		void SetCount(s32 count);
		void SetCountEnd();

		/* 2015/06/30 Koike Added*/
		void SetMixPrevEnd();
		void SetMixPrev(const TMixPrevParam& param);
		void SetMixPrev_Mixed(LONGVECTOR* id_shot_mixvec);
		void SetMixPrevChangeShot(const SOZAIDTVECTOR& sozaivec);
		/* 2015/06/19 Koike Added */
		void SetMixPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec);
		void SetMixPrevMixSozai(s64 id_sozai, BOOL isRndSet);
		void SetMixPrevMixShot(s64 id_shot, BOOL isRndSet);
		s32 SetMixPrevTouchMode(LONGVECTOR* id_shot_mixvec, s32 iconNo);
		void SetMixPrevSlectShot(s64 id_shot);
		LONGVECTOR GetMixPrevChangeVec();
		void ResetField(BOOL fieldflg);
    
    std::string OutputString();
    BOOL IsAni();
    BOOL IsMsg();
    BOOL IsItemView();
    BOOL IsPrev();
    BOOL IsInput();

    BOOL IsActRes(enUGHudAct res);
  };
 
}
