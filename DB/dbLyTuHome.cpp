//utf8
#include "dbLyTuHome.h"
#include "dbSnTutorial.h"
#include "dbUGSerifu.h"
//=========================================================================
// 
// チュートリアル：ホーム画面
//
//=========================================================================
#ifdef __K_DEBUG_ROOT__
//#define __K_DEBUG_SKIP_LOGBO__
#endif
#ifdef __K_MAKE__
//#define __K_DEBUG_SPECIAL_MESSAGE__
#endif

//#define __PROFILE_ERROR_CONTINUE__	// プロフィールチュートリアルのエラーコンテニューをする

namespace db
{
  enum enPage
  {
    pg_Init,
    pg_CapTalk1,
    pg_MamiMenu,
    pg_InMamiMenu,
    pg_OutMamiMenu,
    pg_CapTalk2,

    pg_EditProfile,
    pg_InEditProfile,
    pg_CheckEditProfile,
    pg_ErrEditProfile,

    pg_SpecialProfileLooks,
    pg_SpecialProfileName,

    pg_OutEditProfile,

    pg_CapTalk3,

    pg_GotoShop,
    pg_End,
  };

  enum enConnect
  {
    cn_tutohome,
  };

  const s32 MESSAGE_LINE_MAX = 3;		// メッセージの最大行数は3

  //----------------------------------------------------------------------
  // キャプテンのセリフ関係
  enum enCaptainTalkCategory
  {
    ct_Category1 = 0,
    ct_Category2,
    ct_Category3,
    ct_Category_MAX,
  };
  // トークカテゴリー1
  const s32 CAPTAIN_TALK1_MAX = 4;
  const char* CAPTAIN_TALK1[CAPTAIN_TALK1_MAX][MESSAGE_LINE_MAX] =
  {
    {
      "$CAP_NAME$「あんた＿なかなか＿やるな",
      "ガチャこうしゅうが＿はじまるまで",
    },
    {
      "$CAP_NAME$「じかんを＿つぶそうぜ",
      "ちなみに＿ここが＿ホームロビーだ",
    },
    {
      "$CAP_NAME$「うんえいのおしらせ＿や",
      "あんないが＿かくにんできるぜ",
    },
    {
      "$CAP_NAME$「まず＿あんないに＿いって",
      "おくりものを＿うけとろうぜ",
    },
  };

  // トークカテゴリー2
  const s32 CAPTAIN_TALK2_MAX = 3;
  const char* CAPTAIN_TALK2[CAPTAIN_TALK2_MAX][MESSAGE_LINE_MAX] =
  {
    {
      "$CAP_NAME$「おくりものは＿まいにち",
      "とどく＿うけとり＿わすれるなよ",
    },
    {
      "$CAP_NAME$「ところで＿かんとくさん",
      "そのマスク＿とってくれないか？",
    },
    {
      "$CAP_NAME$「ここで＿なまえと＿かおは",
      "いつでも＿へんこうできるぜ",
    },
  };
  // トークカテゴリー3
  const s32 CAPTAIN_TALK3_MAX = 2;
  const char* CAPTAIN_TALK3[CAPTAIN_TALK3_MAX][MESSAGE_LINE_MAX] =
  {
    {
      "$CAP_NAME$「あらためて＿よろしくな！",
      "$NAME$さん！",
    },
    {
      "$CAP_NAME$「おっ＿ガチャこうしゅうの",
      "じかんだな＿ショップから＿いけるぜ",
    },
  };

  // エラートーク
  enum CAPTAIN_ERR_TALK
  {
    CET_MASK_NAME = 0,
    CET_MASK,
    CET_NAME,
    CET_AKIRAME,
    CET_MAX,
  };
  const char* CAPTAIN_ERR_TALK[CET_MAX][MESSAGE_LINE_MAX] =
  {
    {
      "$CAP_NAME$「なぁ＿マスクを＿とって",
      "なまえを＿おしえてくれよ！",
    },
    {
      "$CAP_NAME$「$NAME$さん＿マスク",
      "とってくれよ＿かみがた＿へんこうだ",
    },
    {
      "$CAP_NAME$「なまえも＿おしえてくれよ",
      "なんて＿よべばいいか＿こまるだろ？",
    },
    {
      "$CAP_NAME$「おう＿わかったぜ！",
	  "プロフィールは＿あとで＿かえてくれ",
    },
  };

