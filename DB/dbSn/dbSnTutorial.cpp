#include "dbSnTutorial.h"
//=========================================================================
// 
// チュートリアルシーンクラス
//
//=========================================================================
namespace db
{
  enum stTutorial
  {
    st_Init,
    st_Opening,
    st_TmSelect,
    st_GameSousa,
    st_Home,
    st_Gatya,
    st_Gousei,
  };


  //-------------------------------------------------------
  // コンストラクタ
  //-------------------------------------------------------
  TSnTutorial::TSnTutorial(TGame* pGame, TMgUI* pMgUI, s32 myScene)
    : TScene(pGame, pMgUI, myScene)
    , frame_(NULL)
    , tuGrp_(NULL)
    , lyTuBar_(NULL)
    , lyTuTest_(NULL)
    , lyTuTmSelect_(NULL)
    , lyTuHome_(NULL)
    , lyTuHmMami_(NULL)
    , lyTuGatya_(NULL)
    //, setlayer_f_(FALSE)
  {
    // ここで今チュートリアルの状態を確認
    // とりあえず仮で数値が返ってくるものとする
    state_ = st_TmSelect;
  }

  //-------------------------------------------------------
  // デストラクタ
  //-------------------------------------------------------
  TSnTutorial::~TSnTutorial()
  {
  }

  //-------------------------------------------------------
  // 準備
  //-------------------------------------------------------
  void TSnTutorial::DoReady(void)
  {
    //SceneBGM(1);
    //pmgEO_->mgNSF_.Play(nsf_home);//BGM

    //メニューに戻ってきたら通信切断
    mid::midSetDisconnect(FALSE);

    //全スプライトたたむ
    pmgEO_->mgFont_.ResetFontSpriteAll();

    //カメラ初期化
    pmgEO_->mgCamera_.SetCamInit();
    pmgEO_->mgCamera_.SetCamData(TRUE, FALSE, mid::midGetDotL(), mid::midGetDotS(), 0, 0);
    pmgEO_->mgCamera_.SetCamPos(0, 0);

    //レイヤーセット
    SetLayer();

    // ここで何かの値を見てどこに入っていくか見る
#ifdef __K_DEBUG_ROOT__
	if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Gousei))
	{
		// なんでここにいんの？
		// とりあえず合成
		state_ = st_Gousei;
	}
    else if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Gatya) &&
      kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_NaBox))
    {
		state_ = st_Gousei;
	}
	else if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Home) ||
		kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Kantoku))
	{
		state_ = st_Gatya;
	}
	else if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_GameSousa))
	{
		state_ = st_Home;
	}
	else if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_TmSelect))
	{
		state_ = st_GameSousa;
	}
	else if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Opening))
	{
		state_ = st_TmSelect;
	}
	else
    {
		state_ = st_TmSelect;
		//state_ = st_Opening;
    }
#else
    if ((pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Gousei)))
    {
      state_ = st_Gousei;
    }
	else if ((pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Gatya)) &&
		(pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_NaBox) ))
	{
		state_ = st_Gousei;
	}
	else if ((pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Home)) ||
		(pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Kantoku)))
	{
		state_ = st_Gatya;
	}
	else if ((pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_GameSousa)))
	{
		state_ = st_Home;
	}
	else if ((pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_TmSelect)))
	{
		state_ = st_GameSousa;
	}
	else if ((pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Opening)))
	{
		state_ = st_TmSelect;
	}
	else
    {
		state_ = st_TmSelect;
      //state_ = st_Opening;
    }
