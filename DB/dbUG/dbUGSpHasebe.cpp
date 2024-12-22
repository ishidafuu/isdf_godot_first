#include "dbUGSpHasebe.h"


namespace db
{
	//コンストラクタ
	TUGSpHasebe::TUGSpHasebe(TUIGenLayer* player)
		: TUGGen(player)
		, ugserifu_(player)
		, timer_(NGNUM)
	{
		stMvk_.Init();
	}

	TUGSpHasebe::~TUGSpHasebe()
	{

	}

	void TUGSpHasebe::MakeGroup(TUGChGirl* uggirl)
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

	void TUGSpHasebe::GenSetMsg(const char* text0)
	{
		serifuvec_.clear();
		serifuvec_.push_back(text0);
		uggirl_->SetMType(gmt_StTalk);
		ugserifu_.SetSerifu(serifuvec_);
	}
	void TUGSpHasebe::GenSetMsg(const char* text0, const char* text1)
	{
		serifuvec_.clear();
		serifuvec_.push_back(text0);
		serifuvec_.push_back(text1);
		uggirl_->SetMType(gmt_StTalk);
		ugserifu_.SetSerifu(serifuvec_);
	}
	void TUGSpHasebe::GenSetMsg(const char* text0, const char* text1, const char* text2)
	{
		serifuvec_.clear();
		serifuvec_.push_back(text0);
		serifuvec_.push_back(text1);
		serifuvec_.push_back(text2);
		uggirl_->SetMType(gmt_StTalk);
		ugserifu_.SetSerifu(serifuvec_);
	}
	void TUGSpHasebe::SetMsg(enHsbMsgType msgtype, s64 number)
	{
		stMvk_.mtype_ = msgtype;


		//メッセージタイプごとの店員と監督の挙動
		switch (msgtype)
		{
		case db::hsm_Visit:
			GenSetMsg("はせべ「こんにちは心", "こちらは＿チームいくせいじょ＿です");
			break;
		case db::hsm_Menu:
			GenSetMsg("はせべ「トレーニングや＿ガチャで", "チームを＿どんどん＿つよくしよう");
			break;
		case db::hsm_SelectTeam:
			GenSetMsg("はせべ「トレーニングを＿したい", "せんしゅがいる＿チームをえらぼう");
			break;
		case db::hsm_SelectChara:
			GenSetMsg("はせべ「チームが＿きまったね", "このなかの＿だれを＿そだてるの？");
			break;
		case db::hsm_SelectTraining:
			{
				std::string strrest = "はせべ「" + pmgEO_->mgDt_.dtChar_.GetName(number) + "ですね";
				GenSetMsg(strrest.c_str(), "どのコーチに＿いらいしますか？");
			}
			break;
		case db::hsm_TeamDisplay:
			GenSetMsg("はせべ「げんざい＿しょじしている", "さいきょうチームは＿こちらです心");
			break;
		case db::hsm_Out:
			GenSetMsg("はせべ「ありがとうございました", "またごりようください心");
			break;
		case db::hsm_Etc:
			GenSetMsg("はせべ「すみません＿こちらの", "メニューは＿じゅんびちゅうです");
			timer_ = 300;
			break;
		default:
			break;
		}
	}

	void TUGSpHasebe::SetStopMsg()
	{
		uggirl_->SetMType(gmt_St);
	}




	void TUGSpHasebe::DoUpdate(BOOL draw_f)
	{
		//進行
		field_->Update(draw_f);
		inside_->Update(draw_f);
		//itemicon_->Update(draw_f);

		uggirl_->Update(draw_f);
		//uglogbo_->Update(draw_f);
		//ugkantoku_.Update(draw_f);
		ugserifu_.Update(draw_f);

		if (timer_ > 0)
		{
			timer_--;
			if (timer_ <= 0)
			{
				timer_ = NGNUM;
				SetMsg(hsm_Menu);
			}
		}
	}

	void TUGSpHasebe::Mv_Default()//入店時
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

	void TUGSpHasebe::DoAction()
	{
		//メッセージタイプごとの店員と監督の挙動
		switch (stMvk_.mtype_)
		{
		default:
		case hsm_Visit: Mv_Default(); break;
		case hsm_Menu: Mv_Default(); break;
		case hsm_Out:Mv_Default(); break;
		}
	}
}
