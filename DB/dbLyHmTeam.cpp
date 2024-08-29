#include "dbLyHmTeam.h"
#include "dbSnHome.h"

#ifdef __K_DEBUG_TACHIE__
#include "../debug/DebugSensyuManager.h"
#endif

namespace db
{

  enum enPage
  {
    pg_Tuto,
    pg_Menu,
    pg_InMenu,
    pg_Idle,
  };

  const s32 OUTW = 6;

  const f32 ex = 20;
  const f32 ey = 36;
  static f32 initTbl[DBMEMBER_ALL][2] =
  {
    { 92, 150 },
    { 124, 170 },
    { 128, 152 },
    { 146, 160 },
    { 172, 172 },
    { 168, 152 },
    { 188, 160 },
  };
  static f32 endTbl[DBMEMBER_ALL][2] =
  {
    { 115 + ex, 169 + ey },
    { 107 + ex - 8, 123 + ey },
    { 220 + ex, 123 + ey },
    { 273 + ex, 123 + ey },
    { 147 + ex + 8, 123 + ey },
    { 309 + ex, 168 + ey },
    { 354 + ex, 168 + ey },
  };
  static f32 frTbl[DBMEMBER_ALL] =
  {
    40.0f,
    24.0f,
    48.0f,
    48.0f,
    24.0f,
    60.0f,
    60.0f,
  };
  static BOOL mirrTbl[DBMEMBER_ALL] =
  {
    FALSE,
    FALSE,
    FALSE,
    TRUE,
    TRUE,
    FALSE,
    TRUE,
  };
  static enChSensyuMotionType nextStTbl[DBMEMBER_ALL] =
  {
    bmt_RabbitJump,
    //bmt_St,
    bmt_Kamae,
    bmt_Cassette,
    bmt_Hdphone,
    bmt_Kamae,
    bmt_Pause,
    bmt_Kamae,
  };

  //コンストラクタ
  TLyHmTeam::TLyHmTeam(TSnHome* pScene) :TLyHmGen(pScene)
    , ugkantoku_(this)
    , ugcap_(this)
    , ugcaball_(this)
    , ugbln_bgm_(this)
    , ugbln_select_(this)
    , ugbln_haikei_(this)
    , ugmsg_(this)
    , teamLv_(0)
    , ballMan1_(NULL)
    , ballMan2_(NULL)
    , tapeMan_(NULL)
    , ugname_(this)
  {
  }

