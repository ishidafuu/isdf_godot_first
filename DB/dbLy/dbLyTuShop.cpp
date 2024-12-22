//utf8
#include "dbLyTuShop.h"
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
    pg_InShop,
    pg_GotoShop,
    pg_OutShop,

    pg_CapTalk2,
    pg_GotoCenter,
    pg_End,
  };

  enum enConnect
  {
    cn_tutoShop,
  };

  const s32 DOORBY = 144;
  const s32 DOORBX[] = { 88, 232, 366 };

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
  const s32 CAPTAIN_SP_TALK1_MAX = 1;
  const char* CAPTAIN_SP_TALK1[CAPTAIN_SP_TALK1_MAX][MESSAGE_LINE_MAX] =
  {
    {
      "$CAP_NAME$「まず＿ガチャを＿まわしに",
      "いこうぜ＿ほかのみせは＿あとだ",
    },
  };

  // トークカテゴリー2
  const s32 CAPTAIN_SP_TALK2_MAX = 2;
  const char* CAPTAIN_SP_TALK2[CAPTAIN_SP_TALK2_MAX][MESSAGE_LINE_MAX] =
  {
    {
      "$CAP_NAME$「オレたち＿いがいにも",
      "あたらしい＿なかまが＿みつかったな",
    },
    {
      "$CAP_NAME$「おっ＿さいごのこうしゅう",
      "しゅーとミックスの＿じかんだぜ",
    },
  };

	//-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
  TLyTuShop::TLyTuShop(TSnTutorial* pScene)
    :TLyTuBase(pScene)
    , ugkantoku_(this)
    , ugcap_(this)
    //, ugbln_item_(this)
    , ugbln_gacha_(this)
    //, ugbln_huku_(this)
    , editErrCnt_(0)
    , subPage_(0)
  {
    stHmShop_.Init();
    for (s32 i = 0; i < SHOPNUM; ++i) ugdoor_.push_back(new TUGDoor(this));
  }

  //-----------------------------------------------------------------
  // デストラクタ
  //-----------------------------------------------------------------
  TLyTuShop::~TLyTuShop()
  {
    for (s32 i = 0; i < SHOPNUM; ++i) Freemem(ugdoor_[i]);
  }

  //-----------------------------------------------------------------
  // ロード
  //-----------------------------------------------------------------
  void TLyTuShop::DoLoad()
  {
    ////監督
    //ugkantoku_.MakeGroup(field_);
    //ugkantoku_.SetPos_Full(97 + 18, 158 + 35);

    //ugcap_.MakeGroup(field_);	// キャプテン
    //ugcap_.SetPos_Full(97 + 18 - 26, 158 + 35);
    //ugcap_.SetKantoku(&ugkantoku_);

    //店
    for (s32 i = 0; i < SHOPNUM; ++i)
    {
      shop_[i] = MakeObj();
      shop_[i]->o_SetParent(field_);
      shop_[i]->o_SetObjGrp(tugrp_->shop_front_[i]);
      shop_[i]->b_SetBtn(TRUE);
    }
    shop_[0]->z_SetPos_Full(24, 56);
    shop_[1]->z_SetPos_Full(152, 64);
    shop_[2]->z_SetPos_Full(320, 56);
    //ドア
    ugdoor_[0]->MakeGroup(shop_[0], tugrp_->shop_door_[0], 40, 40);
    ugdoor_[1]->MakeGroup(shop_[1], tugrp_->shop_door_[1], 56, 32);
    ugdoor_[2]->MakeGroup(shop_[2], tugrp_->shop_door_[2], 32, 40);

    ////バルーン
    //ugbln_item_.MakeGroup_Balloon(field_, "アイテム");
    //ugbln_item_.SetPos_Full(56 + 32, 77 + 16);

    ugbln_gacha_.MakeGroup_Balloon(field_, "ガチャ");
    ugbln_gacha_.SetPos_Full(200 + 32, 77 + 16);
    ugbln_gacha_.SetDraw(FALSE);

    //ugbln_huku_.MakeGroup_Balloon(field_, "ふくびき");
    //ugbln_huku_.SetPos_Full(328 + 32, 77 + 16);


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
    ugcap_.SetWalkType(cwt_FOLLOW);
    ugcap_.SetMirror(TRUE);

  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuShop::DoReady()
  {
    // 基底クラスのDoReadyを呼び出す
    base::DoReady();
    ugserifu_.ChangeFrame(enSerifuFrame_Normal);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuShop::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case pg_CapTalk1:
      captainDt_.talkCategory_ = ct_Category1;
      ugcap_.SetMType(cmt_St);
      SetMsg(CAPTAIN_SP_TALK1[captainDt_.talkNo_][0], CAPTAIN_SP_TALK1[captainDt_.talkNo_][1], CAPTAIN_SP_TALK1[captainDt_.talkNo_][2]);
      captainDt_.talkNo_++;
      ugserifu_.SetDraw(TRUE);
	  ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);
      ugserifu_.SetPos(-160, -30);
      break;

    case db::pg_Walking:
      ugbln_gacha_.SetDraw(TRUE);
      ugfinger_.SetMotionTouch(32, -24);
      ugfinger_.SetDraw(TRUE);
      break;
    case db::pg_Selected:
      ugfinger_.SetDraw(FALSE);
      ugbln_gacha_.SetDraw(FALSE);
      break;
    case db::pg_InShop:
      stHmShop_.inshopNo_ = ugkantoku_.stChar_.moveNo_;
      ugdoor_[stHmShop_.inshopNo_]->OpenDoor();
      subPage_ = 0;
      break;
    case db::pg_GotoShop:
      break;
    case db::pg_OutShop:
      break;
    case pg_CapTalk2:
      captainDt_.talkCategory_ = ct_Category2;
      ugcap_.SetMType(cmt_St);
      SetMsg(CAPTAIN_SP_TALK2[captainDt_.talkNo_][0], CAPTAIN_SP_TALK2[captainDt_.talkNo_][1], CAPTAIN_SP_TALK2[captainDt_.talkNo_][2]);
      captainDt_.talkNo_++;
      ugserifu_.SetDraw(TRUE);
      ugserifu_.SetPos(-160, 30);
      break;
    case pg_GotoCenter:
      ugfinger_.SetRevUD(TRUE);
      ugfinger_.SetMotionTouch(150-72, 60);
      ugfinger_.SetDraw(TRUE);
      pScene_->lyTuBar_->ugfooter_.ugradio_.SetDraw(TRUE);
      for (s32 i = 0; i < pScene_->lyTuBar_->ugfooter_.tabbtn_.size(); i++)
      {
        BOOL isDisp = FALSE;
        if (i == 3){ isDisp = TRUE; }// タブ4つ目 = シュートタブ
        //pScene_->lyTuBar_->ugfooter_.lybtn_[i]->SetDraw(isDisp);
        pScene_->lyTuBar_->ugfooter_.tabbtn_[i]->SetDraw(isDisp);
      }
      break;

    case pg_End:
      pScene_->lyTuBar_->GotoTab(TLyTuBar::tab_Center);
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
  // 
  //-----------------------------------------------------------------
  void TLyTuShop::DoUpdate(BOOL draw_f)
  {
    base::DoUpdate(draw_f);

    switch (stLy_.pageNo_)
    {
    case db::pg_GotoShop:
      //何も処理しない
      break;
    case db::pg_OutShop:
    case db::pg_Selected:
    case db::pg_InShop:
      ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      for (s32 i = 0; i < SHOPNUM; ++i)
      {
        shop_[i]->Update(draw_f, TRUE);
        ugdoor_[i]->Update(draw_f);
      }
      break;
    default:
      ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      for (s32 i = 0; i < SHOPNUM; ++i) shop_[i]->Update(draw_f, FALSE);
      //ugbln_item_.Update(draw_f);
      ugbln_gacha_.Update(draw_f);
      //ugbln_huku_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case db::pg_Init:MvPage_Init(); break;
      case db::pg_CapTalk1:MvPage_CapTalk(); break;
      case db::pg_Walking:MvPage_Walking(); break;
      case db::pg_Selected:MvPage_Selected(); break;
      case db::pg_InShop:MvPage_InShop(); break;
      case db::pg_GotoShop:MvPage_GotoShop(); break;
      case db::pg_OutShop:MvPage_OutShop(); break;
      case db::pg_CapTalk2:MvPage_CapTalk2(); break;
      case db::pg_GotoCenter:MvPage_GotoCenter(); break;
      }
    }

#ifdef __K_DEBUG_ROOT__
    kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
    kdebug::DebugText::GetInstance()->SetDebugText(10, 30, "ホーム説明");
#endif
  }

  void TLyTuShop::OutShop()
  {
    SetFoot_None();
    ugdoor_[stHmShop_.inshopNo_]->OpenDoor();
    subPage_ = 0;
    captainDt_.Init();
    ChangePage(pg_OutShop);
  }
  void TLyTuShop::SetTutoEnd()
  {
    isTutoEnd_ = TRUE;
  }


  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuShop::MvPage_Init(void)
  {
    captainDt_.Init();

    // 今回が初めて
    //if (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Kantoku))
    {
      ChangePage(pg_CapTalk1);
      captainDt_.talkNo_++;
    }
    //// ログボもらってこれからプロフィールエディット
    //else
    //{
    //	ugkantoku_.SetPos_Full(233 + 18 + 11, 110 + 35);
    //	ugkantoku_.SetMirror(FALSE);
    //	ugcap_.SetWalkType(cwt_MYWALK);
    //	ugcap_.SetPos_Full(233 + 18 + 45, 110 + 35);
    //	ugcap_.SetWalkType(cwt_FOLLOW);
    //	ugcap_.UpdateMemPos();
    //	ugcap_.SetMirror(TRUE);

    //	captainDt_.talkNo_ = 0;
    //	ChangePage(pg_CapTalk2);
    //	captainDt_.talkNo_++;
    //}
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuShop::MvPage_CapTalk(void)
  {
    // メッセージの次へが押された
    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      //ガチャやに行こう
      if (captainDt_.talkNo_ >= CAPTAIN_SP_TALK1_MAX)
      {
        ugserifu_.SetDraw(FALSE);
        ChangePage(pg_Walking);
        return;
      }
      else
      {
        ChangePage(pg_CapTalk1);
        captainDt_.talkNo_++;
      }
    }
    // 喋り終わった
    else if (ugserifu_.GetActRes() == enUGSerifuAct_ReadEnd)
    {
      ugcap_.SetMType(cmt_St);
    }
  }

  void TLyTuShop::MvPage_Walking()
  {
    //後ほど統合ボタンに変える
    if (shop_[0]->b_IsDecide())
    {
      //ugkantoku_.z_MoveWork_Full(0, DOORBX[0], DOORBY, 2);
      //ChangePage(pg_Selected);
    }
    else if (shop_[1]->b_IsDecide())
    {
      ugkantoku_.z_MoveWork_Full(1, DOORBX[1], DOORBY, 2);
      ChangePage(pg_Selected);
    }
    else if (shop_[2]->b_IsDecide())
    {
      //ugkantoku_.z_MoveWork_Full(2, DOORBX[2], DOORBY, 2);
      //ChangePage(pg_Selected);
    }
  }
  void TLyTuShop::MvPage_Selected()
  {
    if (ugkantoku_.IsActRes(enUGChGenAct_MoveStop))
    {
      ChangePage(pg_InShop);
    }
  }
  void TLyTuShop::MvPage_InShop()
  {
    switch (subPage_)
    {
      // 監督くんがドア前まで移動
    case 0:
      if (ugdoor_[stHmShop_.inshopNo_]->IsActRes(enUGDoorAct_Half))
      {
        ugkantoku_.SetDraw(FALSE);
        ugcap_.SetWalkType(cwt_MYWALK);
        switch (stHmShop_.inshopNo_)
        {
        case 0:	ugcap_.z_MoveWork_Full(0, DOORBX[0], DOORBY, 2);	break;
        case 1:	ugcap_.z_MoveWork_Full(1, DOORBX[1], DOORBY, 2);	break;
        case 2:	ugcap_.z_MoveWork_Full(2, DOORBX[2], DOORBY, 2);	break;
        }
        subPage_ = 1;
      }
      break;

      // キャプテンがドア前まで移動
    case 1:
      if (ugcap_.IsActRes(enUGChGenAct_MoveStop))
      {
        ugcap_.SetDraw(FALSE);
        ugdoor_[stHmShop_.inshopNo_]->CloseDoor();
        subPage_ = 2;
      }
      break;

      // ドアが閉まったのでそれぞれへ遷移
    case 2:
      if (ugdoor_[stHmShop_.inshopNo_]->IsActRes(enUGDoorAct_Half))
      {
        pScene_->lyTuGatya_->Ready();
        ChangePage(pg_GotoShop);
      }
      break;
    }
  }
  void TLyTuShop::MvPage_GotoShop()
  {

  }
  void TLyTuShop::MvPage_OutShop()
  {
    switch (subPage_)
    {
      // ドアがあくまで待つ
    case 0:
      if (ugdoor_[stHmShop_.inshopNo_]->IsActRes(enUGDoorAct_Half))
      {
        ugkantoku_.SetDraw(TRUE);
        // 監督の横にキャプテン登場
        switch (stHmShop_.inshopNo_)
        {
        case 0:
          ugcap_.SetPos_Full(DOORBX[0], DOORBY);
          ugcap_.z_MoveWork_Full(0, DOORBX[0] + 26, DOORBY, 2);
          break;
        case 1:
          ugcap_.SetPos_Full(DOORBX[1], DOORBY);
          if (ugkantoku_.IsMirror())
          {
            ugcap_.z_MoveWork_Full(1, DOORBX[1] + 26, DOORBY, 2);
          }
          else
          {
            ugcap_.z_MoveWork_Full(1, DOORBX[1] - 26, DOORBY, 2);
          }
          break;
        case 2:
          ugcap_.SetPos_Full(DOORBX[2], DOORBY);
          ugcap_.z_MoveWork_Full(2, DOORBX[2] - 26, DOORBY, 2);
          break;
        }
        ugcap_.SetDraw(TRUE);
        subPage_ = 1;
      }
      break;

      // キャプテンがドア前まで移動
    case 1:
      if (ugcap_.IsActRes(enUGChGenAct_MoveStop))
      {
        ugcap_.SetWalkType(cwt_FOLLOW);
        ugcap_.SetMirror(ugkantoku_.IsMirror());
        ugdoor_[stHmShop_.inshopNo_]->CloseDoor();
        subPage_ = 2;
      }
      break;

    case 2:
      if (ugdoor_[stHmShop_.inshopNo_]->IsActRes(enUGDoorAct_Half))
      {
		  ugkantoku_.SetMirror(TRUE);
        //SetFoot_Tab();//ヘッダフッタはもどさない
        ChangePage(pg_CapTalk2);
      }
      break;
    }
  }

  void TLyTuShop::MvPage_CapTalk2(void)
  {
    // メッセージの次へが押された
    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      //シュートセンターにいこう
      if (captainDt_.talkNo_ >= CAPTAIN_SP_TALK2_MAX)
      {
        ugserifu_.SetDraw(FALSE);
        ChangePage(pg_GotoCenter);
        return;
      }
      else
      {
        ChangePage(pg_CapTalk2);
        captainDt_.talkNo_++;
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
  void TLyTuShop::MvPage_GotoCenter(void)
  {
    if (pScene_->lyTuBar_->ugfooter_.tabbtn_[3]->IsAct())
    {
      pScene_->lyTuBar_->ugfooter_.ugradio_.SetDraw(FALSE);
      ugfinger_.SetDraw(FALSE);
      ChangePage(pg_End);
    }
  }
}
