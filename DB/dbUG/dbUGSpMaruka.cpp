#include "dbUGSpMaruka.h"


namespace db
{

  enum enItemDraw
  {
    itd_None,
    itd_Carry,
    itd_Table,
  };


  //コンストラクタ
  TUGSpMaruka::TUGSpMaruka(TUIGenLayer* player) :TUGGen(player), ugserifu_(player)
  {
    stMaruka_.Init();
  }

  TUGSpMaruka::~TUGSpMaruka()
  {

  }

  void TUGSpMaruka::MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap, TUGChClerk* ugclmaruka, TUGDoor* ugdoor)
  {
    SetParent(parent);
    ugkantoku_ = ugkantoku;
	ugcap_ = ugcap;
    ugserifu_.MakeGroup(field_, 2, enSerifuFrame_Normal);
    ugdoor_ = ugdoor;
    ugclmaruka_ = ugclmaruka;
    ugserifu_.SetParent(field_);
    ugserifu_.SetPos(-160, 48-16);
  }

  //入店する前に店員と番号をセット
  void TUGSpMaruka::Ready()
  {

    SetMsg(enMarukaMsg_Visit);
  }

  void TUGSpMaruka::GenSetMsg(const char* text0)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    ugclmaruka_->SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpMaruka::GenSetMsg(const char* text0, const char* text1)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    ugclmaruka_->SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpMaruka::SetStopMsg()
  {
    ugclmaruka_->SetMType(cmt_Stand);
  }

  void TUGSpMaruka::SetMsg(enMarukaMsg msgtype)
  { 
    stMaruka_.mtype_ = msgtype;

    //メッセージタイプごとの店員と監督の挙動
    switch (stMaruka_.mtype_)
    {
    case enMarukaMsg_Visit: //入店時
      GenSetMsg("掛「おあずかりした＿しゅーとの", "おとどけに＿まいりました");
      break;
    case enMarukaMsg_ShotRcv: //シュートせんたく
      GenSetMsg("掛「きげんを＿すぎると＿はきします", "うけとり＿わすれないで＿くださいね");
      break;
    case enMarukaMsg_OK: //ＯＫ待ち
      GenSetMsg("掛「こちらのしゅーとで","よろしいですか？");
      break;
    case enMarukaMsg_Thanks: //購入
      GenSetMsg("掛「ほかにも＿うけとられる","しゅーとは＿ございますか？");
      break;
    case enMarukaMsg_Out: 
      GenSetMsg("掛「ありがとうございました");
      break;//退出時
    }
  }

  void TUGSpMaruka::DoUpdate(BOOL draw_f)
  {
    //進行
    field_->Update(draw_f);

    ugclmaruka_->Update(draw_f);
    ugkantoku_->Update(draw_f);
	ugcap_->Update(draw_f);
    ugserifu_.Update(draw_f);
    ugdoor_->Update(draw_f);
  }

  void TUGSpMaruka::Mv_Default()//入店時
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))//セリフ読み終わり
    {
      ugclmaruka_->SetMType(cmt_Stand);
    }
    else if (ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))//セリフウエイト
    {
      SetAct(0);//アクション
    }
  }

  void TUGSpMaruka::DoAction()
  {
    //メッセージタイプごとの店員と監督の挙動
    switch (stMaruka_.mtype_)
    {
    case enMarukaMsg_Visit: Mv_Default(); break;//入店時
    case enMarukaMsg_ShotRcv: Mv_Default(); break;
    case enMarukaMsg_OK: Mv_Default(); break;
    case enMarukaMsg_Out: Mv_Default(); break;//退出時
    }

	// メッセージスキップ処理
	if (field_->sc_IsCamEnter())
	{
		if (ugserifu_.IsReadLineEnd())
		{
			ugserifu_.NextSerifu();
			ugclmaruka_->SetMType(cmt_Stand);
		}
		else
		{
			ugserifu_.SkipSerifu();
			ugclmaruka_->SetMType(cmt_Stand);
		}
	}
  }

}
