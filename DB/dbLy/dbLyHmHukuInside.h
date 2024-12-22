#pragma once
#include "dbLyHmGen.h"
#include "dbUGSpHuku.h"
#include "dbUGLiShopMenu.h"
#include "dbUGLiSpHuku.h"
#include "dbUGLiHukuCnt.h"
#include "dbUGLiTousen.h"
#include "dbUGBtnTokusyou.h"
#include "dbUGBtnTokusyou.h"
#include "dbUGPlateProf.h"

namespace db
{


  struct THukuInsideSt
  {
    s64 mid_huku_;
    s64 mid_hukugrp_;
    BOOL msgset_f_;
    s32 tousenpage_;
    s32 grpNo_;
    s32 grade_;
    s32 wait_c_;
    s32 backtopage_;
    s64 id_enemy_;
    void Init()
    {
      wait_c_ = 60;
      Zeromem(this);
    }
  };


  class TLyHmHukuInside :public TLyHmGen
  {
  protected:
    TUGSpHuku ugsp_Huku_;
    TUGLiSpHuku uglistSpHuku_;//ふくびきリスト
    TUGLiHukuCnt uglistHukuCnt_;//ふくびき中身リスト
    TUGLiShopMenu uglistTousenGrade_;
    TUGLiTousen uglistTousen_;//当選者リスト
    TUGPlateProf ugprof_;

	s32 selectNo_;			// リストの何番を選択したか
	SOZAIDTVECTOR prevvec_; // プレビュー用

    std::vector<TUGLiGen*> listvec_;
    void DrawList(TUGLiGen* drawlist);
    void ResetObj();

    void MvPage_Visit();
    void MvPage_Select();//ガチャタイプ選択

    void OutShop();

    void ActListSpHuku();

    void MvPage_Decide();
    void MvPage_TousenGrade();
    void MvPage_Tousen();
    void MvPage_TousenEnd();
    void MvPage_Cnt();
    void MvPage_CntEnd();
	void MvPage_Preview();
    void MvPage_Roll();
    void MvPage_Result();
    void MvPage_Out();

    void Post_Huku();
    void Post_HukuCnt();
    void Post_Tousen();
    void Connect_Huku();
    void Connect_HukuCnt();
    void Connect_Tousen();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    THukuInsideSt stHuku_;

    //コンストラクタ
    explicit TLyHmHukuInside(TSnHome* pScene);
    virtual ~TLyHmHukuInside(void);
    void ClearRecords();

	// メニューの中から開始
	void SetupMenu(void);
  };
 
}
