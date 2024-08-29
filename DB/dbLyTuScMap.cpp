#include "dbLyTuScMap.h"
#include "dbSnScenario.h"
#include "dbUGSerifu.h"
#include "dbUGFinger.h"

const s32 TESTPANELNUM = 4;

namespace db
{
  const s32 SELECTPOSX = 80;
  const s32 SELECTPOSY = -16;
  const s32 BASEX = -160;
  const s32 BASEY = -80;

  const s32 PLBASEX = 30;
  const s32 PLBASEY = -4;
  const s32 PANELDISTX = 104;
  const s32 PANELDISTY = 72;
  const s32 PANELDISTX_Hf = PANELDISTX / 2;
  const s32 PANELDISTY_Hf = PANELDISTY / 2;

  const s32 PLX = -24;
  const s32 CAPX = -48;
  const s32 PLY = 0;
  const s32 CAPY = 0;
  const s32 RTREVY = -3;
  const s32 EATTIME = 90;
  const s32 MVSPD = 12;
  const s32 MVSPD2 = 24;

  const u32 STARTPOS = 0x1;
  const u32 RETROTUTO = 0x2;
  const u32 SMARTTUTO = 0x4;

  enum enDepth
  {
    dpPanel,
    dpBosspanel,
    dpBridge,
    dpRoad,
    dpEnemy,
    dpPlayer,
    dpOKBtn,
  };

  enum enPage
  {
    pg_Text1,			// チュートリアル：テキスト表示1
    pg_TutoStage,		// チュートリアル：チュートリアルステージ表示
    pg_TutoCamMove,		// カメラ移動待ち
    pg_TutoSelect,	// チュートリアル：パネル選択

    pg_Visit,
    pg_SelectPanel,
    pg_Enemy,
  };


  //-------------------------------------------------
  // チュートリアルテキスト
  enum enTutoMapTxtType
  {
    tmtt_INIT = 0,
    //ttt_TUTO_STAGE,
    tmtt_MAX = 3,
  };
  const s32 MSG_LINE_MAX = 3;
  const char* TUTO_MSG_MAP[tmtt_MAX][MSG_LINE_MAX] =
  {
    {
      "掛「シナリオは＿とうじょうする",
      "チームを＿たおしていくモードです",
    },
    {
      "掛「これらのマップで＿そうさの",
      "さいかくにんが＿できます",
    },
    {
      "掛「さいしょは＿こちらのマップから",
      "はじめます＿タッチしてみましょう",
    },
  };

  const s32 MAP_TUTO_RETRO = 1;
  const s32 MAP_TUTO_SUMAHO = 2;
  const s32 MAP_001 = 3;

