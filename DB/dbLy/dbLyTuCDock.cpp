#include "dbLyTuCDock.h"
#include "dbSnTutorial.h"
#include "dbUGPrev.h"

#ifdef __K_DEBUG_ROOT__
//#define __K_DEBUG_SKIP_DOCK__
#endif
namespace db
{

  enum enPage
  {
    pg_Visit,
    pg_PrevR,
    pg_PrevRResult,
    pg_PrevAIntro,
    pg_PrevA,
    //pg_PrevBIntro,
    pg_PrevB,
    pg_PrevC,
    pg_PrevD,
    pg_PrevRIntro,
    pg_Last,
    pg_Out,
    pg_Error,
  };

  enum enConnect
  {
    cn_tutogousei,
  };
  const s32 SOZAIA = 11002;
  const s32 SOZAIB = 172;
  const s32 SOZAIC = 21022;

  const s32 MESSAGE_LINE_MAX = 3;		// メッセージの最大行数は3
  // トークカテゴリー2
  const s32 DOCK_TALK_MAX = 2;
  const STRVECTOR2 DOCK_VISIT =
  {
    {
      "掛「ようこそ＿けんきゅうじょへ",
    },
    {
      "掛「ここでは＿オリジナルしゅーとを",
      "かいはつすることが＿できます！",
    },
    {
      "掛「はじめてつくる＿しゅーとは",
      "とくべつに＿プレゼントします心",
    },
    {
      "掛「すごいきどう・ちょうパワーの",
      "ひっさつしゅーとを＿えらびましょう",
    },
    {
      "掛「えらんだ＿ひっさつしゅーとを",
      "あとで＿つくって＿もらいますね心",
    },
  };
  const STRVECTOR2 DOCK_PREVR =
  {
    {
      "掛「イマイチなら＿べつのしゅーとに＿します",
      "なっとく＿いくまで＿えらんでくださいね心",
    },
    {
      "掛「いちばんさいしょの＿ひっさつしゅーと＿は",
      "ほんとうに＿この＿ひっさつシュート＿でいい？",
    },
  };
  const STRVECTOR2 DOCK_RRESULT =
  {
    {
      "掛「いまの＿ひっさつしゅーとは",
      "この＿そざいだまで＿できています心",
    },
    {
      "掛「おなじように＿つくりたければ",
      "プレゼントじゅんで＿ミックスします",
    },
    {
      "掛「では＿つづいて＿ミックスの",
      "ほうほうを＿せつめいします心",
    },
  };
  const STRVECTOR2 DOCK_PREVA =
  {
    {
      "掛「まずはミックスをする＿ベースを＿きめます",
      "「うえしたＩだま」を＿つかってみます",
    },
    {
      "じょうげの＿なみきどうの＿しゅーとです",
      "いちど＿ミックスするまえに＿なげてみましょう",
    },
  };
  const STRVECTOR2 DOCK_PREVB =
  {
    {
      "つぎに＿ついかのそざいを＿きめます",
      "「わかれるＹだま」を＿まぜてみます",
    },
    {
      "じょうげに＿ぶんれつする＿しゅーとです",
      "これも＿ミックスするまえに＿なげてみましょう",
    },
  };
  const STRVECTOR2 DOCK_PREVC =
  {
    {
      "「うえしたＩだま」「わかれるＹだま」を",
      "ミックスしたしゅーとが＿こちらになります",
    },
  };

  const STRVECTOR2 DOCK_PREVD =
  {
    {
      "さらに＿あたると＿せかいいっしゅう＿こうかの",
      "「いっしゅうだま」もミックスしてみましょう",
    },
  };
  const STRVECTOR2 DOCK_PREVRINTRO =
  {
    {
      "掛「このように＿ミックスを＿すると",
      "そざいのとくせいが＿たされます心",
    },
    {
      "掛「さいごに＿そざいだまの",
      "ステータスを＿みてみましょう心",
    },
    {
      "掛「こちらが＿しゅーとのパワーです",
      "たかいほど＿いりょくが＿あがります",
    },
    {
      "掛「スピンは＿キャッチのしにくさ",
      "はねかえるむきに＿えいきょうします",
    },
    {
      "掛「そざいだまの＿ひんしつ＿です",
      "たかいほど＿すごい＿そざいだまです",
    },
    {
      "掛「ほせいちは＿ミックスすると",
      "ベースに＿プラスされていきます",
    },
    {
      "掛「レアそざいは＿スロットがおおく",
      "たくさん＿ミックスが＿できます",
    },
  };

