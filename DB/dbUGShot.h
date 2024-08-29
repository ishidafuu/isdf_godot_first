#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"
#include "dbUGButton.h"
#include "dbUGRSozaiBase.h"
#include "dbUGRQuality.h"

namespace db
{
  struct TShotSt
  {
    s64 id_shot_;
    s64 id_team_;
    s32 urano_;
    BOOL tipsdraw_f_;
    BOOL tipsmade_f_;
    void Init()
    {
      Zeromem(this);
      id_shot_ = NGNUM;
    }
  };

  //リスト
  class TUGShotUra;
  class TUGShot : public TUGGen
  {
  public:
    TShotSt stShot_;
    //コンストラクタ
    explicit TUGShot(TUIGenLayer* player);
    virtual ~TUGShot(void);
    void MakeGroup(TUIObj* parent);
    void MakeUra();
    void RefreshTips();
    void SetSlot(s32 slotno, s64 mid_sozai, s32 rarity, s32 baserarity);
    void SetShotDt(s64 id_shot, s64 id_team, BOOL first_f, TSortShot* sortshot);
    s32 SetShotDtGousei(LONGVECTOR* id_shot_mixvec);
	s32 SetShotDtGousei(SOZAIDTVECTOR id_shot_mixvec);
    void SetShotDtSell(s64 id_shot);
    void SetShotDtBaraLast(s64 id_shot_base);
    void SetShotDtSozai(TSozaiData sozaidt);
    void SetTipsDraw_f(BOOL tipsdraw_f);
	void SetItemGet(s64 mid_sozairarity, s64 pow, s64 spin);
    void SetLRPos(BOOL leftpos_f);
    void SetUra(BOOL ura_f);
    BOOL IsUra();
  protected:
    void DrawPowSpin(s32 rarity, s32 basepow, s32 basespin, s32 revpow, s32 revspin, BOOL indv_f);
    void DoUpdate(BOOL draw_f);
    void DoAction();
    void ClearSlot();
    TUGRName ugname_;
    TUGRName ugname2_;
    TUGRIcon uglock_;
    TUGRQuality ugqtpow_;
    TUGRQuality ugqtspin_;
    TUGRIcon uggenre_;
    TUGRSozaiBase ugbpow_;
    TUGRSozaiBase ugbspin_;

    //TUIObj* body_;
    //TUIObj* revpower_;
    //TUIObj* revspin_;
    TUIObj* slot_[HSELEMMAX];
    //TUIObj* shotinvno_;
    TUIObj* memname_;
    TUIObj* teamname_;

    TUGShotUra* ura_;
  };
 
}
