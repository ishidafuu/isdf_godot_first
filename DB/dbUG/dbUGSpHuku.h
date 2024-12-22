#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGSerifu.h"
#include "dbUGChKantoku.h"
#include "dbUGChSensyu.h"
#include "dbUGChCaptain.h"
#include "dbUGChClerk.h"
#include "dbUGHuku.h"

namespace db
{

  enum enUGSpHukuAct
  {
    enUGSpHukuAct_SerifuEnd,
    enUGSpHukuAct_HukuEnd,
    enUGSpHukuAct_CupsuleOut,
  };

  //ショップメッセージ
  enum enHukuMsgType
  {
    hkmt_Visit,//入店時
    hkmt_Menu,//メニュー
	hkmt_Jump,// ジャンプで入店
    hkmt_Out,//退出時
    hkmt_Order,//初注文
    hkmt_Order2,//再注文
    hkmt_Buy,//購入時
    hkmt_NoMoney,//金不足
    hkmt_NoSpace,//空不足
    hkmt_Huku,//ガチャ中
    hkmtEND,
  };


  struct TUGSpHukuSt
  {
    enHukuMsgType mtype_;//店員メッセージタイプ
    s32 itemdraw_c;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGSpHuku : public TUGGen
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
    void Mv_Order();//初注文
    void Mv_Order2();//再注文
    void Mv_Buy();//購入時
    void Mv_NoMoney();//金不足
    void Mv_NoSpace();//空不足
    void Mv_Huku();//ガチャ中
    void Mv_Out();//退出時
    void Mv_Default();


    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGSpHukuSt stHuku_;
    TUGSerifu ugserifu_;//セリフ
    TUGHuku ugHuku_;
    //コンストラクタ
    explicit TUGSpHuku(TUIGenLayer* player);
    virtual ~TUGSpHuku(void);
    void MakeGroup();
    void SetItemIcon(TUIObj* itemicon);
    void SetIconGrp(base::TGrPart* icongrp);
    void SetIconPartNo(s32 partno);
    void SetMsg(enHukuMsgType msgtype);
    void DrawBuyIcon(s32 no);
    void Ready();

    void TapHukuField();
    BOOL IsActRes(enUGSpHukuAct res);//返値ゲット
  };
 
}