  TLyHmTeam::~TLyHmTeam()
  {
    ClearRecords();
  }
  void TLyHmTeam::ClearRecords()
  {
    for (s32 i = 0; i < ugmem_.size(); ++i) Freemem(ugmem_[i]);
  }
  void TLyHmTeam::DoLoad()
  {
    banner_ = MakeObj();
    banner_->o_SetObjGrp(hmgrp_->team_banner_);
    banner_->z_SetPos_Full(81, 97);
    banner_->o_SetParent(field_);

    bcolors_ = MakeObj();
    bcolors_->o_SetObjGrPart(hmgrp_->team_bcolors_);
    bcolors_->g_SetPartNo(teamRare_);//レア度
    bcolors_->z_SetPos(5, 5);
    bcolors_->o_SetParent(banner_);


    //チーム名
    ugname_.MakeGroup(field_);
    ugname_.SetPos(18, 1);
    ugname_.SetParent(bcolors_);
    //ugname_.SetNameTeam(teamDef);
    //ugname_.SetNameTeam_Cnt(teamDef);

    //チームのID
    uiteammid_ = MakeObj();
    uiteammid_->o_SetParent(bcolors_);
    uiteammid_->z_SetPos(5, 5);

    // チームカード
    cardside_ = MakeObj();
    cardside_->o_SetObjGrp(hmgrp_->team_cardside_);
    cardside_->z_SetPos_Full(90, 120);
    cardside_->o_SetParent(field_);

    cards_ = MakeObj();
    cards_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_rarity));
    cards_->z_SetPos(0, 7);
    cards_->o_SetParent(cardside_);


    boombox_ = MakeObj();
    boombox_->o_SetObjGrPart(hmgrp_->team_boombox_);
    boombox_->o_SetParent(field_);
    boombox_->z_SetPos_Full(246, 113);
    boombox_->a_AddAnivec(0, 7, 8, at_Loop);
    boombox_->a_SetAniNo(0, TRUE);

    camera_ = MakeObj();
    camera_->o_SetObjGrp(hmgrp_->team_camera_);
    camera_->z_SetPos_Full(349, 173);
    camera_->o_SetParent(field_);

    films_ = MakeObj();
    films_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_films));
    films_->g_SetPartNo(0);//レア度
    films_->z_SetPos_Full(336, 181);
    films_->o_SetParent(field_);

    for (s32 i = 0; i < DBMEMBER_ALL; ++i)
    {
      TUGChSensyu* newsensyu = new TUGChSensyu(this);
      newsensyu->MakeGroup(field_, i);
      ugmem_.push_back(newsensyu);
    }


    const s32 posx = 20;
    const s32 posy = 36;
    ugmem_[0]->SetPos_Full(115 + posx, 169 + posy);//カードポジから持ち位置、1番(監督後ろ)
    ugmem_[1]->SetPos_Full(107 + posx - 8, 123 + posy);//2番(その場、キャッチ左)
    ugmem_[2]->SetPos_Full(220 + posx, 123 + posy);//3番(カセット)
    ugmem_[3]->SetPos_Full(273 + posx, 123 + posy);//4番(ヘッドホン)
    ugmem_[4]->SetPos_Full(147 + posx + 8, 123 + posy);//5番(その場、キャッチ右)
    ugmem_[5]->SetPos_Full(309 + posx, 168 + posy);//6番(モデル)
    ugmem_[6]->SetPos_Full(354 + posx, 168 + posy);//7番(カメラマン)

    ugmem_[0]->SetMType(bmt_St);
    ugmem_[1]->SetMType(bmt_Kamae);
    ugmem_[2]->SetMType(bmt_Cassette);
    ugmem_[3]->SetMType(bmt_Hdphone);
    ugmem_[3]->SetMirror(TRUE);
    ugmem_[4]->SetMType(bmt_Kamae);
    ugmem_[4]->SetMirror(TRUE);
    ugmem_[5]->SetMType(bmt_Pause);
    ugmem_[6]->SetMType(bmt_Kamae);
    ugmem_[6]->SetMirror(TRUE);


    tapes_ = MakeObj();
    tapes_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_tapes));
    tapes_->g_SetPartNo(0);//レア度
    //ugmem_[2]->SetItem(tapes_);

    ball_ = MakeObj();
    ball_->o_SetObjGrPart(hmgrp_->team_ball_);
    ball_->g_SetDepth(UIDP_00BG);
    //ugcaball_.MakeGroup(ugmem_[1], ugmem_[4], ball_);

    ugbln_select_.MakeGroup_Balloon(field_, "しゅーとそうび");
    ugbln_select_.SetPos_Full(110 + 32, 58 + 16);

    ugbln_bgm_.MakeGroup_Balloon(field_, "ＢＧＭ");
    ugbln_bgm_.SetPos_Full(232 + 32, 58 + 16);

    ugbln_haikei_.MakeGroup_Balloon(field_, "はいけい");
    ugbln_haikei_.SetPos_Full(320 + 32, 123 + 14);


#ifdef __K_DEBUG_TACHIE__
    kdebug::DebugSensyuManager::Create(this, field_);
#endif
  }

  void TLyHmTeam::DoReady()
  {
    SetFoot_Tab();
    s64 id_team = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_team);

    //まだどのチームも使ってないとき
    if (id_team == 0) id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(0, mdteam_pk);
#ifdef __K_DEBUG_USER_DATA__
    teamMID_ = kdebug::DebugUDManager::GetInstance()->GetLastTeamID();
#else
    teamMID_ = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_m_team);