  // 特殊セリフ
  enum CAPTAIN_SPECIAL_TALK
  {
    CST_HAGE = 0,		// ハゲ
    CST_BARCODE,		// バーコード頭
    CST_MASK,			// マスク
    CST_CHARA,			// 既存キャラ
    CST_PANTS,			// パンツ
    CST_KUNIO,			// くにお
    CST_KAO_KAIBUTU,	// 怪物顔
    CST_KAMI_KAIBUTU,	// 怪物ヅラ
    CST_KOGANEMUSHI,	// こがねむし
    CST_MAX,
  };
  // 上から順番にチェックしていく
  const char* CAPTAIN_SPECIAL_TALK[CST_MAX][MESSAGE_LINE_MAX] =
  {
    //◆ハゲ
    {
      "$CAP_NAME$「おっ黙黙おう黙黙黙！",
      "（やべぇーっ！＿はげしいーっ！）",
    },
    //◆バーコード
    {
      "$CAP_NAME$「おっ黙黙おう黙黙黙",
      "（おっさん黙黙＿あきらめわりぃな）",
    },
    //◆マスク
    {
      "$CAP_NAME$「ズコー！＿もう１まい",
      "かぶってるんかーーーーーい！",
    },
    //◆パンツ
    {
      "$CAP_NAME$「うわぁ黙黙黙黙",
      "（へ・へんたいだーーーーーーっ！）",
    },
    //◆上下お揃い
    {
      "$CAP_NAME$「おまえ＿＊＊＊＊＊＊！？",
      "黙黙いや＿ひとちがい黙黙だよな？",
    },
    //◆くにお上下(キャプテンくんの顔ヅラとチェック)
    {
      "$CAP_NAME$「おまえは黙黙＿オレ？？？",
    },
    //◆顔怪物
    {
      "$CAP_NAME$「ひいいいいいいいいいっ！",
      "に・にんげんじゃねーーーーーっ！",
    },
    //◆ヅラ怪物
    {
      "$CAP_NAME$「ひいいいいいいいいいっ！",
      "に・にんげんじゃねーーーーーっ！",
    },
    //◆なまえ：こがねむし
    {
      "$CAP_NAME$「そのなまえ＿わかってんな",
      "まぁ＿なにも＿ないんだけどな！",
    },
  };


  const char* KANTOKU_NAME_REPLACE = "$NAME$";

  const s32 HOME_BG_ID = 35;
  const s32 HOME_BGM_ID = 1;

  const s32 MAMI_X = 260 + 32;
  const s32 MAMI_Y = 58 + 16;

  const s32 ERR_NUM = 2;

  //-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
  TLyTuHome::TLyTuHome(TSnTutorial* pScene)
    :TLyTuBase(pScene)
    , ugbtn_mami_(this)
    , ugkantoku_(this)
    , ugcap_(this)
    , ugmami_(this)
    , uglogbo_(this)
    , editErrCnt_(0)
    , subPage_(0)
  {
  }

  //-----------------------------------------------------------------
  // デストラクタ
  //-----------------------------------------------------------------
  TLyTuHome::~TLyTuHome()
  {
  }

  //-----------------------------------------------------------------
  // 真実メニューからの復帰
  //-----------------------------------------------------------------
  void TLyTuHome::ComebackInMamiMenu(void)
  {
    ChangePage(pg_OutMamiMenu);
  }

  //-----------------------------------------------------------------
  // プロフィールからの復帰
  //-----------------------------------------------------------------
  void TLyTuHome::ComebackProfile(void)
  {
    ugbtn_back_->SetDraw(FALSE);
    ChangePage(pg_CheckEditProfile);
  }

  //-----------------------------------------------------------------
  // 暗転した後にチュート終了フラグを立てるため
  //-----------------------------------------------------------------
  void TLyTuHome::SetTutoEnd()
  {
    isTutoEnd_ = TRUE;
  }



