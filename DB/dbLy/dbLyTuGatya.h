//utf8
#pragma once
#include "dbLyTuBase.h"
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
//=========================================================================
// 
// チュートリアル：ガチャ
//
//=========================================================================
namespace db
{
  struct TTuGachaInsideSt
  {
    s64 mid_gacha_;
    s32 wait_c_;
    s32 tutomsgstep_;

    void Init()
    {
      Zeromem(this);
      wait_c_ = 60;
    }
  };

	class TLyTuGatya :public TLyTuBase
	{
		// 基底クラス定義
		typedef TLyTuBase base;
	public:
		//コンストラクタ
		explicit TLyTuGatya(TSnTutorial* pScene);
		virtual ~TLyTuGatya(void);
	protected:
		//オーバーロード
		void DoLoad(void);
		void DoReady(void);
		void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
		void DoUpdate(BOOL draw_f);
    void DoDown(void);
	private:
    TUGSpGacha ugsp_gacha_;
    TUGLiShopMenu uglistMenu_;//トップメニュー
    TUGLiSpGacha uglistGacha_;//ガチャ種類ごとのリスト
    TUGLiNaBox uglistNaBox_;//なつかし種類ごとのリスト
    TUGLiNaBoxNum uglistNaBoxNum_;//なつかしＢＯＸ大人選択

    TTuGachaInsideSt stGacha_;
    std::vector<TUGLiGen*> listvec_;
    void DrawList(TUGLiGen* drawlist);
    void Post_TuGatya(void);
    void Post_TuNaBoxOtona(void);
    void Connect_TuGatya(void);
    void Connect_TuNaBox(void);
    void MvPage_Init();
    void MvPage_GachaInfo();
    void MvPage_SelectTypeG();
    void MvPage_SelectGacha();
	void MvPage_CheckGacha();
    void MvPage_DecideGacha();
    void MvPage_GachaRoll();
    void MvPage_GachaResult();
	void MvPage_GachaResult2();
    void MvPage_NaBoxInfo();
    void MvPage_SelectTypeN();
    void MvPage_SelectNaBox();
    void MvPage_SelectNaBoxNum();
    void MvPage_DecideNaBox();
    void MvPage_NaBoxRoll();
    void MvPage_NaBoxResult();
	void MvPage_NaBoxResult2();
    void MvPage_LastInfo();
    void MvPage_Out();

	// チーム取得時にステータス更新を行う関数
	void CheckGetLogTeam(void);
  };

}
