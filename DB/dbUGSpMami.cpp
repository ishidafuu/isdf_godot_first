#include "dbUGSpMami.h"


namespace db
{
  //コンストラクタ
  TUGSpMami::TUGSpMami(TUIGenLayer* player) :TUGGen(player)
    , ugserifu_(player)
  {
    stMami_.Init();
  }

  TUGSpMami::~TUGSpMami()
  {

  }

  void TUGSpMami::MakeGroup(TUGChGirl* uggirl)
  {
    //field_->z_SetPos_SafeR(0, 0);

    //inside_ = MakeObj();
    ////inside_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_shop_inside));
    //inside_->o_SetParent(field_);
    //inside_->z_SetPos(64, 24);

    ugserifu_.MakeGroup(field_, 3, enSerifuFrame_Normal);//とりあえず3行
    ugserifu_.SetPos(0, 36);
    uggirl_ = uggirl;
  }

  void TUGSpMami::GenSetMsg(const char* text0)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    uggirl_->SetMType(gmt_StTalk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpMami::GenSetMsg(const char* text0, const char* text1)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    uggirl_->SetMType(gmt_StTalk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpMami::GenSetMsg(const char* text0, const char* text1, const char* text2)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    serifuvec_.push_back(text2);
    uggirl_->SetMType(gmt_StTalk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpMami::SetMsg(enMamiMsgType msgtype, s64 number)
  {
    stMami_.mtype_ = msgtype;


    //メッセージタイプごとの店員と監督の挙動
    switch (msgtype)
    {
    case db::mmm_Visit:
      GenSetMsg("まみ「こんにちは！", "こちらは＿あんないじょです心");
      break;
	case db::mmm_Logbo:
		GenSetMsg("まみ「ほんじつの＿おくりものです", "こちらを＿おうけとりください心");
		break;
	case db::mmm_LogboSp:
		GenSetMsg("まみ「いつもありがとうございます心", "とくべつボーナスです心");
		break;
	case db::mmm_LogboSpYokoku:
	{
		std::string strrest = "まみ「あと" + lib_str::IntToStr(number) + "かいで＿こちらの";
		GenSetMsg(strrest.c_str(), "とくべつボーナスを＿さしあげます心");
	}
		break;
	case db::mmm_LogboResult:
		GenSetMsg("まみ「あしたも＿きてくださいね", "こちらを＿よういしておきます心");
		break;
	case db::mmm_LogboShot:
		GenSetMsg("まみ「きょうは＿みっくすしゅーとを", "プレゼントします心"); 
		break;
	case db::mmm_LogboShot2:
	{
		std::string strrest = lib_str::IntToStr(number) + "トッド＿プレゼントします心";
		GenSetMsg("まみ「いらない＿ばあいは", strrest.c_str());
	}
		break;
	case db::mmm_LogboShotPrev:
	{
		std::string strrest = "「" + pmgEO_->mgDt_.dtShElm_.GetDtStr(number, enDtShElmStr::szs_name) + "」です";
		GenSetMsg("まみ「きょうのベースは", strrest.c_str());
		ugserifu_.SetPos(-100, -80);
	}
	break;
	case db::mmm_LogboShotPrev2:
		GenSetMsg("まみ「イマイチなら＿べつのしゅーとに＿します", "なっとく＿いくまで＿えらんでくださいね心");
		break;
	case db::mmm_LogboShotResult:
		GenSetMsg("まみ「それでは＿こちらを", "プレゼントしますね心");
		ugserifu_.SetPos(0, 36);
		break;
	case db::mmm_LogboShotResult2:
		GenSetMsg("まみ「シュートが＿いっぱいですので", "まるかうんそうへ＿おくりますね心");
		ugserifu_.SetPos(0, 36);
		break;
	case db::mmm_LogboSpShot:
		GenSetMsg("まみ「きょうは＿すぺしゃるな", "しゅーとを＿プレゼントします心");
		break;
	case db::mmm_LogboSpShotYokoku:
	{
		std::string strrest = "まみ「あと" + lib_str::IntToStr(number) + "かいで＿こちらの";
		GenSetMsg(strrest.c_str(), "とくべつシュートを＿さしあげます心");
	}
	break;
	case db::mmm_Menu:
      GenSetMsg("まみ「かんとくさんに＿あたらしい", "じょうほうが＿とどいてるみたい心");
      break;
    case db::mmm_GiftBox:
      GenSetMsg("まみ「アイテムが＿とどいているよ", "うけとりきげんに＿ちゅういしてね心");
      break;
    case db::mmm_Out:
      GenSetMsg("まみ「ありがとうございました！", "またごりようください心");
      break;
    default:
      break;
    }

	if (msgtype == db::mmm_Visit || msgtype == db::mmm_Out || msgtype == db::mmm_LogboShot || msgtype == db::mmm_LogboSpShot || msgtype == mmm_LogboShot2)
	{
		ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);
		ugserifu_.ChangeFrame(enSerifuFrame_Normal);
	}
	else if (msgtype == db::mmm_LogboShotPrev || msgtype == db::mmm_LogboShotPrev2)
	{
		ugserifu_.SetMsgChangeMode(enMsgChangeMode_None);
		ugserifu_.ChangeFrame(enSerifuFrame_Wide);
	}
	else
	{
		ugserifu_.SetMsgChangeMode(enMsgChangeMode_Wait2);
		ugserifu_.ChangeFrame(enSerifuFrame_Normal);
	}
  }

  void TUGSpMami::SetStopMsg()
  {
    uggirl_->SetMType(gmt_St);
  }




  void TUGSpMami::DoUpdate(BOOL draw_f)
  {
    //進行
    //field_->Update(draw_f);
    //inside_->Update(draw_f);
    //itemicon_->Update(draw_f);

    uggirl_->Update(draw_f);
    //uglogbo_->Update(draw_f);
    //ugkantoku_.Update(draw_f);
    ugserifu_.Update(draw_f);

  }

  void TUGSpMami::Mv_Default()//入店時
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))//セリフ読み終わり
    {
      uggirl_->SetMType(gmt_St);
	  return;
    }
    else if (ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))//セリフウエイト
    {
      SetAct(0);//アクション
	  return;
    }

	if (field_->sc_IsCamEnter())
	{
		if (ugserifu_.IsReadLineEnd())
		{
			ugserifu_.NextSerifu();
			uggirl_->SetMType(gmt_St);
		}
		else
		{
			ugserifu_.SkipSerifu();
			uggirl_->SetMType(gmt_St);
		}
	}
  }

  void TUGSpMami::DoAction()
  {
    //メッセージタイプごとの店員と監督の挙動
    Mv_Default();
  }
}
