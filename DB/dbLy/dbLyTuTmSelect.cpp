//utf8
#include "dbLyTuTmSelect.h"
#include "dbSnTutorial.h"
#include "dbUGSerifu.h"
#include "dbUGChGirl.h"
#include "dbUGDrTeam.h"
#include "dbUGTeam.h"
#include "dbUGLiTmFilter.h"
//=========================================================================
// 
// チュートリアル：チーム選択
//
//=========================================================================
namespace db
{
  enum enPage
  {
    pg_Init,
    pg_EnterHasebe,	// はせべ登場
    pg_Talk1,		// 語り1
    pg_TeamSelect,	// チーム選択
    pg_TeamSelectCheck,	// チーム選択確認
    pg_Talk2,		// 語り2
  };

  enum enHasebeTalkCategory
  {
    ht_Category1 = 0,
    ht_Category2,
    ht_Category_MAX,
  };

  enum enConnect
  {
    cn_tutotmselect,
  };

  const s32 MESSAGE_LINE_MAX = 3;		// メッセージの最大行数は3

  // トークカテゴリー1
  const s32 HASEBE_TALK1_MAX = 9;
  const char* HASEBE_TALK1[HASEBE_TALK1_MAX][MESSAGE_LINE_MAX] =
  {
    {
      "はせべ「あなたが＿かんとくさんね？",
      "わたしは＿れいほうのすけばん＿はせべです",
    },
    {
      "はせべ「まず＿かんとくさんには",
      "ドッジボールのこうしゅうを＿うけてもらいます",
    },
    {
      "はせべ「プログラムは＿おおきくわけて＿３つ",
    },
    {
      "はせべ「ドッジボールの＿そうさほうほう",
    },
    {
      "はせべ「チーム・ＢＧＭ・はいけいを",
	  "かくとくするための＿ガチャ",
    },
    {
      "はせべ「オリジナルしゅーとの＿さくせいです",
    },
    {
      "はせべ「まずは＿ドッジボールの",
      "そうさほうほうから＿おぼえましょう",
    },
    {
      "はせべ「かんとくこうしゅうに＿いっしょに",
	  "さんかする＿チームを＿えらんでください",
    },
	{
		"はせべ「きにいったチーム＿なつかしいチームを",
		"えらんで＿おーけー！＿を＿おしましょう心",
	},

  };

  const char* HASEBE_INFO[MESSAGE_LINE_MAX] =
  {
    "はせべ「えらばなかったチームも",
	"あとで＿にゅうしゅ＿できます心",
  };

  const char* HASEBE_KAKUNIN[MESSAGE_LINE_MAX] =
  {
    "$NAME$＿で",
    "よろしいでしょうか？",
  };

  // トークカテゴリー2
  const s32 HASEBE_TALK2_MAX = 2;
  const char* HASEBE_TALK2[HASEBE_TALK2_MAX][MESSAGE_LINE_MAX] =
  {
    {
      "はせべ「チームが＿きまりましたね",
      "つづいて＿そうさせつめい＿です心",
    },
    {
      "はせべ「このばしょで＿おこないます",
      "いま＿コートラインを＿ひきますね心",
    },
  };

  const char* UGBTN_OK_TXT = "おーけー";
  const char* UGBTN_KWSK_TXT = "くわしく";
  const char* UGBTN_BACK_TXT = "もどる";

  const char* TEAM_NAME_REPLACE = "$NAME$";

  const s32 ENEMY_TEAM_MID_ID = 251;		// チュートリアルの対戦相手チーム(251:チュートリアーズ)
  const f32 ENTER_HASEBE_TIME = 30.0f;	// はせべ登場フレーム数

  //-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
  TLyTuTmSelect::TLyTuTmSelect(TSnTutorial* pScene)
    :TLyTuBase(pScene)
    , selectMidTeamID_(-1)
    , nextBG_(-1)
    , nextBGM_(-1)
    , ugHasebe_(this)
    , uglistTmFilter_(NULL)
    , ugdrTeam_(NULL)
    , pUGTeam_(NULL)
	, nowChange_(FALSE)
  {
  }