#endif
    teamLv_ = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
    teamRare_ = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(teamMID_, mdm_team_rarity);
    bcolors_->g_SetPartNo(teamRare_);//レア度
    cards_->g_SetPartNo(teamRare_);//レア度

    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(teamMID_, mdm_team_gb_f);
    // 最後に選んだチームを取得してホームのチームにセット
    // s64 lastTeamNo = teamMID_;// pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_team);
    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
    {
      s64 charID = pmgEO_->mgCoM_.GetCharID(teamMID_, i2);
      pmgEO_->mgGrp_.pmgcg_sensyu_[i2]->SetCharDt(charID, teamMID_, gb_f);
      pmgEO_->mgGrp_.MakeSensyuTexture(i2);
    }

    //teamLv_ = 2;
    ugname_.SetNameTeam_Get(teamMID_, teamLv_);
    uiteammid_->f_MakeFont_Mini(lib_str::IntToStr(teamMID_).c_str(), FC2_mini);
    ugname_.SetIcon(FALSE);

    // ReleaseGroupの代わり
    if (ballMan1_ && ballMan1_->IsItem())
    {
      ballMan1_->ReleaseItem();
    }
    if (ballMan2_ && ballMan2_->IsItem())
    {
      ballMan2_->ReleaseItem();
    }
    ballMan1_ = NULL;
    ballMan2_ = NULL;

    if (tapeMan_ && tapeMan_->IsItem())
    {
      tapeMan_->ReleaseItem();
    }
    tapeMan_ = NULL;

    // めんどくさいのでランダム配置用テーブルを用意
    std::vector<s32> rndTbl;
    for (s32 i = 0; i < DBMEMBER_ALL; i++) rndTbl.push_back(i);

    //= { 0, 1, 2, 3, 4, 5, 6 };
    for (s32 i = 0; i < DBMEMBER_ALL; i++)
    {
      ugmem_[i]->Init();
      ugmem_[i]->SetMType(bmt_St);
      if (i == 4 || i == 5 || i == 6)
      {
        ugmem_[i]->SetMirror(TRUE);
      }
      else
      {
        ugmem_[i]->SetMirror(FALSE);
      }

      // どこに配置するかのランダム選定

      s32 s = rndTbl.size();
      s32 r = mid::midGetRandMn(s);
      s32 n = rndTbl[r];
      ugmem_[i]->SetRun((s32)initTbl[i][0], (s32)initTbl[i][1], (s32)endTbl[n][0], (s32)endTbl[n][1], frTbl[n]);
      if (n == 1)
      {
        // キャッチボールするヤツ左 = 0
        ugmem_[i]->SetNextSt(nextStTbl[n], mirrTbl[n], 0);
      }
      else if (n == 4)
      {
        // キャッチボールするヤツ右 = 1
        ugmem_[i]->SetNextSt(nextStTbl[n], mirrTbl[n], 1);
      }
      else
      {
        ugmem_[i]->SetNextSt(nextStTbl[n], mirrTbl[n]);
      }

      std::vector<s32>::iterator itr = rndTbl.begin();
      itr += r;
      rndTbl.erase(itr);
      switch (n)
      {
      case 1:  ballMan1_ = ugmem_[i];  break;
      case 4:  ballMan2_ = ugmem_[i];  break;
      case 2:  tapeMan_ = ugmem_[i];	break;
      }
    }
    //ugcaball_.ReleaseGroup();	  // リリース処理
    ugcaball_.MakeGroup(ballMan1_, ballMan2_, ball_);
    tapeMan_->SetItem(tapes_);
    tapes_->g_SetDraw(FALSE);


    // ホームＢＧＭに合わせたアイコンセット
#ifdef __K_DEBUG_PAD_MODE__
    s32 homeBGM = kdebug::DebugUDManager::GetInstance()->GetHomeBGM();
#else
    s32 homeBGM = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_bgm);
#endif
    s32 bTileNo = pmgEO_->mgDt_.dtBGM_.GetDtPK(homeBGM, enDtBGM_titleid);		// ＢＧＭＩＤからタイトルＩＤを調べる	
    s32 bIconNo = pmgEO_->mgDt_.dtTitle_.GetDtPK(bTileNo, enDtTitle_iconno);	// タイトルＩＤからアイコンＩＤを調べる
    tapes_->g_SetPartNo(bIconNo);//レア度


    // ホームコートに合わせたアイコンセット
#ifdef __K_DEBUG_PAD_MODE__
    s32 homeHaikei = kdebug::DebugUDManager::GetInstance()->GetHomeHaikei();
#else
    s32 homeHaikei = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei);
