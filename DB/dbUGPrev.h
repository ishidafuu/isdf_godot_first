#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
#include "dbUGRadio.h"
#include "dbUGShPad.h"
#include "dbUGShLever.h"

namespace db
{
  enum  enUGPrevAct
  {
    enUGPrevAct_back,
    enUGPrevAct_hit,
		enUGPrevAct_mix,
		enUGPrevAct_mix_clear,
		enUGPrevAct_have_mix,
  };

  struct TPrevSt
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
  struct TPrevParam
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
    TPrevParam(){ Init();}
    void Init(){
      Zeromem(this);
    }
  };

  class TUGPrev :public TUGGen
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
  public:
    TPrevSt stPrev_;
    TUIObj* court_;
    TUIObj* line_;
    //TUIObj* ctrl_;
    TUGButton ugbtn_back_;//ＯＫボタン
		TUGButton ugbtn_mix_;//mixボタン
		TUGButton ugbtn_mix_clear_;//mixクリアボタン
    TUGShPad ugpad_shot_;
    //TUGShPad ugpad_pass_;
    TUGShPad ugpad_jump_;
    TUGShLever uglever_;
    //コンストラクタ
    explicit TUGPrev(TUIGenLayer* player);
    //explicit TUGPrev(TUIGenLayer* player, TMgEngineObj* pmgEO, TMgUI* pmgUI);
    virtual ~TUGPrev(void);
    void MakeGroup(TUIObj* parent);
    //void Ready(s64 id_team, s64 id_shot, BOOL backbtn_f);
    void Ready(const TPrevParam& param);
    void ChangeShot_Sozai(const SOZAIDTVECTOR& sozaivec);

		void SetMixSozai(s64 id_sozai, BOOL isRndSet);
		void SetMixShot(s64 id_shot, BOOL isRndSet);

    void ChangeShot_Mixed(LONGVECTOR* id_shot_mixvec);
    void Down();
    BOOL IsActRes(enUGPrevAct res);

		/* 2015/06/17 Koike Added */
		// 手持ちランダムミックスボタン
		TUGButton ugbtn_have_mix_; //手持ちmixボタン
  };

}