  //-----------------------------------------------------------------
  // デストラクタ
  //-----------------------------------------------------------------
  TLyTuTmSelect::~TLyTuTmSelect()
  {
    //Freemem(ugHasebe_);
    Freemem(uglistTmFilter_);
    Freemem(ugdrTeam_);
    Freemem(pUGTeam_);
  }

  //-----------------------------------------------------------------
  // ロード
  //-----------------------------------------------------------------
  void TLyTuTmSelect::DoLoad()
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::DoReady()
  {
    // 基底クラスのDoReadyを呼び出す
    base::DoReady();
    ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);		// ページ送りはタッチ
    ugserifu_.SetPos(-90, 30);	// 中心下辺り

    hasebeDt_.Init();

    // はせべ生成
    //ugHasebe_ = new TUGChGirl(this);
    ugHasebe_.MakeGroup(field_, 12, FALSE);
    ugHasebe_.SetDepth(UIDP_07BUTTONTOP);
    ugHasebe_.SetPos(0, 20);
    ugHasebe_.SetMirror(TRUE);
    ugHasebe_.SetMType(gmt_St);
    ugHasebe_.SetDraw(hasebeDt_.isReady_);

    // チームリスト
    teamVec_.clear();
    for (s32 i = 0; i < gl_Max; i++){ teamVec_.push_back(TEAM_LIST[i]); }

    // チーム用ドラムのフィルター
    uglistTmFilter_ = new TUGLiTmFilter(this);
    uglistTmFilter_->MakeGroup(field_, FALSE);

    // チーム選択用ドラム
    ugdrTeam_ = new TUGDrTeam(this);
    ugdrTeam_->MakeGroup(field_, TRUE, SIDE0, uglistTmFilter_);
    ugdrTeam_->SetDraw(FALSE);
    ugdrTeam_->SelectUra(FALSE);

    pUGTeam_ = new TUGTeam(this);
    pUGTeam_->MakeGroup(field_, SIDE0);
    pUGTeam_->SetDraw(FALSE);

    uglistTmFilter_->Ready(NGNUM);
    ugdrTeam_->Ready_Cnt(&teamVec_);

    //s64 mid_haikei = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei);
    //mid_haikei = 1;
    //pmgUI_->LoadCourt(mid_haikei);//背景

    pmgEO_->mgNSF_.PlayID(nsf_tuto);//BGM
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case pg_EnterHasebe:
      hasebeDt_.enterTimer_ = ENTER_HASEBE_TIME;
      hasebeDt_.isReady_ = TRUE;
      ugHasebe_.GetField()->g_SetAlp(FALSE, 0);
      break;

    case pg_Talk1:
      hasebeDt_.talkCategory_ = ht_Category1;
      ugHasebe_.SetMType(gmt_StTalk);
      SetMsg(HASEBE_TALK1[hasebeDt_.talkNo_][0], HASEBE_TALK1[hasebeDt_.talkNo_][1], HASEBE_TALK1[hasebeDt_.talkNo_][2]);
      ugserifu_.SetDraw(TRUE);
      break;

