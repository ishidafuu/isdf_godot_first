#include "dbUGSpMaveko.h"


namespace db
{
  //コンストラクタ
  TUGSpMaveko::TUGSpMaveko(TUIGenLayer* player) :TUGGen(player), ugserifu_(player)
  {
    stMvk_.Init();
  }

  TUGSpMaveko::~TUGSpMaveko()
  {

  }

  void TUGSpMaveko::MakeGroup(TUGChGirl* uggirl)
  {
    field_->z_SetPos_SafeLF(0, 0);

    inside_ = MakeObj();
    //inside_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_shop_inside));
    inside_->o_SetParent(field_);
    inside_->z_SetPos(64, 24);

    ugserifu_.MakeGroup(inside_, 3, enSerifuFrame_Normal);//とりあえず3行
    ugserifu_.SetPos(0, 96);

    uggirl_ = uggirl;
  }

  void TUGSpMaveko::GenSetMsg(const char* text0)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    uggirl_->SetMType(gmt_StTalk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpMaveko::GenSetMsg(const char* text0, const char* text1)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    uggirl_->SetMType(gmt_StTalk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpMaveko::GenSetMsg(const char* text0, const char* text1, const char* text2)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    serifuvec_.push_back(text2);
    uggirl_->SetMType(gmt_StTalk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpMaveko::SetMsg(enMvkMsgType msgtype)
  {
    stMvk_.mtype_ = msgtype;


    //メッセージタイプごとの店員と監督の挙動
    switch (msgtype)
    {
    case db::mvm_Visit:
      GenSetMsg("ほしの「こんにちは心", "こちらは＿うんえい＿カウンターです");
      break;
/*
    case db::mvm_Logbo:
      GenSetMsg("ほしの「ほんじつの＿おくりものです", "スターを＿おうけとりください心");
      break;
    case db::mvm_LogboSp:
      GenSetMsg("ほしの「いつもありがとうございます心", "とくべつボーナスです心");
      break;
    case db::mvm_LogboSpYokoku:
    {
      s32 loginnum = (10 - (pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_loginNum) % 10));
      std::string strrest = "ほしの「あと" + lib_str::IntToStr(loginnum) + "かいで＿こちらの";
      GenSetMsg(strrest.c_str(), "とくべつボーナスを＿さしあげます心");
    }
    break;
    case db::mvm_LogboResult:
      GenSetMsg("ほしの「あしたも＿きてくださいね", "スターを＿よういしておきます心");
      break;
*/
    case db::mvm_Menu:
      GenSetMsg("ほしの「ごようけんは＿なんでしょう", "メニューから＿えらんでくださいね");
      break;
    case db::mvm_Info:

      break;
    case db::mvm_Code:
      GenSetMsg("ほしの「コードを＿おもちでしたら", "こちらで＿にゅうりょくしてください");
      break;
    case db::mvm_FrCode:
      GenSetMsg("ほしの「このかたからの", "しょうたいコードですか？");
      break;
    case db::mvm_CodeResult:
      GenSetMsg("ほしの「コードの＿にゅうりょくを", "うけつけました心");
      break;
    case db::mvm_Invite:
      GenSetMsg("ほしの「おともだちに＿このゲームの", "しょうたいじょうを＿おくれます");
      break;
    case db::mvm_GetLog:
      GenSetMsg("ほしの「いままで＿かくとくした", "アイテムの＿いちらんです");
      break;
    case db::mvm_UseLog:
      GenSetMsg("ほしの「いままで＿しようした", "アイテムの＿いちらんです");
      break;
    case db::mvm_Circle:
      break;
    case db::mvm_Mail:
      break;
    case db::mvm_Push:
      break;
    case db::mvm_Logout:
      break;
    case db::mvm_Debug:
      GenSetMsg("ほしの「ふひひ心");
      break;
    case db::mvm_Out:
      GenSetMsg("ほしの「ありがとうございました", "またごりようください心");
      break;
    default:
      break;
    }
  }

  void TUGSpMaveko::SetStopMsg()
  {
    uggirl_->SetMType(gmt_St);
  }




  void TUGSpMaveko::DoUpdate(BOOL draw_f)
  {
    //進行
    field_->Update(draw_f);
    inside_->Update(draw_f);
    //itemicon_->Update(draw_f);

    uggirl_->Update(draw_f);
    //uglogbo_->Update(draw_f);
    //ugkantoku_.Update(draw_f);
    ugserifu_.Update(draw_f);

  }

  void TUGSpMaveko::Mv_Default()//入店時
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))//セリフ読み終わり
    {
      uggirl_->SetMType(gmt_St);
    }
    else if (ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))//セリフウエイト
    {
      SetAct(0);//アクション
    }

	// メッセージスキップ処理
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

  void TUGSpMaveko::DoAction()
  {
    //メッセージタイプごとの店員と監督の挙動
    switch (stMvk_.mtype_)
    {
    case mvm_Visit: Mv_Default(); break;
    case mvm_Menu: Mv_Default(); break;
    case mvm_Info: Mv_Default(); break;
    case mvm_Code: Mv_Default(); break;
    case mvm_Invite: Mv_Default(); break;
    case mvm_GetLog: Mv_Default(); break;
    case mvm_UseLog: Mv_Default(); break;
    case mvm_Circle: Mv_Default(); break;
    case mvm_Mail: Mv_Default(); break;
    case mvm_Push: Mv_Default(); break;
    case mvm_Logout: Mv_Default(); break;
    case mvm_Out:Mv_Default(); break;
    case mvm_Debug:Mv_Default(); break;
    }
  }
}
