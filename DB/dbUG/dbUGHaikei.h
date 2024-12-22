#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"
#include "dbUGButton.h"

namespace db
{
  struct THaikeiSt
  {
    s32 posx_;
    s32 posy_;
    s32 www_;
    TRect parentrect_;
    BOOL tipsdraw_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGHaikei : public TUGGen
  {
  protected:
    TUIObj* picture_;
     TUGRName ugname_;
	 TUIObj* ugFade_;	// 持ってない背景に使う黒フェード
	 TUIObj* ugIcon_;
	 TUIObj* ugNo_;		// 番号
	 TUIObj* ugDate_;	// 発売日表記

    void SetHaikeiPosGen();
    void DoUpdate(BOOL draw_f);
  public:
    THaikeiSt stHaikei_;
    //コンストラクタ
    explicit TUGHaikei(TUIGenLayer* player);
    virtual ~TUGHaikei(void);
    void MakeGroup(TUIObj* parent);// , base::TGrPart* grpicture);
    void SetHaikeiDt(base::TGrp* pgrp, s32 mid_haikei);
    void SetHaikeiDt_ItemView(s64 mid_haikei);
    void SetTipsDraw_f(BOOL tipsdraw_f);
    BOOL IsSameGrp(base::TGrp* pgrp);
    void SetLRPos(BOOL leftpos_f);
	void SetHide(BOOL isHide);
  };
 
}