  //-----------------------------------------------------------------
  // ロード
  //-----------------------------------------------------------------
  void TLyTuHome::DoLoad()
  {
    // dbLyHmHome.cppよりコピペ

    const s32 MAMIY = 136;
    const s32 MAMIYF = MAMIY - mid::midFullDotS_Hf();

    //-------------------------------------------------------
    // ロビー(背景)を作成
    for (s32 i = 0; i < WINDOWNUM; ++i)
    {
      shadow_[i] = MakeObj();
      shadow_[i]->o_SetObjGrp(tugrp_->home_shadow_);
      shadow_[i]->o_SetParent(field_);
      shadow_[i]->g_SetAlp(TRUE, 0x9A);

      window_[i] = MakeObj();
      window_[i]->o_SetObjGrp(tugrp_->home_window_);
      window_[i]->o_SetParent(field_);
      window_[i]->g_SetOvOvj(shadow_[0]);
      window_[i]->g_SetAlp(TRUE, 0x30);
    }
    shadow_[0]->z_SetPos_Full(72 + 0, 44);
    shadow_[1]->z_SetPos_Full(72 + 104, 44);
    shadow_[2]->z_SetPos_Full(72 + 200, 44);
    window_[0]->z_SetPos_Full(75 + 0, 44);
    window_[1]->z_SetPos_Full(75 + 105, 44);
    window_[2]->z_SetPos_Full(75 + 201, 44);


    floorB_ = MakeObj();
    floorB_->o_SetObjGrPart(tugrp_->home_bg_);
    floorB_->g_SetCenter(TRUE);
    floorB_->z_SetPos(0, -4);
    floorB_->o_SetParent(field_);
    floorB_->g_SetOvOvj(window_[0]);

    boardL_ = MakeObj();
    boardL_->o_SetObjGrp(tugrp_->home_board_);
    boardL_->o_SetParent(floorB_);
    boardL_->z_SetPos_Full(117, 90);

    boardR_ = MakeObj();
    boardR_->o_SetObjGrp(tugrp_->home_board_);
    boardR_->o_SetParent(floorB_);
    boardR_->z_SetPos_Full(277, 90);

    floorF_ = MakeObj();
    floorF_->o_SetObjGrPart(tugrp_->home_bg_);
    floorF_->g_SetPartNo(1);
    floorF_->g_SetCenter(TRUE);
    floorF_->o_SetParent(floorB_);
    floorF_->g_SetCharLayer(MAMIYF + 1);

    //flower_ = MakeObj();
    //flower_->o_SetObjGrp(tugrp_->home_flower_);
    //flower_->o_SetParent(floorF_);
    //flower_->z_SetPos_Full(200, 162);
    //flower_->g_SetCharLayer(flower_->stSO_.stZahyou_.y_ + tugrp_->home_flower_->GetHeight() - 2);

    // ログボ
    uglogbo_.MakeGroup(field_);
    uglogbo_.SetPos_Full(167 + 6 + 75, MAMIY - 12);
    uglogbo_.Refresh();

    // BGはドッジの日本の桜
    pmgUI_->LoadCourt(HOME_BG_ID);//背景
    // BGMは？
    //pmgEO_->mgSound_.PlayBGM(HOME_BGM_ID);


    s32 posX = 97 + 18;
    s32 posY = 158 + 35;

    //-------------------------------------------------------
    // 監督作成
    ugkantoku_.MakeGroup(field_, FALSE);
    ugkantoku_.SetPos_Full(posX, posY);

    //-------------------------------------------------------
    // キャプテン作成
    captainDt_.Init();
    ugcap_.SetupMgCharGrp();
    ugcap_.MakeGroup(field_);
    ugcap_.SetPos_Full(posX + 26, posY);
    ugcap_.SetKantoku(&ugkantoku_);
    ugcap_.SetWalkType(cwt_MYWALK);
    ugcap_.SetMirror(TRUE);

    //-------------------------------------------------------
    // 真実作成
    // 吹き出し
    ugbtn_mami_.MakeGroup_Balloon(field_, "あんない");
    ugbtn_mami_.SetPos_Full(MAMI_X, MAMI_Y);
    ugbtn_mami_.SetDraw(FALSE);
    // キャラ
    ugmami_.MakeGroup(field_, 11, FALSE);
    ugmami_.SetPos_Full(279, MAMIY);
    ugmami_.SetMirror(TRUE);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::DoReady()
  {
    // 基底クラスのDoReadyを呼び出す
    base::DoReady();
    ugserifu_.ChangeFrame(enSerifuFrame_Normal);

    pmgEO_->mgNSF_.Play(nsf_home);//BGM
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case pg_CapTalk1:
      captainDt_.talkCategory_ = ct_Category1;
      ugcap_.SetMType(cmt_St);
      SetMsg(CAPTAIN_TALK1[captainDt_.talkNo_][0], CAPTAIN_TALK1[captainDt_.talkNo_][1], CAPTAIN_TALK1[captainDt_.talkNo_][2]);
      ugserifu_.SetDraw(TRUE);
      ugserifu_.SetPos(-160, -30);
      break;

    case pg_MamiMenu:
    {
      ugbtn_mami_.SetDraw(TRUE);	// ここでメニューボタンを表示
      f32 x = 0;
      f32 y = 0;
      ugbtn_mami_.GetField()->z_GetGlobalPos(&x, &y);
      // offset
      x += 24;
      y += 10;
      ugfinger_.SetMotionTouch(x, y);
      ugfinger_.SetDraw(TRUE);
    }
    break;

    case pg_InMamiMenu:
      break;

    case pg_OutMamiMenu:
      // 真実どっかいく
    {
      ugbtn_mami_.SetDraw(FALSE);	// ここでメニューボタンを非表示
      ugmami_.SetMirror(FALSE);
      f32 x = 0;
      f32 y = 0;
      ugmami_.GetField()->z_GetPos(&x, &y);
      ugmami_.z_MoveWork(0, x + 70, y, 2, cmk_Right);		// 柱裏まで移動
      subPage_ = 0;
      ugmami_.SetMType(gmt_Wk);
    }
    break;

    case pg_CapTalk2:
      captainDt_.talkCategory_ = ct_Category2;
      ugcap_.SetMType(cmt_St);
      SetMsg(CAPTAIN_TALK2[captainDt_.talkNo_][0], CAPTAIN_TALK2[captainDt_.talkNo_][1], CAPTAIN_TALK2[captainDt_.talkNo_][2]);
      ugserifu_.SetDraw(TRUE);
      ugserifu_.SetPos(10 - 5, -80);
      ugmami_.SetDraw(FALSE);
      break;

    case pg_EditProfile:
    {
      pScene_->lyTuBar_->ugheadder_.ugbtn_prof_.SetDraw(TRUE);
      ugfinger_.SetMotionTouch(50, -85);
      ugfinger_.SetDraw(TRUE);
    }
    break;

    case pg_InEditProfile:
      break;

    case pg_CheckEditProfile:
      break;

    case pg_ErrEditProfile:
      break;

    case pg_SpecialProfileLooks:
    case pg_SpecialProfileName:
      //pScene_->lyTuBar_->ugheadder_.SetDisable();
      break;

    case pg_OutEditProfile:
      //プロフ変更おわったらヘッダ表示けしとく
      pScene_->lyTuBar_->ugheadder_.SetDisable();
      //pScene_->lyTuBar_->ugheadder_.ugbtn_prof_.SetDraw(FALSE);
      //pScene_->lyTuBar_->ugheadder_.ugbtn_credit_.SetDraw(FALSE);
      //pScene_->lyTuBar_->ugheadder_.ugbtn_ggz_.SetDraw(FALSE);
      break;

    case pg_CapTalk3:
      captainDt_.talkCategory_ = ct_Category3;
      ugcap_.SetMType(cmt_St);

      if (captainDt_.talkNo_ == 0)
      {
        // 名前を入れる処理追加
        std::string str = CAPTAIN_TALK3[captainDt_.talkNo_][1];
        std::string name = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_name_c32);

        std::size_t pos = str.find(KANTOKU_NAME_REPLACE);
        std::size_t size = std::string(KANTOKU_NAME_REPLACE).size();
        str.replace(pos, size, name);

        SetMsg(CAPTAIN_TALK3[captainDt_.talkNo_][0], str.c_str(), CAPTAIN_TALK3[captainDt_.talkNo_][2]);
      }
      else
      {
        SetMsg(CAPTAIN_TALK3[captainDt_.talkNo_][0], CAPTAIN_TALK3[captainDt_.talkNo_][1], CAPTAIN_TALK3[captainDt_.talkNo_][2]);
      }
      ugserifu_.SetDraw(TRUE);
      break;

    case pg_GotoShop:
      ugfinger_.SetRevUD(TRUE);
      ugfinger_.SetMotionTouch(150, 60);
      ugfinger_.SetDraw(TRUE);
      pScene_->lyTuBar_->ugfooter_.ugradio_.SetDraw(TRUE);
      for (s32 i = 0; i < pScene_->lyTuBar_->ugfooter_.tabbtn_.size(); i++)
      {
        BOOL isDisp = FALSE;
        if (i == 4){ isDisp = TRUE; }// タブ5つ目 = ショップタブ
        //pScene_->lyTuBar_->ugfooter_.lybtn_[i]->SetDraw(isDisp);
        pScene_->lyTuBar_->ugfooter_.tabbtn_[i]->SetDraw(isDisp);
      }
      break;

    case pg_End:
      pScene_->lyTuBar_->GotoTab(TLyTuBar::tab_Shop);
      pScene_->lyTuBar_->ugfooter_.ugradio_.SetDraw(FALSE);
      ugfinger_.SetDraw(FALSE);

      //isTutoEnd_ = TRUE;//ここで急にとばせない暗転後にとばす
#ifdef __K_DEBUG_USER_DATA__
      kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_Kantoku);
#endif
      break;

