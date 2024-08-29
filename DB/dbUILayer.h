#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUIGenLayer.h"
#include "dbMgEngineObj.h"
#include "dbMgUI.h"
#include "dbUIFrame.h"
#include "dbUGButton.h"
#include "dbUGBtnTokusyou.h"
#include "dbUGBtnSupply.h"
#include "dbUGRHdCredit.h"
#include "dbUGItemView.h"
//#include "dbUGPrev.h"
namespace db
{

    class TUIFrame;
    class TScene;
    struct  TPrevParam;

    enum enCOType
    {
        coNone,
        coUser,
        coInfo,
        coWorld,
        coClmaster,
        coSvmaster,
        coBannerBmp,
        coEventBmp,
    };

    struct TUILayerSt
    {
        enUIDepth filterdepth_;
        s32 pageNo_;
        s32 lastpageNo_;
        s32 connect_;
        BOOL ready_f_;
        BOOL wait_f_;
        enCOType checkout_;
        BOOL gototitle_f_;
        BOOL hedderlayer_f_;
        BOOL barlayer_f_;

        void Init()
        {
            Zeromem(this);
            connect_ = NGNUM;
            //request_ = NGNUM;
        }
    };

    class TUILayer :public TUIGenLayer
    {
    public:
        static const s32 HEADH = 44;//ヘッダサイズ
        static const s32 FOOTH = 48;//フッタサイズ
        TUILayerSt stLy_;
        //コンストラクタ
        explicit TUILayer(TScene* pScene, TUIFrame* frame);
        virtual ~TUILayer(void);

        void SetFooterPointer();//ポインタセット
        void Load();//読み込み
        void Down();
        void Ready();
        void Update(BOOL draw_f);//更新
        void Draw();//描画
        void Wait(BOOL wait_f);
        BOOL IsMove();
        BOOL IsReady();
    protected:
        TUIObj* field_;
        TUIFrame* frame_;
        TUGButton* ugbtn_back_;
        TUGButton* ugbtn_ok_;
        TUGButton* ugbtn_mid_;
        TUGButton* ugbtn_next_;
        TUGButton* ugbtn_prev_;
        TUGBtnTokusyou* ugbtn_tsh_;
        TUGBtnSupply* ugbtn_mgk_;
        TUGBtnSupply* ugbtn_fkt_;
        TUGRHdCredit* ugr_star_;
        TUGRHdCredit* ugr_todd_;
        TUGRHdCredit* ugr_huku_;
        TUGRHdCredit* ugr_gacha_;

        void ChangePage(s32 pageno);
        void ChangeLastPage();
        void SetFoot_Tab();
        void SetFoot_Btn();
        void SetFoot_BtnHd();
        void SetFoot_None();
        void SetHeadderModal(BOOL modal_f);
        void UndrawBtn();
        void UndrawCredit();

        BOOL IsCheckOut();
        void StartCheckOut(enCOType cotype);

        BOOL IsConnect();
        BOOL IsDisconnect();
        void StartConnect(s32 connectNo);
        void EndConnectOK();
        void EndConnectNG(s32 code);
        void EndConnectNG(s32 code, const char* msg);

        void SetHudMsg(BOOL cancel_f, BOOL tsh_f, STRVECTOR* strvec);
        void SetHudMsgTodd(STRVECTOR* strvec, s32 num, BOOL get_f);
        void SetHudMsgStar(STRVECTOR* strvec, s32 num);
        void SetHudMsgStarSupply(s32 mid_supply, BOOL quse_f);
        void SetHudMsgStarGacha(s32 mid_gacha);
        void SetHudMsgStarNaBox(s32 mid_nabox, BOOL otona_f);
        void SetHudFilter(BOOL draw_f);
        void SetHudEnd();
        void SetPrevEnd();
        void SetHudDraw(BOOL draw_f);
        void SetHudGKErrorMsg();
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
				BOOL IsHudMixPrevCancel();

        void SetHudItemView_GetLog(BOOL msgok_f, BOOL leftpos_f, BOOL filter_f);
        void SetHudItemView_GetLogSingle(s32 logno, BOOL msgok_f, BOOL leftpos_f);
        void SetHudItemView_GachaCnt(s64 mid_gachacnt);
        void SetHudItemView_GachaInit();
        void SetHudItemView_GachaGet(s64 logno);
        void SetHudItemView_NaBoxCnt(s64 mid_naboxcnt);
        void SetHudItemView_HukuCnt(s64 mid_hukucnt);
        void SetHudItemView_Reward(s64 mid_ranking, s32 rank, BOOL msgok_f, BOOL leftpos_f, BOOL filter_f);
        void SetHudItemView_View(s64 itemType, s64 itemID, s64 itemNum, BOOL msgok_f, BOOL leftpos_f);
        void SetHudItemView_Shot(SOZAIDTVECTOR sozai, BOOL leftpos_f);
        void SetHudItemView_SozaiList(SOZAIDTVECTOR sozai, BOOL leftpos_f);
        void SetHudItemView_Drop(s64 itemType, s64 itemID, s64 itemNum, s32 tokkouNum, s32 tokkouMag, BOOL cmp_f);
        //void SetHudPrev(s64 id_team, s64 id_shot, BOOL backbtn_f);
        void SetHudPrev(const TPrevParam& param);
        void SetHudPrev_Mixed(LONGVECTOR* id_shot_mixvec);
        void SetHudPrevChangeShot(const SOZAIDTVECTOR& sozaivec);
        /* 2015/06/19 Koike Added */
        void SetHudPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec);
				void SetHudPrevMixSozai(s64 id_sozai, BOOL isRndSet);	// 素材単品
        void SetHudPrevMixShot(s64 id_shot, BOOL isRndSet);	// 手持ちシュート

				/* 2015/07/02 Koike Added */
				void SetHudMixPrev(const TMixPrevParam& param);
				void SetHudMixPrev_Mixed(LONGVECTOR* id_shot_mixvec);
				void SetHudMixPrevChangeShot(const SOZAIDTVECTOR& sozaivec);
				void SetHudMixPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec);
				void SetHudMixPrevMixSozai(s64 id_sozai, BOOL isRndSet);	// 素材単品
				void SetHudMixPrevMixShot(s64 id_shot, BOOL isRndSet);	// 手持ちシュート
				void SetHudMixPrevTouchMode(LONGVECTOR* id_shot_mixvec, s32 iconNo);
				void SetHudMixPrevSelectShot(s64 id_shot);
				LONGVECTOR GetHudMixPrevChangeVec();
				void SetHudResetField(BOOL fieldflg);

        void SetHudVSWait_Search();
        void SetHudVSWait_EnWait();
        void SetHudVSWaitEnd();
        void SetHudCount(s32 count);
        void SetHudCountEnd();

        virtual void DoLoad() = 0;//読み込み
        virtual void DoReady(){}
        virtual void DoDown(){}
        virtual void DoChangePage(s32 pageno){}
        virtual void DoCheckOut(BOOL rqok_f){}
        virtual void DoConnect(BOOL rqok_f){}
        virtual void DoUpdate(BOOL draw_f){}


    };

}
