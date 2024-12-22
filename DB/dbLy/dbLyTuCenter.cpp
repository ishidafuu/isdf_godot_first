//utf8
#include "dbLyTuCenter.h"
#include "dbSnTutorial.h"
#include "dbUGSerifu.h"
//=========================================================================
// 
// チュートリアル：ショップ画面
//
//=========================================================================
namespace db
{
  enum enPage
  {
    pg_Init,
    pg_CapTalk1,

    pg_Walking,
    pg_Selected,
    pg_InDock,
    pg_OutDock,
	pg_OutDock2,
    pg_CapTalk2,
    pg_GotoHome,
    pg_End,
  };

  enum enDest
  {
    ds_Init = 0,
    ds_BBA,
    ds_Maruka,
    ds_dock1,
    ds_dock2,
    ds_dock3,
    ds_dock4,

    ds_MAX,
  };
  const s32  DOCKNUM = 4;

  //const s32 DOORBY = 144;
  //const s32 DOORBX[] = { 88, 232, 366 };

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
  const s32 CAPTAIN_CT_TALK1_MAX = 2;
  const char* CAPTAIN_CT_TALK1[CAPTAIN_CT_TALK1_MAX][MESSAGE_LINE_MAX] =
  {
    {
      "$CAP_NAME$「みぎにある＿たてものが",
      "ミックスしゅーとけんきゅうじょ＿だ",
    },
    {
      "$CAP_NAME$「いちばん＿うえのかいの",
      "けんきゅうしつに＿いこうぜ！",
    },
  };

  // トークカテゴリー2
  enum CAPTAIN_CT_TALK2
  {
	  CAPTAIN_CT_TALK2_BBA = 2,
	  CAPTAIN_CT_TALK2_MARUKA = 4,
	  CAPTAIN_CT_TALK2_SOUKO = 6,
	  CAPTAIN_CT_TALK2_MAX = 9,
  };
  //const s32 CAPTAIN_CT_TALK2_MAX = 9;
  const char* CAPTAIN_CT_TALK2[CAPTAIN_CT_TALK2_MAX][MESSAGE_LINE_MAX] =
  {
	  {
		  "$CAP_NAME$「そういえば＿ここには",
		  "ミックスけんきゅうじょ＿いがいに",
	  },
	  {
		  "$CAP_NAME$「しゅーとおばば＿と",
		  "しゅーとあずかり＿きのうがあるぜ",
	  },
	  {	 // ●指先おばば
		"$CAP_NAME$「しゅーとおばば＿は",
		"よわい＿そざいだまの＿はんばいや",
	  },
	  {
		  "$CAP_NAME$「しゅーとに＿かんけいする",
		  "てだすけを＿してくれるぜ",
	  },
	  {	// ●指さしあずかり
		  "$CAP_NAME$「しゅーとあずかり＿の",
		  "あずかりきかんは＿２しゅうかん黙黙",
	  },
	  {
		  "$CAP_NAME$「２しゅうかんの＿あいだに",
		  "うけとらないと＿なくなるぜ",
	  },
	  { // ●指さしそうこ
		  "$CAP_NAME$「しゅーと＿かくとくじ＿に",
		  "そうこが＿いっぱいだった＿ばあい",
	  },
	  {
		  "$CAP_NAME$「しゅーとあずかりに",
		  "おくられるから＿きを＿つけろよ！",
	  },
	{
		"$CAP_NAME$「よしこれで＿じゆうだな！",
		"まずは＿しあいを＿やろうぜ！",
	},
  };


  const s32 TUC_START_PX = 97 + 18;
  const s32 TUC_START_PY = 158 + 35;


  //-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
  TLyTuCenter::TLyTuCenter(TSnTutorial* pScene)
    :TLyTuBase(pScene)
    , ugkantoku_(this)
    , ugcap_(this)
    , ugbln_bba_(this)
    , ugbln_maruka_(this)
    , ugcl_maruka_(this)
    //, ugcl_bba_(this)
    , ugdoor_bba_(this)
    , ugsoukonum_inv_(this)
    , ugsoukonum_have_(this)
    , isLockPage_(FALSE)
  {
    for (s32 i = 0; i < DOCKNUM; ++i)
    {
      ugdock_.push_back(new TUGDock(this));
    }
    stCenter_.Init();
  }

