#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
#include "dbUGRadio.h"
#include "dbUGShPad.h"
#include "dbUGShLever.h"
#include "dbUGBtnLabel.h"

namespace db
{
	enum  enUGMixPrevAct
	{
		enUGMixPrevAct_back,
		enUGMixPrevAct_cancel,
		enUGMixPrevAct_hit,
		enUGMixPrevAct_ok,
		enUGMixPrevAct_mix_clear,
		enUGMixPrevAct_shop_mix,
		enUGMixPrevAct_have_mix,
		enUGMixPrevAct_correct,
		enUGMixPrevAct_ball_0, 
		enUGMixPrevAct_ball_1,// 2はベースなので基本は操作しない
		enUGMixPrevAct_ball_2,
		enUGMixPrevAct_ball_3, 
		enUGMixPrevAct_ball_4,
		enUGMixPrevAct_ball_5,
		enUGMixPrevAct_ball_6,
		enUGMixPrevAct_ball_7,
		enUGMixPrevAct_Select_ball,
	};

	enum enUGMixPrevSelectMode
	{
		enUGMixPrevSelectMode_none,
		enUGMixPrevSelectMode_select,
		enUGMixPrevSelectMode_exchange,
	};

	struct TMixPrevSt
	{
		BOOL mvgame_f_;
		s64 id_team_prev_;
		s32 char0_;
		s32 aiNo_;//選択されたパネル番号
		BOOL order_f_;//指示待ち
		BOOL enterCatch_f_;//キャッチ押され瞬間
		s32 ctrltype_;

		void Init()
		{
			Zeromem(this);
			id_team_prev_ = NGNUM;	// 0だとダミーチームと被るため-1で初期化
		}
	};
	struct TMixPrevParam
	{
		s64 id_team;
		s64 id_shot;
		BOOL nonebackbtn_f;
		BOOL nonemixbtn_f;
		BOOL nonehavemixbtn_f;
		s32 char0;
		s32 char1;
		s32 char2;
		s32 char3;
		s32 char4;
		SOZAIDTVECTOR* sozaivec;
		TMixPrevParam(){ Init(); }
		void Init(){
			Zeromem(this);
		}
	};

	class TUGMixPrev :public TUGGen
	{
	protected:
		void NextCtrl();
		//void SetPadAuto(TMgPad* pmgPad);
		void SetPadSmart(TMgPad* pmgPad);
		void SetPadRetro(TMgPad* pmgPad);
		//void SetPadTech(TMgPad* pmgPad);

		void MvPrevGame();

		void MixClearSozai(s64 id_sozai);
		void MixClearShot(s64 id_shot);

		void ResetMixSozai(s64 id_sozai, BOOL isRndSet);
		void ResetMixShot(s64 id_shot, BOOL isRndSet);
		void SelectMixSozai(s32 lowLimit, s32 upLimit);
		SOZAIDTVECTOR sozaiVec_;

		void DoUpdate(BOOL draw_f);
		void DoAction();

		//std::vector<TUGBtnLabel*> ugbtn_shot_;
		std::vector<TUGButton*> ugbtn_shot_;
		TUGRadio ugradio_shot_;
		TUIObj* slot_[HSELEMMAX];
		void ClearSlot();
		void SetSlot(s32 slotno, s64 mid_sozai, s32 rarity, s32 baserarity);

		LONGVECTOR id_shot_mixvec_custum;
		SOZAIDTVECTOR changeVec_;
		s32 selectMode;
		s32 baserarelity;
		s32 baseposition;
		s32 fromno;
		s32 tono;
		void ExchangeBall(LONGVECTOR* id_shot_mixvec);
		void ResetBtn(s32 btnno);

	public:
		TMixPrevSt stPrev_;
		TUIObj* court_;
		TUIObj* line_;
		//TUIObj* ctrl_;
		TUGButton ugbtn_back_;//戻るボタン
		TUGButton ugbtn_cancel_;//やめるボタン
		TUGButton ugbtn_ok_;//OKボタン
		TUGButton ugbtn_change_;//さいごをはずすorベースをかえるボタン
		TUGButton ugbtn_mix_shop_;//みせせんたくorみせランダムボタン
		TUGButton ugbtn_mix_have_;//てもちランダムボタン
		TUGButton ugbtn_select_have_;//てもちせんたくボタン
		TUGShPad ugpad_shot_;
		//TUGShPad ugpad_pass_;
		TUGShPad ugpad_jump_;
		TUGShLever uglever_;
		//コンストラクタ
		explicit TUGMixPrev(TUIGenLayer* player);
		//explicit TUGPrev(TUIGenLayer* player, TMgEngineObj* pmgEO, TMgUI* pmgUI);
		virtual ~TUGMixPrev(void);
		void MakeGroup(TUIObj* parent);
		//void Ready(s64 id_team, s64 id_shot, BOOL backbtn_f);
		void Ready(const TMixPrevParam& param);
		void ChangeShot_Sozai(const SOZAIDTVECTOR& sozaivec);

		void SetMixSozai(s64 id_sozai, BOOL isRndSet);
		void SetMixShot(s64 id_shot, BOOL isRndSet);

		void ChangeShot_Mixed(LONGVECTOR* id_shot_mixvec);
		void Down();

		BOOL IsActRes(enUGMixPrevAct res);
		s32 IsTouchMode(LONGVECTOR* id_shot_mixvec, s32 iconNo);
		s32 GetTouchMode();
		void SetSelectShot(s64 id_shot);
		LONGVECTOR GetChangeSozaiVec();
		void ResetField(BOOL fieldflg);
	};

}
