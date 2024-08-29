#include "dbSnHome.h"

namespace db
{
  enum enNextScene
  {
    ns_Scenario,
    ns_Title,
    ns_VSShiai,
  };

  //メニュー///////////////////////////
  TSnHome::TSnHome(TGame* pGame, TMgUI* pMgUI, s32 myScene)
    : TScene(pGame, pMgUI, myScene)
  {
    //page_ = mp_MapMove;
    stHm_.Init();
    //setlayer_f_ = FALSE;
    //goto_f_ = FALSE;
  }

  TSnHome::~TSnHome()
  {
    FreeLayer();
  }

  //レディ
  void TSnHome::DoReady()
  {
    stHm_.Init();

    //SceneBGM(1);
    if (GetLastScene() != snTutorial)//チュートリアル終了時は曲同じなので買えない
    {
      pmgEO_->mgNSF_.Play(nsf_home);//BGM
    }

    //メニューに戻ってきたら通信切断
    mid::midSetDisconnect(FALSE);

    //全スプライトたたむ
    pmgEO_->mgFont_.ResetFontSpriteAll();

    //カメラ初期化
    pmgEO_->mgCamera_.SetCamInit();
    pmgEO_->mgCamera_.SetCamData(TRUE, FALSE, mid::midGetDotL(), mid::midGetDotS(), 0, 0);
    pmgEO_->mgCamera_.SetCamPos(0, 0);


    //pmgEO_->stSeq_.PushFromHome();//ホームから来ました
    if (pmgEO_->stSeq_.IsNoSv()) pmgUI_->LoadBG("vs_bg");//背景
    //レイヤーセット
    SetLayer();
    //メニューには不要
    //でreturnしてるやつをけす
  }

  void TSnHome::DoSetLayer()
  {

	  mid::midLog("DoSetLayer");
    //if (setlayer_f_) return;

    stHm_.Init();


    frame_ = new TUIFrame(this);
    lyHmBar_ = new TLyHmBar(this);
    frame_->SetHedder(lyHmBar_);

    hmGrp_ = new TLyHmGrp(this);
    lyHmHome_ = new TLyHmHome(this);
    lyHmMaveko_ = new TLyHmMaveko(this);
    lyHmMami_ = new TLyHmMami(this);
	lyHmHasebe_ = new TLyHmHasebe(this);

    lyHmShiai_ = new TLyHmShiai(this);
    lyHmShVS_ = new TLyHmShVSHum(this);

    if (IsSTuShiai())
    {
      lySTuShiai_ = new TLySTuShiai(this);
    }
    else
    {
      lySTuShiai_ = NULL;
    }

	lyHmEvent_ = new TLyHmEvent(this);
	lyHmEvEvent_ = new TLyHmEvEvent(this);

    lyHmShop_ = new TLyHmShop(this);
    lyHmShopInside_ = new TLyHmShopInside(this);
    lyHmGachaInside_ = new TLyHmGachaInside(this);
    lyHmHukuInside_ = new TLyHmHukuInside(this);

    lyHmTeam_ = new TLyHmTeam(this);
    lyHmTmSelect_ = new TLyHmTmSelect(this);
    lyHmTmHaikei_ = new TLyHmTmHaikei(this);
    lyHmTmBGM_ = new TLyHmTmBGM(this);

    if (IsSTuTeam())
    {
      lySTuTeam_ = new TLySTuTeam(this);
    }
    else
    {
      lySTuTeam_ = NULL;
    }
    lyHmCenter_ = new TLyHmCenter(this);
    lyHmCDock_ = new TLyHmCDock(this);
    lyHmCBBA_ = new TLyHmCBBA(this);
    lyHmCMaruka_ = new TLyHmCMaruka(this);
    //lyHmCTuto_ = new TLyHmCTuto(this);

    lyTechno_ = new TLyTechno(this, this->frame_, lyHmBar_);
    lyProfile_ = new TLyProfile(this, this->frame_, lyHmBar_);


    //タブの切り替えで初期化が入るページ群
    lyHmBar_->tabs_.push_back(lyHmHome_);
    lyHmBar_->tabs_.push_back(lyHmMaveko_);
    lyHmBar_->tabs_.push_back(lyHmMami_);
	lyHmBar_->tabs_.push_back(lyHmHasebe_);

    lyHmBar_->tabs_.push_back(lyHmShiai_);
    lyHmBar_->tabs_.push_back(lyHmShVS_);
    if (IsSTuShiai()) lyHmBar_->tabs_.push_back(lySTuShiai_);

	lyHmBar_->tabs_.push_back(lyHmEvent_);
	lyHmBar_->tabs_.push_back(lyHmEvEvent_);
    lyHmBar_->tabs_.push_back(lyHmShop_);
    lyHmBar_->tabs_.push_back(lyHmShopInside_);
    lyHmBar_->tabs_.push_back(lyHmGachaInside_);
    lyHmBar_->tabs_.push_back(lyHmHukuInside_);

    lyHmBar_->tabs_.push_back(lyHmTeam_);
    lyHmBar_->tabs_.push_back(lyHmTmSelect_);
    lyHmBar_->tabs_.push_back(lyHmTmHaikei_);
    lyHmBar_->tabs_.push_back(lyHmTmBGM_);
    if (IsSTuTeam()) lyHmBar_->tabs_.push_back(lySTuTeam_);
    lyHmBar_->tabs_.push_back(lyHmCenter_);
    lyHmBar_->tabs_.push_back(lyHmCDock_);
    lyHmBar_->tabs_.push_back(lyHmCBBA_);
    lyHmBar_->tabs_.push_back(lyHmCMaruka_);
    //lyHmBar_->tabs_.push_back(lyHmCTuto_);
    //lyHmBar_->tabs_.push_back(lyHmTechno_);

    //UIMgに登録
    pmgUI_->Init((enSceneName)GetMyScene());
    pmgUI_->SetUIFrame(frame_);

    pmgUI_->SetUILayer(lyHmBar_);
    pmgUI_->SetUILayer(lyTechno_);
    pmgUI_->SetUILayer(lyProfile_);

    pmgUI_->SetUILayer(hmGrp_);//リソースなので先に読むように先にセット
    pmgUI_->SetUILayer(lyHmHome_);
    pmgUI_->SetUILayer(lyHmMaveko_);
    pmgUI_->SetUILayer(lyHmMami_);
	pmgUI_->SetUILayer(lyHmHasebe_);

    pmgUI_->SetUILayer(lyHmShiai_);
    pmgUI_->SetUILayer(lyHmShVS_);
    if (IsSTuShiai()) pmgUI_->SetUILayer(lySTuShiai_);

	pmgUI_->SetUILayer(lyHmEvent_);
	pmgUI_->SetUILayer(lyHmEvEvent_);
    
    pmgUI_->SetUILayer(lyHmShop_);
    pmgUI_->SetUILayer(lyHmShopInside_);
    pmgUI_->SetUILayer(lyHmGachaInside_);
    pmgUI_->SetUILayer(lyHmHukuInside_);

    pmgUI_->SetUILayer(lyHmTeam_);
    pmgUI_->SetUILayer(lyHmTmSelect_);
    pmgUI_->SetUILayer(lyHmTmHaikei_);
    pmgUI_->SetUILayer(lyHmTmBGM_);

    if (IsSTuTeam()) pmgUI_->SetUILayer(lySTuTeam_);
    

    pmgUI_->SetUILayer(lyHmCenter_);
    pmgUI_->SetUILayer(lyHmCDock_);
    pmgUI_->SetUILayer(lyHmCBBA_);
    pmgUI_->SetUILayer(lyHmCMaruka_);
    //pmgUI_->SetUILayer(lyHmCTuto_);

    pmgUI_->LoadLayer();

    s64 mid_haikei = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei);
    pmgUI_->LoadCourt(mid_haikei);//背景

