#pragma once
#include "dbLyTuGen.h"
#include "dbUGSpMami.h"
#include "dbUGButton.h"
#include "dbUGLiMenu.h"
#include "dbUGLiLogGetItem.h"
#include "dbUGLiLogUseItem.h"
#include "dbUGLogboIcon.h"
#include "dbUGPlateProf.h"
namespace db
{
	class TLyTuHmMami :public TLyTuGen
	{
		struct TMamiSt
		{
			BOOL msgset_f_;
			s32 wait_c_;
			s32 pageNo2_;
			s32 logpage_;
			s64 mid_cpcode_;
			s32 codeketa_;

			BOOL logbo_f_;
			s64 nextitemType_;
			s64 nextitemID_;
			s64 nextitemNum_;

			s32 endpage_;

			void Init()
			{
				Zeromem(this);

				wait_c_ = 60;
			}
		};

	public:
		TMamiSt stMami_;
		//コンストラクタ
		explicit TLyTuHmMami(TSnTutorial* pScene);
		virtual ~TLyTuHmMami(void);

	protected:
		TUGChGirl* ugmami_;
		TUGLiLogGetItem uglistGetLog_;
		TUGLiLogUseItem uglistUseLog_;
		TUGPlateProf ugprof_;
		TUGLogboIcon* uglogbo_;

		TUGSerifu* ugserifu_;	//セリフ
		STRVECTOR serifuvec_;
		
		void DrawList(TUGLiGen* drawlist);
		void ResetObj();
		void SwitchLogPageBtn();
		void RefreshCpCodeList();

		void MvPage_Visit();
		void MvPage_Logbo();
		void MvPage_LogboSp();
		void MvPage_LogboSpYokoku();
		void MvPage_LogboResult();

		void MvPage_Out();

		void SetEndPage();

		//通信
		void Post_Logbo();
		void Post_LogboSp();
		void Connect_Logbo();

		//オーバーロード
		void DoLoad();
		void DoReady();
		void DoDown();
		void DoChangePage(s32 pageno);
		void DoConnect(BOOL rqok_f);
		void DoUpdate(BOOL draw_f);

		// メッセージセット
		void SetMsg(s32 type);
		void SetMsg(const char* text0, const char* text1 = NULL, const char* text2 = NULL);
	};

}