  //-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
  TLyTuScMap::TLyTuScMap(TSnScenario* pScene) :TLyScGen(pScene)
    , ugkantoku_(this)
    , ugcap_(this)
    , selectMapNo_(-1)
    , tuTxtNo_(0)
    , pPanel_(NULL)
  {
    stMap_.Init();
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  TLyTuScMap::~TLyTuScMap()
  {
    ClearPanelVec();

    //ボス画像クリア
    //    pmgEO_->mgGrp_.ClearBossGrp();
    pmgEO_->mgGrp_.ClearBossSt();
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::DoLoad()
  {
    //field_->sc_CamSwipe(TRUE);//スワイプオン

    //スワイプ用オブジェ
    map_ = MakeObj();
    map_->o_SetParent(field_);
    map_->sc_CamSwipe(TRUE);//スワイプオン
    //カメラ位置用オブジェ
    camera_ = MakeObj();
    ugkantoku_.MakeGroup(map_, FALSE);
    ugkantoku_.SetPos(PLBASEX + PLX, PLBASEY);
    ugkantoku_.SetMType(kmt_Ds);

    //ugcap_.SetupMgCharGrp();
    //ugcap_.MakeGroup(map_);
    //ugcap_.SetPos(PLBASEX + PLX - 26, PLBASEY);
    //ugkantoku_.SetCaptain(&ugcap_);
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::DoResetBtn()
  {
    SetFoot_BtnHd();
    //ugbtn_back_->SetFont("もどる");
    //ugbtn_back_->SetDraw(TRUE);
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::DoReady()
  {
    stMap_.nowmapNo_ = 1;//通常マップは１から
    ClearPanelVec();

    //ボス画像作成用ベクタのクリア
    pmgEO_->mgGrp_.ClearBossSt();

    //マップパネル
    BOOL startpos_f = FALSE;
    s32 startpos = 0;	// スタート位置は0のレトロチュートリアルマップ

    // マップに表示していいのはチュートリアル2ステージと通常の1ステージ目のみ
    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Maptip_.GetLineNum(); ++i)
    {
      //マップが違う
      if (pmgEO_->mgCoM_.mdm_Maptip_.GetLineDt(i, mdm_maptip_mapNo) != stMap_.nowmapNo_) continue;

      // 今回用のステージ以外は表示しない
      if (pmgEO_->mgCoM_.mdm_Maptip_.GetLineDt(i, mdm_maptip_pk) != MAP_TUTO_RETRO &&
        pmgEO_->mgCoM_.mdm_Maptip_.GetLineDt(i, mdm_maptip_pk) != MAP_TUTO_SUMAHO &&
        pmgEO_->mgCoM_.mdm_Maptip_.GetLineDt(i, mdm_maptip_pk) != MAP_001)
      {
        continue;
      }

      TUGScPanel* newpanel = new TUGScPanel(this);
      newpanel->MakeGroup(map_, stMap_.nowmapNo_, i, &ugpanelvec_);//行番号で保存
      ugpanelvec_.push_back(newpanel);
    }

    //ホームから来たフラグをたたむ
    BOOL fromhome_f = (pScene_->GetLastScene() == snHome); //pmgEO_->stSeq_.PopFromHome();//.EtcSS.fromhome_f_ = FALSE;

    //現状を反映したマップの開放
    OpenMap(startpos);

    //初期位置
    s32 posx = ugpanelvec_[startpos]->GetMDt(mdm_maptip_posX)* PANELDISTX;
    s32 posy = ugpanelvec_[startpos]->GetMDt(mdm_maptip_posY)* PANELDISTY;
    ugkantoku_.SetPos(posx + PLBASEX + PLX, posy + PLBASEY);
    camera_->z_SetPos(posx, posy);
    map_->sc_CamSetPos(posx, posy);
    //スクロール画面サイズ
    SetScrollSize();

    //押せるものはボタンになる
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      ugpanelvec_[i]->SetPanelButton(TRUE);
    }

    pmgEO_->mgGrp_.MakeBossTexture();//テクスチャ作成

    // 使うものを取得しておく
    pUGSerifu_ = pScene_->lyTuScBar_->GetSerifuWindow();
    pUGFinger_ = pScene_->lyTuScBar_->GetFinger();
    pUGFinger2_ = pScene_->lyTuScBar_->GetFinger2();
    pSerifuVec_ = pScene_->lyTuScBar_->GetSerifuVec();

    SetFoot_BtnHd();
    ResetBtn();

    // チュートリアルなのでヘッダーやフッダーはロックをかける
    pScene_->pmgUI_->SetModal(TRUE);
    //ヘッダがモーダルでもきくのをカット
    SetHeadderModal(FALSE);
    
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::DoDown()
  {
  }
  
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case db::pg_Text1:
      SetMsg(tuTxtNo_++);
      pUGSerifu_->ChangeFrame(enSerifuFrame_Normal);
      pUGSerifu_->SetMsgChangeMode(enMsgChangeMode_Touch);
      pUGSerifu_->SetDraw(TRUE);
      map_->sc_CamSwipe(FALSE);//スワイプオフ
      break;

    case db::pg_TutoStage:
    {
      SetMsg(tuTxtNo_++);
      pUGSerifu_->SetDraw(TRUE);

      TUGScPanel* pTutoRetro = NULL;
      TUGScPanel* pTutoSumaho = NULL;
      for (s32 i = 0; i < ugpanelvec_.size(); ++i)
      {
        s32 pk = ugpanelvec_[i]->stScPanel_.mid_maptip_;
        if (pk == MAP_TUTO_RETRO){ pTutoRetro = ugpanelvec_[i]; }
        else if (pk == MAP_TUTO_SUMAHO){ pTutoSumaho = ugpanelvec_[i]; }
      }
      if (pTutoRetro && pTutoSumaho)
      {
        // スクロール情報はmap_に入っている
        f32 mapX = 0;
        f32 mapY = 0;
        map_->z_GetGlobalPos(&mapX, &mapY);

        // 矢印1の位置
        f32 retroX = 0;
        f32 retroY = 0;
        pTutoRetro->GetField()->z_GetPos(&retroX, &retroY);
        retroX += mapX + 30;	// オフセット
        retroY += mapY - 65;
        pUGFinger_->GetField()->z_SetPos(retroX, retroY);
        pUGFinger_->SetDraw(TRUE);
        pUGFinger_->SetDepth(enUIDepth::UIDP_09MSGBOX);
        pUGFinger_->SetRevUD(TRUE);
        //pUGFinger_->SetMotionLookPoint(retroX, retroY);

        // 矢印2の位置
        f32 sumahoX = 0;
        f32 sumahoY = 0;
        pTutoSumaho->GetField()->z_GetPos(&sumahoX, &sumahoY);
        sumahoX += mapX + 30;
        sumahoY += mapY - 65;
        pUGFinger2_->GetField()->z_SetPos(sumahoX, sumahoY);
        pUGFinger2_->SetDraw(TRUE);
        pUGFinger2_->SetDepth(enUIDepth::UIDP_09MSGBOX);
        pUGFinger2_->SetRevUD(TRUE);
        //pUGFinger2_->SetMotionLookPoint(sumahoX, sumahoY);
      }
    }
    break;

    case db::pg_TutoCamMove:
    {
      //pUGFinger_->ClearMotion();
      pUGFinger_->SetDraw(FALSE);
      //pUGFinger2_->ClearMotion();
      pUGFinger2_->SetDraw(FALSE);

      for (s32 i = 0; i < ugpanelvec_.size(); ++i)
      {
        s32 pk = ugpanelvec_[i]->stScPanel_.mid_maptip_;
        if (pk == MAP_001)
        {
          selectMapNo_ = i;
          pPanel_ = ugpanelvec_[i];
        }
        else
        {
          // 該当のパネル以外は押せなくする
          ugpanelvec_[i]->SetPanelButton(FALSE);
        }
      }
      // カメラ移動処理
      f32 mapX = 0;
      f32 mapY = 0;
      mapX = map_->stSO_.stCamSw_.posX_;
      mapY = map_->stSO_.stCamSw_.posY_;
      f32 panelX = 0;
      f32 panelY = 0;
      if (pPanel_)
      {
        pPanel_->GetField()->z_GetGlobalPos(&panelX, &panelY);
        mapX = (panelX > 0) ? mapX + panelX : mapX - panelX;
        //mapY = (panelY > 0) ? mapY + panelY : mapY - panelY;
        camera_->z_MoveSetspd(mapX, mapY, 4);
      }
    }
    break;

    case db::pg_TutoSelect:
    {
      SetMsg(tuTxtNo_++);
      pUGSerifu_->SetMsgChangeMode(enMsgChangeMode_None);
      pUGSerifu_->SetDraw(TRUE);
      f32 mapX = 0;
      f32 mapY = 0;
      map_->z_GetGlobalPos(&mapX, &mapY);
      // 矢印の初期位置
      f32 panelX = 0;
      f32 panelY = 0;
      pPanel_->GetField()->z_GetPos(&panelX, &panelY);
      panelX += mapX + 30;	// オフセット
      panelY += mapY - 55;
      pUGFinger_->SetDraw(TRUE);
      pUGFinger_->SetDepth(enUIDepth::UIDP_09MSGBOX);
      pUGFinger_->SetRevUD(TRUE);
      pUGFinger_->SetMotionLookPoint(panelX, panelY);
      
      //パネルだけ有効
      pPanel_->SetPanelButtonModal(TRUE);
      //pScene_->pmgUI_->SetModal(FALSE);
    }
    break;

    case db::pg_Visit:
      break;
    case db::pg_SelectPanel:
      //パネルだけ有効を解除
      pPanel_->SetPanelButtonModal(FALSE);

      pUGSerifu_->SetDraw(FALSE);
      pUGFinger_->ClearMotion();
      pUGFinger_->SetDraw(FALSE);
      ResetBtn();
      break;
    case db::pg_Enemy:
      break;
    default:
      break;
    }
  }
  
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::DoUpdate(BOOL draw_f)
  {
    //ヘッダ動作時はポーズ
    Pause(pScene_->lyTuScBar_->IsBarMove());
    //描画の時以外はポーズ時抜ける
    if ((draw_f == FALSE) && stMap_.paused_f_) return;

    map_->Update(draw_f);
    //pUGSerifu_->Update(draw_f);
    //pUGFinger_->Update(draw_f);

    //switch (stLy_.pageNo_)
    //{
    //default:
    ugkantoku_.Update(draw_f);
    //ugcap_.Update(draw_f);
    camera_->Update(draw_f);
    for (s32 i = 0; i < ugpanelvec_.size(); ++i) ugpanelvec_[i]->Update(draw_f);
    //  break;
    //}

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case db::pg_Text1:MvPage_Text();	break;
      case db::pg_TutoStage:MvPage_TutoStage();	break;
      case db::pg_TutoCamMove:MvPage_TutoCamMove();	break;
      case db::pg_TutoSelect:  MvPage_TutoSelect(); break;

      case db::pg_Visit:MvPage_Visit(); break;
      case db::pg_SelectPanel:MvPage_SelectPanel();  break;
      case db::pg_Enemy:MvPage_Enemy(); break;
      }
    }

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


#ifdef __K_MAKE__HOGE
    s32 POSY = 10;

