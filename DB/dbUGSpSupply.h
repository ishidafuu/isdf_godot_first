#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGSerifu.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"
#include "dbUGChSensyu.h"
#include "dbUGChClerk.h"

namespace db
{
  //ショップメッセージ
  enum enSupplyMsgType
  {
    spmt_Visit,//入店時
    spmt_Out,//退出時
    spmt_Menu,//メニュー
    spmt_Order,//初注文
    spmt_Order2,//再注文
    spmt_Buy,//購入時
    spmt_NoMoney,//金不足
    spmt_NoSpace,//空不足
    spmtEND,
  };

  struct TUGSpSupplySt
  {
    enSupplyMsgType mtype_;//店員メッセージタイプ
    s32 itemdraw_c;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGSpSupply : public TUGGen
  {
  protected:
    TUIObj* blueback_;
    TUIObj* inside_;
    TUIObj* itemicon_;//品物

    TUGChClerk ugclerk_;
    TUGChKantoku ugkantoku_;
	TUGChCaptain ugcap_;

    STRVECTOR serifuvec_;

    void GenSetMsg(const char* text0);
    void GenSetMsg(const char* text0, const char* text1);
    void GenSetMsg(const char* text0, const char* text1, const char* text2);

    void Mv_Visit();//入店時
    void Mv_Menu();//メニュー
    void Mv_Out();//退出時
    void Mv_Order();//初注文
    void Mv_Order2();//再注文
    void Mv_Buy();//購入時
    void Mv_NoMoney();//金不足
    void Mv_NoSpace();//空不足
    void Mv_Default();


    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGSpSupplySt stSupply_;
    TUGSerifu ugserifu_;//セリフ
    //コンストラクタ
    explicit TUGSpSupply(TUIGenLayer* player);
    virtual ~TUGSpSupply(void);
    void MakeGroup();
    void SetItemIcon(TUIObj* itemicon);
    //    void SetIconGrp(base::TGrPart* icongrp);
    void SetIconPartNo(s32 partno);
    void SetMsg(enSupplyMsgType msgtype);
    void DrawBuyIcon(s32 no);
    void Ready();
  };

}
