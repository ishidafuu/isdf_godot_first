#include "dbLyTuHmMami.h"
#include "dbSnTutorial.h"

namespace db
{

  enum enPage
  {
    pg_Visit,
    pg_Logbo,
    pg_LogboSp,
    pg_LogboSpYokoku,
    pg_LogboResult,
    pg_Out,
  };

  enum enConnect
  {
    cn_Logbo,
  };

  //----------------------------------------------------------------------
  // 真実のセリフ関係
  enum enMamiTalkType
  {
    mtt_Visit = 0,
    mtt_Logbo,
    mtt_LogboSp,
    mtt_LogboSpYokoku,
    mtt_LogboResult,
    mtt_Out,
    mtt_Max,
  };
  const s32 MESSAGE_LINE_MAX = 3;		// メッセージの最大行数は3
  const char* MAMI_TALK[mtt_Max][MESSAGE_LINE_MAX] =
  {
    {	// mtt_Visit
      "まみ「こんにちは心",
      "こちらは＿うんえい＿カウンターです",
    },
    {	// mtt_Logbo
      "まみ「ほんじつの＿おくりものです",
      "こちらを＿おうけとりください心",
    },
    {	// mtt_LogboSp
      "まみ「いつもありがとうございます心",
      "とくべつボーナスです心",
    },
    {	// mtt_LogboSpYokoku			
    },	// ここは特殊な処理が入るので別
    {	// mtt_LogboResult
      "まみ「あしたも＿きてくださいね",
      "スターを＿よういしておきます心",
    },
    {	// mtt_Out
      "まみ「ありがとうございました",
      "またごりようください心",
    },
  };


  //コンストラクタ
  TLyTuHmMami::TLyTuHmMami(TSnTutorial* pScene)
    :TLyTuGen(pScene)
    , uglistGetLog_(this)
    , uglistUseLog_(this)
    , ugprof_(this)
  {
    stMami_.Init();
  }

  TLyTuHmMami::~TLyTuHmMami()
  {
  }



  void TLyTuHmMami::DoLoad()
  {
    //field_ = MakeObj();

    //ショップフィールド全生成
    ugmami_ = pScene_->lyTuHome_->GetMami();
    uglogbo_ = pScene_->lyTuHome_->GetLogbo();
    ugserifu_ = pScene_->lyTuHome_->GetSerifuWindow();

    uglistGetLog_.MakeGroup(field_, FALSE);
    uglistUseLog_.MakeGroup(field_, FALSE);
    ugprof_.MakeGroup(field_, FALSE);
  }

  //入店時の処理
  void TLyTuHmMami::DoReady()
  {
    stMami_.Init();
    SetMsg(mtt_Visit);
    DrawList(NULL);
	ugbtn_back_->SetFont("もどる");
    ugbtn_ok_->SetFont("おーけー");
    ugbtn_prev_->SetFont("まえのページ");
    ugbtn_next_->SetFont("つぎのページ");
	SetHudFilter(FALSE);
  }

  void TLyTuHmMami::DoDown()
  {
    ugserifu_->SetDraw(FALSE);
    pScene_->lyTuHome_->ComebackInMamiMenu();
    //pScene_->lyTuHome_->Ready();
  }

