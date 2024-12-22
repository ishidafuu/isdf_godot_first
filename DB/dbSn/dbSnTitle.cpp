#include "dbSnTitle.h"

namespace db
{

  TSnTitle::TSnTitle(TGame* pGame, TMgUI* pMgUI, s32 myScene)
    : TScene(pGame, pMgUI, myScene)
  {
    //setlayer_f_ = FALSE;
    //goto_f_ = FALSE;
  }

  TSnTitle::~TSnTitle()
  {
    FreeLayer();
  }

  void TSnTitle::DoSetLayer()
  {
    //if (setlayer_f_) return;
    frame_ = new TUIFrame(this);
    tiGrp_ = new TLyTiGrp(this);
    lyTitle_ = new TLyTiTitle(this);

    pmgUI_->Init((enSceneName)GetMyScene());
    //UIMgに登録hmGrp_
    pmgUI_->SetUIFrame(frame_);
    pmgUI_->SetUILayer(tiGrp_);
    pmgUI_->SetUILayer(lyTitle_);
    pmgUI_->LoadLayer();

    //setlayer_f_ = TRUE;
    //goto_f_ = FALSE;
  }
  void TSnTitle::DoFreeLayer()
  {

    //if (setlayer_f_ == FALSE) return;
    pmgUI_->FreeUILayer();
    pmgUI_->Init(snEND);
    Freemem(frame_);

    //setlayer_f_ = FALSE;
  }
  //タイトル用レディ
  void TSnTitle::DoReady()
  {
    pmgEO_->mgSound_.StopALL();
    pmgEO_->LoadData();
    pmgEO_->LoadGrp();
    pmgEO_->mgNSF_.Play(nsf_title);//BGM

    //全スプライトたたむ
    pmgEO_->mgFont_.ResetFontSpriteAll();

//    //音量設定
//    if (mid::midIsIOS())
//    {
//      pmgEO_->mgSound_.SetBGMVol(50);
//      pmgEO_->mgSound_.SetSEVol(50);
//    }
    //ＵＩレイヤセット
    SetLayer();
    //pmgEO_->stSeq_.PushLastScene(TStSnSeq::ls_Def);//普通のところから来ました
    lyTitle_->Ready();
  }

  void TSnTitle::GoToHome()
  {
    //goto_f_ = TRUE;
    SetGotoNext();
  }

  //更新
  void TSnTitle::DoUpdate()
  {
    if (IsGotoNext())
    {
#ifdef __K_DEBUG_USER_DATA__
        // チュートリアルが終わってなければこっちに行く
        if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Gousei))
#else
        //合成が最後のチュートリアル
        if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Gousei))
#endif
        {
            SetNextSceneQuick(snHome);
        }
        else
        {
            SetNextSceneQuick(snTutorial);
        }
      FreeLayer();
      return;
    }

    if (IsSetLayerFlag() == FALSE) return;

    pmgUI_->Update();
  }
  //描画
  void TSnTitle::DoDraw()
  {
    if (IsSetLayerFlag() == FALSE)
    {
      mid::midSkipDraw();
      return;
    }
    //背景はレイヤ側で描画
    pmgUI_->Draw(FALSE);
  }
  //更新
  void TSnTitle::DoGotoTitleFilter()
  {
    //lyShBar_->SetFilterAll(FALSE);
  }
}
