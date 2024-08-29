#pragma once
#include "dbLyHmGen.h"
#include "dbUGSpGacha.h"
#include "dbUGLiShopMenu.h"
#include "dbUGLiShopMenu.h"
#include "dbUGLiGaCnt.h"
#include "dbUGLiProb.h"
#include "dbUGLiTousen.h"
#include "dbUGLiNaBox.h"
#include "dbUGLiNaBoxCnt.h"
#include "dbUGLiNaBoxNum.h"
#include "dbUGLiSpGacha.h"
#include "dbUGPlateProf.h"

namespace db
{


  struct TGachaInsideSt
  {
    s64 mid_gacha_;
    BOOL msgset_f_;
    BOOL otona_f_;
    s32 gahcatype_;
    BOOL todd_f_;

    s32 tousenpage_;
    s32 grpNo_;
    s32 grade_;
    s32 wait_c_;
    s32 backtopage_;
    void Init()
    {
      wait_c_ = 60;
      Zeromem(this);
    }
  };


  class TLyHmGachaInside :public TLyHmGen
  {
  public:
    //コンストラクタ
    explicit TLyHmGachaInside(TSnHome* pScene);
    virtual ~TLyHmGachaInside(void);

	// メニューの中から開始
	void SetupMenu(void);

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
    void RefreshTousenGrade(BOOL star_f);

	void CheckGetLogTeam(void);	// チーム取得時にステータス更新を行う関数

    void MvPage_Visit();
    void MvPage_SelectType();//ガチャタイプ選択

    void OutShop();

    void MvPage_SelectNaBox();//なつかしの種類選択
    void MvPage_SelectNaBoxNum();//なつかしの通常／大人買い選択
    void MvPage_DecideNaBox();
    void MvPage_NaBoxCnt();
    void MvPage_NaBoxCntEnd();
	void MvPage_Preview();
    void MvPage_SelectGacha();
    void MvPage_DecideGacha();
    void MvPage_TousenGrade();
    void MvPage_Tousen();
    void MvPage_TousenEnd();
    void MvPage_Prob();
    void MvPage_ProbCnt();
    void MvPage_ProbCntEnd();
    void MvPage_NaBoxRoll();
    void MvPage_GachaRoll();
    void MvPage_NaBoxResult();
    void MvPage_GachaResult();
    void MvPage_Out();

    void Post_Gacha();
    void Post_Tousen();
    void Post_NaBox();
    void Post_NaBoxOtona();
    void Connect_Gacha();
    void Connect_Tousen();
    void Connect_NaBox();
    void Connect_NaBoxOtona();

    void ActListMenu();
    TUGSpGacha ugsp_gacha_;
    TUGLiShopMenu uglistMenu_;//トップメニュー
    TUGLiSpGacha uglistGacha_;//ガチャ種類ごとのリスト
    TUGLiGaCnt uglistProbCnt_;//ガチャ中身リスト
    TUGLiProb uglistProb_;//当選確率
    TUGLiShopMenu uglistTousenGrade_;
    TUGLiTousen uglistTousen_;//当選者リスト
    TUGPlateProf ugprof_;
    TUGLiNaBox uglistNaBox_;//なつかし種類ごとのリスト
    TUGLiNaBoxNum uglistNaBoxNum_;//なつかしＢＯＸ大人選択
    TUGLiNaBoxCnt uglistNaBoxCnt_;//なつかし中身リスト
    std::vector<TUGLiGen*> listvec_;
    TGachaInsideSt stGacha_;

	BOOL isChangeBGM_;
	s64 introBGMID_;
	s32 introTimer_;	// イントロ用タイマー
	s32 selectListNo_;// リストの何番を選択したか

	SOZAIDTVECTOR prevvec_; // プレビュー用
  };
 
}
