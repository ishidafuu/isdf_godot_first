#include "dbLyTuScTSL.h"
#include "dbSnScenario.h"
#include "dbUGSerifu.h"
#include "dbUGFinger.h"

namespace db
{

  enum enPage
  {
    pg_TutoStamina,
    pg_TutoStart,

    pg_TmSelect,
    pg_GotoVS,
  };

  //-------------------------------------------------
  // チュートリアルテキスト
  enum enTutoTSLTxtType
  {
    ttsltt_STAMINA1 = 0,	// スタミナの説明1
    ttsltt_STAMINA2,	// スタミナの説明2
    ttsltt_START,		// しあいかいしへの誘導テキスト
    ttsltt_SELECT,	// しあいかいしボタン待ち(チーム選択)

    ttsltt_MAX,
  };
  const s32 MSG_LINE_MAX = 3;
  const char* TUTO_MSG_TSL[ttsltt_MAX][MSG_LINE_MAX] =
  {
    {
      "掛「しきするチームを＿えらんでね",
      "つかったチームは＿つかれてしまい",
    },
    {
      "掛「スタミナが＿かいふくするまで",
      "しばらく＿つかえなくなります",
    },
    {
      "掛「チームが＿きまったら",
      "しあいかいしを＿タッチしてね",
    },
  };

  enum enConnect
  {
    cn_play,
    cn_mgk,
  };

#ifdef __DEBUG_POSITION_CHECK__
  std::vector<std::vector<s32>> pos(4, std::vector<s32>(2));
  std::vector<BOOL> LR(4, FALSE);
  std::vector<BOOL> UD(4, FALSE);
  std::vector<BOOL> DRAW(4, TRUE);
  const s32 PX = 0;
  const s32 PY = 1;
  s32 no = 0;
  void TLyTuScTSL::debug()
  {
    if (kdebug::DebugController::GetInstance()->IsRepeatButton(XINPUT_BUTTON_RIGHT_STICK_UP, 4))	 { pos[no][PY]--; }
    else if (kdebug::DebugController::GetInstance()->IsRepeatButton(XINPUT_BUTTON_RIGHT_STICK_DOWN, 4)){ pos[no][PY]++; }
    else if (kdebug::DebugController::GetInstance()->IsRepeatButton(XINPUT_BUTTON_RIGHT_STICK_RIGHT, 4)){ pos[no][PX]++; }
    else if (kdebug::DebugController::GetInstance()->IsRepeatButton(XINPUT_BUTTON_RIGHT_STICK_LEFT, 4)){ pos[no][PX]--; }
    else if (kdebug::DebugController::GetInstance()->IsRepeatButton(XINPUT_BUTTON_LEFT_STICK_UP, 4))	 { pos[no][PY] -= 10; }
    else if (kdebug::DebugController::GetInstance()->IsRepeatButton(XINPUT_BUTTON_LEFT_STICK_DOWN, 4)){ pos[no][PY] += 10; }
    else if (kdebug::DebugController::GetInstance()->IsRepeatButton(XINPUT_BUTTON_LEFT_STICK_RIGHT, 4)){ pos[no][PX] += 10; }
    else if (kdebug::DebugController::GetInstance()->IsRepeatButton(XINPUT_BUTTON_LEFT_STICK_LEFT, 4)){ pos[no][PX] -= 10; }
    else if (kdebug::DebugController::GetInstance()->IsRepeatButton(XINPUT_BUTTON_LEFT_SHOULDER))	 { no--;  if (no < 0) { no = 3; } }
    else if (kdebug::DebugController::GetInstance()->IsRepeatButton(XINPUT_BUTTON_RIGHT_SHOULDER)){ no++; if (no > 3) { no = 0; } }
    else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_LEFT_TRIGGER))	 { LR[no] = !LR[no]; }
    else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_RIGHT_TRIGGER)){ UD[no] = !UD[no]; }
    else if (kdebug::DebugController::GetInstance()->IsTriggerButton(XINPUT_BUTTON_START)){ DRAW[no] = !DRAW[no]; }


    pUGSerifu_->SetPos(pos[0][PX], pos[0][PY]);
    pUGFinger_->GetField()->z_SetPos(pos[1][PX], pos[1][PY]);
    pUGFinger2_->GetField()->z_SetPos(pos[2][PX], pos[2][PY]);
    pUGFinger3_->GetField()->z_SetPos(pos[3][PX], pos[3][PY]);
    pUGFinger_->hand_->g_SetMirror(LR[1]);
    pUGFinger2_->hand_->g_SetMirror(LR[2]);
    pUGFinger3_->hand_->g_SetMirror(LR[3]);
    pUGFinger_->SetRevUD(UD[1]);
    pUGFinger2_->SetRevUD(UD[2]);
    pUGFinger3_->SetRevUD(UD[3]);
    pUGSerifu_->SetDraw(DRAW[0]);
    pUGFinger_->SetDraw(DRAW[1]);
    pUGFinger2_->SetDraw(DRAW[2]);
    pUGFinger3_->SetDraw(DRAW[3]);

    kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
    char c[256];
    switch (no)
    {
    default:
    case 0: sprintf_s(c, "セリフ"); break;
    case 1: sprintf_s(c, "指1"); break;
    case 2: sprintf_s(c, "指2"); break;
    case 3: sprintf_s(c, "指3"); break;
    }
    s32 ty = 20;
    kdebug::DebugText::GetInstance()->SetDebugText(10, ty, c);
    sprintf(c, "x:%d", pos[no][PX]);
    ty += 8;
    kdebug::DebugText::GetInstance()->SetDebugText(10, ty, c);
    sprintf(c, "y:%d", pos[no][PY]);
    ty += 8;
    kdebug::DebugText::GetInstance()->SetDebugText(10, ty, c);
    if (LR[no]){ sprintf(c, "左右：反転"); }
    else			{ sprintf(c, "左右：通常"); }
    ty += 8;
    kdebug::DebugText::GetInstance()->SetDebugText(10, ty, c);
    if (UD[no]){ sprintf(c, "上下：反転"); }
    else			{ sprintf(c, "上下：通常"); }
    ty += 8;
    kdebug::DebugText::GetInstance()->SetDebugText(10, ty, c);
    if (DRAW[no]){ sprintf(c, "描画：ON"); }
    else			{ sprintf(c, "描画：OFF"); }
    ty += 8;
    kdebug::DebugText::GetInstance()->SetDebugText(10, ty, c);
  }