  const STRVECTOR2 DOCK_LAST =
  {
    {
      "掛「これで＿かんとくこうしゅうは",
      "おわりです＿おつかれさまでした心",
    },
  };

  //コンストラクタ
  TLyTuCDock::TLyTuCDock(TSnTutorial* pScene)
    :TLyTuBase(pScene)
    , ugshot_(this)
    //, ugtutomsg_(this)
  {
    stCTuto_.Init();
    dockDt_.Init();
  }

  TLyTuCDock::~TLyTuCDock()
  {
  }
  //読み込み
  void TLyTuCDock::DoLoad()
  {
    ugshot_.MakeGroup(field_);
  }
  //準備
  void TLyTuCDock::DoReady()
  {
    //ReadyDockを用意しているので書かない
  }
  //終了
  void TLyTuCDock::DoDown()
  {
    //uggousei_.Down();
    pScene_->lyTuCenter_->Ready();//退出
    pScene_->lyTuCenter_->OutDock();
  }
  //接続完了
  void TLyTuCDock::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_tutogousei: Connect_TutoGousei(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        ChangePage(pg_Error);
      }
    }
    else
    {
      ChangePage(pg_Error);
    }
  }
  //ページ切り替え
  void TLyTuCDock::DoChangePage(s32 pageno)
  {
    SOZAIDTVECTOR sozaivec;
    //const s32 SOZAID = 30360;
    //uggousei_.SetGouseiMsg(enGouseiMsg_Base);
    ResetObj();

    switch (pageno)
    {
    case db::pg_Visit:
      SetDockMsg(DOCK_VISIT, dockDt_.talkNo_);
      ugdock_->ugdoor_.OpenCloseDoor();//扉開ける
      break;

    case db::pg_PrevR:
    {
      ResetSozaiVec();
      TPrevParam param;
      param.nonebackbtn_f = TRUE;
      param.nonemixbtn_f = TRUE;
			param.nonehavemixbtn_f = TRUE;
      param.sozaivec = &prevsozaivec_;
      SwitchLongMsg(TRUE);
      SetDockMsg(DOCK_PREVR, dockDt_.talkNo_);
      ugserifu_.SetMsgChangeMode(enMsgChangeMode_None);
      SetHudPrev(param);

      //SetHudPrevChangeShot(prevsozaivec_);
    }
    break;
    case db::pg_PrevRResult:
      SetDockMsg(DOCK_RRESULT, dockDt_.talkNo_);
      SetHudEnd();
      SetHudItemView_GetLog(TRUE, TRUE, FALSE);
	  ugfinger_.SetDraw(FALSE);
      //ugtutomsg_.IncMsg();
      break;

    case db::pg_PrevAIntro:
      break;
    case db::pg_PrevA:
    {
      TSozaiData sozai;
      sozai.SetMidSozai(SOZAIA);
      sozaivec.push_back(sozai);
      TPrevParam param;
      param.nonebackbtn_f = TRUE;
      param.nonemixbtn_f = TRUE;
			param.nonehavemixbtn_f = TRUE;
      param.sozaivec = &sozaivec;
      SwitchLongMsg(TRUE);
      SetDockMsg(DOCK_PREVA, dockDt_.talkNo_);
      SetHudPrev(param);

      //SetHudPrevChangeShot(sozaivec);
    }
    break;

    case db::pg_PrevB:
    {
      TSozaiData sozai;
      sozai.SetMidSozai(SOZAIB);
      sozaivec.push_back(sozai);
      TPrevParam param;
      param.nonebackbtn_f = TRUE;
      param.nonemixbtn_f = TRUE;
			param.nonehavemixbtn_f = TRUE;
      param.sozaivec = &sozaivec;

      SwitchLongMsg(TRUE);
      SetDockMsg(DOCK_PREVB, dockDt_.talkNo_);
      ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);
      SetHudPrev(param);

      //SetHudPrevChangeShot(sozaivec);
    }
    break;
    case db::pg_PrevC:
    {
      TSozaiData sozai;
      sozai.SetMidSozai(SOZAIA);
      sozaivec.push_back(sozai);
      sozai.SetMidSozai(SOZAIB);
      sozaivec.push_back(sozai);
      TPrevParam param;
      param.nonebackbtn_f = TRUE;
      param.nonemixbtn_f = TRUE;
			param.nonehavemixbtn_f = TRUE;
      param.sozaivec = &sozaivec;

      SwitchLongMsg(TRUE);
      SetDockMsg(DOCK_PREVC, dockDt_.talkNo_);
      SetHudPrev(param);

	  /* 2015/06/16 Koike Added 指を置く */
	  const s32 FXB = 56 + 8;
	  const s32 FYB = 64 - 14;
	  ugfinger_.SetMotionTouch(FXB, FYB);
	  ugfinger_.SetDraw(TRUE);

    }
    break;

    case db::pg_PrevD:
    {
      TSozaiData sozai;
      sozai.SetMidSozai(SOZAIA);
      sozaivec.push_back(sozai);
      sozai.SetMidSozai(SOZAIB);
      sozaivec.push_back(sozai);
      sozai.SetMidSozai(SOZAIC);
      sozaivec.push_back(sozai);
      TPrevParam param;
      param.nonebackbtn_f = TRUE;
      param.nonemixbtn_f = TRUE;
			param.nonehavemixbtn_f = TRUE;
      param.sozaivec = &sozaivec;
      SwitchLongMsg(TRUE);
      SetDockMsg(DOCK_PREVD, dockDt_.talkNo_);
      SetHudPrev(param);

	  /* 2015/06/16 Koike Added 指を置く */
	  const s32 FXB = 56 + 8;
	  const s32 FYB = 64 - 14;
	  ugfinger_.SetMotionTouch(FXB, FYB);
	  ugfinger_.SetDraw(TRUE);

    }
    break;
    case db::pg_PrevRIntro:
      SetHudEnd();
      SetDockMsg(DOCK_PREVRINTRO, dockDt_.talkNo_);
      ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);
	  ugfinger_.SetDraw(FALSE);
      break;

    case db::pg_Last:
      SetDockMsg(DOCK_LAST, dockDt_.talkNo_);
      ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);
      break;
    case db::pg_Out:
      ugdock_->ugdoor_.OpenCloseDoor();//扉開ける
      break;
    case db::pg_Error:
      break;
    default:
      break;
    }

  }
  //更新
  void TLyTuCDock::DoUpdate(BOOL draw_f)
  {

    base::DoUpdate(draw_f);
    ugshot_.Update(draw_f);
    ugdock_->Update(draw_f);//末端で管理する
    ugkantoku_->Update(draw_f);
    ugcap_->Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case db::pg_Visit:MvPage_Visit(); break;
      case db::pg_PrevR:MvPage_PrevR(); break;
      case db::pg_PrevRResult:MvPage_PrevRResult(); break;
        //case db::pg_PrevAIntro:MvPage_PrevAIntro(); break;
      case db::pg_PrevA:MvPage_PrevA(); break;
        //case db::pg_PrevBIntro:MvPage_PrevBIntro(); break;
      case db::pg_PrevB:MvPage_PrevB(); break;
      case db::pg_PrevC:MvPage_PrevC(); break;
      case db::pg_PrevD:MvPage_PrevD(); break;
      case db::pg_PrevRIntro:MvPage_PrevRIntro(); break;
      case db::pg_Last:MvPage_Last(); break;
      case db::pg_Out:MvPage_Out(); break;
      case db::pg_Error:MvPage_Error(); break;
      default:
        break;
      }

      if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))
      {
        ugdock_->ugclerk_.SetMType(cmt_Stand);
      }


      // メッセージスキップ、ページ送り
      //ugtutomsg_.CheckSkipMsg();
    }
  }
  //入店時の処理
  void TLyTuCDock::ReadyDock(TUGDock* ugdock, s32 dockno)
  {
    base::DoReady();

    ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);
    ugdock_ = ugdock;
    ugkantoku_ = &pScene_->lyTuCenter_->ugkantoku_;
    ugcap_ = &pScene_->lyTuCenter_->ugcap_;

    //親の交換大丈夫か？
    ugkantoku_->SetParent(ugdock_->GetField());
    ugkantoku_->SetPos(56, 40);
    ugkantoku_->GetField()->g_SetLimX(0, 0);
    ugkantoku_->SetMirror(FALSE);
    ugcap_->SetParent(ugdock_->GetField());
    ugcap_->SetWalkType(cwt_MYWALK);
    ugcap_->SetPos(30, 40);
    ugcap_->SetMType(cmt_St);
    ugcap_->GetField()->g_SetLimX(0, 0);

    //uggousei_.Ready(ugdock, dockno);

    stCTuto_.Init();
    ugbtn_back_->SetFont("おーけー！");
    ugbtn_ok_->SetFont("ダメダメ！");

    //ugtutomsg_.FirstMsg();

    syouhinvec_add_.clear();
    syouhinvec_base_.clear();
    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_shopType) == enShopType_BuySozai)
      {
        s32 mid_spsyouhin = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_pk);
        syouhinvec_add_.push_back(mid_spsyouhin);
      }
      else if (pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_shopType) == enShopType_BuySozaiVip)
      {
        s32 mid_spsyouhin = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_pk);
        syouhinvec_base_.push_back(mid_spsyouhin);
      }
    }


    TSozaiData tusozai;
    tusozai.Init();
    tusozai.mid_sozai_ = SOZAIA / SOZAIMID;
    tusozai.rarity_ = SOZAIA % SOZAIMID;
    tusozai.indvpow_ = 2;
    tusozai.indvspin_ = 4;
    ugshot_.SetShotDtSozai(tusozai);
    ugshot_.SetTipsDraw_f(TRUE);
    ugshot_.SetLRPos(TRUE);
    ugshot_.SetDraw(FALSE);
    ugshot_.SetDepth(UIDP_04HEADER);

    ResetObj();
    Ready();
    pmgEO_->stSeq_.PushNeedOmakase();//おまかせリフレッシュフラグ
  }
  //素材リセット
  void TLyTuCDock::ResetSozaiVec()
  {
    spsyouhinvec_.clear();
    prevsozaivec_.clear();
    spsyouhinvec_.push_back(syouhinvec_base_[mid::midGetRandMn(syouhinvec_base_.size())]);
    spsyouhinvec_.push_back(syouhinvec_add_[mid::midGetRandMn(syouhinvec_add_.size())]);
    spsyouhinvec_.push_back(syouhinvec_add_[mid::midGetRandMn(syouhinvec_add_.size())]);
    spsyouhinvec_.push_back(syouhinvec_add_[mid::midGetRandMn(syouhinvec_add_.size())]);
    for (s32 i = 0; i < spsyouhinvec_.size(); ++i)
    {
      TSozaiData sozai;
      sozai.SetMidSozai(pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(spsyouhinvec_[i], mdm_spsyouhin_itemID));
      prevsozaivec_.push_back(sozai);
    }
  }
  //オブジェリセット
  void TLyTuCDock::ResetObj()
  {
    ugbtn_back_->SetDraw(FALSE);
    ugbtn_ok_->SetDraw(FALSE);
    dockDt_.talkNo_ = 0;
    stCTuto_.prevfirstback_f_ = FALSE;
    SwitchLongMsg(FALSE);
  }

  //セリフセット
  BOOL TLyTuCDock::SetDockMsg(const STRVECTOR2& msg, s32 line)
  {
    if (line >= msg.size()) return TRUE;

    ugdock_->ugclerk_.SetMType(cmt_Talk);
    serifuvec_ = msg[line];
    ugserifu_.SetDepth(UIDP_09MSGBOX);
    ugserifu_.SetSerifu(serifuvec_);
    ugserifu_.SetDraw(TRUE);
    ++dockDt_.talkNo_;

    return FALSE;
  }
  //セリフ長さ切り替え
  void TLyTuCDock::SwitchLongMsg(BOOL longmsg_f)
  {
    if (longmsg_f)
    {
      ugserifu_.ChangeFrame(enSerifuFrame_Wide);
      ugserifu_.SetPos(-96, -90);
    }
    else
    {
      ugserifu_.ChangeFrame(enSerifuFrame_Normal);
      ugserifu_.SetPos(0, -38);
    }
    //ugtutomsg_.SetDraw(longmsg_f);
    //ugserifu_.SetDraw(!longmsg_f);
  }
  //POST：プレゼント
  void TLyTuCDock::Post_TutoGousei()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_TutoGousei();
      //ChangePage(pg_PrevRResult);
    }
    else
    {

      StartConnect(cn_tutogousei);
      pmgEO_->mgPo_.poTuto_.Ready_Gousei(&spsyouhinvec_);
    }
  }
  //接続：プレゼント
  void TLyTuCDock::Connect_TutoGousei()
  {
#ifdef __K_DEBUG_USER_DATA__
    kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_Gousei);