  //-----------------------------------------------------------------
  // デストラクタ
  //-----------------------------------------------------------------
  TLyTuCenter::~TLyTuCenter()
  {
    for (s32 i = 0; i < DOCKNUM; ++i)
    {
      Freemem(ugdock_[i]);
    }
  }

  //-----------------------------------------------------------------
  // ロード
  //-----------------------------------------------------------------
  void TLyTuCenter::DoLoad()
  {
    ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);

    //合成ドック
    for (s32 i = 0; i < DOCKNUM; ++i)
    {
      ugdock_[i]->MakeGroup(field_, tugrp_->center_shutter_, tugrp_->center_dockdoor_, i);
    }
    RefreshDock();

    ugcl_maruka_.MakeGroup(field_, ct_Maruka);
    ugcl_maruka_.SetPos_Full(97, 160);

    //ugcl_bba_.MakeGroup(field_, ct_BBA);
    //ugcl_bba_.SetDepth(UIDP_01BGOBJ);
    //ugcl_bba_.SetPos_Full(197, 124);

    //ドア
    ugdoor_bba_.MakeGroup(field_, tugrp_->center_bbadoor_, 187 - mid::midFullDotL_Hf(), 96 - mid::midFullDotS_Hf());
    ugdoor_bba_.GetField()->g_SetRevDepthLv(1);
    //ugdoor_bba_.SetDraw(FALSE);

    ////バルーン
    ugbln_bba_.MakeGroup_Balloon(field_, "しゅーとおばば");
    ugbln_bba_.SetPos_Full(172, 98);
	ugbln_bba_.SetDraw(FALSE);

    ugbln_maruka_.MakeGroup_Balloon(field_, "あずかり");
    ugbln_maruka_.SetPos_Full(92, 98);
	ugbln_maruka_.SetDraw(FALSE);

    ugsoukonum_have_.MakeGroup(field_, FALSE);
    ugsoukonum_have_.SetPos_Full(76, 70);
    ugsoukonum_inv_.MakeGroup(field_, TRUE);
    ugsoukonum_inv_.SetPos_Full(116, 70);

    //トラック
    track_ = MakeObj();
    track_->o_SetParent(field_);
    track_->o_SetObjGrp(tugrp_->center_track_);
    track_->z_SetPos_Full(0, 112 - 16);

    pole_ = MakeObj();
    pole_->o_SetParent(field_);
    pole_->o_SetObjGrp(tugrp_->center_pole_);
    pole_->z_SetPos_Full(171, 56);


    s32 posX = 97 + 18;
    s32 posY = 158 + 35;