    // 座標の確認
    if (pUGFinger_)
    {
      kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
      f32 localX = 0;
      f32 localY = 0;
      f32 globalX = 0;
      f32 globalY = 0;
      pUGFinger_->GetField()->z_GetPos(&localX, &localY);
      pUGFinger_->GetField()->z_GetGlobalPos(&globalX, &globalY);
      char str_c[256];
      sprintf(str_c, "finger");
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
      POSY += 6;
      sprintf(str_c, "Local x:%f, y:%f", localX, localY);
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
      sprintf(str_c, "Global x:%f, y:%f", globalX, globalY);
      POSY += 6;
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
    }

    TUGScPanel* pTutoRetro = NULL;
    TUGScPanel* pTutoSumaho = NULL;
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      s32 pk = ugpanelvec_[i]->stScPanel_.mid_maptip_;
      if (pk == MAP_TUTO_RETRO){ pTutoRetro = ugpanelvec_[i]; }
      else if (pk == MAP_TUTO_SUMAHO){ pTutoSumaho = ugpanelvec_[i]; }
    }
    if (pTutoRetro)
    {
      f32 localX = 0;
      f32 localY = 0;
      f32 globalX = 0;
      f32 globalY = 0;
      pTutoRetro->GetField()->z_GetPos(&localX, &localY);
      pTutoRetro->GetField()->z_GetGlobalPos(&globalX, &globalY);
      char str_c[256];
      POSY += 6;
      sprintf(str_c, "panel");
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
      POSY += 6;
      sprintf(str_c, "Local x:%f, y:%f", localX, localY);
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
      POSY += 6;
      sprintf(str_c, "Global x:%f, y:%f", globalX, globalY);
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
    }

    if (camera_)
    {
      f32 localX = 0;
      f32 localY = 0;
      f32 globalX = 0;
      f32 globalY = 0;
      camera_->z_GetPos(&localX, &localY);
      camera_->z_GetGlobalPos(&globalX, &globalY);
      char str_c[256];
      POSY += 6;
      sprintf(str_c, "camera");
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
      POSY += 6;
      sprintf(str_c, "Local x:%f, y:%f", localX, localY);
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
      POSY += 6;
      sprintf(str_c, "Global x:%f, y:%f", globalX, globalY);
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
    }

    if (field_)
    {
      f32 localX = 0;
      f32 localY = 0;
      f32 globalX = 0;
      f32 globalY = 0;
      field_->z_GetPos(&localX, &localY);
      field_->z_GetGlobalPos(&globalX, &globalY);
      char str_c[256];
      POSY += 6;
      sprintf(str_c, "field");
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
      POSY += 6;
      sprintf(str_c, "Local x:%f, y:%f", localX, localY);
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
      POSY += 6;
      sprintf(str_c, "Global x:%f, y:%f", globalX, globalY);
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
    }

    if (map_)
    {
      f32 localX = 0;
      f32 localY = 0;
      f32 globalX = 0;
      f32 globalY = 0;
      map_->z_GetPos(&localX, &localY);
      map_->z_GetGlobalPos(&globalX, &globalY);
      char str_c[256];
      POSY += 6;
      sprintf(str_c, "map");
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
      POSY += 6;
      sprintf(str_c, "Local x:%f, y:%f", localX, localY);
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
      POSY += 6;
      sprintf(str_c, "Global x:%f, y:%f", globalX, globalY);
      kdebug::DebugText::GetInstance()->SetDebugText(5, POSY, str_c);
    }
