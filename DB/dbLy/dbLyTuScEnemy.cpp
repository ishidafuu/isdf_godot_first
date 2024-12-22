#include "dbLyTuScEnemy.h"
#include "dbSnScenario.h"
#include "dbUGSerifu.h"
#include "dbUGFinger.h"

namespace db
{
  const s32 TMNUM = 3;

  enum enPage
  {
    pg_TutoEnemyTxt,	// 敵の説明
    pg_TutoItemTxt,	// ドロップアイテムの説明
    pg_TutoBattleBtnTxt,	// たいけつボタンの説明
    pg_TutoHPTxt,		// HPの説明
    pg_TutoNeedHPTxt,	// 必要なHPの説明

    pg_Enemy,
    pg_GotoTSL,
    pg_Idle,
  };

  //-------------------------------------------------
  // チュートリアルテキスト
  enum enTutoEnemyTxtType
  {
    tett_INIT = 0,
    //ttt_TUTO_STAGE,
    tett_MAX = 6,
  };
  const s32 MSG_LINE_MAX = 3;
  const char* TUTO_MSG_ENEMY[tett_MAX][MSG_LINE_MAX] =
  {
    {
      "掛「このマップで＿しゅつげんする",
      "チームが＿ひょうじされます",
    },
    {
      "掛「ドロップアイテムの＿コンプで",
	  "さらに＿アイテムが＿もらえます",
    },
    {
      "掛「たいけつ！を＿タッチすると",
      "あいてチームが＿けっていします",
    },
    {
      "掛「けっていすると＿ＨＰがへります",
      "ＨＰは＿じかんで＿かいふくします",
    },
    {
      "掛「たいけつに＿ひつような＿ＨＰは",
      "こちらを＿かくにんして＿ください",
    },
    {
      "掛「では＿たいけつ！＿しましょう心",
    },
  };

  enum enConnect
  {
    cn_ready,
  };

