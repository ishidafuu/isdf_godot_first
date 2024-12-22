#include "dbUGSpGacha.h"


namespace db
{

  enum enItemDraw
  {
    itd_None,
    itd_Carry,
    itd_Table,
  };


  //コンストラクタ
  TUGSpGacha::TUGSpGacha(TUIGenLayer* player) :TUGGen(player),
    ugclerk_(player),
    ugkantoku_(player),
    ugcap_(player),
    ugserifu_(player),
    ugnabox_(player),
    uggacha_(player)
  {
    stGacha_.Init();
  }

  TUGSpGacha::~TUGSpGacha()
  {
  }

  static s32 limX = -160;
  static s32 limW = 152;
  void TUGSpGacha::MakeGroup()
  {
    field_->o_SetObjFilterGrp(448, 168, 0xFF, 0x38, 0x68, 0x9A);
    field_->g_SetCenter(TRUE);

    inside_ = MakeObj();
    inside_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_shop));
    inside_->g_SetPartNo(1);
    inside_->o_SetParent(field_);
    inside_->z_SetPos(-160, -60);

    itemicon_ = MakeObj();
    itemicon_->o_SetParent(inside_);
    itemicon_->z_SetPos(46, 32);

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

    ugclerk_.MakeGroup(inside_, ct_Gacha);
    ugclerk_.SetPos(40, 72);

    uggacha_.MakeGroup(inside_, &ugkantoku_, &ugcap_, &ugclerk_);
    ugnabox_.MakeGroup(inside_, &ugkantoku_, &ugcap_, &ugclerk_);
  }
  void TUGSpGacha::Ready()
  {
    stGacha_.Init();
    SetMsg(gamt_Visit);
  }
  void TUGSpGacha::SetIconGrp(base::TGrPart* icongrp)
  {
    itemicon_->o_SetObjGrPart(icongrp);
  }
  void TUGSpGacha::SetIconPartNo(s32 partno)
  {
    itemicon_->g_SetPartNo(partno);
  }

  void TUGSpGacha::GenSetMsg(const char* text0)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpGacha::GenSetMsg(const char* text0, const char* text1)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpGacha::GenSetMsg(const char* text0, const char* text1, const char* text2)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    serifuvec_.push_back(text2);
    ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }

  //メッセージ＆連動アクション選択
  void TUGSpGacha::SetMsg(enGachaMsgType msgtype)
  {
    stGacha_.mtype_ = msgtype;
    //セリフ
    switch (msgtype)
    {
    case db::gamt_Visit:GenSetMsg("掛「ふぉっ＿ふぉっ＿ふぉっ", "よくきたのォ"); break;
    case db::gamt_Menu: GenSetMsg("掛「ガチャで＿あそんでいくのかい？"); break;
    case db::gamt_Out:GenSetMsg("掛「またくるんじゃぞ＿おぬしの", "みらいに＿こううんが＿あるように！"); break;
    case db::gamt_SelectStar:GenSetMsg("掛「スターガチャじゃな", "どれくらい＿まわしていくかのォ？"); break;
    case db::gamt_DecideStar:GenSetMsg("掛「ふぉっ＿ふぉっ＿ふぉっ", "いいものが＿あたると＿いいのォ"); break;
    case db::gamt_SelectNaBox:GenSetMsg("掛「ＢＯＸガチャじゃな", "では＿ひきたいＢＯＸをえらぶんじゃ"); break;
    case db::gamt_DecideNaBox:GenSetMsg("掛「このＢＯＸから＿ひくんじゃな", "ほしいものが＿すぐにでるといいのォ"); break;
    case db::gamt_SelectTodd:GenSetMsg("掛「トッドガチャじゃな", "どれくらい＿まわしていくかのォ？"); break;
    case db::gamt_DecideTodd:GenSetMsg("掛「あたる＿みらいを＿イメージして", "うんを＿おぬしに＿よびこむんじゃ"); break;
    case db::gamt_Back:GenSetMsg("掛「まようことも＿たいせつじゃ", "たがチャンスを＿のがしてはならんぞ"); break;
    case db::gamt_Buy:GenSetMsg("掛「いいものは＿あたったかのォ？", "まけたまま＿あきらめてはならんぞォ"); break;
    case db::gamt_NoMoney:GenSetMsg("掛「おかねが＿たりないみたいじゃ", "ガチャをひかせることは＿できんのォ"); break;
	
	case db::gamt_JumpInStar:GenSetMsg("掛「おしらせを＿みてきたんじゃな", "きほんは＿チーム＿じゃからのォ"); break;			// スターガチャに直接入ってきた
	case db::gamt_JumpInNaBoxFromHome:GenSetMsg("掛「おしらせを＿みてきたんじゃな", "はいけい・ＢＧＭを＿ＧＥＴじゃぞィ"); break;	// なつかしＢＯＸに直接入ってきた(ホームから)
	case db::gamt_JumpInNaBoxFromTeam:GenSetMsg("掛「よくきたのォ＿ほしい", "はいけい・ＢＧＭを＿ＧＥＴじゃぞィ"); break;	// なつかしＢＯＸに直接入ってきた(チームから)
	case db::gamt_JumpInTodd:GenSetMsg("掛「おしらせを＿みてきたんじゃな", "トッドでチームを＿げっとじゃ"); break;			// トッドガチャに直接入ってきた

    case db::gamt_TuGachaVisit:GenSetMsg("掛「はじめてみる＿かおじゃのォ", "ここは＿ガチャの＿みせじゃ"); break;//入店時
    case db::gamt_TuGachaInfo:GenSetMsg("掛「うりものは＿ガチャとＢＯＸじゃ", "ガチャからは＿チームが＿でるぞィ"); break;//せつめい
	case db::gamt_TuReady:GenSetMsg("掛「まずガチャを＿ためすんじゃ", "トッドガチャを＿えらびなされ"); ugserifu_.SetMsgChangeMode(enMsgChangeMode_None); break;//トッドを選べ
    case db::gamt_TuSelectTodd:GenSetMsg("掛「１０れんトッドガチャを", "えらぶんじゃ＿トッドが＿かかるぞィ"); break;//トッドを選べ
    case db::gamt_TuCheckTodd:GenSetMsg("掛「トッド１０れんを＿するには", "１０００トッド＿ひつよう＿なんじゃ"); break;//スターガチャ購入確認
	case db::gamt_TuDecideTodd:GenSetMsg("掛「たしかに＿ちょうだいした", "では＿ガチャを＿まわすんじゃ"); ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);  break;//スターガチャ購入許可
	case db::gamt_TuGacha:GenSetMsg("掛「がめんを＿タッチじゃ！","タッチじゃ！＿おしっぱなしじゃ！"); ugserifu_.SetMsgChangeMode(enMsgChangeMode_None);  break;//スターガチャ購入確認
	case db::gamt_TuBuy:GenSetMsg("掛「チームの＿かくとくほうほうは", "りかいしたかのォ？");  ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);  break;//購入時

	case db::gamt_TuNNaBoxVisit:GenSetMsg("掛「つぎに＿なつかしＢＯＸについて", "おしえよう"); ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch); break;//つぎはなつかし
	case db::gamt_TuNNaBoxInfo:GenSetMsg("掛「つぎに＿なつかしＢＯＸについて", "おしえよう"); ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);  break;//つぎはなつかし
    case db::gamt_TuNNaBoxInfo2:GenSetMsg("掛「なつかしＢＯＸには＿みんなの", "おもいでが＿つまって＿いるんじゃ"); break;//せつめい
	case db::gamt_TuNReady:GenSetMsg("掛「なにごとも＿ためすのが＿はやい", "なつかしＢＯＸを＿えらびなされ"); ugserifu_.SetMsgChangeMode(enMsgChangeMode_None); break;//なつかしを選べ
    case db::gamt_TuNSelectNaBox:GenSetMsg("掛「はじめてＢＯＸを＿えらぶんじゃ", ""); break;//はじめてＢＯＸを選べ
    case db::gamt_TuNSelectNaBoxNum:GenSetMsg("掛「ＢＯＸおとながいなら＿いちどに", "すべてを＿かくとく＿できるんじゃ"); break;//おとながいを選べ
    case db::gamt_TuNDecideNaBox:GenSetMsg("掛「こんかいは＿とくべつに", "おとながいを＿０スターに＿するぞィ"); break; //購入確認
    //case db::gamt_TuNNaBox:GenSetMsg("掛「", ""); break; //ぼたんおせ
	case db::gamt_TuNBuy:GenSetMsg("掛「ＢＧＭ・はいけいを＿いくつか", "かくとく＿したようじゃのォ");  ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);  break;//購入時
	case db::gamt_TuLastInfo:GenSetMsg("掛「てにいれた＿はいけい・ＢＧＭは", "ホームコートに＿できるぞィ"); ugserifu_.SetMsgChangeMode(enMsgChangeMode_None); break; //ぼたんおせ
    case db::gamt_TuOut:GenSetMsg("掛「では＿さらばじゃ！", ""); break;//さらば
    case db::gamt_Gacha: break;
    case db::gamt_NaBox:break;
    case db::gamt_NaBoxOtona: break;
    case db::gamtEND: break;
    }




    //アクション
    switch (msgtype)
    {
    case db::gamt_Visit:
    case db::gamt_TuGachaVisit:
    case db::gamt_TuNNaBoxVisit:
      ugkantoku_.SetMirror(TRUE);
      ugkantoku_.z_MoveWork(0, 76, 72, 1, cmk_None);
      ugcap_.SetMirror(TRUE);
      ugcap_.z_MoveWork(0, 76 + 26, 72, 1, cmk_None);
      break;
    //case db::gamt_Buy:
    //  ugclerk_.SetMType(cmt_Hand);
    //  ugkantoku_.SetMType(kmt_Take);
    //  break;
    case db::gamt_Out:
    case db::gamt_TuOut:
      ugkantoku_.SetMType(kmt_Wk);
      ugkantoku_.SetMirror(FALSE);
      ugkantoku_.z_MoveWork(0, 76 + 60, 72, 1);
      ugcap_.SetMType(cmt_Wk);
      ugcap_.SetMirror(FALSE);
      ugcap_.z_MoveWork(0, 76 + 60 + 26, 72, 1);
      break;
    case db::gamt_Gacha://ガチャ初期化
    case db::gamt_TuGacha:
      uggacha_.Act_FallGacha();//ガチャ落ちてくる命令
      stGacha_.mtype_ = gamt_Gacha;
      break;
    case db::gamt_NaBox://ガチャ初期化
      ugnabox_.SetNaBoxResult(FALSE);//通信の後で
      ugnabox_.Act_FallNaBox();//ガチャ落ちてくる命令
      break;
    case db::gamt_NaBoxOtona://ガチャ初期化
    case db::gamt_TuNNaBox:
      ugnabox_.SetNaBoxResult(TRUE);//通信の後で
      ugnabox_.Act_FallNaBox();//ガチャ落ちてくる命令
      stGacha_.mtype_ = gamt_NaBox;
      break;
    default:
      break;
    }
  }
  void TUGSpGacha::NaBoxRefresh(s64 mid_nabox)
  {
    ugnabox_.RefreshBox(mid_nabox);//通信の前にリフレッシュ掛ける（通信終わると当たりが開いてしまっているため）
  }
  void TUGSpGacha::TapGachaField()
  {
    uggacha_.TapField();
  }
  void TUGSpGacha::TapBoxGachaField()
  {
    ugnabox_.TapField();
  }

  void TUGSpGacha::DrawBuyIcon(s32 no)
  {
    itemicon_->g_SetPartNo(no);
    itemicon_->g_SetDraw(TRUE);
    stGacha_.itemdraw_c = 30;
  }


  void TUGSpGacha::DoUpdate(BOOL draw_f)
  {
    //進行
    field_->Update(draw_f);
    inside_->Update(draw_f);
    itemicon_->Update(draw_f);

    ugclerk_.Update(draw_f);//uggacha_でupdate
    ugkantoku_.Update(draw_f);//uggacha_でupdate
    ugcap_.Update(draw_f);//uggacha_でupdate
    ugserifu_.Update(draw_f);

    if (stGacha_.mtype_ != gamt_NaBox) uggacha_.Update(draw_f);
    if (stGacha_.mtype_ != gamt_Gacha) ugnabox_.Update(draw_f);

    //kdebug::DebugController* pCtr = kdebug::DebugController::GetInstance();
    //if (pCtr->IsRepeatButton(XINPUT_BUTTON_RIGHT_STICK_RIGHT)){ limW++;}
    //if (pCtr->IsRepeatButton(XINPUT_BUTTON_RIGHT_STICK_LEFT)){ limW--; }
    //if (pCtr->IsRepeatButton(XINPUT_BUTTON_RIGHT_STICK_UP)){ limW+=10; }
    //if (pCtr->IsRepeatButton(XINPUT_BUTTON_RIGHT_STICK_DOWN)){ limW-=10; }
    //if (pCtr->IsRepeatButton(XINPUT_BUTTON_LEFT_STICK_RIGHT)){ limX++; }
    //if (pCtr->IsRepeatButton(XINPUT_BUTTON_LEFT_STICK_LEFT)){ limX--; }
    //if (pCtr->IsRepeatButton(XINPUT_BUTTON_LEFT_STICK_UP)){ limX+=10; }
    //if (pCtr->IsRepeatButton(XINPUT_BUTTON_LEFT_STICK_DOWN)){ limX-=10; }
    //if (pCtr->IsTriggerButton(XINPUT_BUTTON_A)){ limX = -160; limW = 152; }
    //kdebug::DebugText* pTxt = kdebug::DebugText::GetInstance();
    //pTxt->SetColor(kdebug::COL_RED);
    //char str_c[256];
    //sprintf_s(str_c, "limX:%d  limW:%d", limX, limW);
    //pTxt->SetDebugText(5, 20, str_c);
  }

  void TUGSpGacha::Mv_Visit()
  {
    //if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd)) ugclerk_.SetMType(cmt_Stand);
    Mv_Default();
    if (ugkantoku_.IsActRes(enUGChGenAct_MoveStop)) ugkantoku_.SetMType(kmt_St);
    if (ugcap_.IsActRes(enUGChGenAct_MoveStop)) ugcap_.SetMType(cmt_St);

  }
  void TUGSpGacha::Mv_Gacha()//ガチャ中
  {
    //ガチャの終了を検知
    if (uggacha_.IsActRes(enUGGachaAct_End))
    {
      SetAct(enUGSpGachaAct_GachaEnd);//アクション
    }
    else if (uggacha_.IsActRes(enUGGachaAct_CupsuleOut))
    {
      SetAct(enUGSpGachaAct_CupsuleOut, uggacha_.GetActIndex());//アクション
    }
  }
  void TUGSpGacha::Mv_BoxGacha()//ガチャ中
  {
    //ガチャの終了を検知
    if (ugnabox_.IsAct())
    {
      SetAct(enUGSpGachaAct_GachaEnd);//アクション
    }
  }
  void TUGSpGacha::Mv_Default()
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))//セリフ読み終わり
    {
      ugclerk_.SetMType(cmt_Stand);
    }
    else if (ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))//セリフウエイト
    {
      SetAct(enUGSpGachaAct_SerifuEnd);//アクション
    }
  }

  void TUGSpGacha::DoAction()
  {
    if (lib_num::AprTo0(&stGacha_.itemdraw_c))
    {
      itemicon_->g_SetDraw(FALSE);
    }

    //メッセージタイプごとの店員と監督の挙動
    switch (stGacha_.mtype_)
    {
    case gamt_Visit: Mv_Visit(); break;//入店時
    case gamt_Gacha:Mv_Gacha(); break;//ガチャ中
    case gamt_NaBox:Mv_BoxGacha(); break;//ガチャ中
    default:Mv_Default();break;
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

  BOOL TUGSpGacha::IsActRes(enUGSpGachaAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }
}