#endif
    switch (state_)
    {
    case st_Init:		Init();			break;
    case st_Opening:	Opening();		break;
    case st_TmSelect:	TeamSelect();	break;
    case st_GameSousa:	GameSousa();	break;
    case st_Home:		HomeSousa();	break;
    case st_Gatya:		GatyaSousa();	break;
    case st_Gousei:		Gousei();		break;
    }

    //ここで曲を決める
    
    switch (state_)
    {
    case st_TmSelect:
      pmgEO_->mgNSF_.Play(nsf_tuto);//BGM
      break;
    default:
      pmgEO_->mgNSF_.Play(nsf_home);//BGM
      break;
    }
  }

  //-------------------------------------------------------
  // レイヤーセット
  //-------------------------------------------------------
  void TSnTutorial::DoSetLayer(void)
  {
    //if (setlayer_f_) return;

    frame_ = new TUIFrame(this);
    tuGrp_ = new TLyTuGrp(this);
    lyTuBar_ = new TLyTuBar(this);

    frame_->SetHedder(lyTuBar_);

    //lyTuTest_ = new TLyTuTest(this);
    lyTuTmSelect_ = new TLyTuTmSelect(this);
    lyTuHome_ = new TLyTuHome(this);
    lyTuHmMami_ = new TLyTuHmMami(this);

    lyTuGatya_ = new TLyTuGatya(this);
    lyTuShop_ = new TLyTuShop(this);

    lyTuCenter_ = new TLyTuCenter(this);
    lyTuCDock_ = new TLyTuCDock(this);

    lyTuProfile_ = new TLyTuProfile(this, this->frame_, lyTuBar_);

    lyTuTmSelect_->SetFooterPointer();

    //タブ移動でDownするレイヤー
    lyTuBar_->tabs_.push_back(lyTuTmSelect_);
    lyTuBar_->tabs_.push_back(lyTuHome_);
    lyTuBar_->tabs_.push_back(lyTuHmMami_);
    //lyTuBar_->tabs_.push_back(lyTuProfile_);
    lyTuBar_->tabs_.push_back(lyTuShop_);
    lyTuBar_->tabs_.push_back(lyTuGatya_);
    lyTuBar_->tabs_.push_back(lyTuCenter_);
    lyTuBar_->tabs_.push_back(lyTuCDock_);

    //UIMgに登録
    pmgUI_->Init((enSceneName)GetMyScene());
    pmgUI_->SetUIFrame(frame_);
    pmgUI_->SetUILayer(tuGrp_);	// リソースクラス

    pmgUI_->SetUILayer(lyTuBar_);
    pmgUI_->SetUILayer(lyTuProfile_);
    //pmgUI_->SetUILayer(lyTuTest_);
    pmgUI_->SetUILayer(lyTuTmSelect_);
    pmgUI_->SetUILayer(lyTuHome_);
    pmgUI_->SetUILayer(lyTuHmMami_);
    pmgUI_->SetUILayer(lyTuShop_);
    pmgUI_->SetUILayer(lyTuGatya_);
    pmgUI_->SetUILayer(lyTuCenter_);
    pmgUI_->SetUILayer(lyTuCDock_);

    pmgUI_->LoadLayer();

    // レイヤーセット完了
    //setlayer_f_ = TRUE;
  }

  //-------------------------------------------------------
  // レイヤーの解放
  //-------------------------------------------------------
  void TSnTutorial::DoFreeLayer(void)
  {
    // レイヤーがセットされてなければ何もしない
    //if (setlayer_f_ == FALSE) return;

    pmgUI_->FreeUILayer();
    pmgUI_->Init(snEND);
    //Freemem(lyTuTmSelect_);
    //Freemem(lyTuTest_);
    //Freemem(lyTuBar_);
    //Freemem(tuGrp_);
    //Freemem(frame_);

    //setlayer_f_ = FALSE;
  }

  //-------------------------------------------------------
  // 更新
  //-------------------------------------------------------
  void TSnTutorial::DoUpdate(void)
  {
    // レイヤーがセットされてなければ更新するものなし
    if (IsSetLayerFlag() == FALSE) return;
    pmgUI_->Update();

    switch (state_)
    {
      //case st_Init:
      //	break;

    case st_Opening:
      break;

    case st_TmSelect:
      // チームセレクトが終わったら
      // 操作説明へ
      if (lyTuTmSelect_->IsTutoEnd())
      {
        GameSousa();
      }
      break;

    case st_GameSousa:
      // ここは別のシーン(チュートリアル試合シーン)なので
      // 特に何もしない
      break;

    case st_Home:
      // ホームの説明が終わったら
      // ガチャ説明へ
      if (lyTuHome_->IsTutoEnd())
      {
        GatyaSousa();
      }
      break;

    case st_Gatya:
      // ガチャについての説明が終わったら
      // シュート合成へ
      if (lyTuShop_->IsTutoEnd())
      {
        Gousei();
      }
      break;

    case st_Gousei:
      // ここは別のシーン(チュートリアルシュート合成シーン)なので
      // 特に何もしない？
      if (lyTuCenter_->IsTutoEnd())
      {
        EndTuto();
      }
      //チュートリアルから普通のホームシーンに切り替わるように
      break;
    }
  }

  //-------------------------------------------------------
  // 描画
  //-------------------------------------------------------
  void TSnTutorial::DoDraw()
  {
    // レイヤーがセットされてなければ更新するものなし
    if (IsSetLayerFlag() == FALSE) return;
    pmgUI_->Draw(TRUE);
  }

  //-------------------------------------------------------
  // 描画
  //-------------------------------------------------------
  void TSnTutorial::DoGotoTitleFilter()
  {
    SetNextSceneQuick(snTitle);
    FreeLayer();
  }

  //-------------------------------------------------------
  // 初期化
  //-------------------------------------------------------
  void TSnTutorial::Init(void)
  {
    state_ = st_Init;
  }
  //-------------------------------------------------------
  // 初期化:オープニング
  //-------------------------------------------------------
  void TSnTutorial::Opening(void)
  {
    state_ = st_Opening;
  }
  //-------------------------------------------------------
  // 初期化:チーム選択
  //-------------------------------------------------------
  void TSnTutorial::TeamSelect(void)
  {
    // チーム選択
    lyTuBar_->Ready();
    lyTuTmSelect_->Ready();
    state_ = st_TmSelect;
    
  }
  //-------------------------------------------------------
  // 初期化:操作説明
  //-------------------------------------------------------
  void TSnTutorial::GameSousa(void)
  {
	  s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
#ifdef __K_DEBUG_USER_DATA__
	  s32 mid_team = kdebug::DebugUDManager::GetInstance()->GetLastTeamID();
#else
    s32 mid_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(0, mdteam_id_m_team);
#endif
    //チーム選択後、中断したときここを通るので、チーム指定が必要
    s32 no = 0;
    for (s32 i = 0; i < gl_Max; i++)
    {
      if (TEAM_LIST[i] == mid_team)
      {
        no = i;
        break;
      }
    }
    
    s32 mid_court = BG_LIST[no];
    s32 mid_bgm = BGM_LIST[no];
    // 操作説明
    pmgEO_->stResult_.Init();
    pmgEO_->stDrop_.Init();
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	pmgEO_->stShiai_.SetFirstTuto(id_kantoku, mid_team, mid_court, mid_bgm, nsf_jgl_ready, nsf_jgl_win, nsf_jgl_lose, pmgEO_->mgCoU_.GetCtrl());
#else
	pmgEO_->stShiai_.SetFirstTuto(id_kantoku, mid_team, mid_court, mid_bgm, pmgEO_->mgCoU_.GetCtrl());
#endif
    pmgEO_->stSeq_.SetFirstTuto();
    SetNextSceneQuick(snShiai);
    FreeLayer();
    state_ = st_GameSousa;
  }
  //-------------------------------------------------------
  // 初期化:
  //-------------------------------------------------------
  void TSnTutorial::HomeSousa(void)
  {
    lyTuBar_->Ready();
    lyTuHome_->Ready();
    state_ = st_Home;
    
  }
  //-------------------------------------------------------
  // 初期化:
  //-------------------------------------------------------
  void TSnTutorial::GatyaSousa(void)
  {
    lyTuBar_->Ready();
    pmgUI_->LoadBG("shop_bg");//背景
    lyTuShop_->Ready();
    state_ = st_Gatya;
  }
  //-------------------------------------------------------
  // 初期化:
  //-------------------------------------------------------
  void TSnTutorial::Gousei(void)
  {
    lyTuBar_->Ready();
    pmgUI_->LoadBG("dock_bg");//背景
    lyTuCenter_->Ready();
    state_ = st_Gousei;
  }
  void TSnTutorial::EndTuto(void)
  {
    SetNextSceneQuick(snHome);
    FreeLayer();
  }
}