    default:
      break;
    }
    stLy_.pageNo_ = pageno;
  }

  //-----------------------------------------------------------------
  //接続完了
  //-----------------------------------------------------------------
  void TLyTuHome::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_tutohome: Connect_TuHome(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
      }
    }
    else
    {
      //通信エラー
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::DoUpdate(BOOL draw_f)
  {
    base::DoUpdate(draw_f);

    floorB_->Update(draw_f);
    floorF_->Update(draw_f);
    boardL_->Update(draw_f);
    boardR_->Update(draw_f);
    //flower_->Update(draw_f);
    uglogbo_.Update(draw_f);
    ugkantoku_.Update(draw_f);
    ugcap_.Update(draw_f);

    for (s32 i = 0; i < WINDOWNUM; ++i)
    {
      shadow_[i]->Update(draw_f);
      window_[i]->Update(draw_f);
    }

    ugmami_.Update(draw_f);
    switch (stLy_.pageNo_)
    {
    case pg_InMamiMenu:
      break;
    default:
      ugbtn_mami_.Update(draw_f);
      break;
    }

    switch (stLy_.pageNo_)
    {
    case pg_Init:
      Init();
      break;

    case pg_CapTalk1:
    case pg_CapTalk2:
    case pg_CapTalk3:
      CapTalk();
      break;

    case pg_MamiMenu:
      MamiMenu();
      break;

    case pg_InMamiMenu:
      InMamiMenu();
      break;

    case pg_OutMamiMenu:
      OutMamiMenu();
      break;

    case pg_EditProfile:
      EditProfile();
      break;

    case pg_InEditProfile:
      InEditProfile();
      break;

    case pg_CheckEditProfile:
      CheckEditProfile();
      break;

    case pg_ErrEditProfile:
      ErrEditProfile();
      break;

      //   case pg_SpecialProfileLooks:
      //     SpecialProfileLooks();
      //     break;

      //case pg_SpecialProfileName:
      //	SpecialProfileName();
      //	break;

    case pg_OutEditProfile:
      OutEditProfile();
      break;

    case pg_GotoShop:
      GotoShop();
      break;

    default:
      break;
    }

#ifdef __K_DEBUG_ROOT__
    kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
    kdebug::DebugText::GetInstance()->SetDebugText(10, 30, "ホーム説明");
#endif
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::Post_TuHome()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_TuHome();
    }
    else
    {
      // ここに送信処理を書く
      StartConnect(cn_tutohome);
      pmgEO_->mgPo_.poTuto_.Ready_SetBit(enTutoBitType_Home);
    }
  }


  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::Connect_TuHome()
  {

#ifdef __K_DEBUG_USER_DATA__
    kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_Home);
