#include "dbSnShiai.h"

namespace db
{
  TSnShiai::TSnShiai(TGame* pGame, TMgUI* pMgUI, s32 myScene)
    : TScene(pGame, pMgUI, myScene)
  {
    //setlayer_f_ = FALSE;
    //goto_f_ = FALSE;
    //tutorial_f_ = FALSE;
  }

  TSnShiai::~TSnShiai()
  {
    FreeLayer();
  }


  void TSnShiai::DoSetLayer()
  {
	  // ランキングへ
	  if (pmgEO_->stSeq_.GetShiaiRanking())
	  {
		  SetRankingLayer();
	  }
	  else
	  {
		  SetShiaiLayer();
	  }
  }

  void TSnShiai::SetShiaiLayer()
  {
	  if (IsSetLayerFlag()) return;

	  frame_ = new TUIFrame(this);
	  lyShBar_ = new TLyShBar(this);
	  frame_->SetHedder(lyShBar_);

	  if (pmgEO_->stShiai_.IsVSHum())
	  {
		  lyShiai_ = new TLyShiaiVS(this, &lyShBar_->ugshiai_);
	  }
	  else if (pmgEO_->stSeq_.IsTutoRetro())
	  {
		  lyShiai_ = new TLyShiaiTutoRetro(this, &lyShBar_->ugshiai_);
	  }
	  else if (pmgEO_->stSeq_.IsTutoSmart())
	  {
		  lyShiai_ = new TLyShiaiTutoSmart(this, &lyShBar_->ugshiai_);
	  }
	  else if(pmgEO_->stSeq_.IsTutoAuto())
	  {
		  lyShiai_ = new TLyShiaiTutoAuto(this, &lyShBar_->ugshiai_);
	  }
	  else
	  {
		  lyShiai_ = new TLyShiai(this, &lyShBar_->ugshiai_);
	  }

	  lyShPause_ = new TLyShPause(this);
	  lyShRetry_ = new TLyShRetry(this);
	  lyShResult_ = new TLyShResult(this);
	  lyShVSError_ = new TLyShVSError(this);

    pmgUI_->Init((enSceneName)GetMyScene());
	  //UIMgに登録
	  pmgUI_->SetUIFrame(frame_);
	  pmgUI_->SetUILayer(lyShBar_);

	  pmgUI_->SetUILayer(lyShiai_);
	  pmgUI_->SetUILayer(lyShPause_);
	  pmgUI_->SetUILayer(lyShRetry_);
	  pmgUI_->SetUILayer(lyShResult_);
	  pmgUI_->SetUILayer(lyShVSError_);

	  //pmgUI_->SetUILayer(lyTechno_);
	  pmgUI_->LoadLayer();
	  //setlayer_f_ = TRUE;
	  //goto_f_ = FALSE;
	  //backhome_f_ = FALSE;
	  nextScene_ = snShiai;
	  //lyShBar_->Ready();
	  //フレームレートここで切り替え
	  mid::midSet30FPS(pmgEO_->stShiai_.IsFPS30());

	  lyShBar_->ReadyShiai();
	  isRanking_ = FALSE;
  }
  void TSnShiai::SetRankingLayer()
  {
	  if (IsSetLayerFlag()) return;

	  frame_ = new TUIFrame(this);
	  lyShBar_ = new TLyShBar(this);
	  frame_->SetHedder(lyShBar_);

	  lyShRank_ = new TLyShRank(this);

	  //UIMgに登録
    pmgUI_->Init((enSceneName)GetMyScene());
	  pmgUI_->SetUIFrame(frame_);
	  pmgUI_->SetUILayer(lyShBar_);
	  pmgUI_->SetUILayer(lyShRank_);
	  pmgUI_->LoadLayer();
	  //backhome_f_ = FALSE;
	  nextScene_ = snShiai;
	  lyShBar_->ReadyRanking();

	  isRanking_ = TRUE;
  }


  void TSnShiai::DoFreeLayer()
  {
    if (IsSetLayerFlag() == FALSE) return;

    pGame_->TexRerease();
    pmgUI_->FreeUILayer();
    pmgUI_->Init(snEND);
    Freemem(frame_);
    //setlayer_f_ = FALSE;

    //その他後始末
    pmgEO_->mgCoU_.CleanOtherKantoku();//他人情報削除
    mid::midSet30FPS(FALSE);//フレームレート戻す
  }
  void TSnShiai::GoToMap()
  {
    SetGotoNext();
    //goto_f_ = TRUE;
    //backhome_f_ = FALSE;
	nextScene_ = snScenario;
  }
  void TSnShiai::GoToHome()
  {
    SetGotoNext();
    //goto_f_ = TRUE;
    //backhome_f_ = TRUE;
	nextScene_ = snHome;
  }
  void TSnShiai::GoToEventHome()
  {
	  SetGotoNext();
	  //goto_f_ = TRUE;
	  //backhome_f_ = TRUE;
	  nextScene_ = snHome;
  }
  void TSnShiai::GoToRanking()
  {
	  pmgEO_->stSeq_.SetShiaiRanking(TRUE);
	  SetGotoNext();
	  //backhome_f_ = FALSE;
	  nextScene_ = snShiai;
  }
  //void TSnShiai::GoToTutorial()
  //{
  //  goto_f_ = TRUE;
  //  tutorial_f_ = TRUE;
  //}

  //レディ
  void TSnShiai::DoReady()
  {
	  // 初回チュートリアルの時だけそのまま流しっぱなし
	if(!  pmgEO_->stSeq_.IsFirstTuto() )
	{
	    pmgEO_->mgSound_.StopALL();
	}

    //全スプライトたたむ
    pmgEO_->mgFont_.ResetFontSpriteAll();

//    //音量設定
//    pmgEO_->mgSound_.SetBGMVol(50);
//    pmgEO_->mgSound_.SetSEVol(50);

    //ＵＩレイヤセット
    SetLayer();
  }

  //更新
  void TSnShiai::DoUpdate()
  {
    if (IsGotoNext())// || IsGotoTitle())
    {

      //enSceneName next = (backhome_f_)
      //  ? snHome
      //  : snScenario;
		enSceneName next = nextScene_;

      //初回チュートだけチュートリアルシーンにもどる
      if (pmgEO_->stSeq_.IsFirstTuto())
      {
        next = snTutorial;
      }

      //if (tutorial_f_)
      //{
      //  // 試合のチュートリアルが終わったので
      //  // チュートリアルシーンに帰る
      //  next = snTutorial;
      //}
      //else
      //{
      //  next = (backhome_f_)
      //    ? snHome
      //    : snScenario;
      //}
      //SetNextScene(next, FALSE);
      SetNextSceneQuick(next);
      //レイヤ解放
      FreeLayer();
    }
    if (IsSetLayerFlag() == FALSE) return;
    //UIのアップデート
    pmgUI_->Update();
  }
  //描画
  void TSnShiai::DoDraw()
  {
    if (IsSetLayerFlag() == FALSE) return;
	pmgUI_->Draw(isRanking_);
  }
  //更新
  void TSnShiai::DoGotoTitleFilter()
  {
    lyShBar_->SetFilterAll(TRUE);
  }
}