#endif //#ifdef __K_MAKE__
  }
  
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::ClearPanelVec()
  {
    for (s32 i = 0; i < ugpanelvec_.size(); ++i) Freemem(ugpanelvec_[i]);
    ClearGrp();//読み込んだパネル画像pgrvec_をクリア
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::SetScrollSize()
  {
    BOOL zeropos_f = TRUE;
    //マップパネル
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      //まだ空いてないのもチェック★
      if (ugpanelvec_[i]->GetAreaState() == as_None)continue;

      s32 posx = ugpanelvec_[i]->GetMDt(mdm_maptip_posX);
      s32 posy = ugpanelvec_[i]->GetMDt(mdm_maptip_posY);
      if (zeropos_f || (posx < stMap_.left_)) stMap_.left_ = posx;
      if (zeropos_f || (posx > stMap_.right_)) stMap_.right_ = posx;
      if (zeropos_f || (posy < stMap_.top_)) stMap_.top_ = posy;
      if (zeropos_f || (posy > stMap_.bottom_)) stMap_.bottom_ = posy;

      zeropos_f = FALSE;
    }

    map_->sc_CamSwipeData(
      (stMap_.left_ * PANELDISTX) - PANELDISTX,
      (stMap_.top_ * PANELDISTY) - PANELDISTY,
      (stMap_.right_ * PANELDISTX) + PANELDISTX,// + BASEX - 8,
      (stMap_.bottom_ * PANELDISTY) + PANELDISTY// + BASEY - PANELDISTY- PANELDISTY);//広がるたび範囲拡大する
      );
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::OpenChildStart(s64 posNo)
  {
    //if (posNo == 0) return;

    for (s32 i2 = 0; i2 < ugpanelvec_.size(); ++i2)
    {
      //if (ugpanelvec_[i2]->GetIDMapTip() == mid_maptip)
      if (ugpanelvec_[i2]->GetMDt(mdm_maptip_posNo) == posNo)
      {
        //子がクリアしてたらそこでストップ。この子のチェックで以降を行う
        if (ugpanelvec_[i2]->IsClear()) break;

        //開いた場合その子もチェック
        if (ugpanelvec_[i2]->CheckParent())
        {
          OpenChildlen(ugpanelvec_[i2]);
        }
      }
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::OpenChild(s64 posNo)
  {
    if (posNo == 0) return;

    for (s32 i2 = 0; i2 < ugpanelvec_.size(); ++i2)
    {
      //if (ugpanelvec_[i2]->GetIDMapTip() == mid_maptip)
      if (ugpanelvec_[i2]->GetMDt(mdm_maptip_posNo) == posNo)
      {
        //子がクリアしてたらそこでストップ。この子のチェックで以降を行う
        if (ugpanelvec_[i2]->IsClear()) break;

        //開いた場合その子もチェック
        if (ugpanelvec_[i2]->CheckParent())
        {
          OpenChildlen(ugpanelvec_[i2]);
        }
      }
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::OpenChild(s32 posx, s32 posy)
  {
    OpenChild(pmgEO_->mgCoM_.GetMapTipPosNo(stMap_.nowmapNo_, posx, posy));
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::OpenChildlen(TUGScPanel* parent)
  {
    const s32 CHILD = 3;//1× 2親 3長 4子

    s32 posx = parent->GetMDt(mdm_maptip_posX);
    s32 posy = parent->GetMDt(mdm_maptip_posY);

    //４方向の子と遠子に命令を送る
    if (parent->GetMDt(mdm_maptip_roadUp) >= CHILD)      OpenChild(posx, posy - 1);
    if (parent->GetMDt(mdm_maptip_roadDown) >= CHILD) OpenChild(posx, posy + 1);
    if (parent->GetMDt(mdm_maptip_roadLeft) >= CHILD)    OpenChild(posx - 1, posy);
    if (parent->GetMDt(mdm_maptip_roadRight) >= CHILD) OpenChild(posx + 1, posy);
    OpenChild(parent->GetMDt(mdm_maptip_fc0));
    OpenChild(parent->GetMDt(mdm_maptip_fc1));
    OpenChild(parent->GetMDt(mdm_maptip_fc2));
    OpenChild(parent->GetMDt(mdm_maptip_fc3));
    OpenChild(parent->GetMDt(mdm_maptip_fc4));
    OpenChild(parent->GetMDt(mdm_maptip_fc5));
    OpenChild(parent->GetMDt(mdm_maptip_fc6));
    OpenChild(parent->GetMDt(mdm_maptip_fc7));
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::OpenSelf(TUGScPanel* parent)
  {

  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::OpenMap(s32 startpos)
  {

    //１行目はスタート地点としてオープン
    OpenChildStart(ugpanelvec_[startpos]->GetMDt(mdm_maptip_posNo));

    //クリアチェック
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      s64 mid_maptip = ugpanelvec_[i]->GetMIDMapTip();
      //クリアチェック
      for (s32 i2 = 0; i2 < pmgEO_->mgCoU_.mdMaptip_.GetLineNum(); ++i2)
      {
        if (pmgEO_->mgCoU_.mdMaptip_.GetLineDt(i2, mdmaptip_id_m_maptip) == mid_maptip)
        {
          ugpanelvec_[i]->SetClear();
          break;
        }
      }
    }

    //クリアチェック
    if (pmgEO_->stSeq_.IsNoSv())
    {
      for (s32 i = 0; i < ugpanelvec_.size(); ++i)
      {
        s64 mid_maptip = ugpanelvec_[i]->GetMIDMapTip();
        //クリアチェック
        ugpanelvec_[i]->SetClear();

      }
    }

    //オープンチェック
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      //クリアのみ
      if (ugpanelvec_[i]->IsClear() == FALSE) continue;
      //      OpenSelf(ugpanelvec_[i]);
      OpenChildlen(ugpanelvec_[i]);
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::Pause(BOOL pause_f)
  {
    if (stMap_.paused_f_)
    {
      //map_->sc_CamSwipe(FALSE);
    }
    else
    {
      if (stMap_.paused_f_ != pause_f)
      {
        switch (stLy_.pageNo_)
        {
        case db::pg_Enemy: break;
        default:
          //map_->sc_CamSwipe(TRUE);
          break;
        }
      }
    }
    stMap_.paused_f_ = pause_f;
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::MvPage_Text(void)
  {
    // メッセージの次へが押された
    if (pUGSerifu_->GetActRes() == enUGSerifuAct_WaitEnd)
    {
      pUGSerifu_->SetDraw(FALSE);
      ChangePage(pg_TutoStage);
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::MvPage_TutoStage(void)
  {
    if (pUGSerifu_->GetActRes() == enUGSerifuAct_WaitEnd)
    {
      pUGSerifu_->SetDraw(FALSE);
      pUGFinger_->SetDraw(FALSE);
      ChangePage(pg_TutoCamMove);
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::MvPage_TutoCamMove(void)
  {
    map_->sc_CamSetPos(
      (camera_->stSO_.stZahyou_.x_),
      (camera_->stSO_.stZahyou_.y_));

    // カメラがスクロールした
    if (camera_->stSO_.stZahyou_.stopping_f_)
    {
      ChangePage(pg_TutoSelect);
    }

  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::MvPage_TutoSelect(void)
  {
    // パネルを選択した
    if (pPanel_->IsAct())
    {
      ChangePage(pg_SelectPanel);
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::MvPage_Visit()
  {
    ChangePage(pg_SelectPanel);
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::MvPage_SelectPanel()
  {
    //   pmgEO_->stSeq_.GetMidMapTip() = pPanel_->GetActIndex();//mid_maptipであってるか
    //pmgEO_->mgStSn_.EtcSS.lno_maptip_ = pPanel_->GetLNoMapTip();
    //pmgEO_->mgStSn_.EtcSS.clear_f_ = pPanel_->IsClear();//クリア済み
    //   pmgEO_->mgStSn_.EtcSS.SetShiaiType_Normal();//通常試合

    s32 posx = pPanel_->stScPanel_.posX_* PANELDISTX;
    s32 posy = pPanel_->stScPanel_.posY_* PANELDISTY;
    ugkantoku_.GetField()->z_MoveBrk(posx + PLBASEX + PLX, posy + PLBASEY, MVSPD2, EATTIME);
    camera_->z_SetPos(map_->stSO_.stCamSw_.posX_, map_->stSO_.stCamSw_.posY_);
    camera_->z_MoveBrk(posx + SELECTPOSX, posy + SELECTPOSY, MVSPD, EATTIME);

    pmgEO_->stSeq_.ReadyMapTip(pPanel_->GetMIDMapTip(), pPanel_->GetLNoMapTip(), pPanel_->IsClear(), TStSnSeq::tt_None);
    ChangePage(pg_Enemy);

    pScene_->lyTuScBar_->MoveLayer(tscl_Enemy);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::MvPage_Enemy()
  {
    //マップスクロール＆プレート選択
    if (ugkantoku_.GetField()->stSO_.stZahyou_.eat_ == 0)
    {
      ugkantoku_.SetMType(kmt_St);
    }
    map_->sc_CamSetPos(
      (camera_->stSO_.stZahyou_.x_),
      (camera_->stSO_.stZahyou_.y_));
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::SetMsg(const s32 msgNo)
  {
    SetMsg(TUTO_MSG_MAP[msgNo][0], TUTO_MSG_MAP[msgNo][1], TUTO_MSG_MAP[msgNo][2]);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScMap::SetMsg(const char* text0, const char* text1, const char* text2)
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
}