    //setlayer_f_ = TRUE;
    //goto_f_ = FALSE;

    lyHmBar_->ReadyHome();

    //まみのこうしん
    pmgEO_->stSeq_.PushNeedMami();
    //まるかのこうしん
    pmgEO_->stSeq_.PushNeedMaruka();

    mid::midLog("DoSetLayer ok");
  }
  void TSnHome::DoFreeLayer()
  {
    //if (setlayer_f_ == FALSE) return;
    pmgUI_->FreeUILayer();
    pmgUI_->Init(snEND);
    Freemem(frame_);

    //setlayer_f_ = FALSE;
  }

  void TSnHome::GotoNextScene()
  {
    //goto_f_ = TRUE;
    SetGotoNext();
  }
  void TSnHome::SetNextScenario()
  {
    stHm_.nextscene_ = ns_Scenario;
  }
  void TSnHome::SetNextTitle()
  {
    stHm_.nextscene_ = ns_Title;
  }
  void TSnHome::SetNextVSShiai()
  {
    stHm_.nextscene_ = ns_VSShiai;
  }

  //更新
  void TSnHome::DoUpdate()
  {

    if (IsGotoNext())// || IsGotoTitle())
    {
      switch (stHm_.nextscene_)
      {
      case ns_Title:SetNextSceneQuick(snTitle);  break;
      case ns_VSShiai:SetNextSceneQuick(snShiai);  break;
      default:SetNextSceneQuick(snScenario); break;
      }

      FreeLayer();
    }

	if (IsSetLayerFlag() == FALSE)
	{
	  mid::midSkipDraw();
	  return;
	}

    pmgUI_->Update();

  }

  //描画
  void TSnHome::DoDraw()
  {
	if (IsSetLayerFlag() == FALSE)
	{
	  mid::midSkipDraw();
	  return;
	}
    pmgUI_->Draw(TRUE);

  }

  void TSnHome::DoGotoTitleFilter()
  {
    lyHmBar_->SetFilterAll(TRUE);
  }

  BOOL TSnHome::IsSTuShiai()
  {
#ifdef __K_DEBUG_USER_DATA__
    return (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubEventOpen));
#else
    return ((pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubEventOpen) == FALSE));
#endif
  }

  BOOL TSnHome::IsSTuTeam()
  {
#ifdef __K_DEBUG_USER_DATA__
    return (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubTeam));
#else
    return (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubTeam) == FALSE);
#endif
  }

}
