//utf8
#pragma once
#include "dbLyTuBase.h"
#include "dbUGChKantoku.h"
#include "dbUGChGirl.h"
#include "dbUGButton.h"
#include "dbUGDoor.h"
#include "dbUGChCaptain.h"
//=========================================================================
// 
// チュートリアル：ホーム画面
//
//=========================================================================
namespace db
{
  struct TTuShopSt
  {
    s32 inshopNo_;
    void Init()
    {
      Zeromem(this);
    }
  };

	class TLyTuShop :public TLyTuBase
	{
		// 基底クラス定義
		typedef TLyTuBase base;

		struct CharaDt
		{
			s32 talkCategory_;	// 喋りのカテゴリー番号
			s32 talkNo_;		// 今喋っているテキスト番号
			void Init(void)
			{
				talkCategory_ = 0;
				talkNo_ = 0;
			}
		};


	public:
		//コンストラクタ
		explicit TLyTuShop(TSnTutorial* pScene);
		virtual ~TLyTuShop(void);

		// 真実メニューからの復帰用
		void ComebackInMamiMenu(void);

		// プロフィールからの復帰用
		void ComebackProfile(void);

		// セリフ用ウィンドウのゲッター
		inline TUGSerifu* GetSerifuWindow(void){ return &ugserifu_;}
    //がちゃ屋から出るとき呼ばれる
    void OutShop();
    //チュート終了フラグを立てる
    void SetTutoEnd();

	protected:
    static const s32 SHOPNUM = 3;
    TUIObj* shop_[SHOPNUM];
    TUGChKantoku ugkantoku_;
    TUGChCaptain ugcap_;
    TUGButton ugbln_gacha_;

    std::vector<TUGChGirl*> ugjk_;
    std::vector<TUGDoor*> ugdoor_;
    TTuShopSt stHmShop_;

		CharaDt	captainDt_;
		CharaDt mamiDt_;
		s32 editErrCnt_;

		//オーバーロード
		void DoLoad(void);
		void DoReady(void);
		void DoChangePage(s32 pageno);
		void DoUpdate(BOOL draw_f);
	private:
		s32 subPage_;
    void MvPage_Init(void);
    void MvPage_CapTalk(void);
    void MvPage_Walking();
    void MvPage_Selected();
    void MvPage_InShop();
    void MvPage_GotoShop();
    void MvPage_OutShop();
    void MvPage_CapTalk2(void);
    void MvPage_Idle();
    void MvPage_GotoCenter(void);
	};

}
