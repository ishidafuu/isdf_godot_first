#include "dbUGSpSupply.h"


namespace db
{

  enum enItemDraw
  {
    itd_None,
    itd_Carry,
    itd_Table,
  };


  //コンストラクタ
  TUGSpSupply::TUGSpSupply(TUIGenLayer* player) 
	  :TUGGen(player)
	  , ugclerk_(player)
	  , ugkantoku_(player)
	  , ugcap_(player)
	  , ugserifu_(player)
  {
    stSupply_.Init();
  }

  TUGSpSupply::~TUGSpSupply()
  {

  }

  void TUGSpSupply::MakeGroup()
  {

    field_->o_SetObjFilterGrp(448, 168, 0xFF, 0x38, 0x68, 0x9A);
    //field_->z_SetPos_SafeLF(0, 0);
    field_->g_SetCenter(TRUE);
    //field_->z_SetPos_SafeLF(0, 0);

    inside_ = MakeObj();
    inside_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_shop));
    inside_->o_SetParent(field_);
    inside_->z_SetPos(-160, -60);

    itemicon_ = MakeObj();
    itemicon_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_supply));
    itemicon_->o_SetParent(inside_);
    itemicon_->z_SetPos(48, 36);


    ugserifu_.MakeGroup(field_, 3, enSerifuFrame_Normal, UIDP_03OBJ);//とりあえず3行
    ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);
    ugserifu_.SetPos(-160, 28);

    ugkantoku_.MakeGroup(inside_, FALSE);
    ugkantoku_.SetPos(76 + 60, 72);
    ugkantoku_.SetMirror(TRUE);

	ugcap_.MakeGroup(inside_);
	ugcap_.SetPos(76 + 60 + 26, 72);
	ugcap_.SetMirror(TRUE);
	ugcap_.SetWalkType(cwt_MYWALK);	// 自分で歩かせる
	ugcap_.GetField()->g_SetLimX(-160, 152);

    ugclerk_.MakeGroup(inside_, ct_Supply);
    ugclerk_.SetPos(40, 72);

  }
  void TUGSpSupply::Ready()
  {
    stSupply_.Init();
    SetMsg(spmt_Visit);
  }
  void TUGSpSupply::SetIconPartNo(s32 partno)
  {
    itemicon_->g_SetPartNo(partno);
  }

  void TUGSpSupply::GenSetMsg(const char* text0)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpSupply::GenSetMsg(const char* text0, const char* text1)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpSupply::GenSetMsg(const char* text0, const char* text1, const char* text2)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    serifuvec_.push_back(text2);
    ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }

  //メッセージ＆連動アクション選択
  void TUGSpSupply::SetMsg(enSupplyMsgType msgtype)
  {
    stSupply_.mtype_ = msgtype;

    //メッセージタイプごとの店員と監督の挙動
    switch (msgtype)
    {
    case db::spmt_Visit:

      GenSetMsg("掛「いらっしゃいませ＿こんにちは！", "ここは＿アイテムショップです心");
      ugkantoku_.SetMirror(TRUE);
      ugkantoku_.z_MoveWork(0, 76, 72, 1);
	  ugcap_.SetMirror(TRUE);
	  ugcap_.z_MoveWork(0, 76 + 26, 72, 1);
      break;
    case db::spmt_Menu:
      GenSetMsg("掛「おきゃくさま＿ごちゅうもんは", "いかがなさいますか？");
      break;
    case db::spmt_Order:
      GenSetMsg("掛「こちらで＿よろしいですか？");
      break;
    case db::spmt_Order2:
      GenSetMsg("掛「どうも＿ありがとうございます", "ほかには＿ございますか？");
      break;
    case db::spmt_Buy:
      GenSetMsg("掛「どうも＿ありがとうございます");
      ugclerk_.SetMType(cmt_Hand);
      ugkantoku_.SetMType(kmt_Take);
      break;
    case db::spmt_NoMoney:
      GenSetMsg("掛「あの黙黙＿おきゃくさま黙黙", "スターが＿たりないですね");
      break;
    case db::spmt_NoSpace:
      GenSetMsg("掛「あの黙黙＿おきゃくさま黙黙", "もちものが＿いっぱいですよ");
      break;
    case db::spmt_Out:
      GenSetMsg("掛「ありがとうございました心");
      ugkantoku_.SetMType(kmt_Wk);
      ugkantoku_.SetMirror(FALSE);
      ugkantoku_.z_MoveWork(0, 76 + 60, 72, 1);
	  ugcap_.SetMType(cmt_Wk);
	  ugcap_.SetMirror(FALSE);
	  ugcap_.z_MoveWork(0, 76 + 60 + 26, 72, 1);
      break;
    default:
      break;
    }
  }

  void TUGSpSupply::DrawBuyIcon(s32 no)
  {
    itemicon_->g_SetPartNo(no);
    itemicon_->g_SetDraw(TRUE);
    stSupply_.itemdraw_c = 30;
  }


  void TUGSpSupply::DoUpdate(BOOL draw_f)
  {
    //進行
    field_->Update(draw_f);
    inside_->Update(draw_f);
    itemicon_->Update(draw_f);

    ugclerk_.Update(draw_f);
    ugkantoku_.Update(draw_f);
	ugcap_.Update(draw_f);
    ugserifu_.Update(draw_f);
  }


  void TUGSpSupply::Mv_Visit()
  {
    Mv_Default();
    if (ugkantoku_.IsActRes(enUGChGenAct_MoveStop)) ugkantoku_.SetMType(kmt_St);
	if (ugcap_.IsActRes(enUGChGenAct_MoveStop)) ugcap_.SetMType(cmt_St);
  }
  void TUGSpSupply::Mv_Menu()//メニュー
  {
    Mv_Default();
  }
  void TUGSpSupply::Mv_Out()//退出時
  {
    Mv_Default();
  }
  void TUGSpSupply::Mv_Order()//初注文
  {
    Mv_Default();
  }
  void TUGSpSupply::Mv_Order2()//再注文
  {
    Mv_Default();
  }
  void TUGSpSupply::Mv_Buy()//購入時
  {
    Mv_Default();
  }
  void TUGSpSupply::Mv_NoMoney()//金不足
  {
    Mv_Default();
  }
  void TUGSpSupply::Mv_NoSpace()//空不足
  {
    Mv_Default();
  }

  void TUGSpSupply::Mv_Default()
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))//セリフ読み終わり
    {
      ugclerk_.SetMType(cmt_Stand);
    }
    else if (ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))//セリフウエイト
    {
      SetAct(0);//アクション
    }
  }


  void TUGSpSupply::DoAction()
  {
    if (lib_num::AprTo0(&stSupply_.itemdraw_c))
    {
      itemicon_->g_SetDraw(FALSE);
    }

    //メッセージタイプごとの店員と監督の挙動
    switch (stSupply_.mtype_)
    {
    case spmt_Visit: Mv_Visit(); break;//入店時
    case spmt_Menu: Mv_Menu(); break;//メニュー
    case spmt_Out: Mv_Out(); break;//退出時
    case spmt_Order: Mv_Order(); break;//初注文
    case spmt_Order2: Mv_Order2(); break;//再注文
    case spmt_Buy:Mv_Buy(); break;//購入時
    case spmt_NoMoney:Mv_NoMoney(); break;//金不足
    case spmt_NoSpace:Mv_NoSpace(); break;//空不足
    }

	// メッセージスキップ処理
	if (field_->sc_IsCamEnter())
	{
		if (ugserifu_.IsReadLineEnd())
		{
			ugserifu_.NextSerifu();
			ugclerk_.SetMType(cmt_Stand);
		}
		else
		{
			ugserifu_.SkipSerifu();
			ugclerk_.SetMType(cmt_Stand);
		}
	}
  }
}
