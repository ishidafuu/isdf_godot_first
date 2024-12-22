#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
#include "dbUGBannerParam.h"
//=====================================================================================================
//
//	バナークラス
//
//=====================================================================================================
namespace db
{
	// バナー画像の切り替わりタイプ
	enum enDisplayChangeType
	{
		edct_SnowNoise = 0,	// 砂嵐
		edct_Custom,		// 鯖からDLしてきたものを使う
		edct_MAX,
	};

	// 文字の表示タイプ
	enum enTextDrawType
	{
		etdt_Scroll = 0,	// スクロール
		etdt_Flash,			// 点滅
	};

	class TUGBanner :public TUGGen
	{
	public:
		explicit TUGBanner(TUIGenLayer* player);
		virtual ~TUGBanner(void);
		void MakeGroup(TUIObj* parent);

		// パラメータのセット
		void SetParam(TUGBannerParam param);
		// バナーの設定(パラメータのセット後に呼ぶ)
		BOOL SetTab(s32 tabNo = 0);
		void SetNextTab(void);	// 次のタブへ
		void SetPrevTab(void);	// 前のタブへ

		// テキストデータのクリア(セットする際は最初に1回必ずクリアしておく)
		void ClearTextData(void);
		// テキストデータのセット(1行分)
		void SetTextData(std::string str, s32 type);
		// テキストデータのセット(全て)
		void SetTextData(STRVECTOR strVec, INTVECTOR typeVec);

		// 画面切替方法のクリア(セットする際は最初に1回必ずクリアしておく)
		void ClearDisplayType(void);
		// 画面切替方法セット(1個分)
		void SetDisplayChangeType(s32 no, s32 type);
		// 画面切替方法セット(全て)
		void SetDisplayChangeType(INTVECTOR typeVec);

		// バナー画像のリフレッシュ
		void Refresh(void);


		BOOL IsAct(void);
		s32 GetBannerID(void);

	protected:
		void DoChangePage(s32 page);
		void DoUpdate(BOOL draw_f);
		void DoAction(void);

		// 何もデータが入ってないとき用
		void SetupEmpty(void);


		// 遷移
		void MvPage_Init(void);
		void MvPage_BannerDisplay(void);
		void MvPage_BannerChange(void);

		// バナー
		void SetBanner(s64 mid_event_id);

		// 画面切替系
		void InitDispParam(void);
		BOOL SetDisplays(s32 no);
		void SetDrawDisplays(BOOL isDraw);
		void UpdateDispSnowNoise(s32 no);
		void UpdateDispCustom(s32 no);

		// テキスト系
		void InitTextParam(void);
		BOOL SetText(s32 no);
		void UpdateTextScroll(void);
		void UpdateTextFlash(void);

		s32 pageNo_;		// 内部遷移状態
	
		
		base::TGrp* pGrpBanner_;	// バナーの枠のデータ
		base::TGrp* pGrpBtnR_;		// ボタン右のデータ
		base::TGrp* pGrpBtnL_;		// ボタン左のデータ

		TUIObj* pFrame_;	// バナーの枠
		TUIObj*	pText_;		// テキスト
		std::vector<TUIObj*> pBannerVec_;		// バナー本体
		std::vector<TUIObj*> pDisplaysVec_;		// バナー切替用の画像
		TUGButton ugBtnPrev_;	// 前のタブへ
		TUGButton ugBtnNext_;	// 次のタブへ
		TUGButton ugBln_;	// バルーン

		s64 mid_banner_id_;	// バナー画像ID
		s32 tabNo_;			// 今表示してるタブ番号
		BOOL isTabBack_;		// タブ戻るか

		std::vector<TUGBannerParam> tabParam_;	// 各タブのパラメータ
		

		//------------------------------------------------------------
		// バナー画像切替
		INTVECTOR dispChangeTypeVec_;
		INTVECTOR animNoVec_;	// アニメーション用
		s32 dispDrawTimer_;		// 画面切替演出時間用タイマー


		//------------------------------------------------------------
		// テキスト描画
		STRVECTOR strVec_;	// 描画する文字列
		INTVECTOR textDrawTypeVec_;
		s32 textDrawNo_;		// 今描画している文字列番号

		// スクロールで使う変数
		s32 textWaitTimer_;	// テキスト用ウェイトタイマー
		s32 textWidth_;		// 文字列の横幅
		f32 textMoveX_;		// スクロール位置

		// フラッシュで使う変数
		f32 textFlashTimer_;	// フラッシュタイマー
		s32 textFlashNo_;		// 今のフラッシュタイム位置
	};
}