#endif
    s32 hTileNo = pmgEO_->mgDt_.dtCourt_.GetDtPK(homeHaikei, enDtCourt_titleid);	// 背景ＩＤからタイトルＩＤを調べる	
    s32 hIconNo = pmgEO_->mgDt_.dtTitle_.GetDtPK(hTileNo, enDtTitle_iconno);		// タイトルＩＤからアイコンＩＤを調べる
    films_->g_SetPartNo(hIconNo);//レア度
  }


  void TLyHmTeam::Idle()
  {
    ChangePage(pg_Idle);
  }
  void TLyHmTeam::MvPage_Menu()
  {
    if (ugbln_select_.IsAct())
    {
      pScene_->lyHmTmSelect_->Ready();
      ChangePage(pg_InMenu);
    }
    else if (ugbln_haikei_.IsAct())
    {
      pScene_->lyHmTmHaikei_->Ready();
      ChangePage(pg_InMenu);
    }
    else if (ugbln_bgm_.IsAct())
    {
      pScene_->lyHmTmBGM_->Ready();
      ChangePage(pg_InMenu);
    }
  }

  void TLyHmTeam::MvPage_Tuto()
  {
    BOOL isEnd = FALSE;

#ifdef __K_DEBUG_PAD_MODE__
    if (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubScenario) ||	// シナリオマップのサブチュートリアルが終わってなければとりあえず入れる
      kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubTeam))		// チームのサブチュートリアルが終わってたらもちろん入れる
    {
      isEnd = TRUE;
    }
#else
    if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubScenario) == FALSE ||
      pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubTeam) == TRUE)
    {
      isEnd = TRUE;
    }