#endif
    ChangePage(pg_PrevRResult);
  }
  //入店
  void TLyTuCDock::MvPage_Visit()
  {
    //ドック

    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      if (SetDockMsg(DOCK_VISIT, dockDt_.talkNo_))
      {
        //ChangePage(pg_PrevR);//とばすとき
#ifdef __K_DEBUG_SKIP_DOCK__
        // デバッグの効率化の為シュート関係全てスキップ
        ChangePage(pg_Last);
#else
        ChangePage(pg_PrevR);
#endif
    }
  }
    // 喋り終わった
    else if (ugserifu_.GetActRes() == enUGSerifuAct_ReadEnd)
    {
      ugdock_->ugclerk_.SetMType(cmt_Stand);
    }
}
  void TLyTuCDock::MvPage_PrevA()
  {
    //プレビュー中

    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      // 次のテキストを表示した後は
      // 文字送りアイコン非表示版にする
      SetDockMsg(DOCK_PREVA, dockDt_.talkNo_);
      ugserifu_.SetMsgChangeMode(enMsgChangeMode_None);

	  /* 2015/06/16 Koike Added 指を置く */
	  const s32 FXB = 56 + 8;
	  const s32 FYB = 64 - 14;
	  ugfinger_.SetMotionTouch(FXB, FYB);
	  ugfinger_.SetDraw(TRUE);
    }

    if (IsHudPrevHit())
    {
      if (stCTuto_.prevfirstback_f_)
        ugbtn_back_->SetDraw(TRUE);
		ugfinger_.SetDraw(FALSE);
        stCTuto_.prevfirstback_f_ = TRUE;
    }
    if (ugbtn_back_->IsAct()) ChangePage(pg_PrevB);
  }
  void TLyTuCDock::MvPage_PrevB()
  {
    //プレビュー中

    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      SetDockMsg(DOCK_PREVB, dockDt_.talkNo_);
      ugserifu_.SetMsgChangeMode(enMsgChangeMode_None);

	  /* 2015/06/16 Koike Added 指を置く */
	  const s32 FXB = 56 + 8;
	  const s32 FYB = 64 - 14;
	  ugfinger_.SetMotionTouch(FXB, FYB);
	  ugfinger_.SetDraw(TRUE);
    }

    if (IsHudPrevHit())
    {
	  if (stCTuto_.prevfirstback_f_)
	  {
		ugbtn_back_->SetDraw(TRUE);
		ugfinger_.SetDraw(FALSE);
	  }
	  else
		stCTuto_.prevfirstback_f_ = TRUE;
    }
    if (ugbtn_back_->IsAct()) ChangePage(pg_PrevC);
  }
  void TLyTuCDock::MvPage_PrevC()
  {
    //プレビュー中

    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      SetDockMsg(DOCK_PREVC, dockDt_.talkNo_);
    }

    if (IsHudPrevHit())
    {
	  if (stCTuto_.prevfirstback_f_)
	  {
		ugbtn_back_->SetDraw(TRUE);
		ugfinger_.SetDraw(FALSE);
	  }
	  else
		stCTuto_.prevfirstback_f_ = TRUE;
    }
    if (ugbtn_back_->IsAct()) ChangePage(pg_PrevD);
  }
  void TLyTuCDock::MvPage_PrevD()
  {
    //プレビュー中

    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      SetDockMsg(DOCK_PREVD, dockDt_.talkNo_);
    }

    if (IsHudPrevHit())
    {
	  if (stCTuto_.prevfirstback_f_)
	  {
		ugbtn_back_->SetDraw(TRUE);
		ugfinger_.SetDraw(FALSE);
	  }
      else
        stCTuto_.prevfirstback_f_ = TRUE;
    }
    if (ugbtn_back_->IsAct()) ChangePage(pg_PrevRIntro);
  }
  void TLyTuCDock::MvPage_PrevRIntro()
  {
    //ドック

    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      if (SetDockMsg(DOCK_PREVRINTRO, dockDt_.talkNo_))
      {
        ugfinger_.SetDraw(FALSE);
        ugshot_.SetDraw(FALSE);
        ChangePage(pg_Last);
      }
      else
      {
        const s32 FST = 2;
        switch (dockDt_.talkNo_)
        {
        case FST:
          ugshot_.SetDraw(TRUE);
          break;
        case FST + 1:
          ugfinger_.SetPos(-96 + 4, -32 + 20);//pow
          ugfinger_.SetDraw(TRUE);
          break;
        case FST + 2:
          ugfinger_.SetPos(-96 + 60 + 8, -32 + 20);//spin
          break;
        case FST + 3:
          ugfinger_.SetPos(-96 + 80, -86 + 20);//qt
          ugfinger_.SetRevUD(TRUE);
          break;
        case FST + 4:
          ugfinger_.SetPos(-112 + 8, -24 + 20 + 4);//rev
          ugfinger_.SetRevUD(FALSE);
          break;
        case FST + 5:
          ugfinger_.SetPos(-96 + 24, -32 + 24 + 20);//slot
          break;
        case FST + 6:
          ugfinger_.SetDraw(FALSE);
          ugshot_.SetDraw(FALSE);
          break;
        }
      }
    }
    // 喋り終わった
    else if (ugserifu_.GetActRes() == enUGSerifuAct_ReadEnd)
    {
      ugdock_->ugclerk_.SetMType(cmt_Stand);
    }

  }
  void TLyTuCDock::MvPage_PrevR()
  {

	/* 2015/06/16 Koike Added 指を置く */
	if (ugserifu_.GetActRes() == enUGSerifuAct_ReadEnd)
	{
		if (dockDt_.talkNo_ == 1)
		{
			const s32 FXB = 56 + 8;
			const s32 FYB = 64 - 14;
			ugfinger_.SetMotionTouch(FXB, FYB);
			// 指を置いてみる
			ugfinger_.SetDraw(TRUE);
		}
	}

    if (IsHudPrevHit())
    {
      if (stCTuto_.prevfirstback_f_)
      {
        if (stCTuto_.prevhit_f_ == FALSE)
        {
          stCTuto_.prevhit_f_ = TRUE;
          SetDockMsg(DOCK_PREVR, 1);
		  ugfinger_.SetDraw(FALSE);
        }
        ugbtn_back_->SetDraw(TRUE);
        ugbtn_ok_->SetDraw(TRUE);
      }
      else
      {
        stCTuto_.prevfirstback_f_ = TRUE;
      }
    }

    if (ugbtn_back_->IsAct())
    {
      //ChangePage(pg_PrevRResult);
      //POSTする内容
      ugbtn_back_->SetDraw(FALSE);
      ugbtn_ok_->SetDraw(FALSE);
      Post_TutoGousei();
    }
    else if (ugbtn_ok_->IsAct())
    {
      //stCTuto_.prevretry_f_ = TRUE;
      stCTuto_.prevhit_f_ = FALSE;
      ResetSozaiVec();
      SetHudPrevChangeShot(prevsozaivec_);
      ugbtn_back_->SetDraw(FALSE);
      ugbtn_ok_->SetDraw(FALSE);
    }
  }
  void TLyTuCDock::MvPage_PrevRResult()
  {

    //獲得リスト閉じる
    if (IsHudItemView())
    {
      SetDockMsg(DOCK_RRESULT, dockDt_.talkNo_);
      ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);
    }

    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      if (SetDockMsg(DOCK_RRESULT, dockDt_.talkNo_))
      {
        ChangePage(pg_PrevA);
      }
    }
    // 喋り終わった
    else if (ugserifu_.GetActRes() == enUGSerifuAct_ReadEnd)
    {
      ugdock_->ugclerk_.SetMType(cmt_Stand);
    }
  }
  void TLyTuCDock::MvPage_Last()
  {
    //ドック

    if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
    {
      if (SetDockMsg(DOCK_LAST, dockDt_.talkNo_))
      {
        ChangePage(pg_Out);
      }
    }
    // 喋り終わった
    else if (ugserifu_.GetActRes() == enUGSerifuAct_ReadEnd)
    {
      ugdock_->ugclerk_.SetMType(cmt_Stand);
    }

  }
  void TLyTuCDock::MvPage_Out()
  {
    if (ugdock_->ugdoor_.IsActRes(enUGDoorAct_Half))
    {
      ugkantoku_->SetDraw(FALSE);
      ugcap_->SetDraw(FALSE);
    }
    else if (ugdock_->ugdoor_.IsActRes(enUGDoorAct_End))
    {
      Down();
    }

    //if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))
    //{
    //  ugdock_->ugclerk_.SetMType(cmt_Stand);
    //}
    //if (ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    //{
    //  Down();
    //}

    //if (ugserifu_.IsAct())
    //{
    //  if (ugserifu_.NextSerifu()) Down();
    //}
  }
  void TLyTuCDock::MvPage_Error()
  {
    if (IsHudOK())
    {
      Post_TutoGousei();
    }
  }
}
