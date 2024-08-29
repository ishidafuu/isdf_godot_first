#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUIGenLayer.h"
#include "dbUGItemView.h"
//#include "dbMgEngineObj.h"
//#include "dbMgUI.h"



namespace db
{

  struct TUIFrameSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGFilter;
  class TUGHeadder;
  class TUGFooter;
  class TUGHud;
  class TScene;
  class TLyBarGen;
  struct TPrevParam;
	struct TMixPrevParam;

  class TUIFrame :public TUIGenLayer
  {
  protected:
    TUIObj* field_;
    TUGHud* ughud_;
   
    virtual void DoLoad(){};//読み込み
    virtual void DoUpdate(BOOL draw_f){};
  public:
    TUGHeadder* ugheadder_;
    TUGFooter* ugfooter_;//フッタのボタンポインタが必要なのでパブリック

    static const s32 HEADH = 44;//ヘッダサイズ
    static const s32 FOOTH = 48;//フッタサイズ
    TUIFrameSt stLy_;

    
    //コンストラクタ
    explicit TUIFrame(TScene* pScene);
    virtual ~TUIFrame(void);
    void SetHedder(TLyBarGen* lybar);
    void SetHedderEnable();
    void SetHedderDisable();

    BOOL IsAni();
    BOOL IsMsg();
    BOOL IsPrev();
    void SetAniStart();
    void SetAniStart_Login();
    void SetAniStart_Auth();
    void SetAniEnd();

    BOOL IsConnect();
    BOOL IsDisconnect();
    void StartConnect(s32 connectNo);
    void EndConnectOK();
    void EndConnectNG(s32 code, const char* msg);

		void SetHudMsg(BOOL cancel_f, BOOL tsh_f, STRVECTOR* strvec);
		void SetHudMsgTodd(STRVECTOR* strvec, s32 num, BOOL get_f);
		void SetHudMsgStar(STRVECTOR* strvec, s32 num);
		void SetHudMsgStarSupply(s32 mid_supply, BOOL quse_f);
		void SetHudMsgStarGacha(s32 mid_gacha);
		void SetHudMsgStarNaBox(s32 mid_nabox, BOOL otona_f);
		void SetHudFilter(BOOL draw_f);
		void SetHudGKErrorMsg();
		void SetMsgEnd();
		void SetItemViewEnd();
		void SetItemViewDraw(BOOL draw_f);
		void SetPrevEnd();
		BOOL IsHudOK();
		BOOL IsHudCancel();
		BOOL IsHudItemView();
		BOOL IsHudPrevBack();
		BOOL IsHudPrevMix();
		BOOL IsHudPrevMixClear();
		BOOL IsHudPrevHit();
		BOOL IsHudIntro(void);
		TUGItemViewParam GetHudItemView(void);
		/* 2015/06/19 Koike Added */
		BOOL IsHudPrevHaveMix();
		BOOL IsHudMixPrevUndo();
		BOOL IsHudMixPrevBall_0();
		BOOL IsHudMixPrevBall_1();
		BOOL IsHudMixPrevBall_2();
		BOOL IsHudMixPrevBall_3();
		BOOL IsHudMixPrevBall_4();
		BOOL IsHudMixPrevBall_5();
		BOOL IsHudMixPrevBall_6();
		BOOL IsHudMixPrevBall_7();
		BOOL IsHudMixPrevSelectBall();
		BOOL IsHudMixPrevSelectShot();
		BOOL IsHudMixPrevSelectMode();
		BOOL IsHudMixPrevCancel();

		void SetHudItemView_GetLog(BOOL msgok_f, BOOL leftpos_f, BOOL filter_f);
		void SetHudItemView_GetLogSingle(s32 logno, BOOL msgok_f, BOOL leftpos_f);
		void SetHudItemView_GachaCnt(s64 mid_gachacnt);
		void SetHudItemView_GachaInit();
		void SetHudItemView_GachaGet(s32 logno);
		void SetHudItemView_NaBoxCnt(s64 mid_naboxcnt);
		void SetHudItemView_HukuCnt(s64 mid_hukucnt);
		void SetHudItemView_Reward(s64 mid_ranking, s32 rank, BOOL msgok_f, BOOL leftpos_f, BOOL filter_f);
		void SetHudItemView_View(s64 itemType, s64 itemID, s64 itemNum, BOOL msgok_f, BOOL leftpos_f);
		void SetHudItemView_Shot(SOZAIDTVECTOR sozai, BOOL leftpos_f);
		void SetHudItemView_SozaiList(SOZAIDTVECTOR sozai, BOOL leftpos_f);
		void SetHudItemView_Drop(s64 itemType, s64 itemID, s64 itemNum, s32 tokkouNum, s32 tokkouMag, BOOL comp_f);

		//void SetHudPrev(s64 id_team, s64 id_shot, BOOL backbtn_f);
		void SetHudPrev(const TPrevParam& param);
		void SetHudPrev_Mixed(LONGVECTOR* id_shot_mixvec);
		void SetHudPrevChangeShot(const SOZAIDTVECTOR& sozaivec);
		/* 2015/06/19 Koike Added */
		void SetHudPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec);

		void SetHudPrevMixSozai(s64 id_sozai, BOOL isRndSet);
		void SetHudPrevMixShot(s64 id_shot, BOOL isRndSet);

		/* 2015/06/30 Koike Added */
		void SetHudMixPrev(const TMixPrevParam& param);
		void SetHudMixPrev_Mixed(LONGVECTOR* id_shot_mixvec);
		void SetHudMixPrevChangeShot(const SOZAIDTVECTOR& sozaivec);
		void SetHudMixPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec);
		void SetHudMixPrevMixSozai(s64 id_sozai, BOOL isRndSet);
		void SetHudMixPrevMixShot(s64 id_shot, BOOL isRndSet);
		s32 SetHudMixPrevTouchMode(LONGVECTOR* id_shot_mixvec, s32 iconNo);
		void SetHudMixPrevSelectShot(s64 id_shot);
		LONGVECTOR GetHudMixPrevChangeVec();
		void SetHudResetField(BOOL fieldflg);
		//-----------------------------------------------

		void SetHudInput(s32 outw, BOOL alponly_f);
		void SetHudInput(s32 outw, BOOL alponly_f, INTVECTOR defmcvec);
		std::string HudOutputString();

		void SetHudVSWait_Search();
		void SetHudVSWait_EnWait();
		void SetHudVSWaitEnd();

		void SetHudCount(s32 count);
		void SetHudCountEnd();

		BOOL IsAct();

		void Load();//読み込み
		void Update(BOOL draw_f);//更新
		void Draw();//描画
  };
 
}
