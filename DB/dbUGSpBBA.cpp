#include "dbUGSpBBA.h"


namespace db
{

  enum enItemDraw
  {
    itd_None,
    itd_Carry,
    itd_Table,
  };


  //コンストラクタ
  TUGSpBBA::TUGSpBBA(TUIGenLayer* player) :TUGGen(player), ugserifu_(player)
  {
    stBBA_.Init();
  }

  TUGSpBBA::~TUGSpBBA()
  {

  }

  void TUGSpBBA::MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap, TUGChClerk* ugclbba, TUGDoor* ugdoor)
  {
    SetParent(parent);
    ugkantoku_ = ugkantoku;
	ugcap_ = ugcap;
    ugserifu_.MakeGroup(field_, 2, enSerifuFrame_Normal);

    ugclbba_ = ugclbba;
    ugdoor_ = ugdoor;
    ugserifu_.SetParent(field_);
    ugserifu_.SetPos(-160, 12+24);
  }

  //入店する前に店員と番号をセット
  void TUGSpBBA::Ready()
  {

    SetBBAMsg(enBBAMsg_Visit);
  }
  void TUGSpBBA::Down()
  {
    SetAct(0);//退出アクション
  }

  void TUGSpBBA::GenSetBBAMsg(const char* text0)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    ugclbba_->SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpBBA::GenSetBBAMsg(const char* text0, const char* text1)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    ugclbba_->SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpBBA::SetStopMsg()
  {
    ugclbba_->SetMType(cmt_Stand);
  }

  void TUGSpBBA::SetBBAMsg(enBBAMsg msgtype)
  { 
    stBBA_.mtype_ = msgtype;
    ugserifu_.SetDraw(TRUE);
    //メッセージタイプごとの店員と監督の挙動
    switch (stBBA_.mtype_)
    {
    case enBBAMsg_Visit: //入店時
      ugdoor_->OpenDoor();//扉開ける
      GenSetBBAMsg("掛「おやおや＿いらっしゃい", "しゅーとおばばに＿ごようかね？");
      break;
    case enBBAMsg_Menu: //メニュー
      GenSetBBAMsg("掛「どんな＿ようじがあるのかね？","おばばに＿きかせておくれ");
      break;
    case enBBAMsg_ShotInv: //シュートインベントリ
      GenSetBBAMsg("掛「おお＿そうかい＿しゅーとを", "もてるかずを＿ふやしたいんだね？");
      break;
    case enBBAMsg_Bara: //合成分解
      GenSetBBAMsg("掛「おお＿そうかい＿ミックスした", "しゅーとを＿ばらしたいんだね？");
      break;
    case enBBAMsg_SellMenu: //シュート売却
      GenSetBBAMsg("掛「おお＿そうかい＿そうかい", "てもちのしゅーとを＿うりたいんだね");
      break;
    case enBBAMsg_Sell: //シュート売却
      GenSetBBAMsg("掛「うりたいしゅーとが＿きまったら", "おばばに＿おしえておくれ");
      break;
    case enBBAMsg_BuyMenu: //シュート売却
      GenSetBBAMsg("掛「しゅーとを＿かいたいんだね", "いろいろ＿とりそろえているよ");
      break;
    case enBBAMsg_Buy: //シュート売却
      GenSetBBAMsg("掛「ほしい＿しゅーとが＿きまったら", "おばばに＿おしえておくれ");
      break;
    case enBBAMsg_OK: //ＯＫ待ち
      GenSetBBAMsg("掛「これで＿いいのかい？");
      break;
    case enBBAMsg_Thanks: //購入
      GenSetBBAMsg("掛「そうかい＿どうもありがとうね","ほかにも＿ようじは＿あるのかい？");
      break;
    case enBBAMsg_Out: 
      ugdoor_->CloseDoor();//扉開ける
      GenSetBBAMsg("掛「このおばばに＿あいたくなったら","いつでも＿かおを＿みせにきておくれ");
      break;//退出時
    case enBBAMsg_NoneBara:
      GenSetBBAMsg("掛「ミックスされた＿しゅーとが", "ひとつもない＿みたいだよ？");
      break;
    case enBBAMsg_NoneEmp:
      GenSetBBAMsg("掛「あきしゅーとわくが", "たりない＿みたいだよ？");
      break;
    case enBBAMsg_NoneSell:
      GenSetBBAMsg("掛「おやおや＿うれる＿しゅーとが", "ひとつもない＿みたいだよ？");
      break;
    case enBBAMsg_InvMax:
      GenSetBBAMsg("掛「ごめんね＿これいじょうは", "しゅーとわくを＿ふやせないよ");
      break;
    }
  }

  void TUGSpBBA::DoUpdate(BOOL draw_f)
  {
    //進行
    field_->Update(draw_f);

    ugclbba_->Update(draw_f);
    ugdoor_->Update(draw_f);
    ugkantoku_->Update(draw_f);
	ugcap_->Update(draw_f);
    ugserifu_.Update(draw_f);
  }


  void TUGSpBBA::Mv_Default()
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd)) ugclbba_->SetMType(cmt_Stand);
    
  }
  void TUGSpBBA::Mv_Out()//退出時
  {
    if (ugdoor_->IsActRes(enUGDoorAct_Half))
    {
//      ugkantoku_->SetDraw(FALSE);
    }
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))
    {
      ugclbba_->SetMType(cmt_Stand);
    }
    if (ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      Down();
    }
  }

  void TUGSpBBA::DoAction()
  {
    //メッセージタイプごとの店員と監督の挙動
    switch (stBBA_.mtype_)
    {
    case enBBAMsg_Out: Mv_Out(); break;//退出時
    default: Mv_Default(); break;//それいがい
    }

	// メッセージスキップ処理
	if (field_->sc_IsCamEnter())
	{
		if (ugserifu_.IsReadLineEnd())
		{
			ugserifu_.NextSerifu();
			ugclbba_->SetMType(cmt_Stand);
		}
		else
		{
			ugserifu_.SkipSerifu();
			ugclbba_->SetMType(cmt_Stand);
		}
	}
  }

}