  void TLyTuHmMami::DrawList(TUGLiGen* drawlist)
  {
    if (drawlist != NULL)
    {
      drawlist->SetDraw(TRUE);
      //drawlist->SetZeroPos();
    }
    ugbtn_back_->SetDraw(TRUE);
    ResetObj();
  }
  void TLyTuHmMami::ResetObj()
  {
    ugbtn_prev_->SetDraw(FALSE);
    ugbtn_next_->SetDraw(FALSE);
    ugbtn_ok_->SetDraw(FALSE);
    ugprof_.SetDraw(FALSE);
  }
  void TLyTuHmMami::SwitchLogPageBtn()
  {
    ugbtn_prev_->SetDraw((stMami_.logpage_ > 0));
    ugbtn_next_->SetDraw((stMami_.logpage_ < stMami_.endpage_));
  }
  void TLyTuHmMami::DoChangePage(s32 pageno)
  {
    switch ((enPage)pageno)
    {
    case db::pg_Visit:
      ugbtn_back_->SetDraw(FALSE);
      break;
    case db::pg_Logbo:
      DrawList(NULL);
      SetMsg(mtt_Logbo);
      SetHudEnd();
      SetHudItemView_GetLogSingle(0, TRUE, TRUE);
      ugbtn_back_->SetDraw(FALSE);
      break;
    case db::pg_LogboSp:
      SetMsg(mtt_LogboSp);
      SetHudEnd();
      SetHudItemView_GetLogSingle(1, TRUE, TRUE);
      ugbtn_back_->SetDraw(FALSE);
      break;
    case db::pg_LogboSpYokoku:
      SetMsg(mtt_LogboSpYokoku);
      SetHudEnd();
      SetHudItemView_View(stMami_.nextitemType_, stMami_.nextitemID_, stMami_.nextitemNum_, FALSE, TRUE);
      break;
    case db::pg_LogboResult:
      SetMsg(mtt_LogboResult);
      break;
    case db::pg_Out:
      UndrawBtn();
      SetMsg(mtt_Out);
      break;

    default:
      break;
    }

  }
  //入店
  void TLyTuHmMami::MvPage_Visit()
  {
    if (ugserifu_->IsActRes(enUGSerifuAct_WaitEnd))
    {
      if (uglogbo_->IsLogobo())
      {
        Post_Logbo();
      }

#ifdef __K_DEBUG_ROOT__
	  Post_Logbo();
#endif
    }
  }
  //ログボ
  void TLyTuHmMami::MvPage_Logbo()
  {
    //アイテム獲得ボタン
    if (IsHudItemView())
    {
      uglogbo_->Refresh();

      if ((pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineNum() > 1))
      {
        ChangePage(pg_LogboSp);
      }
      else
      {
        s32 loginnum = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_loginNum);
        s32 splogbonum = (pmgEO_->mgCoM_.mdm_Logbo_.GetLineNum() - 1);
        const s32 LOGBOSPITV = 10;
        s32 nextlogbosp = ((loginnum % (splogbonum * LOGBOSPITV)) / LOGBOSPITV) + 1;

        stMami_.nextitemType_ = pmgEO_->mgCoM_.mdm_Logbo_.GetLineDt(nextlogbosp, mdm_logbo_itemType);
        stMami_.nextitemID_ = pmgEO_->mgCoM_.mdm_Logbo_.GetLineDt(nextlogbosp, mdm_logbo_itemID);
        stMami_.nextitemNum_ = pmgEO_->mgCoM_.mdm_Logbo_.GetLineDt(nextlogbosp, mdm_logbo_itemNum);

        ChangePage(pg_LogboSpYokoku);
      }
    }
  }
  //スペシャルログボ
  void TLyTuHmMami::MvPage_LogboSp()
  {
    //アイテム獲得ボタン
    if (IsHudItemView())
    {
      ChangePage(pg_Out);
    }
  }
  //スペシャルログボよこく
  void TLyTuHmMami::MvPage_LogboSpYokoku()
  {
    if (IsHudItemView())
    {
      ChangePage(pg_Out);
    }
  }
  //スペシャルログボ
  void TLyTuHmMami::MvPage_LogboResult()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Out);
    }
  }


  void TLyTuHmMami::MvPage_Out()
  {
    // 真実が最後のセリフをしゃべった
    if (ugserifu_->IsActRes(enUGSerifuAct_WaitEnd))
    {
      Down();
    }
  }


  //*******************
  //POSTログボ
  void TLyTuHmMami::Post_Logbo()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Logbo();
    }
    else
    {
      StartConnect(cn_Logbo);
      pmgEO_->mgPo_.poTuto_.Ready_logbo();
    }
  }

  //POSTの結果から最終ページ番号取得
  void TLyTuHmMami::SetEndPage()
  {
    stMami_.endpage_ = pmgEO_->mgPo_.GetValue("endpage");
  }

  //*******************
  //接続ログボ
  void TLyTuHmMami::Connect_Logbo()
  {
    ChangePage(pg_Logbo);
  }

  //接続完了
  void TLyTuHmMami::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_Logbo: Connect_Logbo(); break;
        }
      }
      else
      {

        switch (stLy_.connect_)
        {
        case cn_Logbo: ChangePage(pg_Out); break;
        }
        //エラーメッセージ(ここでstLy_.connect_のクリアが入ってしまう)
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
      }
    }
    else
    {
      //通信エラー

      SetHudEnd();
      ChangePage(pg_Out);
    }
  }
  //*******************

  void TLyTuHmMami::DoUpdate(BOOL draw_f)
  {
    switch (stLy_.pageNo_)
    {
    case pg_Out:
      //ugmami_->Update(draw_f);//下から借りてきてるやつなのでここで更新しない
      //ugserifu_->Update(draw_f);
      break;
    default:
      //ugmami_->Update(draw_f);
      //ugserifu_->Update(draw_f);
      uglistGetLog_.Update(draw_f);
      uglistUseLog_.Update(draw_f);
      ugprof_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Visit: MvPage_Visit(); break;
      case pg_Logbo: MvPage_Logbo(); break;
      case pg_LogboSp: MvPage_LogboSp(); break;
      case pg_LogboSpYokoku: MvPage_LogboSpYokoku(); break;
      case pg_LogboResult: MvPage_LogboResult(); break;
      case pg_Out: MvPage_Out(); break;
      }
    }

  }




  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHmMami::SetMsg(s32 type)
  {
    // メッセージウィンドウのタイプ
    switch (type)
    {
    case mtt_Logbo:
    case mtt_LogboSp:
    case mtt_LogboSpYokoku:
      ugserifu_->SetMsgChangeMode(enMsgChangeMode_None);
      break;
    default:
      ugserifu_->SetMsgChangeMode(enMsgChangeMode_Touch);
      break;
    }

    // 文字列置換
    switch (type)
    {
    case mtt_LogboSpYokoku:
    {
      s32 loginnum = (10 - (pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_loginNum) % 10));
      std::string strrest = "まみ「あと" + lib_str::IntToStr(loginnum) + "かいで＿こちらの";
      SetMsg(strrest.c_str(), "とくべつボーナスを＿さしあげます心");
    }
    break;

    default:
      SetMsg(MAMI_TALK[type][0], MAMI_TALK[type][1], MAMI_TALK[type][2]);
      break;
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuHmMami::SetMsg(const char* text0, const char* text1, const char* text2)
  {
    std::string strNone = "";

    serifuvec_.clear();
    serifuvec_.push_back(text0);
    if (text1 && strNone.compare(text1) != 0)
    {
      serifuvec_.push_back(text1);
    }
    if (text2 && strNone.compare(text2) != 0)
    {
      serifuvec_.push_back(text2);
    }
    ugserifu_->SetSerifu(serifuvec_);
    ugserifu_->SetDraw(TRUE);
    ugserifu_->SetPos(10-5, -90);
  }
}
