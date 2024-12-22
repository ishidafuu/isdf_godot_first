#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
#include "dbUGRadio.h"
#include "dbUGBtnShTime.h"
#include "dbUGShCount.h"
#include "dbUGShCatch.h"
#include "dbUGShKantoku.h"
#include "dbUGBtnShCtrl.h"
#include "dbUGBtnShAI.h"
#include "dbUGShPad.h"
#include "dbUGShLever.h"

namespace db
{
 
  enum  enUGShiaiAct
  {
    enUGShiaiAct_pause,
    enUGShiaiAct_retry,
    enUGShiaiAct_result,
    enUGShiaiAct_tuto,//ActIndexで内容を判断
    enUGShiaiAct_gameset,
  };


  const s32 AINUM = 4;
  const s32 PADBTN = 3;


  struct TShiaiSt
  {
    s32 aiNo_;//選択されたパネル番号
    BOOL order_f_;//指示待ち
    BOOL enterCatch_f_;//キャッチ押され瞬間

    BOOL changeAI_f_;
    s32 time_;
    BOOL puase_f_;

    BOOL manualA_f_;
    BOOL manualB_f_;
    BOOL manualJ_f_;

    BOOL cancelPause_f_;

    //BOOL tuto_f_;
    BOOL vs_f_;

    BOOL hidescore_f_;
    BOOL hideretro_f_;
    s32 retrotutostep_;

    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGShiai :public TUGGen
  {
  public:
    //コンストラクタ
    explicit TUGShiai(TUIGenLayer* player);
    virtual ~TUGShiai(void);
    void MakeGroup(TUIObj* parent);
    void Ready_Shiai();
    void Ready_Tuto(BOOL retro_f);
    void Ready_VS();
    void CanselPause();
    void Retry();
    void SendTutoAct(s32 tutoact);
    void SendTutoGameAct(BOOL tonormal_f);
    void SetHideScore(BOOL hide_f);
    void SetHideRetro(BOOL hide_f);
    void SetRetroStep(s32 totostep, TUGShPad::enPadMsgType of_f);
    void Dissconnect();
    BOOL IsActRes(enUGShiaiAct res);
    s32 GetCtrl();

    TUGBtnShTime ugbtn_time_;
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();

    void SerifuSelect(enKantokuSerifu sno);
    void MvAuto();
    void MvSmart();
    void MvRetro();
    void MvTech();
	void MvDebug(void);
    void PauseSetDraw(BOOL draw_f);
    void PauseBtnAct();
    void SetPadAuto(TMgPad* pmgPad);
    void SetPadSmart(TMgPad* pmgPad);
    void SetPadRetro(TMgPad* pmgPad);
    void SetPadTech(TMgPad* pmgPad);


    TUIObj* header_;//ヘッダーフィールド
    //TUIObj* order_;
    TUGBtnShCtrl ugbtn_ctrl_;
    TUGShCatch ugcatch_;
    TUGShCount ugcount_;
    TUGShKantoku ugkantoku_;
    TUGShAI ugai_;
    TUGShPad ugpad_shot_;
    TUGShPad ugpad_pass_;
    TUGShPad ugpad_jump_;
    TUGShLever uglever_;
    TShiaiSt stShiai_;

	TUGButton ugKillBtn_;
  };
 
}