    //-------------------------------------------------------
    // 監督作成
    ugkantoku_.MakeGroup(field_, FALSE);
	ugkantoku_.SetPos_Full(TUC_START_PX, TUC_START_PY);
    s32 limX = -mid::midGetDotL_Hf();
    s32 limW = mid::midGetDotL_Hf();
    ugkantoku_.GetField()->g_SetLimX(limX, limW);
	ugkantoku_.SetMirror(TRUE);
    //-------------------------------------------------------
    // キャプテン作成
    captainDt_.Init();
    ugcap_.SetupMgCharGrp();
    ugcap_.MakeGroup(field_);
	ugcap_.SetPos_Full(TUC_START_PX - 26, TUC_START_PY);
    ugcap_.SetKantoku(&ugkantoku_);
    ugcap_.SetWalkType(cwt_FOLLOW);
    ugcap_.GetField()->g_SetLimX(limX, limW);
    ugcap_.SetMirror(FALSE);

  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuCenter::DoReady()
  {
    // 基底クラスのDoReadyを呼び出す
    base::DoReady();
    ugserifu_.ChangeFrame(enSerifuFrame_Normal);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuCenter::DoChangePage(s32 pageno)
  {
    s32 no = pageno;
    RefreshSoukoNum();
    if (isLockPage_)
    {
      // 自動で呼ばれるChangePageでは処理をしない
      return;
    }


    switch (no)
    {
    case pg_Init:
      break;
    case pg_CapTalk1:
      captainDt_.talkCategory_ = ct_Category1;
      ugcap_.SetMType(cmt_St);
      SetMsg(CAPTAIN_CT_TALK1[captainDt_.talkNo_][0], CAPTAIN_CT_TALK1[captainDt_.talkNo_][1], CAPTAIN_CT_TALK1[captainDt_.talkNo_][2]);
      captainDt_.talkNo_++;
      ugserifu_.SetDraw(TRUE);
      ugserifu_.SetPos(-160, -30);
      break;
    case db::pg_Walking:
      ugfinger_.SetMotionTouch(128, -56);
      ugfinger_.SetDraw(TRUE);
      ugcap_.SetWalkType(cwt_FOLLOW);
      SetDockWait(FALSE);
      break;
    case db::pg_Selected:
      //SetFoot_None();
      ugfinger_.SetDraw(FALSE);
      for (s32 i = 0; i < DOCKNUM; ++i)
      {
        ugdock_[i]->ugbtn_.SetDraw(FALSE);
      }
      break;
    case db::pg_InDock:
      SetDockWait(TRUE);
      isLockPage_ = TRUE;
      break;
    case db::pg_OutDock:
    {
      s32 limX = -mid::midGetDotL_Hf();
      s32 limW = mid::midGetDotL_Hf();
      ugkantoku_.SetParent(field_);
      ugkantoku_.SetPos_Full(224 + 40 + 20, 48 + (3 * 40) + 40);
      ugkantoku_.SetDraw(TRUE);
	  ugkantoku_.z_MoveWork_Full(ds_Init, TUC_START_PX, TUC_START_PY, 2);
      ugkantoku_.GetField()->g_SetLimX(limX, limW);
      ugcap_.SetParent(field_);
      ugcap_.SetPos_Full(224 + 2 + 20, 48 + (3 * 40) + 40);
      ugcap_.SetDraw(TRUE);
      ugcap_.SetWalkType(cwt_FOLLOW);
      ugcap_.GetField()->g_SetLimX(limX, limW);
      break;
    }
	case db::pg_OutDock2:
		ugcap_.SetWalkType(cwt_MYWALK);
		ugkantoku_.GetField()->g_SetLimX(0, 0);
		ugcap_.z_MoveWork_Full(ds_Init, TUC_START_PX - 26, TUC_START_PY, 2);
		break;
    case pg_CapTalk2:
		ugcap_.GetField()->g_SetLimX(0, 0);
      captainDt_.talkCategory_ = ct_Category2;
      ugcap_.SetMType(cmt_St);
      SetMsg(CAPTAIN_CT_TALK2[captainDt_.talkNo_][0], CAPTAIN_CT_TALK2[captainDt_.talkNo_][1], CAPTAIN_CT_TALK2[captainDt_.talkNo_][2]);
      ugserifu_.SetDraw(TRUE);
      ugserifu_.SetPos(-50, 20);
	  // 指さし
	  {
		  f32 px = 0;
		  f32 py = 0;
		  switch (captainDt_.talkNo_)
		  {
			 case CAPTAIN_CT_TALK2_BBA:
				 ugbln_bba_.SetDraw(TRUE);
				 ugbln_bba_.GetField()->z_GetPos(&px, &py);
				 ugfinger_.SetMotionLookPoint(px+30, py+20);
				 ugfinger_.SetDraw(TRUE);
			 case CAPTAIN_CT_TALK2_BBA+1:
				 break;

			 case CAPTAIN_CT_TALK2_MARUKA:
				 ugbln_bba_.SetDraw(FALSE);
				 ugbln_maruka_.SetDraw(TRUE);
				 ugbln_maruka_.GetField()->z_GetPos(&px, &py);
				 ugfinger_.SetMotionLookPoint(px+30, py+10);
				 ugfinger_.SetDraw(TRUE);
			 case CAPTAIN_CT_TALK2_MARUKA+1:
				 break;

			 case CAPTAIN_CT_TALK2_SOUKO:
				 ugbln_bba_.SetDraw(FALSE);
				 ugbln_maruka_.SetDraw(FALSE);
				 ugsoukonum_inv_.GetField()->z_GetPos(&px, &py);
				 ugfinger_.SetMotionLookPoint(px+30, py+10);
				 ugfinger_.SetDraw(TRUE);
			 case CAPTAIN_CT_TALK2_SOUKO+1:
				 break;

			 default:
				 ugfinger_.SetDraw(FALSE);
				 ugbln_bba_.SetDraw(FALSE);
				 ugbln_maruka_.SetDraw(FALSE);
				 break;
		  }
	  }

	  captainDt_.talkNo_++;
      break;

    case pg_GotoHome:
      break;

    case pg_End:
      pScene_->lyTuBar_->GotoTab(TLyTuBar::tab_End);

#ifdef __K_DEBUG_USER_DATA__
      kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_Kantoku);
#endif
      break;
    }
    stLy_.pageNo_ = pageno;
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuCenter::DoUpdate(BOOL draw_f)
  {
    base::DoUpdate(draw_f);

    track_->Update(draw_f);//トラック
    pole_->Update(draw_f);//ポール
    ugdoor_bba_.Update(draw_f);
	ugbln_bba_.Update(draw_f);
	ugbln_maruka_.Update(draw_f);
    ugsoukonum_have_.Update(draw_f);
    ugsoukonum_inv_.Update(draw_f);
    switch (stLy_.pageNo_)
    {
    case db::pg_InDock:
      //ugdock_は中で管理
      break;
    case db::pg_Selected:
      ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      ugcl_maruka_.Update(draw_f);
      //ugcl_bba_.Update(draw_f);
      for (s32 i = 0; i < DOCKNUM; ++i) ugdock_[i]->Update(draw_f);
      break;
    default:
      ugdoor_bba_.Update(draw_f);
      ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      ugcl_maruka_.Update(draw_f);
      for (s32 i = 0; i < DOCKNUM; ++i) ugdock_[i]->Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      //if (pScene_->lyHmBar_->IsBarAction()) return;
      switch (stLy_.pageNo_)
      {
      case db::pg_Init:MvPage_Init(); break;
      case db::pg_CapTalk1:MvPage_CapTalk(); break;
      case db::pg_Walking:MvPage_Walking(); break;
      case db::pg_Selected:MvPage_Selected(); break;
      case db::pg_InDock:MvPage_InDock(); break;
      case db::pg_OutDock:MvPage_OutDock(); break;
	  case db::pg_OutDock2:MvPage_OutDock2(); break;
      case db::pg_CapTalk2:MvPage_CapTalk2(); break;
      case db::pg_GotoHome:MvPage_GotoHome(); break;
      }
    }

#ifdef __K_DEBUG_ROOT__
    kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
    kdebug::DebugText::GetInstance()->SetDebugText(10, 30, "ホーム説明");
#endif
  }

  // ドックから出る時に呼んでもらう
  void TLyTuCenter::OutDock(void)
  {
    isLockPage_ = FALSE;
    ChangePage(pg_OutDock);
  }
  void TLyTuCenter::SetTutoEnd()
  {
    isTutoEnd_ = TRUE;
  }
  void TLyTuCenter::RefreshSoukoNum()
  {
    //倉庫表示リフレッシュ
    ugsoukonum_have_.Refresh();
    ugsoukonum_inv_.Refresh();
  }
  void TLyTuCenter::SetDockWait(BOOL wait_f)
  {
    for (s32 i = 0; i < DOCKNUM; ++i)
    {
      if (stCenter_.dockNo_ == i) continue;
      ugdock_[i]->SetWait(wait_f);
    }
  }
  //ドック更新
  void TLyTuCenter::RefreshDock()
  {
    s32 opendocknum = pmgEO_->mgCoU_.mdDock_.GetLineNum();
    if (pmgEO_->stSeq_.IsNoSv())  opendocknum = 2;

    for (s32 i = 0; i < DOCKNUM; ++i)
    {
      if (i < opendocknum)
      {
        ugdock_[i]->SetEnable(pmgEO_->mgCoU_.mdDock_.GetLineDt(i, mddock_pk), FALSE);
      }
      else
      {
        ugdock_[i]->SetDisable((i == opendocknum));
      }

      if (i != 0) ugdock_[i]->ugbtn_.SetDraw(FALSE);
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuCenter::MvPage_Init(void)
  {
    captainDt_.Init();
    ChangePage(pg_CapTalk1);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuCenter::MvPage_CapTalk(void)
  {
    // メッセージの次へが押された
    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      //ガチャやに行こう
      if (captainDt_.talkNo_ >= CAPTAIN_CT_TALK1_MAX)
      {
        ugserifu_.SetDraw(FALSE);
        ChangePage(pg_Walking);
        //ChangePage(pg_End);
        captainDt_.talkNo_ = 0;
        return;
      }
      else
      {
        ChangePage(pg_CapTalk1);
      }
    }
    // 喋り終わった
    else if (ugserifu_.GetActRes() == enUGSerifuAct_ReadEnd)
    {
      ugcap_.SetMType(cmt_St);
    }
  }

  void TLyTuCenter::MvPage_Walking()
  {
    //ドック表示管理
    for (s32 i = 0; i < DOCKNUM; ++i) ugdock_[i]->MoveState();

    //#ifdef __K_DEBUG_USER_DATA__
    //    // ローカル環境はデバッグのフラグを見る
    //    if (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Gousei))
    //#else
    //    if ((pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Gousei) == FALSE) && (mid::midIsDXL() == FALSE))
    //#endif
    //    {
    //      ChangePage(pg_TutoGousei);
    //    }
    //    else
    {
      for (s32 i = 0; i < DOCKNUM; ++i)
      {
        if (ugdock_[i]->IsAct())
        {
          ugkantoku_.z_MoveWork_Full(ds_dock1 + i, 224 + 40 + 20, 48 + (3 * 40) + 40, 2, 224 + 40 + 1);
          ChangePage(pg_Selected);
        }
      }
    }
  }
  void TLyTuCenter::MvPage_Selected()
  {
    if (!ugkantoku_.IsActRes(enUGChGenAct_MoveStop)){ return; }
    ChangePage(pg_InDock);
    pScene_->lyTuCDock_->ReadyDock(ugdock_[stCenter_.dockNo_], stCenter_.dockNo_);
  }
  void TLyTuCenter::MvPage_InDock()
  {

  }
  void TLyTuCenter::MvPage_OutDock()
  {
    if (!ugkantoku_.IsActRes(enUGChGenAct_MoveStop)){ return; }
    //ChangePage(pg_Walking);
    //SetFoot_Tab();//ヘッダフッタはもどさない
    ChangePage(pg_OutDock2);
  }
  void TLyTuCenter::MvPage_OutDock2()
  {
	  if (!ugcap_.IsActRes(enUGChGenAct_MoveStop)){ return; }
	  //ChangePage(pg_Walking);
	  //SetFoot_Tab();//ヘッダフッタはもどさない
	  ugkantoku_.SetMirror(TRUE);
	  ugcap_.SetMirror(FALSE);
	  ChangePage(pg_CapTalk2);
  }
  void TLyTuCenter::MvPage_CapTalk2(void)
  {
    // メッセージの次へが押された
    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      //シュートセンターにいこう
      if (captainDt_.talkNo_ >= CAPTAIN_CT_TALK2_MAX)
      {
        ugserifu_.SetDraw(FALSE);
        //ChangePage(pg_GotoHome);
        ChangePage(pg_End);
        return;
      }
      else
      {
        ChangePage(pg_CapTalk2);
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
  void TLyTuCenter::MvPage_GotoHome(void)
  {
    if (field_->sc_IsCamEnter())
    {
      ChangePage(pg_End);
    }

    //if (pScene_->lyTuBar_->ugfooter_.tabbtn_[3]->IsAct())
    //{
    //  pScene_->lyTuBar_->ugfooter_.ugradio_.SetDraw(FALSE);
    //  ugfinger_.SetDraw(FALSE);
    //  ChangePage(pg_End);
    //}
  }
}