    case pg_Talk2:
      hasebeDt_.talkCategory_ = ht_Category2;
      ugHasebe_.SetMType(gmt_StTalk);
      ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);
      SetMsg(HASEBE_TALK2[hasebeDt_.talkNo_][0], HASEBE_TALK2[hasebeDt_.talkNo_][1], HASEBE_TALK2[hasebeDt_.talkNo_][2]);
      ugserifu_.SetDraw(TRUE);
      break;

    case pg_TeamSelect:
      ugserifu_.SetDraw(FALSE);
      ugdrTeam_->SetDraw(TRUE);
      pUGTeam_->SetDraw(FALSE);
      ugbtn_back_->SetFont(UGBTN_OK_TXT);

	  {
		  // 裏だったらおーけーボタンを押させない
		  BOOL isUra = ugdrTeam_->IsUra();
		  ugbtn_back_->SetDraw(isUra);
		  std::string s = isUra ? UGBTN_BACK_TXT : UGBTN_KWSK_TXT;
		  ugbtn_next_->SetFont(s.c_str());
	  }

      // はせべ邪魔じゃないところへ移動
	  ugHasebe_.z_MoveWork(0, 100 - 15, 20, 2, cmk_Left);
      hasebeDt_.isWalk_ = TRUE;
      // 一番最初に選んでいるチームの背景とBGMをセット
      //TeamSelect();
      break;

    case pg_TeamSelectCheck:
    {
      // 確認メッセージ
      // チーム名を取得してテキスト内へ
      std::string str = HASEBE_KAKUNIN[0];
      std::string name = pmgEO_->mgCoM_.mdm_Team_.GetLineDtStr(selectMidTeamID_, mdm_team_name_c32);

      std::size_t pos = str.find(TEAM_NAME_REPLACE);
      std::size_t size = std::string(TEAM_NAME_REPLACE).size();
      str.replace(pos, size, name);

      STRVECTOR strvec;
      strvec.clear();
      for (s32 i = 0; i < MESSAGE_LINE_MAX; i++)
      {
        if (i == 0)
        {
          strvec.push_back(str);
        }
        else
        {
          if (HASEBE_KAKUNIN[i] != NULL)
          {
            strvec.push_back(HASEBE_KAKUNIN[i]);
          }
        }
      }
      SetHudMsg(TRUE, FALSE, &strvec);
      ugbtn_back_->SetDraw(FALSE);
      ugbtn_next_->SetDraw(FALSE);

      // チームカードセット
      s64 teamMidID = ugdrTeam_->GetMIDTeam();
      s64 teamID = ugdrTeam_->GetIDTeam();
      pUGTeam_->SetItemCnt(teamMidID, FALSE);
      //pUGTeam_->MakeIconTexture();
      pUGTeam_->SetPos(-160, -40);
      pUGTeam_->SetDraw(TRUE);
      pUGTeam_->SetTipsDraw_f(TRUE);
      ugdrTeam_->SetDraw(FALSE);
    }
    break;

    default:
      break;
    }
    ugHasebe_.SetDraw(hasebeDt_.isReady_);
    stLy_.pageNo_ = pageno;
  }


  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_tutotmselect: Connect_TuTmSelect(); break;
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
  void TLyTuTmSelect::DoUpdate(BOOL draw_f)
  {
    base::DoUpdate(draw_f);

    ugHasebe_.Update(draw_f);
    uglistTmFilter_->Update(draw_f);
    ugdrTeam_->Update(draw_f);
    pUGTeam_->Update(draw_f);

    switch (stLy_.pageNo_)
    {
    case pg_Init:
      Init();
      break;

    case pg_EnterHasebe:
      EnterHasebe();
      break;

    case pg_Talk1:
    case pg_Talk2:
      Talk();
      break;

    case pg_TeamSelect:
      TeamSelect();
    case pg_TeamSelectCheck:
      TeamSelectCheck();
      break;

    default:
      break;
    }

#ifdef __K_DEBUG_ROOT__
    kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
    kdebug::DebugText::GetInstance()->SetDebugText(10, 30, "チーム選択");
#endif
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::Post_TuTmSelect()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_TuTmSelect();
    }
    else
    {
      // ここに送信処理を書く
      StartConnect(cn_tutotmselect);
      pmgEO_->mgPo_.poTuto_.Ready_TmSelect(selectMidTeamID_);
    }
  }


  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::Connect_TuTmSelect()
  {
    //DoChangePage(pg_Init);
    isTutoEnd_ = TRUE;
    //次のステップで指定する
    //pmgEO_->stShiai_.GetMidCourt() = nextBG_;
    //pmgEO_->mgStSn_.EtcSS.mid_bgm_ = nextBGM_;
    //pmgEO_->stShiai_.GetMidTeam(SIDE0) = selectMidTeamID_;	// プレイヤーチーム
    //pmgEO_->stShiai_.GetMidTeam(SIDE1) = ENEMY_TEAM_MID_ID;	// 敵チーム

#ifdef __K_DEBUG_USER_DATA__
	kdebug::DebugUDManager::GetInstance()->SetLastTeamID(selectMidTeamID_);
    kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_TmSelect);
