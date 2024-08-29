#pragma once
#include "dbLyHmGen.h"
#include "dbUGLiHaikei.h"
#include "dbUGDrHaikei.h"
#include "dbUGFilter.h"
//=====================================================================================================
//	
//
//
//=====================================================================================================

namespace db
{

	struct TUGHaikeiListSt
	{
		s32 titleid_;
		s32 iconno_;
		s32 mid_haikei_;
		BOOL dramdraw_f_;
		void Init()
		{
			Zeromem(this);
			titleid_ = NGNUM;
			mid_haikei_ = NGNUM;
		}
	};

	enum enUGHaikeiAct
	{
		enUGHaikeiAct_Select = 0,
	};


	class TUGHaikeiList :public TUGGen
	{
	public:
		//コンストラクタ
		explicit TUGHaikeiList(TUIGenLayer* player);
		virtual ~TUGHaikeiList(void);

		void MakeGroup(TUIObj* parent);
		BOOL IsAct(void);

		void Ready(void);
		void Ready(s64 mid_event);
		void Down(void);

		s64 GetHaikeiID(void){ return stHaikei_.mid_haikei_; }

	protected:
		void RefreshHaikeiDt(BOOL first_f);
		void SetTitle(s32 recordno);

		void MvPage_Haikei();

		//オーバーロード
		void DoUpdate(BOOL draw_f);
		void DoAction(void);

		TUGHaikeiListSt stHaikei_;
		TUIObj* nowtitle_;//ヘッダ現在タイトル表示
		TUIObj* nowcourt_;//フッタ現在コート表示
		TUGLiHaikei uglistHaikei_;//タイトルリスト
		INTVECTOR mid_haikei_vec_;
		TUGDrHaikei ugdrum_haikei_;//ドラム
		TUGFilter ugfilter_;

		s64 mid_event_;	// 特効用
	};

}