  //-----------------------------------------------------------------
  //コンストラクタ
  //-----------------------------------------------------------------
  TLyTuScEnemy::TLyTuScEnemy(TSnScenario* pScene)
    :TLyScGen(pScene)
    , pUGFinger_(NULL)
    , pUGFinger2_(NULL)
    , pUGFinger3_(NULL)
    , pUGSerifu_(NULL)
    , tuTxtNo_(0)
  {
    for (s32 i = 0; i < TMNUM; ++i) ugenteam_.push_back(new TUGScEnemy(this));
  }
  //-----------------------------------------------------------------
  //デストラクタ
  //-----------------------------------------------------------------
  TLyTuScEnemy::~TLyTuScEnemy()
  {
    for (s32 i = 0; i < TMNUM; ++i) Freemem(ugenteam_[i]);
  }
  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::DoLoad()
  {
    field_->g_SetDepth(UIDP_02BGOBJHI);//手前

    filter_ = MakeObj();
    filter_->o_SetParent(field_);
    filter_->o_SetObjFilterGrp(mid::midFullDotL_Hf(), mid::midFullDotS(), 0x80, 0, 0, 0);
    filter_->g_SetARGB(TRUE, 0x80, 0x00, 0x00, 0x00);
    filter_->z_SetPos_SafeR(0, 0);

    //チーム情報（ドロップなど）
    for (s32 i = 0; i < TMNUM; ++i)
    {
      ugenteam_[i]->MakeGroup(field_, i);
    }
  }
  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::DoResetBtn()
  {
    SetFoot_BtnHd();
    ugbtn_ok_->SetFont("たいけつ！");
    ugbtn_ok_->SetDraw(FALSE);
  }
  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::DoReady()
  {
    // 使うものを取得しておく
    pUGSerifu_ = pScene_->lyTuScBar_->GetSerifuWindow();
    pUGFinger_ = pScene_->lyTuScBar_->GetFinger();
    pUGFinger2_ = pScene_->lyTuScBar_->GetFinger2();
    pUGFinger3_ = pScene_->lyTuScBar_->GetFinger3();
    pSerifuVec_ = pScene_->lyTuScBar_->GetSerifuVec();

    pUGSerifu_->SetDraw(FALSE);
    pUGFinger_->SetDraw(FALSE);
    pUGFinger2_->SetDraw(FALSE);
    pUGFinger3_->SetDraw(FALSE);

    // チュートリアルなのでヘッダーやフッダーはロックをかける
    pScene_->pmgUI_->SetModal(TRUE);
  }
  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::RedayEnemy(s64 mid_maptip, s32 lno_maptip)
  {
    Ready();
    //雑魚画像作成用ベクタのクリア
    pmgEO_->mgGrp_.ClearZakoSt();
    for (s32 i = 0; i < TMNUM; ++i)
    {
      ugenteam_[i]->Ready(mid_maptip, lno_maptip);//敵１のときは消すような処理を入れる
    }
    pmgEO_->mgGrp_.MakeZakoTexture();//テクスチャ作成
    ResetBtn();

	ugbtn_ok_->SetModal(TRUE);
	ugbtn_ok_->SetDraw(TRUE);
	ugbtn_ok_->GetField()->b_SetBtn(FALSE);
  }


  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScEnemy::SetMsg(const s32 msgNo)
  {
    SetMsg(TUTO_MSG_ENEMY[msgNo][0], TUTO_MSG_ENEMY[msgNo][1], TUTO_MSG_ENEMY[msgNo][2]);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuScEnemy::SetMsg(const char* text0, const char* text1, const char* text2)
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
  void TLyTuScEnemy::MvPage_TutoEnemyTxt(void)
  {
    if (pUGSerifu_->GetActRes() == enUGSerifuAct_WaitEnd)
    {
      ChangePage(pg_TutoItemTxt);
    }
  }
  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::MvPage_TutoItemTxt(void)
  {
    if (pUGSerifu_->GetActRes() == enUGSerifuAct_WaitEnd)
    {
      ChangePage(pg_TutoBattleBtnTxt);
    }
  }
  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::MvPage_TutoBattleBtnTxt(void)
  {
    if (pUGSerifu_->GetActRes() == enUGSerifuAct_WaitEnd)
    {
      ChangePage(pg_TutoHPTxt);
    }
  }
  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::MvPage_TutoHPTxt(void)
  {
    if (pUGSerifu_->GetActRes() == enUGSerifuAct_WaitEnd)
    {
      ChangePage(pg_TutoNeedHPTxt);
    }
  }
  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::MvPage_TutoNeedHPTxt(void)
  {
    if (pUGSerifu_->GetActRes() == enUGSerifuAct_WaitEnd)
    {
      ChangePage(pg_Enemy);
    }
  }
  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::MvPage_Enemy()
  {
    //キャンセルボタンによって選択番号のマップを中心にするように移動、
    //パネル選択状態に状態遷移

    if (ugbtn_ok_->IsAct())
    {
      ugbtn_ok_->SetModal(FALSE);//もどす
      pUGFinger_->ClearMotion();
      pUGFinger_->hand_->g_SetMirror(FALSE);
      pUGFinger_->SetRevUD(FALSE);
      pUGFinger_->SetDraw(FALSE);
      Post_Ready();
    }
  }
  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::MvPage_GotoTSL()
  {

  }

  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::MvPage_Idle()
  {

  }

  //-----------------------------------------------------------------
  //POST：対決！
  //-----------------------------------------------------------------
  void TLyTuScEnemy::Post_Ready()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Ready();//テスト★
    }
    else
    {
      StartConnect(cn_ready);
      pmgEO_->mgPo_.poTuto_.Ready_ScReady(pmgEO_->stSeq_.GetMidMapTip());
    }
  }
  //-----------------------------------------------------------------
  //接続：敵選択完了
  //-----------------------------------------------------------------
  void TLyTuScEnemy::Connect_Ready()
  {
    ChangePage(pg_GotoTSL);
    pScene_->lyTuScBar_->MoveLayer(tscl_TSL);
  }

  //-----------------------------------------------------------------
  //接続完了
  //-----------------------------------------------------------------
  void TLyTuScEnemy::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_ready: Connect_Ready(); break;
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
  void TLyTuScEnemy::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case db::pg_TutoEnemyTxt:
    {
      SetMsg(tuTxtNo_++);
      pUGSerifu_->SetMsgChangeMode(enMsgChangeMode_Touch);
      pUGSerifu_->SetPos(-170 + 15, -72);
      pUGSerifu_->SetDraw(TRUE);

      // 指、敵3チームのところへ動く
      pUGFinger_->GetField()->z_SetPos(-10, -36);
      pUGFinger2_->GetField()->z_SetPos(-10, 5);
      pUGFinger3_->GetField()->z_SetPos(-10, 45);
      pUGFinger_->hand_->g_SetMirror(TRUE);
      pUGFinger2_->hand_->g_SetMirror(TRUE);
      pUGFinger3_->hand_->g_SetMirror(TRUE);
      pUGFinger_->SetRevUD(FALSE);
      pUGFinger2_->SetRevUD(FALSE);
      pUGFinger3_->SetRevUD(FALSE);
      pUGFinger_->SetDraw(TRUE);
      pUGFinger2_->SetDraw(TRUE);
      pUGFinger3_->SetDraw(TRUE);
    }
    break;

    case db::pg_TutoItemTxt:
    {
      SetMsg(tuTxtNo_++);
      pUGSerifu_->SetDraw(TRUE);

      // 指、ドロップアイテムのところへ
      pUGFinger_->SetMotionMoveLine(25, 45, 100, 45);
      pUGFinger_->hand_->g_SetMirror(TRUE);
      pUGFinger_->SetRevUD(FALSE);
      pUGFinger_->SetDraw(TRUE);

      pUGFinger2_->hand_->g_SetMirror(FALSE);
      pUGFinger3_->hand_->g_SetMirror(FALSE);
      pUGFinger2_->SetDraw(FALSE);
      pUGFinger3_->SetDraw(FALSE);
    }
    break;

    case pg_TutoBattleBtnTxt:
      SetMsg(tuTxtNo_++);
      pUGSerifu_->SetDraw(TRUE);

      // 指、たいけつボタンのところへ
      pUGFinger_->ClearMotion();
      pUGFinger_->GetField()->z_SetPos(7, 55);
      pUGFinger_->hand_->g_SetMirror(TRUE);
      pUGFinger_->SetRevUD(TRUE);
      pUGFinger_->SetDraw(TRUE);
      break;

    case pg_TutoHPTxt:
      SetMsg(tuTxtNo_++);
      pUGSerifu_->SetDraw(TRUE);

      // 指、ヘッダーのかんとくのHPのところへ
      pUGFinger_->SetMotionMoveLine(30, -85, -10, -85);
      pUGFinger_->hand_->g_SetMirror(FALSE);
      pUGFinger_->SetRevUD(FALSE);
      pUGFinger_->SetDraw(TRUE);
      break;

    case pg_TutoNeedHPTxt:
      SetMsg(tuTxtNo_++);
      pUGSerifu_->SetDraw(TRUE);

      // 指、ステージパネルの必要HPのところへ
      pUGFinger_->ClearMotion();
      pUGFinger_->GetField()->z_SetPos(-16, 22);
      pUGFinger_->hand_->g_SetMirror(FALSE);
      pUGFinger_->SetDraw(TRUE);
      break;

    case db::pg_Enemy:
      SetMsg(tuTxtNo_++);
      ugbtn_ok_->SetDraw(TRUE);
      pUGSerifu_->SetMsgChangeMode(enMsgChangeMode_None);
      pUGSerifu_->SetDraw(TRUE);

      // ヘッダーやフッダーのロック解除
      //pScene_->pmgUI_->SetModal(FALSE);
      //ugbtn_ok_->SetModal(TRUE);
	  ugbtn_ok_->GetField()->b_SetBtn(TRUE);

      pUGFinger_->SetMotionLookPoint(7, 55);
      pUGFinger_->hand_->g_SetMirror(TRUE);
      pUGFinger_->SetRevUD(TRUE);
      pUGFinger_->SetDraw(TRUE);

      break;

    default:
      break;
    }
  }

  //-----------------------------------------------------------------
  //
  //-----------------------------------------------------------------
  void TLyTuScEnemy::DoUpdate(BOOL draw_f)
  {
    filter_->Update(draw_f);

    //ugbtn_ok_->Update(draw_f);
    for (s32 i = 0; i < TMNUM; ++i) ugenteam_[i]->Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_TutoEnemyTxt:MvPage_TutoEnemyTxt(); break;
      case pg_TutoItemTxt:MvPage_TutoItemTxt();	break;
      case pg_TutoBattleBtnTxt:MvPage_TutoBattleBtnTxt();	break;
      case pg_TutoHPTxt:MvPage_TutoHPTxt(); break;
      case pg_TutoNeedHPTxt:MvPage_TutoNeedHPTxt(); break;

      case pg_Enemy:MvPage_Enemy(); break;
      case pg_GotoTSL:MvPage_GotoTSL(); break;
      case pg_Idle:MvPage_Idle(); break;
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
  }
}
