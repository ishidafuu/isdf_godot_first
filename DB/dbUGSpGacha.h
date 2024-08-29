#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGSerifu.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"
#include "dbUGChSensyu.h"
#include "dbUGChClerk.h"
#include "dbUGGacha.h"
#include "dbUGNaBox.h"

namespace db
{

  enum enUGSpGachaAct
  {
    enUGSpGachaAct_SerifuEnd,
    enUGSpGachaAct_GachaEnd,
    enUGSpGachaAct_CupsuleOut,
  };

  //ショップメッセージ
  enum enGachaMsgType
  {
    gamt_Visit,//入店時
    gamt_Menu,//メニュー
    gamt_Out,//退出時
    gamt_SelectStar, //スターガチャ
    gamt_DecideStar, //スターガチャ購入確認
    gamt_SelectNaBox, //スターガチャ
    gamt_DecideNaBox, //スターガチャ購入確認
    gamt_SelectTodd, //スターガチャ
    gamt_DecideTodd, //スターガチャ購入確認
    gamt_Back,//戻り
    gamt_Buy,//購入時
    gamt_NoMoney,//金不足
	
	gamt_JumpInStar,	// スターガチャに飛んできた
	gamt_JumpInTodd,	// トッドガチャに飛んできた
	gamt_JumpInNaBoxFromHome,	// なつかしBOXに飛んできた(ホームから)
	gamt_JumpInNaBoxFromTeam,	// なつかしBOXに飛んできた(チームから)

    gamt_Gacha,//ガチャ中
    gamt_NaBox,//ガチャ中
    gamt_NaBoxOtona,//ガチャ中
    //チュートGacha
    gamt_TuGachaVisit,//入店時
    gamt_TuGachaInfo,//せつめい
    gamt_TuReady,//トッドを選べ
    gamt_TuSelectTodd,//１０れんを選べ
	gamt_TuCheckTodd, // 購入確認
    gamt_TuDecideTodd, //購入許可
    gamt_TuGacha, //ぼたんおせ
    gamt_TuBuy,//購入時

    gamt_TuNNaBoxVisit,//つぎはなつかし（中断されたときのみ）
    gamt_TuNNaBoxInfo,//つぎはなつかし
    gamt_TuNNaBoxInfo2,//せつめい
    gamt_TuNReady,//なつかしを選べ
    gamt_TuNSelectNaBox,//はじめてＢＯＸを選べ
    gamt_TuNSelectNaBoxNum,//おとながいを選べ
    gamt_TuNDecideNaBox, //購入確認
    gamt_TuNNaBox, //ぼたんおせ
    gamt_TuNBuy,//購入時

    gamt_TuLastInfo,//つぎはなつかし
    gamt_TuOut,//さらば
    gamtEND,
  };

  struct TUGSpGachaSt
  {
    enGachaMsgType mtype_;//店員メッセージタイプ
    s32 itemdraw_c;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGSpGacha : public TUGGen
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
    void Mv_Gacha();//ガチャ中
    void Mv_BoxGacha();//ガチャ中
    void Mv_Default();


    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGSpGachaSt stGacha_;
    TUGSerifu ugserifu_;//セリフ
    TUGGacha uggacha_;
    TUGNaBox ugnabox_;
    //コンストラクタ
    explicit TUGSpGacha(TUIGenLayer* player);
    virtual ~TUGSpGacha(void);
    void MakeGroup();
    void SetItemIcon(TUIObj* itemicon);
    void SetIconGrp(base::TGrPart* icongrp);
    void SetIconPartNo(s32 partno);
    void SetMsg(enGachaMsgType msgtype);
    void DrawBuyIcon(s32 no);
    void Ready();

    void TapGachaField();
    void TapBoxGachaField();
    void NaBoxRefresh(s64 mid_nabox);

    BOOL IsActRes(enUGSpGachaAct res);//返値ゲット
  };

}
