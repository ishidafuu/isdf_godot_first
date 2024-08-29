#pragma once
#include "dbLyHmGen.h"
#include "dbUGSpHasebe.h"
#include "dbUGButton.h"
#include "dbUGLiMenu.h"
#include "dbUGPlateProf.h"
#include "dbUGInfoIcon.h"
#include "dbUGDrTeam.h"
#include "dbUGButton.h"
#include "dbUGLiTmFilter.h"
#include "dbUGLiMember.h"
#include "dbUGChStatePlate.h"

//=====================================================================================================
//
//
//
//
//=====================================================================================================
namespace db
{
	class TestUITrainingMenu;

	struct THasebeSt
	{
		BOOL msgset_f_;
		s32 wait_c_;
		s32 pageNo2_;

		void Init()
		{
			Zeromem(this);

			wait_c_ = 60;
		}
	};

	class TLyHmHasebe :public TLyHmGen
	{
	public:
		//コンストラクタ
		explicit TLyHmHasebe(TSnHome* pScene);
		virtual ~TLyHmHasebe(void);

		THasebeSt stHasebe_;

	protected:
		void DrawList(TUGLiGen* drawlist);
		void ResetObj(void);

		void RefreshCoachMenu(void);

		void MvPage_Visit(void);
		void MvPage_Menu(void);
		void MvPage_TrTeamSelect(void);
		void MvPage_TrCharaSelect(void);
		void MvPage_TrCoachSelect(void);
		void MvPage_TrCheck(void);
		void MvPage_TrCharaLvUp(void);
		void MvPage_TrOneMoreCheck(void);
		void MvPage_TeamDisplay(void);

		void MvPage_TmMenu(void);
		void MvPage_TmCharaMenu(void);
		void MvPage_TmSellCheck(void);
		// エラーが出た時に
		// 再送信チャレンジ or タイトルに戻す処理
		void MvPage_TmSellRetry(void);

		void MvPage_TmFilter();
		void MvPage_Out(void);

		void Post_Training(void);
		void Post_TmSell(void);

		void Connect_Training(void);
		void Connect_TmSell(void);

		//オーバーロード
		void DoLoad(void);
		void DoReady(void);
		void DoDown(void);
		void DoChangePage(s32 pageno);
		void DoConnect(BOOL rqok_f);
		void DoUpdate(BOOL draw_f);


		TUGSpHasebe ugsp_Hasebe_;
		TUGLiMenu uglistMenu_;
		TUGRName ugMenuTitle_;
		TUGPlateProf ugprof_;
		TUGInfoIcon* uginfo_;

		TUGLiMenu uglistCoachMenu_;
		TUGRName ugCoachMenuTitle_;
		//std::map<s64, BOOL> isHaveCoachKen_;


		// チームリスト
		// MakeGroupの段階でしか左右位置セットできないので
		// バカらしいけど2つ用意する
		TUGLiTmFilter uglistTmFilterL_;
		TUGLiTmFilter uglistTmFilterR_;
		TUGDrTeam ugDrTeam_;

		//メンバーリスト
		TUGLiMember uglistMember_;

		// キャラステータスプレート
		TUGChStatePlate ugChStatePlate_;


		s32 id_supply_;			// 選択したコーチ
		s64 mid_team_select_;	// 選択したチームMID
		s32 posNoSelect_;		// 選択したキャラのポジション番号
		s64 id_team_select_;	// 選択したチームID

		s64 mid_team_sell_;

		std::vector<TUGLiGen*> listvec_;
		s32 subPage_;
		s32 waitTimer_;		// 結果見せ用タイマー

		BOOL isChangePriceType_;	// 購入形式が変わったタイミングで立てるフラグ

		// 今回限り
		TUGTitle ugtitle_;
		TUGTeam ugteam_;
	};
}