#endif
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::Init(void)
  {
    ugdrTeam_->SetDraw(FALSE);
    ugfilter_.SetDraw(FALSE);
    ugbtn_back_->SetDraw(FALSE);
    ugbtn_next_->SetDraw(FALSE);
    ugserifu_.SetDraw(FALSE);

    DoChangePage(pg_EnterHasebe);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::EnterHasebe(void)
  {
    hasebeDt_.enterTimer_--;
    if (hasebeDt_.enterTimer_ < 0)
    {
      ugHasebe_.GetField()->g_SetAlp(TRUE, 255);
      hasebeDt_.talkNo_ = 0;
      DoChangePage(pg_Talk1);
      hasebeDt_.talkNo_++;
      return;
    }
    else
    {
      f32 ratio = (f32)((ENTER_HASEBE_TIME - hasebeDt_.enterTimer_) / ENTER_HASEBE_TIME);
      f32 alpha = 255 * ratio;
      ugHasebe_.GetField()->g_SetAlp(FALSE, alpha);
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::Talk(void)
  {
    // メッセージの次へが押された
    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      // オープニングトーク
      if (hasebeDt_.talkCategory_ == ht_Category1)
      {
        if (hasebeDt_.talkNo_ >= HASEBE_TALK1_MAX)
        {
          DoChangePage(pg_TeamSelect);
          return;
        }
        else
        {
          DoChangePage(pg_Talk1);
          hasebeDt_.talkNo_++;
        }
      }
      // チーム選択後のトーク
      else if (hasebeDt_.talkCategory_ == ht_Category2)
      {
        if (hasebeDt_.talkNo_ >= HASEBE_TALK2_MAX)
        {
          if (hasebeDt_.talkNo_ == HASEBE_TALK2_MAX)
          {
            ugfilter_.SetDraw(TRUE);
            ugfilter_.SetFilterDepth(db::enUIDepth::UIDP_10ALLFILTER);
            ugfilter_.OnFilter(ugfilter_fadeout2, FALSE, TRUE);

            // これをシーケンスに使うのは違う気がするけどとりあえず
            hasebeDt_.talkNo_++;
          }
          else
          {
            // フェードが終わったら切り替え
            if (ugfilter_.GetActRes() == ugfilter_fadeout2)
            {
              // ここまでを保存
              Post_TuTmSelect();
            }
          }
          return;
        }
        else
        {
          DoChangePage(pg_Talk2);
          hasebeDt_.talkNo_++;
        }
      }
    }
    // 喋り終わった
    else if (ugserifu_.GetActRes() == enUGSerifuAct_ReadEnd)
    {
      ugHasebe_.SetMType(gmt_St);
    }
  }


  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::TeamSelect(void)
  {
    if (ugdrTeam_->IsStop())
    {
      s32 midTeamID = ugdrTeam_->GetMIDTeam();
      ugbtn_back_->SetDraw(TRUE);
      // 現在選択中のBG、BGMを設定
      if (selectMidTeamID_ != midTeamID)
      {
        ChangeBG(midTeamID);	// 背景切り替え
        ChangeBGM(midTeamID);	// BGM切り替え

        selectMidTeamID_ = midTeamID;
		nowChange_ = TRUE;
      }
    }
    // フェードが終わったら切り替え
    if (ugfilter_.GetActRes() == ugfilter_fadeout2)
    {
      pmgUI_->LoadCourt(nextBG_);			// BG
      //pmgEO_->mgSound_.PlayBGM(nextBGM_);	// BGM
      pmgEO_->mgNSF_.PlayID(nextBGM_);//BGM
	  nowChange_ = FALSE;
    }
	// ロールされた
	if (ugdrTeam_->IsActRes(enUGDrTeamAct::enUGDrTeamAct_Roll))
	{
		if(!ugdrTeam_->IsUra())
		{
			ugbtn_next_->SetFont(UGBTN_KWSK_TXT);
		}
	}


    // はせべ歩いてるなら歩き終わるのチェックしておく
    if (hasebeDt_.isWalk_)
    {
      // 歩き終わった
      if (ugHasebe_.GetActRes() == enUGChGenAct_MoveStop)
      {
        hasebeDt_.isWalk_ = FALSE;
        ugserifu_.SetPos(20-15, 30);	// 右側下辺り
        ugserifu_.SetMsgChangeMode(enMsgChangeMode_None);
        ugserifu_.ChangeFrame(enSerifuFrame_Normal);
        SetMsg(HASEBE_INFO[0], HASEBE_INFO[1], HASEBE_INFO[2]);
        ugserifu_.SetDraw(TRUE);
        ugbtn_back_->SetDraw(TRUE);
        ugbtn_next_->SetDraw(TRUE);
      }
    }
    else
    {
      // くわしく
      if (ugbtn_next_->IsAct())
      {
        // 裏だったらおーけーボタンを押させない
		  ugdrTeam_->ura_.SetTeamUraDt(ugdrTeam_->GetMIDTeam());
		  ugdrTeam_->RevUra();
		  
		  BOOL isUra = ugdrTeam_->IsUra();
        ugbtn_back_->SetDraw(isUra);
		std::string s = isUra ? UGBTN_BACK_TXT : UGBTN_KWSK_TXT;
        ugbtn_next_->SetFont(s.c_str());
      }

      // おーけーが押された
      if (ugbtn_back_->IsAct() && !hasebeDt_.isWalk_ && !nowChange_)
      {
        ChangePage(pg_TeamSelectCheck);
      }
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::TeamSelectCheck(void)
  {
    // おーけーを押した
    if (IsHudOK())
    {
      hasebeDt_.talkCategory_ = ht_Category2;
      hasebeDt_.talkNo_ = 0;
      DoChangePage(pg_Talk2);
      hasebeDt_.talkNo_++;
      ugserifu_.SetPos(20-15, 30);	// 右側下辺り
    }
    // キャンセルを押した
    else if (IsHudCancel())
    {
      ChangePage(pg_TeamSelect);
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::ChangeBG(s32 midTeam)
  {
    ugfilter_.SetDraw(TRUE);
    ugfilter_.SetFilterDepth(db::enUIDepth::UIDP_01BGOBJ);	// フェードは背景より1個前
    ugfilter_.OnFilter(ugfilter_fadeout2, FALSE, TRUE);

//#ifdef __BG_TABLE_MODE__
    s32 no = 0;
    for (s32 i = 0; i < gl_Max; i++)
    {
      if (TEAM_LIST[i] == midTeam)
      {
        no = i;
        break;
      }
    }
    nextBG_ = BG_LIST[no];
//#else
//    nextBG_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(midTeam, mdm_team_id_m_haikei);
//#endif
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTmSelect::ChangeBGM(s32 midTeam)
  {
//#ifdef __BGM_TABLE_MODE__
    s32 no = 0;
    for (s32 i = 0; i < gl_Max; i++)
    {
      if (TEAM_LIST[i] == midTeam)
      {
        no = i;
        break;
      }
    }
    nextBGM_ = BGM_LIST[no];
//#else
//    nextBGM_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(midTeam, mdm_team_id_m_bgm);
//#endif
    pmgEO_->mgSound_.StopBGM();
  }
}