#endif
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::Init(void)
  {
    captainDt_.Init();

    // 今回が初めて
#ifdef __K_DEBUG_USER_DATA__
#ifdef __K_DEBUG_SKIP_LOGBO__
    // ログボスキップ
    if(0)
#else
    if (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Home))
#endif // __K_DEBUG_SKIP_LOGBO__
#else
    if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Home) == FALSE)
#endif
    {
      ChangePage(pg_CapTalk1);
      captainDt_.talkNo_++;
    }
    // ログボもらってこれからプロフィールエディット
    else
    {
      ugbtn_mami_.SetDraw(FALSE);
      ugmami_.SetDraw(FALSE);

      ugkantoku_.SetPos_Full(233 + 18 + 11, 110 + 35);
      ugkantoku_.SetMirror(FALSE);
      ugcap_.SetWalkType(cwt_MYWALK);
      ugcap_.SetPos_Full(233 + 18 + 45, 110 + 35);
      ugcap_.SetWalkType(cwt_FOLLOW);
      ugcap_.UpdateMemPos();
      ugcap_.SetMirror(TRUE);

      captainDt_.talkNo_ = 0;
      ChangePage(pg_CapTalk2);
      captainDt_.talkNo_++;


      // 現在の監督と同じものを入れておく
      hairCharaNo_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtHair);
      faceCharaNo_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtFace);
      name_ = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_name_c32);
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::CapTalk(void)
  {
    // メッセージの次へが押された
    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      // 
      switch (captainDt_.talkCategory_)
      {
      case  ct_Category1:
        if (captainDt_.talkNo_ >= CAPTAIN_TALK1_MAX)
        {
          ugserifu_.SetDraw(FALSE);
          ChangePage(pg_MamiMenu);
          return;
        }
        else
        {
          ChangePage(pg_CapTalk1);
          captainDt_.talkNo_++;
        }
        break;

      case ct_Category2:
        if (captainDt_.talkNo_ >= CAPTAIN_TALK2_MAX)
        {
          ugserifu_.SetDraw(FALSE);
          ChangePage(pg_EditProfile);
          return;
        }
        else
        {
          ChangePage(pg_CapTalk2);
          captainDt_.talkNo_++;
        }
        break;

      case ct_Category3:
        if (captainDt_.talkNo_ >= CAPTAIN_TALK3_MAX)
        {
          ugserifu_.SetDraw(FALSE);
          ChangePage(pg_GotoShop);
          return;
        }
        else
        {
          ChangePage(pg_CapTalk3);
          captainDt_.talkNo_++;
        }
        break;
      }

    }
    // 喋り終わった
    else if (ugserifu_.GetActRes() == enUGSerifuAct_ReadEnd)
    {
      ugcap_.SetMType(cmt_St);
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::MamiMenu(void)
  {
    switch (subPage_)
    {
      // 真実をタッチ
    case 0:
      if (ugbtn_mami_.IsAct())
      {
        ugfinger_.SetDraw(FALSE);
        ugkantoku_.z_MoveWork_Full(1, 233 + 18 + 11, 110 + 35, 2, MAMI_X);
        ugcap_.SetWalkType(cwt_FOLLOW);
        ugcap_.UpdateMemPos();
        subPage_ = 1;
      }
      break;

      // 監督歩き終わり
    case 1:
      if (ugkantoku_.GetActRes() == enUGChGenAct_MoveStop)
      {
        ugcap_.SetWalkType(cwt_MYWALK);
        ugcap_.z_MoveWork_Full(1, 233 + 18 + 45, 110 + 35, 2, MAMI_X);
        subPage_ = 2;
      }
      break;

      // キャプテン歩き終わり
    case 2:
      if (ugcap_.GetActRes() == enUGChGenAct_MoveStop)
      {
        pScene_->lyTuHmMami_->Ready();
        ChangePage(pg_InMamiMenu);
      }
      break;
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::InMamiMenu(void)
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::OutMamiMenu(void)
  {
    // ふたりで見送る
    f32 x = 0;
    f32 y = 0;
    ugmami_.GetField()->z_GetGlobalPos(&x, &y);
    f32 capX = 0;
    f32 capY = 0;
    ugcap_.GetField()->z_GetGlobalPos(&capX, &capY);
    if (capX < x)
    {
      ugcap_.SetMirror(FALSE);
    }
    else
    {
      ugcap_.SetMirror(TRUE);
    }


    // 真実画面外へ(柱の後ろへ)
    if (ugmami_.GetActRes() == enUGChGenAct_MoveStop)
    {
      subPage_ = 0;
      ugcap_.SetMirror(TRUE);
      captainDt_.talkNo_ = 0;
      ChangePage(pg_CapTalk2);
      captainDt_.talkNo_++;
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::EditProfile(void)
  {
    if (pScene_->lyTuBar_->ugheadder_.ugbtn_prof_.IsAct())
    {
      pScene_->lyTuProfile_->Ready();
      ChangePage(pg_InEditProfile);
      ugfinger_.SetDraw(FALSE);
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::InEditProfile(void)
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHome::CheckEditProfile(void)
  {
    s32 type = 0;

    // 何もしてない？
    if (!pScene_->lyTuProfile_->IsHairChange() &&
      !pScene_->lyTuProfile_->IsNameChange())
    {
      editErrCnt_++;
      type = CET_MASK_NAME;
    }
    // マスクを取っていない？
    else if (!pScene_->lyTuProfile_->IsHairChange())
    {
      editErrCnt_++;
      type = CET_MASK;
    }
    // 名前変えてない？
    else if (!pScene_->lyTuProfile_->IsNameChange())
    {
      editErrCnt_++;
      type = CET_NAME;
    }
    // チュートリアルクリア
    else
    {
      // プロフィール編集チュートリアル終了
      subPage_ = 0;
      captainDt_.talkNo_ = 0;
      // test
#ifdef __K_DEBUG_SPECIAL_MESSAGE__
      ChangePage(pg_SpecialProfileLooks);
#else
      ChangePage(pg_OutEditProfile);
#endif
      return;
    }

    // ここにきたということはエラーできてる
#ifndef	__PROFILE_ERROR_CONTINUE__
	// プロフィールのエラーコンティニューモードじゃなければ
	// 一発で諦める
	editErrCnt_ = ERR_NUM + 1;
#endif

    // 2回までは許す
    if (editErrCnt_ <= ERR_NUM)
    {
      switch (type)
      {
      case CET_MASK_NAME:
        SetMsg(CAPTAIN_ERR_TALK[CET_MASK_NAME][0], CAPTAIN_ERR_TALK[CET_MASK_NAME][1], CAPTAIN_ERR_TALK[CET_MASK_NAME][2]);
        ugserifu_.SetDraw(TRUE);
        break;

      case CET_MASK:
      {
        // 名前を入れる処理追加
        std::string str = CAPTAIN_ERR_TALK[CET_MASK][0];
        std::string name = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_name_c32);

        std::size_t pos = str.find(KANTOKU_NAME_REPLACE);
        std::size_t size = std::string(KANTOKU_NAME_REPLACE).size();
        str.replace(pos, size, name);

        SetMsg(str.c_str(), CAPTAIN_ERR_TALK[CET_MASK][1], CAPTAIN_ERR_TALK[CET_MASK][2]);
        ugserifu_.SetDraw(TRUE);
      }
      break;

      case CET_NAME:
        SetMsg(CAPTAIN_ERR_TALK[CET_NAME][0], CAPTAIN_ERR_TALK[CET_NAME][1], CAPTAIN_ERR_TALK[CET_NAME][2]);
        ugserifu_.SetDraw(TRUE);
        break;
      }
      // 強制的にプロフィールに戻る
      ChangePage(pg_ErrEditProfile);
    }
    // 2回以上もどるを押した
    else
    {
      //わけありね
      // わけありメッセージ
      SetMsg(CAPTAIN_ERR_TALK[CET_AKIRAME][0], CAPTAIN_ERR_TALK[CET_AKIRAME][1], CAPTAIN_ERR_TALK[CET_AKIRAME][2]);
      ugserifu_.SetDraw(TRUE);
      ChangePage(pg_OutEditProfile);
    }
  }

  //-----------------------------------------------------------------
  //  プロフィール編集終了(スペシャルメッセージ)
  //-----------------------------------------------------------------
  void TLyTuHome::SpecialProfileLooks(void)
  {
    switch (subPage_)
    {
      // スペシャルメッセージチェック
    case 0:
    {
      switch (captainDt_.talkNo_)
      {
        // ハゲ
      case CST_HAGE:
        // はげは専用ビットフラグでチェック
        SetMsg(CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][0], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][1], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][2]);
        ugserifu_.SetDraw(TRUE);
        break;

      case CST_BARCODE:
        // バーコード頭も専用ビットフラグでチェック
        SetMsg(CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][0], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][1], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][2]);
        ugserifu_.SetDraw(TRUE);
        break;

      case CST_MASK:
        // マスクも専用ビットフラグでチェック
        SetMsg(CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][0], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][1], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][2]);
        ugserifu_.SetDraw(TRUE);
        break;

      case CST_CHARA:
        // 設定した顔ヅラから一致するキャラがあるかチェック
        SetMsg(CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][0], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][1], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][2]);
        ugserifu_.SetDraw(TRUE);
        break;

      case CST_KUNIO:
        // 上記のくにおバージョン
        SetMsg(CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][0], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][1], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][2]);
        ugserifu_.SetDraw(TRUE);
        break;

      case CST_KOGANEMUSHI:
        // 名前をチェックする
        SetMsg(CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][0], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][1], CAPTAIN_SPECIAL_TALK[captainDt_.talkNo_][2]);
        ugserifu_.SetDraw(TRUE);
        break;
      }
      subPage_ = 1;
    }
    break;
    // スペシャルメッセージ表示中
    case 1:
      if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
      {
        // 最後のチェック終わり
        if (captainDt_.talkNo_ > CST_MAX)
        {
          captainDt_.talkNo_++;
        }
        else
        {
          // 戻る
          subPage_ = 0;
        }
      }
      break;
    default:
      break;
    }

    // 全部のチェックが終わったら終了
    if (captainDt_.talkNo_ > CST_MAX)
    {
      ChangePage(pg_OutEditProfile);
    }
  }

  //-----------------------------------------------------------------
  //  プロフィール編集終了(スペシャルメッセージ)
  //-----------------------------------------------------------------
  void TLyTuHome::SpecialProfileName(void)
  {
  }

  //-----------------------------------------------------------------
  //  エラーメッセージ表示　→　再度プロフィール編集へ
  //-----------------------------------------------------------------
  void TLyTuHome::ErrEditProfile(void)
  {
    // メッセージの次へが押された
    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      ugserifu_.SetDraw(FALSE);
      pScene_->lyTuProfile_->Ready();
      ChangePage(pg_InEditProfile);
    }
  }

  //-----------------------------------------------------------------
  //  プロフィール編集終了
  //-----------------------------------------------------------------
  void TLyTuHome::OutEditProfile(void)
  {
    // ホームチュートリアル終わりの言葉
    if (editErrCnt_ <= ERR_NUM)
    {
      captainDt_.talkCategory_ = ct_Category3;
      captainDt_.talkNo_ = 0;
      ChangePage(pg_CapTalk3);
      captainDt_.talkNo_++;
    }
    else
    {
      if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
      {
        captainDt_.talkCategory_ = ct_Category3;
        captainDt_.talkNo_ = 1;	//0がよろしくメッセージなので1から
        ChangePage(pg_CapTalk3);
        captainDt_.talkNo_++;
      }
    }
  }

  //-----------------------------------------------------------------
  //  
  //-----------------------------------------------------------------
  void TLyTuHome::GotoShop(void)
  {
    if (pScene_->lyTuBar_->ugfooter_.tabbtn_[4]->IsAct())
    {
      ChangePage(pg_End);
    }
  }
}
