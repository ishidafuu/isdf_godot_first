#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGSerifu.h"
#include "dbUGChGirl.h"


namespace db
{
	//ショップメッセージ
	enum enHsbMsgType
	{
		hsm_Visit,
		hsm_Menu,
		hsm_SelectTeam,
		hsm_SelectChara,
		hsm_SelectTraining,
		hsm_TeamDisplay,
		hsm_Out,
		hsm_Etc,
	};


	struct TUGSpHasebeSt
	{
		enHsbMsgType mtype_;//店員メッセージタイプ
		void Init()
		{
			Zeromem(this);
		}
	};

	class TUGSpHasebe : public TUGGen
	{

	public:
		TUGSpHasebeSt stMvk_;
		TUGSerifu ugserifu_;//セリフ

		//コンストラクタ
		explicit TUGSpHasebe(TUIGenLayer* player);
		virtual ~TUGSpHasebe(void);
		void MakeGroup(TUGChGirl* uggirl);
		void SetStopMsg();
		void SetMsg(enHsbMsgType msgtype, s64 number = NGNUM);


	protected:
		void GenSetMsg(const char* text0);
		void GenSetMsg(const char* text0, const char* text1);
		void GenSetMsg(const char* text0, const char* text1, const char* text2);

		void Mv_Default();

		void DoUpdate(BOOL draw_f);
		void DoAction();

		TUIObj* inside_;
		TUGChGirl* uggirl_;

		TUIObj* logbostar_;
		TUIObj* logbosp_;

		STRVECTOR serifuvec_;
		s32 timer_;
	};

}
