#include "dbLyShBar.h"
#include "dbSnShiai.h"

namespace db
{

  enum enPage
  {
    pg_Bar,
    pg_Filter,
    pg_Retry,
    pg_RetryStar,
    pg_Retire,
    pg_Result,
  };

  enum enHudWait
  {
    hud_Message,
    hud_Retry,
    hud_RetryStar,
    hud_Retire,
    hud_Result,
  };


  //コンストラクタ
  TLyShBar::TLyShBar(TSnShiai* pScene) : TLyBarGen(pScene, pScene->frame_)
  {
    pScene_ = pScene;
    stShBar_.Init();
    //試合用
    SetMakeShiai();
  }

  TLyShBar::~TLyShBar()
  {

  }
  void TLyShBar::DoOpenHeader()
  {

  }
  void TLyShBar::DoCloseHeader()
  {

  }
  void TLyShBar::DoJumpHeader()
  {
  
  }
  void TLyShBar::DoOpenTechno()
  {

  }
  void TLyShBar::DoOpenProfile()
  {

  }
  void TLyShBar::DoMoveFilter()
  {

  }
  void TLyShBar::DoMoveFilterAll()
  {
    switch (stShBar_.nowLayer_)
    {
    case shl_Map:pScene_->GoToMap(); break;
    case shl_Home:pScene_->GoToHome(); break;
	case shl_EventHome:pScene_->GoToEventHome(); break;
	case shl_Rank:pScene_->GoToRanking(); break;
    }
  }
  void TLyShBar::DoBackBtn()
  {

  }
  void TLyShBar::ReadyShiai()
  {
    Ready();
    SetNoneBar(TRUE);
    pScene_->lyShiai_->Ready();
  }
  void TLyShBar::ReadyRanking()
  {
	  Ready();
	  SetNoneBar(TRUE);
	  pScene_->lyShRank_->Ready();
	  pmgUI_->LoadBG("vs_bg");//背
	  //pmgUI_->LoadCourt(4);
  }
  void TLyShBar::IdleShiai()
  {
    pScene_->lyShiai_->Idle();
  }

  void TLyShBar::RestartShiai()
  {
    pScene_->lyShiai_->Restart();
  }

  void TLyShBar::MoveLayer(enShLayer mvlayer)
  {
    switch (mvlayer)
    {
    case shl_Play://プレー
      SetNoneBar(TRUE);//バーを消す
      switch (stShBar_.nowLayer_)
      {
      case db::shl_Retry: ugshiai_.Retry(); break;//復活処理
      case db::shl_Pause: ugshiai_.CanselPause(); break;//ポーズ解除
      }
      RestartShiai();
      break;
    case shl_Retry://リトライ確認
      SetNoneBar(FALSE);
      pScene_->lyShRetry_->Ready();
      IdleShiai();
      break;
    case shl_Pause://ポーズ
      SetNoneBar(FALSE);
      pScene_->lyShPause_->Ready();
      IdleShiai();
      break;
    case shl_Result: //結果
      SetNoneBar(FALSE);
      pScene_->lyShResult_->Ready();
      IdleShiai();
      break;
    case shl_VSError: //対戦エラー
      SetNoneBar(FALSE);
      pScene_->lyShVSError_->Ready();
      IdleShiai();
      break;
    case shl_Map: //マップへ戻る
      SetFilterAll(FALSE);
      break;
    case shl_Home: //ホームへ戻る
      SetFilterAll(FALSE);
      break;
	case shl_Rank:	// ランキング画面
		SetFilterAll(FALSE);
		break;
	case shl_EventHome: // イベントのホームへ戻る
		SetFilterAll(FALSE);
		break;
	}

    //レイヤ変更
    stShBar_.nowLayer_ = mvlayer;
  }



}