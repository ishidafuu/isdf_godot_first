#include "dbUGSpHuku.h"


namespace db
{

  //コンストラクタ
  TUGSpHuku::TUGSpHuku(TUIGenLayer* player) :TUGGen(player),
    ugclerk_(player),
    ugkantoku_(player),
	ugcap_(player),
    ugserifu_(player),
    ugHuku_(player)
  {
    stHuku_.Init();
  }

  TUGSpHuku::~TUGSpHuku()
  {

  }

  void TUGSpHuku::MakeGroup()
  {

    field_->o_SetObjFilterGrp(448, 168, 0xFF, 0x38, 0x68, 0x9A);
    //field_->z_SetPos_SafeLF(0, 0);
    field_->g_SetCenter(TRUE);
    //field_->z_SetPos_SafeLF(0, 0);

    inside_ = MakeObj();
    inside_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_shop));
    inside_->g_SetPartNo(2);
    inside_->o_SetParent(field_);
    inside_->z_SetPos(-160, -60);

    itemicon_ = MakeObj();
    itemicon_->o_SetParent(inside_);
    itemicon_->z_SetPos(46, 32);

    ugserifu_.MakeGroup(field_, 3, enSerifuFrame_Normal, UIDP_03OBJ);//とりあえず3行
    ugserifu_.SetPos(-160, 28);

    ugkantoku_.MakeGroup(inside_, FALSE);
    ugkantoku_.SetPos(76 + 60, 72);
    ugkantoku_.SetMirror(TRUE);

	ugcap_.MakeGroup(inside_);
	ugcap_.SetPos(76 + 60 + 2, 72);
	ugcap_.SetMirror(TRUE);
	ugcap_.SetWalkType(cwt_MYWALK);	// 自分で歩かせる
	ugcap_.GetField()->g_SetLimX(-160, 152);

    ugclerk_.MakeGroup(inside_, ct_Hukubiki);
    ugclerk_.SetPos(40, 72);

    ugHuku_.MakeGroup(inside_, &ugkantoku_, &ugcap_, &ugclerk_);
  }
  void TUGSpHuku::Ready()
  {
    stHuku_.Init();
    SetMsg(hkmt_Visit);
  }
  void TUGSpHuku::SetIconGrp(base::TGrPart* icongrp)
  {
    itemicon_->o_SetObjGrPart(icongrp);
  }
  void TUGSpHuku::SetIconPartNo(s32 partno)
  {
    itemicon_->g_SetPartNo(partno);
  }

  void TUGSpHuku::GenSetMsg(const char* text0)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpHuku::GenSetMsg(const char* text0, const char* text1)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpHuku::GenSetMsg(const char* text0, const char* text1, const char* text2)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    serifuvec_.push_back(text2);
    ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }

  //メッセージ＆連動アクション選択
  void TUGSpHuku::SetMsg(enHukuMsgType msgtype)
  {
    stHuku_.mtype_ = msgtype;

    //#ifdef __K_MAKE__

    //	メニュー
  	//	掛「ムフフ＿アイテムと＿こうかんで
  	//	すてきな＿サービスをいたしますよ心
  	
    //	
    //	

    //	アイテムこうかん				
    //	※交換品所持				
    //	＊「こちらが＿おきゃくさまの＿
    //	こうかんに＿つかえるアイテムです

    //	アイテムこうかん				
    //	※交換品なし				
    //	＊「こうかんできる＿アイテムを
    // おもちではないようですね……

    //	ふくびき	
    //	全商品	
    //	＊「このふくびきを＿まわすのですね
    //	さあぁっ＿しふくのひとときをっ……▼

    //こうかん	
    //	全商品	
    //＊「おきゃくさま＿このアイテムと
    //	こうかんしても＿よろしいですか？


    //メッセージタイプごとの店員と監督の挙動
    switch (msgtype)
    {
    case db::hkmt_Visit:

      GenSetMsg("掛「あら＿いらっしゃいませ");
      ugkantoku_.SetMirror(TRUE);
      ugkantoku_.z_MoveWork(0, 76, 72, 1);
	  ugcap_.SetMirror(TRUE);
	  ugcap_.z_MoveWork(0, 76 + 2, 72, 1);
      break;
    case db::hkmt_Menu:
      GenSetMsg("掛「ふくびきけんは＿１００スターを", "つかうごとに＿おくられますよ");
      break;
	case db::hkmt_Jump:
		GenSetMsg("掛「おしらせの＿しょうひんは", "こちらに＿はいっていますよ＿ムフフ");
		break;
    case db::hkmt_Order:
      GenSetMsg("掛「こちらでよろしいですか？");
      break;
    case db::hkmt_Order2:
      GenSetMsg("掛「さらに＿ごりようなさいますか？");
      break;
    case db::hkmt_Buy:
      GenSetMsg("掛「ムフフ＿いかがでしょうか？");
      ugclerk_.SetMType(cmt_Hand);
      ugkantoku_.SetMType(kmt_Take);
      break;
    case db::hkmt_NoMoney:
      GenSetMsg("掛「ムフフ＿サービスに＿ひつような", "アイテムが＿たりないようですね黙黙");
      break;
    case db::hkmt_NoSpace:
      GenSetMsg("掛「あきがありませんね黙");
      break;
    case db::hkmt_Out:
      GenSetMsg("掛「ムフフ＿またいらしてください");
      ugkantoku_.SetMType(kmt_Wk);
      ugkantoku_.SetMirror(FALSE);
      ugkantoku_.z_MoveWork(0, 76 + 60, 72, 1);
	  ugcap_.SetMType(cmt_Wk);
	  ugcap_.SetMirror(FALSE);
	  ugcap_.z_MoveWork(0, 76 + 60 + 2, 72, 1);
      break;
    case db::hkmt_Huku://ガチャ初期化
      ugHuku_.Act_FallHuku();//ガチャ落ちてくる命令
      break;
    default:
      break;
    }

  }
  void TUGSpHuku::TapHukuField()
  {
    ugHuku_.TapField();
  }

  void TUGSpHuku::DrawBuyIcon(s32 no)
  {
    itemicon_->g_SetPartNo(no);
    itemicon_->g_SetDraw(TRUE);
    stHuku_.itemdraw_c = 30;
  }


  void TUGSpHuku::DoUpdate(BOOL draw_f)
  {
    //進行
    field_->Update(draw_f);
    inside_->Update(draw_f);
    itemicon_->Update(draw_f);

    ugclerk_.Update(draw_f);//ugHuku_でupdate
    ugkantoku_.Update(draw_f);//ugHuku_でupdate
	ugcap_.Update(draw_f);
    ugserifu_.Update(draw_f);
    ugHuku_.Update(draw_f);
  }


  void TUGSpHuku::Mv_Visit()
  {
    //if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd)) ugclerk_.SetMType(cmt_Stand);
    Mv_Default();
    if (ugkantoku_.IsActRes(enUGChGenAct_MoveStop)) ugkantoku_.SetMType(kmt_St);
	if (ugcap_.IsActRes(enUGChGenAct_MoveStop)) ugcap_.SetMType(cmt_St);

  }
  void TUGSpHuku::Mv_Menu()//メニュー
  {
    Mv_Default();
  }

  void TUGSpHuku::Mv_Order()//初注文
  {
    Mv_Default();
  }
  void TUGSpHuku::Mv_Order2()//再注文
  {
    Mv_Default();
  }
  void TUGSpHuku::Mv_Buy()//購入時
  {
    Mv_Default();
  }
  void TUGSpHuku::Mv_NoMoney()//金不足
  {
    Mv_Default();
  }
  void TUGSpHuku::Mv_NoSpace()//空不足
  {
    Mv_Default();
  }
  void TUGSpHuku::Mv_Huku()//ガチャ中
  {
    ////ガチャの終了を検知
    //if (ugHuku_.IsAct())
    //{
    //  SetAct(enUGSpHukuAct_HukuEnd);//アクション
    //}
    //ガチャの終了を検知
    if (ugHuku_.IsActRes(enUGHukuAct_End))
    {
      SetAct(enUGSpHukuAct_HukuEnd);//アクション
    }
    else if (ugHuku_.IsActRes(enUGHukuAct_CupsuleOut))
    {
      SetAct(enUGSpHukuAct_CupsuleOut, ugHuku_.GetActIndex());//アクション
    }
  }
  void TUGSpHuku::Mv_Out()//退出時
  {
    Mv_Default();
  }
  void TUGSpHuku::Mv_Default()
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))//セリフ読み終わり
    {
      ugclerk_.SetMType(cmt_Stand);
    }
    else if (ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))//セリフウエイト
    {
      SetAct(enUGSpHukuAct_SerifuEnd);//アクション
    }
  }


  void TUGSpHuku::DoAction()
  {
    if (lib_num::AprTo0(&stHuku_.itemdraw_c))
    {
      itemicon_->g_SetDraw(FALSE);
    }

    //メッセージタイプごとの店員と監督の挙動
    switch (stHuku_.mtype_)
    {
    case hkmt_Visit: Mv_Visit(); break;//入店時
    case hkmt_Menu: Mv_Menu(); break;//メニュー
    case hkmt_Order: Mv_Order(); break;//初注文
    case hkmt_Order2: Mv_Order2(); break;//再注文
    case hkmt_Buy:Mv_Buy(); break;//購入時
    case hkmt_NoMoney:Mv_NoMoney(); break;//金不足
    case hkmt_NoSpace:Mv_NoSpace(); break;//空不足
    case hkmt_Huku:Mv_Huku(); break;//ガチャ中
    case hkmt_Out: Mv_Out(); break;//退出時
	default: Mv_Default(); break;
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

  BOOL TUGSpHuku::IsActRes(enUGSpHukuAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }
}