#endif // __DEBUG_POSITION_CHECK__


  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  TLyTuScTSL::TLyTuScTSL(TSnScenario* pScene) :TLyScGen(pScene)
    , ugdrTeam_(this)
    , ugencard_(this)
    , uglistTmFilter_(this)
    , ugtitle_me_(this)
    , ugtitle_en_(this)
    , id_prev_team_(NGNUM)
    , mid_prev_team_(NGNUM)
    , pUGSerifu_(NULL)
    , pUGFinger_(NULL)
    , pUGFinger2_(NULL)
    , pUGFinger3_(NULL)
    , pSerifuVec_(NULL)
    , tuTxtNo_(0)
  {
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  TLyTuScTSL::~TLyTuScTSL()
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::DoLoad()
  {
    stTSL_.Init();
    //field_ = MakeObj();
    filter_ = MakeObj();
    filter_->z_SetPos_SafeLF(0, 0);
    filter_->o_SetObjFilterGrp(512, 512, 0xc0, 0x00, 0x00, 0x00);

    //ドラム
    uglistTmFilter_.MakeGroup(field_, FALSE);
    ugdrTeam_.MakeGroup(field_, TRUE, SIDE0, &uglistTmFilter_);
    //    ugdrTeam_.SetDtNum(pmgEO_->mgCoU_.mdTeam_.GetLineNum());

    //敵チームカード
    ugencard_.MakeGroup(field_, SIDE1);
    ugencard_.SetLRPos(FALSE);
    ugencard_.StaminaNone(TRUE);

    ugtitle_me_.MakeGroup(field_, FALSE);
    ugtitle_me_.SetMessage("あなたのチーム");
    ugtitle_me_.SetLRPos(TRUE);
    ugtitle_en_.MakeGroup(field_, FALSE);
    ugtitle_en_.SetMessage("あいてのチーム");
    ugtitle_en_.SetLRPos(FALSE);

    //初期位置
    //初期チーム読み込み
    LoadLookPosTeam();
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::DoReady()
  {
    stTSL_.Init();
    //ugdrTeam_.RefreshRoll(TRUE);Readyの中に入れてみた
    uglistTmFilter_.Ready(NGNUM);
    ugdrTeam_.ReadyFilter();

    ResetBtn();

    for (s32 i = 0; i < pmgEO_->mgCoU_.mdMap_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoU_.mdMap_.GetLineDt(i, mdmap_id_m_maptip) == pmgEO_->stSeq_.GetMidMapTip())
      {
        stTSL_.id_map_ = pmgEO_->mgCoU_.mdMap_.GetLineDt(i, mdmap_pk);
        break;
      }
    }

    //敵番号
    stTSL_.enemyno_ = pmgEO_->mgCoU_.mdMap_.GetPKDt(stTSL_.id_map_, mdmap_enemyNo);
    enmdm_Maptip enemyTeam = mdm_maptip_id_m_teamA;
    switch (stTSL_.enemyno_)
    {
    case 0: enemyTeam = mdm_maptip_id_m_teamA; break;
    case 1: enemyTeam = mdm_maptip_id_m_teamB; break;
    case 2:enemyTeam = mdm_maptip_id_m_teamC; break;
    }

	// マップデータから敵情報をとるので相手監督IDはNGNUM
	stTSL_.id_kantoku_en_ = NGNUM;

    //敵ＭＩＤ
    stTSL_.mid_team_en_ = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), enemyTeam);
    //敵読み込み
    LoadTeamEnemy(stTSL_.mid_team_en_);

    // 使うものを取得しておく
    pUGSerifu_ = pScene_->lyTuScBar_->GetSerifuWindow();
    pUGFinger_ = pScene_->lyTuScBar_->GetFinger();
    pUGFinger2_ = pScene_->lyTuScBar_->GetFinger2();
    pUGFinger3_ = pScene_->lyTuScBar_->GetFinger3();
    pSerifuVec_ = pScene_->lyTuScBar_->GetSerifuVec();

    //// チュートリアルなのでヘッダーやフッダーはロックをかける
    //pScene_->pmgUI_->SetModal(TRUE);


  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::DoDown()
  {

  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::DoResetBtn()
  {
    SetFoot_BtnHd();

    //BOOL homebtn_f = ((pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei) != 0)
    //  && (pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_bgm) != 0)
    //  && pmgEO_->mgStSn_.EtcSS.clear_f_);

    ugbtn_ok_->SetFont("しあいかいし");

    ugbtn_mgk_->Refresh();
    ugbtn_mgk_->SetDraw(FALSE);
    ugbtn_ok_->SetDraw(FALSE);
    ugbtn_back_->SetDraw(FALSE);
    ugbtn_next_->SetDraw(FALSE);
    ugbtn_mid_->SetDraw(FALSE);
    uglistTmFilter_.SetDraw(FALSE);
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case db::pg_TutoStamina:


      SetMsg(tuTxtNo_++);
      pUGSerifu_->ChangeFrame(enSerifuFrame_Normal);
      pUGSerifu_->SetMsgChangeMode(enMsgChangeMode_Touch);
      pUGSerifu_->SetDraw(TRUE);
      pUGSerifu_->SetPos(0, 30);

      // スタミナの指さし
      pUGFinger_->GetField()->z_SetPos(-165, 43);
      pUGFinger_->hand_->g_SetMirror(TRUE);
      pUGFinger_->SetRevUD(FALSE);
      pUGFinger_->SetDraw(TRUE);
      break;

    case db::pg_TutoStart:
      SetMsg(tuTxtNo_++);
      pUGSerifu_->ChangeFrame(enSerifuFrame_Normal);
      pUGSerifu_->SetMsgChangeMode(enMsgChangeMode_Touch);
      pUGSerifu_->SetDraw(TRUE);
      pUGFinger_->SetDraw(FALSE);
      break;

    case db::pg_TmSelect:
      ResetBtn();
      ugbtn_ok_->SetDraw(TRUE);
      pUGSerifu_->SetDraw(FALSE);

      // スタミナの指さし
      pUGFinger_->GetField()->z_SetPos(92, 50);
      pUGFinger_->hand_->g_SetMirror(FALSE);
      pUGFinger_->SetRevUD(TRUE);
      pUGFinger_->SetDraw(TRUE);
      pUGFinger_->SetMotionLookPoint(92, 50);

      // ヘッダーやフッダーのロックをはずす
      pScene_->pmgUI_->SetModal(FALSE);
	  pScene_->lyTuScBar_->ugheadder_.ugbtn_prof_.GetField()->b_SetBtn(FALSE);
	  pScene_->lyTuScBar_->ugheadder_.ugbtn_credit_.GetField()->b_SetBtn(FALSE);
	  pScene_->lyTuScBar_->ugheadder_.ugbtn_ggz_.GetField()->b_SetBtn(FALSE);
      break;

    case db::pg_GotoVS:
      pUGSerifu_->SetDraw(FALSE);
      pUGFinger_->ClearMotion();
      pUGFinger_->SetDraw(FALSE);
      break;
    }
  }
  
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  //接続完了
  void TLyTuScTSL::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_play: Connect_Play(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        ChangePage(pg_TmSelect);
      }
    }
    else
    {
      //通信エラー
      ChangePage(pg_TmSelect);
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::DoUpdate(BOOL draw_f)
  {

    if ((draw_f == FALSE) && pScene_->lyTuScBar_->IsBarMove()) return;

    switch (stLy_.pageNo_)
    {
    case db::pg_GotoVS:
      ugdrTeam_.Update(draw_f);
      ugencard_.Update(draw_f);
      break;
    default:
      ugdrTeam_.Update(draw_f);
      uglistTmFilter_.Update(draw_f);
      ugencard_.Update(draw_f);
	  ugtitle_me_.Update(draw_f);
	  ugtitle_en_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case db::pg_TutoStamina: MvPage_TutoStamina(); break;
      case db::pg_TutoStart: MvPage_TutoStart(); break;

      case db::pg_TmSelect:MvPage_TSL(); break;
      case db::pg_GotoVS:MvPage_GotoVS(); break;
      }
    }

#ifdef __DEBUG_POSITION_CHECK__
    switch (stLy_.pageNo_)
    {
    case db::pg_TutoStamina:
    case db::pg_TutoStart:
      break;
    default:
      //debug();
      break;
    }
#endif

    // メッセージスキップ処理
    if (field_->sc_IsCamEnter())
    {
      if (pUGSerifu_->IsReadLineEnd())
      {
        pUGSerifu_->NextSerifu();
      }
      else
      {
        pUGSerifu_->SkipSerifu();
      }
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::LoadLookPosTeam()
  {
    //同じＩＤの時は読み込みいらない
    s32 mid_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(ugdrTeam_.GetDataNo(), mdteam_id_m_team);

    if (stTSL_.mid_team_ == mid_team) return;
    //if (stSelect_.teamid_ == testdt_[ugdrTeam_.GetDataNo()]) return;

	stTSL_.id_kantoku_my_ = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
    stTSL_.mid_team_ = mid_team;
    stTSL_.id_team_ = pmgEO_->mgCoU_.mdTeam_.GetLineDt(ugdrTeam_.GetDataNo(), mdteam_pk);
    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
    for (s32 c = 0; c < DBMEMBER_ALL; ++c)
    {
      s32 charid = pmgEO_->mgCoM_.GetCharID(stTSL_.mid_team_, c);
      pmgEO_->mgGrp_.pmgChGrp_[SIDE0][c]->SetCharDt(charid, stTSL_.mid_team_, gb_f);
      pmgEO_->mgGrp_.pmgChGrp_[SIDE0][c]->SetGrTile(SIDE0, c);
    }
    pmgEO_->mgGrp_.MakeIconTexture_Chest(SIDE0, FALSE, FALSE);
    pmgEO_->mgGrp_.MakeIconTexture_Body(SIDE0, 0, 0, FALSE, FALSE);
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  //敵チームカード＆情報
  void TLyTuScTSL::LoadTeamEnemy(s32 mid_team)
  {
    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
    for (s32 c = 0; c < DBMEMBER_ALL; ++c)
    {
      s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);
      pmgEO_->mgGrp_.pmgChGrp_[SIDE1][c]->SetCharDt(charid, mid_team, gb_f);
      pmgEO_->mgGrp_.pmgChGrp_[SIDE1][c]->SetGrTile(SIDE1, c);
    }
    pmgEO_->mgGrp_.MakeIconTexture_Chest(SIDE1, FALSE, FALSE);
    pmgEO_->mgGrp_.MakeIconTexture_Body(SIDE1, 0, 0, TRUE, FALSE);

    ugencard_.SetTeamDt3(NGNUM, NGNUM, mid_team, NGNUM, TRUE);
    ugencard_.SetTipsDraw_f(TRUE);


    ////専用BGMがある時はそちらに差し替え
    //s32 mid_haikei = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_id_m_haikei);
    //s32 mid_bgm = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_id_m_bgm);
    //if (mid_haikei != 0) pmgEO_->stShiai_.GetMidCourt() = mid_haikei;
    //if (mid_bgm != 0) pmgEO_->mgStSn_.EtcSS.mid_bgm_ = mid_bgm;
  }


  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::SetMsg(const s32 msgNo)
  {
    SetMsg(TUTO_MSG_TSL[msgNo][0], TUTO_MSG_TSL[msgNo][1], TUTO_MSG_TSL[msgNo][2]);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::SetMsg(const char* text0, const char* text1, const char* text2)
  {
    std::string strNone = "";

    pSerifuVec_->clear();
    pSerifuVec_->push_back(text0);
    if (text1 && strNone.compare(text1) != 0)
    {
      pSerifuVec_->push_back(text1);
    }
    if (text2 && strNone.compare(text2) != 0)
    {
      pSerifuVec_->push_back(text2);
    }
    pUGSerifu_->SetSerifu(*pSerifuVec_);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::MvPage_TutoStamina()
  {
    // チュートリアルなのでヘッダーやフッダーはロックをかける
    if (pScene_->pmgUI_->IsModal() == FALSE) pScene_->pmgUI_->SetModal(TRUE);

    if (pUGSerifu_->GetActRes() == enUGSerifuAct_WaitEnd)
    {
      if (tuTxtNo_ == ttsltt_STAMINA2)
      {
        // スタミナの説明テキストは
        // 2ページあるのでもう一回
        ChangePage(pg_TutoStamina);
      }
      else
      {
        ChangePage(pg_TutoStart);
      }
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::MvPage_TutoStart()
  {
    if (pUGSerifu_->GetActRes() == enUGSerifuAct_WaitEnd)
    {
      ChangePage(pg_TmSelect);
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::MvPage_TSL()
  {
    if (ugbtn_ok_->IsAct() || ugbtn_mid_->IsAct())//おーけーボタン
    {
      stTSL_.id_team_ = ugdrTeam_.GetIDTeam();
      stTSL_.mid_team_ = ugdrTeam_.GetMIDTeam();

      stTSL_.homecourt_f_ = ugbtn_mid_->IsAct();
      SetEtcSS();

      Post_Play();
    }
    else if (ugdrTeam_.IsActRes(enUGDrTeamAct_Stop))//読み込みが必要なタイミング
    {
      //現在のチームのＩＤ，MID
      stTSL_.id_team_ = ugdrTeam_.GetIDTeam();
      stTSL_.mid_team_ = ugdrTeam_.GetMIDTeam();
      //キャプテンとメンバーの画像をロード
      //LoadLookPosTeam();
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScTSL::MvPage_GotoVS()
  {

  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  //POST：開始
  void TLyTuScTSL::Post_Play()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Play();
    }
    else
    {
      StartConnect(cn_play);
      pmgEO_->mgPo_.poScenario_.Ready_play(stTSL_.id_team_);
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  //接続：敵選択完了
  void TLyTuScTSL::Connect_Play()
  {
    pScene_->lyTuScBar_->MoveLayer(tscl_VSFilter);
    SetHudEnd();
    ChangePage(pg_GotoVS);

#ifdef __K_DEBUG_PAD_MODE__
    kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_SubScenario);
#endif
  }

  //両者情報セット
  void TLyTuScTSL::SetEtcSS()
  {
    s64 id_map = 0;
    for (s32 i = 0; i < pmgEO_->mgCoU_.mdMap_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoU_.mdMap_.GetLineDt(i, mdmap_id_m_maptip) == pmgEO_->stSeq_.GetMidMapTip())
      {
        id_map = pmgEO_->mgCoU_.mdMap_.GetLineDt(i, mdmap_pk);
        break;
      }
    }

    //敵番号
    s32 enemyno = pmgEO_->mgCoU_.mdMap_.GetPKDt(id_map, mdmap_enemyNo);
    enmdm_Maptip enemyTeam = mdm_maptip_id_m_teamA;
    enmdm_Maptip dropPtn = mdm_maptip_id_m_drop_A;
    enmdm_Maptip caper = mdm_maptip_catchA;
    enmdm_Maptip ncaper = mdm_maptip_niceA;
    switch (enemyno)
    {
    case 0:
      enemyTeam = mdm_maptip_id_m_teamA;
      dropPtn = mdm_maptip_id_m_drop_A;
      caper = mdm_maptip_catchA;
      ncaper = mdm_maptip_niceA;
      break;
    case 1:
      enemyTeam = mdm_maptip_id_m_teamB;
      dropPtn = mdm_maptip_id_m_drop_B;
      caper = mdm_maptip_catchB;
      ncaper = mdm_maptip_niceB;
      break;
    case 2:
      enemyTeam = mdm_maptip_id_m_teamC;
      dropPtn = mdm_maptip_id_m_drop_C;
      caper = mdm_maptip_catchC;
      ncaper = mdm_maptip_niceC;
      break;
    }

    //敵ＭＩＤ
    //s32 mid_team_en = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), enemyTeam);
    //LoadTeamEnemy(mid_team_en);//敵読み込み

    //コート//BGM
    s32 mid_court = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), mdm_maptip_id_m_haikei);
    s32 mid_bgm = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), mdm_maptip_id_m_bgm);
    //敵専用BGMがある時はそちらに差し替え
    if (stTSL_.homecourt_f_)//ホーム
    {
      mid_court = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei);
      mid_bgm = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_bgm);
    }
    else//敵専用
    {
      s32 mid_court_en = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(stTSL_.mid_team_en_, mdm_team_id_m_haikei);
      s32 mid_bgm_en = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(stTSL_.mid_team_en_, mdm_team_id_m_bgm);
      if (mid_court_en != 0) mid_court = mid_court_en;
      if (mid_bgm_en != 0) mid_bgm = mid_bgm_en;
    }
    pmgEO_->stResult_.Init();
    pmgEO_->stDrop_.Init();
    pmgEO_->stShiai_.SetShiai(
	  stTSL_.id_kantoku_my_,
	  stTSL_.id_kantoku_en_,
      stTSL_.id_team_,
      stTSL_.mid_team_,
      stTSL_.mid_team_en_,
	  FALSE,	// 1Pはプレイヤー
	  TRUE,		// 2PはCOM
      mid_court,
      mid_bgm,
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	  nsf_jgl_ready,
	  nsf_jgl_win,
	  nsf_jgl_lose,
#endif
      100,
      pmgEO_->mgCoU_.GetCtrl(),
      pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), mdm_maptip_shiaiSec),
      pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), caper),
      pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), ncaper),
      FALSE);//必ず通常試合


    //ドロップアイテム
    s64 mid_drop = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), dropPtn);
    s32 itemType = 0;
    s32 itemID = 0;
    s32 itemNum = 0;
    s32 dropNo = pmgEO_->mgCoU_.mdMap_.GetPKDt(id_map, mdmap_dropNo);

    if (dropNo == 3)
    {
      enmdm_Maptip droptype = mdm_maptip_itemType_sdA;
      enmdm_Maptip dropid = mdm_maptip_itemID_sdA;
      enmdm_Maptip dropnum = mdm_maptip_itemNum_sdA;
      switch (enemyno)
      {
      case 0:
        droptype = mdm_maptip_itemType_sdA;
        dropid = mdm_maptip_itemID_sdA;
        dropnum = mdm_maptip_itemNum_sdA;
        break;
      case 1:
        droptype = mdm_maptip_itemType_sdB;
        dropid = mdm_maptip_itemID_sdB;
        dropnum = mdm_maptip_itemNum_sdB;
        break;
      case 2:
        droptype = mdm_maptip_itemType_sdC;
        dropid = mdm_maptip_itemID_sdC;
        dropnum = mdm_maptip_itemNum_sdC;
        break;
      }
      itemType = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), droptype);
      itemID = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), dropid);
      itemNum = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), dropnum);
    }
    else
    {
      enmdm_Drop droptype = mdm_drop_itemType_0;
      enmdm_Drop dropid = mdm_drop_itemID_0;
      enmdm_Drop dropnum = mdm_drop_itemNum_0;
      switch (dropNo)
      {
      case 0:
        droptype = mdm_drop_itemType_0;
        dropid = mdm_drop_itemID_0;
        dropnum = mdm_drop_itemNum_0;
        break;
      case 1:
        droptype = mdm_drop_itemType_1;
        dropid = mdm_drop_itemID_1;
        dropnum = mdm_drop_itemNum_1;
        break;
      case 2:
        droptype = mdm_drop_itemType_2;
        dropid = mdm_drop_itemID_2;
        dropnum = mdm_drop_itemNum_2;
        break;
      }
      itemType = pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, droptype);
      itemID = pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, dropid);
      itemNum = pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, dropnum);
    }

    pmgEO_->stDrop_.SetShiai(dropNo, itemType, itemID, itemNum, NGNUM, NGNUM,
      pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, mdm_drop_itemType_Cmp),
      pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, mdm_drop_itemID_Cmp),
      pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, mdm_drop_itemNum_Cmp));

  }
}