#endif

    // チュートリアルフラグがたってるかどうか
    if (!pScene_->lySTuTeam_ || pScene_->lySTuTeam_->IsTutoEnd())
    {
      isEnd = TRUE;
    }

    if (isEnd)
    {
      ChangePage(pg_Menu);
    }
  }

  void TLyHmTeam::MvPage_InMenu()
  {
    for (s32 i = 0; i < DBMEMBER_ALL; i++)
    {
      ugmem_[i]->Init();
    }
  }

  void TLyHmTeam::MvPage_Idle()
  {

  }

  ////lp_Wait,
  //void TLyHmTeam::DoUpdate0(BOOL draw_f)
  //{
  //  if (pScene_->lyHmBar_->IsUnderDraw(2) && draw_f)
  //  {
  //    field_->Update(draw_f);
  //    banner_->Update(draw_f);
  //    bcolors_->Update(draw_f);
  //    boombox_->Update(draw_f);
  //    camera_->Update(draw_f);
  //    films_->Update(draw_f);
  //    cardside_->Update(draw_f);
  //    tapes_->Update(draw_f);
  //  }
  //}

  bool isPushBtn = false;
  s64 offsetNo = 0;
  void TLyHmTeam::DoUpdate(BOOL draw_f)
  {
#ifdef __K_DEBUG_HOME_TEAM__
    DxLib::XINPUT_STATE xInput;
    if (DxLib::GetJoypadXInputState(DX_INPUT_PAD1, &xInput) == -1) return;


    //===================================================================
    // ホーム画面チーム切替デバッグ
    //---------------------------------
    // チームID切替
    if (xInput.Buttons[XINPUT_BUTTON_Y] == 1)
    {
      if (!isPushBtn)
      {
        teamMID_++;
        offsetNo = 0;
        for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
        {
          s64 charID = pmgEO_->mgCoM_.GetCharID(teamMID_, i2);
          pmgEO_->mgGrp_.pmgcg_sensyu_[i2]->SetCharDt(charID, teamMID_);
          pmgEO_->mgGrp_.MakeSensyuTexture(i2);
        }
        ugname_.SetNameTeam_Get(teamMID_, teamLv_);
        uiteammid_->f_MakeFont_Mini(lib_str::IntToStr(teamMID_).c_str(), FC2_mini);
        ugname_.SetIcon(FALSE);
        isPushBtn = true;
      }
    }
    else if (xInput.Buttons[XINPUT_BUTTON_X] == 1)
    {
      if (!isPushBtn)
      {
        teamMID_--;
        if (teamMID_ <= 0){ teamMID_ = 0; }
        offsetNo = 0;
        for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
        {
          s64 charID = pmgEO_->mgCoM_.GetCharID(teamMID_, i2);
          pmgEO_->mgGrp_.pmgcg_sensyu_[i2]->SetCharDt(charID, teamMID_);
          pmgEO_->mgGrp_.MakeSensyuTexture(i2);
        }
        ugname_.SetNameTeam_Get(teamMID_, teamLv_);
        uiteammid_->f_MakeFont_Mini(lib_str::IntToStr(teamMID_).c_str(), FC2_mini);
        ugname_.SetIcon(FALSE);
        isPushBtn = true;
      }
    }
    else if (xInput.Buttons[XINPUT_BUTTON_START] == 1)
    {
      if (!isPushBtn)
      {
        teamMID_ += 10;
        offsetNo = 0;
        for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
        {
          s64 charID = pmgEO_->mgCoM_.GetCharID(teamMID_, i2);
          pmgEO_->mgGrp_.pmgcg_sensyu_[i2]->SetCharDt(charID, teamMID_);
          pmgEO_->mgGrp_.MakeSensyuTexture(i2);
        }
        ugname_.SetNameTeam_Get(teamMID_, teamLv_);
        uiteammid_->f_MakeFont_Mini(lib_str::IntToStr(teamMID_).c_str(), FC2_mini);
        ugname_.SetIcon(FALSE);
        isPushBtn = true;
      }
    }
    else if (xInput.Buttons[XINPUT_BUTTON_BACK] == 1)
    {
      if (!isPushBtn)
      {
        teamMID_ -= 10;
        offsetNo = 0;
        if (teamMID_ <= 0){ teamMID_ = 0; }
        for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
        {
          s64 charID = pmgEO_->mgCoM_.GetCharID(teamMID_, i2);
          pmgEO_->mgGrp_.pmgcg_sensyu_[i2]->SetCharDt(charID, teamMID_);
          pmgEO_->mgGrp_.MakeSensyuTexture(i2);
        }
        ugname_.SetNameTeam_Get(teamMID_, teamLv_);
        uiteammid_->f_MakeFont_Mini(lib_str::IntToStr(teamMID_).c_str(), FC2_mini);
        ugname_.SetIcon(FALSE);
        isPushBtn = true;
      }
    }
    else
    {
      isPushBtn = false;
    }
#endif

    switch (stLy_.pageNo_)
    {
    case pg_InMenu:
    case pg_Idle:
      //banner_->Update(draw_f);
      //bcolors_->Update(draw_f);
      //boombox_->Update(draw_f);
      //camera_->Update(draw_f);
      //films_->Update(draw_f);
      //cardside_->Update(draw_f);
      //tapes_->Update(draw_f);
      break;
    default:
      banner_->Update(draw_f);
      bcolors_->Update(draw_f);
      ugname_.Update(draw_f);
      uiteammid_->Update(draw_f);
      boombox_->Update(draw_f);
      camera_->Update(draw_f);
      films_->Update(draw_f);
      cardside_->Update(draw_f);
      cards_->Update(draw_f);
      if (!tapeMan_->isRunning_)
      {
        tapes_->g_SetDraw(TRUE);
        tapes_->Update(draw_f);
      }
      ugbln_select_.Update(draw_f);
      ugbln_bgm_.Update(draw_f);
      ugbln_haikei_.Update(draw_f);
#ifdef __K_DEBUG_TACHIE__
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
      {

        if (ballMan1_ == ugmem_[i] || ballMan2_ == ugmem_[i]) continue;
        ugmem_[i]->Update(false);
      }
      ugcaball_.Update(false);
      kdebug::DebugSensyuManager::GetInstance()->Update(draw_f);
#else
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
      {

        if (ballMan1_ == ugmem_[i] || ballMan2_ == ugmem_[i]) continue;
        ugmem_[i]->Update(draw_f);
      }
      ugcaball_.Update(draw_f);
#endif	// K_DEBUG_TACHIE__
      break;
    }

    if (draw_f == FALSE)
    {
      if (pScene_->lyHmBar_->IsBarAction()) return;
      switch (stLy_.pageNo_)
      {
      case pg_Tuto:MvPage_Tuto(); break;
      case pg_Menu:MvPage_Menu(); break;
      case pg_InMenu:MvPage_InMenu(); break;
      case pg_Idle:MvPage_Idle(); break;
      }
    }
  }
}
