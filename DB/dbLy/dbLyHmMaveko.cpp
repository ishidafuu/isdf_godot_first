#include "dbLyHmMaveko.h"
#include "dbSnHome.h"

namespace db
{

  enum enPage
  {
    pg_Visit,
    pg_Menu,
    pg_Help,
    pg_Info,
    pg_Code,
    //pg_Code_fr_input,
    //pg_Code_fr_decide,
    //pg_Code_fr_result,
    pg_Code_cp_input,
    pg_Code_cp_result,
    //pg_Invite,
    pg_GetLog,
    pg_GetLogEnd,
    pg_UseLog,
    pg_Circle,
    pg_Push,
    pg_GotoTitle,
    pg_Logout,
    pg_Debug,
    pg_Out,
  };

  enum enMenuList
  {
    ml_Info,
    ml_Code,//アップルＮＧ
    //ml_Invite,//いったんけす
    ml_GetLog,
    ml_UseLog,
    ml_Help,
    ml_Otoi,
    //    ml_Push,
    ml_Circle,
    ml_Upgrade,
    ml_Mobage,
    ml_GotoTitle,
    ml_Logout,
    ml_Debug,
    mlEND,
  };

  enum enConnect
  {
    cn_NewInfo,
    cn_FrCode,
    cn_FrCodeDecide,
    cn_CpCode,
    cn_GetLogRedy,
    cn_GetLog,
    cn_UseLogReady,
    cn_UseLog,
    cn_InviteMember,
    cn_Debug,
  };


  //コンストラクタ
  TLyHmMaveko::TLyHmMaveko(TSnHome* pScene)
    :TLyHmGen(pScene)
    , ugsp_maveko_(this)
    , uglistMenu_(this)
    , uglistCode_(this)
    , uglistInvite_(this)
    , uglistInviteMember_(this)
    , uglistDebug_(this)
    , uglistGetLog_(this)
    , uglistUseLog_(this)
    , ugprof_(this)
  {
    stMaveko_.Init();
  }

  TLyHmMaveko::~TLyHmMaveko()
  {
  }



  void TLyHmMaveko::DoLoad()
  {
    //field_ = MakeObj();

    //ショップフィールド全生成
    ugsp_maveko_.MakeGroup(&pScene_->lyHmHome_->ugmabeko_);

    //ここの設定はuglistにうつす
    uglistMenu_.MakeGroup(field_, FALSE);
    uglistCode_.MakeGroup(field_, FALSE);
    uglistInvite_.MakeGroup(field_, FALSE);
    uglistInviteMember_.MakeGroup(field_, FALSE);
    uglistGetLog_.MakeGroup(field_, FALSE);
    uglistUseLog_.MakeGroup(field_, FALSE);
    uglistDebug_.MakeGroup(field_, FALSE);
    ugprof_.MakeGroup(field_, FALSE);


    listvec_.clear();
    listvec_.push_back(&uglistMenu_);
    listvec_.push_back(&uglistCode_);
    listvec_.push_back(&uglistInvite_);
    listvec_.push_back(&uglistInviteMember_);
    listvec_.push_back(&uglistGetLog_);
    listvec_.push_back(&uglistUseLog_);
    listvec_.push_back(&uglistDebug_);

    //メニューレコード
    for (s32 i = 0; i < mlEND; ++i)
    {
      if (mid::midIsDebug() == FALSE)
      {
        if (i == ml_Debug) continue;//デバッグふさぐ
      }

      if (mid::midIsIOS())
      {
        if (i == ml_Code) continue;//コードふさぐ
      }

      //if (pmgEO_->mgCoU_.mdw_Version_.GetLineDt(0, mdw_version_circleID) == 0)//公式サークル未完成
      //{
      //  if (i == ml_Circle) continue;//サークルふさぐ
      //}


      TUGRcNormal* newrecord = (TUGRcNormal*)uglistMenu_.MakeObj_Record();
      newrecord->SetCommonID(i);//コードにゅうりょくをふさぐ都合、数字がずれるので
      switch (i)
      {
      case ml_Info:newrecord->ugname_.SetNameIcon("うんえいからの＿おしらせ", ugi_Info); break;
      case ml_Code:newrecord->ugname_.SetNameIcon("コードにゅうりょく", ugi_EditProf);  break;//いったんけす
        //case ml_Invite:newrecord->ugname_.SetNameIcon("ゲームに＿しょうたいする", ugi_Invite);  break;//いったんけす
      case ml_GetLog:newrecord->ugname_.SetNameIcon("アイテム＿かくとくりれき", ugi_GetLog); break;
      case ml_UseLog:newrecord->ugname_.SetNameIcon("もちもの＿しょうひりれき", ugi_UseLog); break;
      case ml_Help:newrecord->ugname_.SetNameIcon("ヘルプ", ugi_Nazo); break;
      case ml_Otoi:newrecord->ugname_.SetNameIcon("おといあわせ", ugi_Mobage); break;
        //      case ml_Push:newrecord->ugname_.SetNameIcon("プッシュつうち", ugi_); break;
      case ml_Circle:newrecord->ugname_.SetNameIcon("こうしき＿サークル", ugi_Mobage); break;
      case ml_Upgrade:newrecord->ugname_.SetNameIcon("かいいんアップグレード", ugi_Mobage); break;
      case ml_Mobage:newrecord->ugname_.SetNameIcon("Ｍｏｂａｇｅ", ugi_Mobage); break;
      case ml_GotoTitle:newrecord->ugname_.SetNameIcon("タイトルへ＿もどる", ugi_GotoTitle); break;
      case ml_Logout:newrecord->ugname_.SetNameIcon("ログアウト", ugi_Logout); break;
      case ml_Debug:newrecord->ugname_.SetNameIcon("心＿でばっぐ＿心", ugi_GiftBox); break;
        //default: newrecord->ugname_.SetNameIcon("あすどふ"); break;
      }
    }
    for (s32 i = 0; i < enPostDebugEND; ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)uglistDebug_.MakeObj_Record();
      switch (i)
      {
      case enPostDebug_AddItem:newrecord->ugname_.SetName("アイテムついか"); break;
      case enPostDebug_ZeroItem:newrecord->ugname_.SetName("アイテムぜんけし");  break;
      case enPostDebug_AddCredit:newrecord->ugname_.SetName("トッドスターついか"); break;
      case enPostDebug_ZeroCredit:newrecord->ugname_.SetName("トッドスターぜんけし");  break;
      case enPostDebug_DecStamina:newrecord->ugname_.SetName("スタミナだうん"); break;
      case enPostDebug_ZeroShot:newrecord->ugname_.SetName("しゅーとぜんけし");  break;
      case enPostDebug_AddShot:newrecord->ugname_.SetName("しゅーとついか");  break;
      case enPostDebug_ResetLogbo:newrecord->ugname_.SetName("ろぐぼリセット");  break;
      case enPostDebug_AddTeam:newrecord->ugname_.SetName("チームついか");  break;
      case enPostDebug_TeamLvMax:newrecord->ugname_.SetName("チームＬＶＭＡＸ");  break;
      case enPostDebug_TeamLvMin:newrecord->ugname_.SetName("チームＬＶＭＩＮ");  break;
      case enPostDebug_TeamChLvMax:newrecord->ugname_.SetName("キャラＬＶＭＡＸ");  break;
      case enPostDebug_TeamChLvMin:newrecord->ugname_.SetName("キャラＬＶＭＩＮ");  break;
      case enPostDebug_AddGiftBox:newrecord->ugname_.SetName("おくりものＢＯＸついか");  break;
      case enPostDebug_DelDock:newrecord->ugname_.SetName("どっくさくじょ");  break;
      case enPostDebug_ResetNaBox:newrecord->ugname_.SetName("なつかしＢＯＸリセット");  break;
      case enPostDebug_AddHaikei:newrecord->ugname_.SetName("はいけいついか"); break;
      case enPostDebug_AddBGM:newrecord->ugname_.SetName("ＢＧＭついか"); break;
      case enPostDebug_AddMap:newrecord->ugname_.SetName("マップついか"); break;
      case enPostDebug_DelHaikeiBGM:newrecord->ugname_.SetName("はいけいＢＧＭさくじょ"); break;
        //case enPostDebug_ResetTuto:newrecord->ugname_.SetName("チュートリアルリセット"); break;
      case enPostDebug_ResetTutoSub:newrecord->ugname_.SetName("サブチュートリセット"); break;
      case enPostDebug_DelKantoku:newrecord->ugname_.SetName("かんとくさくじょ"); break;
      }
    }

    //キャンペーンコードリスト
    RefreshCpCodeList();

  }

  //入店時の処理
  void TLyHmMaveko::DoReady()
  {
    stMaveko_.Init();
    uginfo_ = &pScene_->lyHmHome_->uginfo_;

    ugsp_maveko_.SetMsg(mvm_Visit);
    DrawList(NULL);
    ugbtn_back_->SetFont("もどる");
    ugbtn_ok_->SetFont("おーけー");
    ugbtn_prev_->SetFont("まえのページ");
    ugbtn_next_->SetFont("つぎのページ");
    ugbtn_back_->SetDraw(FALSE);
  }

  void TLyHmMaveko::DoDown()
  {
    pScene_->lyHmHome_->Ready();
  }

  void TLyHmMaveko::DrawList(TUGLiGen* drawlist)
  {
    for (s32 i = 0; i < listvec_.size(); ++i)
    {
      listvec_[i]->SetDraw(FALSE);
    }
    if (drawlist != NULL)
    {
      drawlist->SetDraw(TRUE);
      //drawlist->SetZeroPos();
    }
    ugbtn_back_->SetDraw(TRUE);
    ResetObj();
  }
  void TLyHmMaveko::ResetObj()
  {
    ugbtn_prev_->SetDraw(FALSE);
    ugbtn_next_->SetDraw(FALSE);
    ugbtn_ok_->SetDraw(FALSE);
    ugprof_.SetDraw(FALSE);
  }
  void TLyHmMaveko::SwitchLogPageBtn()
  {
    ugbtn_prev_->SetDraw((stMaveko_.logpage_ > 0));
    ugbtn_next_->SetDraw((stMaveko_.logpage_ < stMaveko_.endpage_));
  }
  void TLyHmMaveko::DoChangePage(s32 pageno)
  {
    switch ((enPage)pageno)
    {
    case db::pg_Visit:
      DrawList(&uglistMenu_);
      break;
    case db::pg_Menu:
      DrawList(&uglistMenu_);
      ugsp_maveko_.SetMsg(mvm_Menu);
      ugbtn_back_->SetDraw(TRUE);
      break;
    case db::pg_Help:
      mid::midModalPanel_HelpList(&pmgEO_->mgCoM_.mdm_Help_.model_, 0);//パネル表示
      break;
    case db::pg_Info:
    {
      ugsp_maveko_.SetMsg(mvm_Info);
      INTVECTOR infovec = pmgEO_->mgCoInfo_.GetSortInfo();
      mid::midModalPanel_Info(&pmgEO_->mgCoInfo_.mdm_Info_.model_, &infovec);//お知らせパネル表示
    }
    break;

    case db::pg_GetLog:
      DrawList(&uglistGetLog_);
      uglistGetLog_.Refresh();
      ugsp_maveko_.SetMsg(mvm_GetLog);
      SetHudEnd();
      break;
    case db::pg_GetLogEnd:
      uglistGetLog_.ChangeListEnd(TRUE);//リストエンドに切り替える
      ResetObj();
      break;
    case db::pg_UseLog:
      DrawList(&uglistUseLog_);
      uglistUseLog_.Refresh();
      ugsp_maveko_.SetMsg(mvm_UseLog);
      SetHudEnd();
      break;
    case db::pg_Circle:
      //ugsp_maveko_.SetMsg(mvm_Circle);
      mid::midJumpCircle(pmgEO_->mgCoM_.mdm_Client_.GetLineDt(0, mdm_client_mbgCircleID));//サークル
      break;
    case db::pg_Push:
      ugsp_maveko_.SetMsg(mvm_Push);
      break;
    case db::pg_Logout:
      //      ugsp_maveko_.SetMsg(mvm_Logout);
      mid::midMobageLogOut();//ログアウト
      break;
    case db::pg_GotoTitle:
    {
      STRVECTOR strvec = { "タイトルがめんに＿もどります", "よろしいですか？" };
      SetHudMsg(TRUE, FALSE, &strvec);
    }
    break;
    case db::pg_Debug:
      DrawList(&uglistDebug_);
      ugsp_maveko_.SetMsg(mvm_Debug);
      break;
    case db::pg_Out:
      UndrawBtn();
      ugsp_maveko_.SetMsg(mvm_Out);
      break;


    case db::pg_Code:
      ugsp_maveko_.SetMsg(mvm_Code);
      DrawList(&uglistCode_);
      uglistCode_.ChangeListEnd(FALSE);
      break;
    case db::pg_Code_cp_input:
      DrawList(NULL);
      frame_->SetHudInput(stMaveko_.codeketa_, TRUE);
      ugbtn_back_->SetDraw(FALSE);
      break;
    case db::pg_Code_cp_result:
      ugsp_maveko_.SetMsg(mvm_CodeResult);
      SetHudEnd();
      SetHudItemView_GetLog(FALSE, FALSE, TRUE);
      break;

    default:
      break;
    }

  }



  //入店
  void TLyHmMaveko::MvPage_Visit()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Out);
    }
    else if (ugsp_maveko_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      ChangePage(pg_Menu);
    }
    else if (uglistMenu_.IsAct())
    {
      ActListMenu();
    }
  }
  //メニュー
  void TLyHmMaveko::MvPage_Menu()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Out);//退出
    }
    else if (uglistMenu_.IsAct())
    {
      ActListMenu();
    }
  }

  //ヘルプ
  void TLyHmMaveko::MvPage_Help()
  {
    //モーダルパネルが消えたらメニューに戻る
    if (mid::midIsDrawModalPanel() == FALSE)
    {
      ChangePage(pg_Menu);//メニューに戻る
    }
  }

  //うんえいからの＿おしらせ
  void TLyHmMaveko::MvPage_Info()
  {
    //モーダルパネルが消えたらメニューに戻る
    if (mid::midIsDrawModalPanel() == FALSE)
    {
      ChangePage(pg_Menu);//メニューに戻る
    }
  }


  //アイテム＿かくとくりれき
  void TLyHmMaveko::MvPage_GetLog()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Menu);//メニューに戻る
    }
    //    else if (uglistGetLog_.IsActRes(enUGLiAct_Record))//いったんけす
    //    {
    //      //獲得詳細
    //      ChangePage(pg_GetLogEnd);
    //    }
    else if ((stMaveko_.logpage_ > 0) && ugbtn_prev_->IsAct())//前ページ
    {
      //アイテム＿かくとくりれき
      --stMaveko_.logpage_;
      Post_GetLog();
    }
    else if (ugbtn_next_->IsAct())//次ページ
    {
      //アイテム＿かくとくりれき
      ++stMaveko_.logpage_;
      Post_GetLog();
    }
  }
  void TLyHmMaveko::MvPage_GetLogEnd()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_GetLog);//もどる
    }
  }
  //もちもの＿しょうひりれき
  void TLyHmMaveko::MvPage_UseLog()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Menu);//メニューに戻る
    }
    else if ((stMaveko_.logpage_ > 0) && ugbtn_prev_->IsAct())//前ページ
    {
      //アイテム＿かくとくりれき
      --stMaveko_.logpage_;
      Post_UseLog();
    }
    else if (ugbtn_next_->IsAct())//次ページ
    {
      //アイテム＿かくとくりれき
      ++stMaveko_.logpage_;
      Post_UseLog();
    }
  }
  //公式サークル
  void TLyHmMaveko::MvPage_Circle()
  {
    ChangePage(pg_Menu);//メニューに戻る
  }
  //プッシュつうち
  void TLyHmMaveko::MvPage_Push()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Menu);//メニューに戻る
    }
  }
  //タイトルへ
  void TLyHmMaveko::MvPage_GotoTilte()
  {
    if (IsHudOK())//おーけー
    {
      Down();
      pScene_->lyHmBar_->SetFilterAll(FALSE);
      pScene_->SetNextTitle();
    }
    else if (IsHudCancel())//きゃんせる
    {
      ChangePage(pg_Menu);//メニューに戻る
    }
  }
  //ログアウト
  void TLyHmMaveko::MvPage_Logout()
  {
    switch (mid::midResLogout())
    {
    case NGNUM:

      break;
    case 0:
      //ログアウト
      Down();
      pScene_->lyHmBar_->SetFilterAll(FALSE);
      pScene_->SetNextTitle();
      break;
    case 1:
      //ログアウトキャンセル
      ChangePage(pg_Menu);//メニューに戻る
      break;
    }

  }
  //デバッグ
  void TLyHmMaveko::MvPage_Debug()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Menu);//メニューに戻る
    }
    else if (uglistDebug_.IsAct())
    {
      //雑然としてるのでPost関数は用意しない
      StartConnect(cn_Debug);
      pmgEO_->mgPo_.poDebug_.Ready((enPostDebug)uglistDebug_.GetActIndex());
    }

  }

  void TLyHmMaveko::MvPage_Out()
  {
    if (ugsp_maveko_.IsAct())
    {
      Down();
    }
  }


  //コードにゅうりょく
  void TLyHmMaveko::MvPage_Code()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Menu);//メニューに戻る
    }
    else if (uglistCode_.IsAct())
    {
      s32 cpcode = uglistCode_.records_[uglistCode_.GetActIndex()]->GetCommonID();
      //if (cpcode == 0)
      //{
      //  ChangePage(pg_Code_fr_input);//友達コード入力画面へ
      //}
      //else
      {
        stMaveko_.mid_cpcode_ = uglistCode_.records_[uglistCode_.GetActIndex()]->GetCommonID();
        stMaveko_.codeketa_ = pmgEO_->mgCoM_.mdm_Campaign_.GetPKDt(stMaveko_.mid_cpcode_, mdm_campaign_inputLen);
        ChangePage(pg_Code_cp_input);//それ以外のコード入力画面へ
      }
    }
  }
  void TLyHmMaveko::MvPage_Code_cp_input()
  {
    //キャンペーンコード入力
    if (frame_->IsHudCancel())
    {
      ChangePage(pg_Code);//もどる
    }
    else if (frame_->IsHudOK())
    {
      //POSTキャンペーンコード
      inputcode_ = frame_->HudOutputString();
      Post_CpCode();
    }
  }
  void TLyHmMaveko::MvPage_Code_cp_result()
  {
    //アイテム獲得ボタン
    if (IsHudItemView())
    {
      ChangePage(pg_Code);//もどる
    }
  }


  //POSTフレンドコード
  void TLyHmMaveko::Post_NewInfo()
  {
    StartCheckOut(coInfo);
    pmgEO_->mgCoInfo_.StartConnect();
    //if (pmgEO_->stSeq_.IsNoSv()) Connect_FrCode();
  }

  //POSTかくとくログ（最初）
  void TLyHmMaveko::Post_GetLogReady()
  {
    StartConnect(cn_GetLogRedy);
    //通信はページの更新と変わらない
    pmgEO_->mgPo_.poHome_.Ready_getitemlog(stMaveko_.logpage_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_GetLogReady();
  }
  //POSTかくとくログページの更新
  void TLyHmMaveko::Post_GetLog()
  {
    StartConnect(cn_GetLog);
    pmgEO_->mgPo_.poHome_.Ready_getitemlog(stMaveko_.logpage_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_GetLog();
  }
  //POST使用ログ（最初）
  void TLyHmMaveko::Post_UseLogReady()
  {
    StartConnect(cn_UseLogReady);
    pmgEO_->mgPo_.poHome_.Ready_useitemlog(stMaveko_.logpage_);
    if (pmgEO_->stSeq_.IsNoSv()) Connect_UseLogReady();
  }
  //POST使用ログ
  void TLyHmMaveko::Post_UseLog()
  {
    StartConnect(cn_UseLog);
    pmgEO_->mgPo_.poHome_.Ready_useitemlog(stMaveko_.logpage_);
    if (pmgEO_->stSeq_.IsNoSv()) Connect_UseLog();
  }
  //POST招待メンバー
  void TLyHmMaveko::Post_InviteMember()
  {

  }
  //POSTの結果から最終ページ番号取得
  void TLyHmMaveko::SetEndPage()
  {
    stMaveko_.endpage_ = pmgEO_->mgPo_.GetValue("endpage");
  }


  //POSTキャンペーンコード
  void TLyHmMaveko::Post_CpCode()
  {
    StartConnect(cn_CpCode);
    pmgEO_->mgPo_.poHome_.Ready_cpcode(stMaveko_.mid_cpcode_, inputcode_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_CpCode();
  }




  //*******************
  //接続新規おしらせ
  void TLyHmMaveko::Connect_NewInfo()
  {
    ChangePage(pg_Info);
  }

  //接続獲得ログ（最初）
  void TLyHmMaveko::Connect_GetLogReady()
  {
    SetEndPage();
    ChangePage(pg_GetLog);
    SwitchLogPageBtn();
  }
  //接続獲得ログ
  void TLyHmMaveko::Connect_GetLog()
  {
    //リストリフレッシュ
    uglistGetLog_.Refresh();
    SwitchLogPageBtn();
    SetHudEnd();
  }
  //接続使用ログ（最初）
  void TLyHmMaveko::Connect_UseLogReady()
  {
    SetEndPage();
    //１ページ目と全体数読み込めたら表示ページに移る
    ChangePage(pg_UseLog);
    SwitchLogPageBtn();
  }
  //接続使用ログ
  void TLyHmMaveko::Connect_UseLog()
  {
    uglistUseLog_.Refresh();
    SwitchLogPageBtn();
    SetHudEnd();
    //リストリフレッシュ
  }
  //接続招待メンバー
  void TLyHmMaveko::Connect_InviteMember()
  {

  }
  //デバッグ
  void TLyHmMaveko::Connect_Debug()
  {
    //ヘッダリフレッシュ
    pScene_->lyHmBar_->RefreshHeadder();
  }
  
  //接続キャンペーンコード
  void TLyHmMaveko::Connect_CpCode()
  {
    ChangePage(pg_Code_cp_result);
  }

  void TLyHmMaveko::DoCheckOut(BOOL rqok_f)
  {
    if (rqok_f)
    {
      switch (stLy_.checkout_)
      {
      case coInfo: Connect_NewInfo(); break;
      }
    }
    else
    {
      SetHudEnd();
      ChangePage(pg_Menu);
    }
  }
  //接続完了
  void TLyHmMaveko::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        //case cn_FrCode: Connect_FrCode(); break;
        //case cn_FrCodeDecide: Connect_FrCodeDecide(); break;
        case cn_CpCode: Connect_CpCode(); break;
        case cn_GetLogRedy: Connect_GetLogReady(); break;
        case cn_GetLog: Connect_GetLog(); break;
        case cn_UseLogReady: Connect_UseLogReady(); break;
        case cn_UseLog: Connect_UseLog(); break;
        case cn_InviteMember: Connect_InviteMember(); break;
        case cn_Debug: Connect_Debug(); break;
        }
      }
      else
      {

        switch (stLy_.connect_)
        {
        //case cn_FrCode: ChangePage(pg_Code); break;
        //case cn_FrCodeDecide: ChangePage(pg_Code); break;
        case cn_CpCode: ChangePage(pg_Code); break;
        case cn_GetLogRedy: ChangePage(pg_Menu); break;
        case cn_GetLog: break;
        case cn_UseLogReady: ChangePage(pg_Menu); break;
        case cn_UseLog:  break;
        case cn_InviteMember: ChangePage(pg_Menu); break;
        case cn_Debug: ChangePage(pg_Menu); break;
        }
        //エラーメッセージ(ここでstLy_.connect_のクリアが入ってしまう)
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
      }
    }
    else
    {
      //通信エラー

      SetHudEnd();
      ChangePage(pg_Menu);
    }
  }
  //*******************

  void TLyHmMaveko::DoUpdate(BOOL draw_f)
  {
    switch (stLy_.pageNo_)
    {
    case pg_Out: MvPage_Visit();
      ugsp_maveko_.Update(draw_f);
      break;
    default:
      ugsp_maveko_.Update(draw_f);
      uglistMenu_.Update(draw_f);
      uglistCode_.Update(draw_f);
      uglistInvite_.Update(draw_f);
      uglistInviteMember_.Update(draw_f);
      uglistGetLog_.Update(draw_f);
      uglistUseLog_.Update(draw_f);
      uglistDebug_.Update(draw_f);
      ugprof_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Visit: MvPage_Visit(); break;
      case pg_Menu: MvPage_Menu(); break;
      case pg_Help: MvPage_Help(); break;
      case pg_Info: MvPage_Info(); break;

      case pg_GetLog: MvPage_GetLog(); break;
      case pg_GetLogEnd: MvPage_GetLogEnd(); break;
      case pg_UseLog: MvPage_UseLog(); break;
      case pg_Circle: MvPage_Circle(); break;
      case pg_Push: MvPage_Push(); break;
      case pg_GotoTitle: MvPage_GotoTilte(); break;
      case pg_Logout: MvPage_Logout(); break;
      case pg_Debug: MvPage_Debug(); break;
      case pg_Out: MvPage_Out(); break;

        case pg_Code: MvPage_Code(); break;
        case pg_Code_cp_input: MvPage_Code_cp_input(); break;
        case pg_Code_cp_result: MvPage_Code_cp_result(); break;
      }
    }

  }

  void TLyHmMaveko::ActListMenu()
  {
    switch ((enMenuList)uglistMenu_.GetCommonID())
    {
    case db::ml_Info:
      if (pmgEO_->stSeq_.PopNewInfo())
      {
        Post_NewInfo();
      }
      else
      {
        ChangePage(pg_Info);
      }

      break;
    case db::ml_Help:
      ChangePage(pg_Help); 
      break;
    case db::ml_Code:
      ChangePage(pg_Code);
      break;//アップルＮＧ
      //case db::ml_Invite:ChangePage(pg_Invite); break;//いったんけす
    case db::ml_GetLog:
      stMaveko_.logpage_ = 0;
      Post_GetLogReady();
      break;
    case db::ml_UseLog:
      stMaveko_.logpage_ = 0;
      Post_UseLogReady();
      break;
    case db::ml_Circle:ChangePage(pg_Circle); break;
    case db::ml_Otoi: mid::midOtoiawase(); break;//おといあわせ
      //      case db::ml_Push:ChangePage(pg_Push); break;
    case db::ml_Upgrade:mid::midMobageUpGrade2(); break;//会員アップグレード
    case db::ml_Mobage: mid::midGotoMobage(); break;//Mobage
    case db::ml_GotoTitle:ChangePage(pg_GotoTitle); break;
    case db::ml_Logout:ChangePage(pg_Logout); break;
    case db::ml_Debug:ChangePage(pg_Debug); break;
    }
  }



void TLyHmMaveko::RefreshCpCodeList()
{
  uglistCode_.ClearRecord();
  ////１行目は友達招待
  //if (pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_kantoku_inv) == 0)//未招待時のみ
  //{
  //  TUGRcNormal* newrecord = (TUGRcNormal*)uglistCode_.MakeObj_Record();
  //  newrecord->ugname_.SetNameIcon("しょうたいコード", ugi_Invite);
  //  newrecord->SetCommonID(0);
  //}

  //残りはキャンペーンコード
  for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Campaign_.GetLineNum(); ++i)
  {
      
    if (pmgEO_->mgCoM_.IsTimer(pmgEO_->mgCoM_.mdm_Campaign_.GetLineDt(i, mdm_campaign_id_m_timer)) == FALSE) continue;

    TUGRcNormal* newrecord = (TUGRcNormal*)uglistCode_.MakeObj_Record();
    newrecord->SetCommonID(pmgEO_->mgCoM_.mdm_Campaign_.GetLineDt(i, mdm_campaign_pk));
    newrecord->ugname_.SetNameIcon(pmgEO_->mgCoM_.mdm_Campaign_.GetLineDtStr(i, mdm_campaign_name_c64), ugi_GiftBox);
  }
}


//case db::pg_Code_fr_input:
//  DrawList(NULL);
//  frame_->SetHudInput(MBGIDKETA, TRUE);
//  ugbtn_back_->SetDraw(FALSE);
//  break;
//case db::pg_Code_fr_decide:
//  DrawList(NULL);
//  ugbtn_ok_->SetDraw(TRUE);
//  ugprof_.SetProfFriend();
//  ugprof_.SetDraw(TRUE);
//  ugsp_maveko_.SetMsg(mvm_FrCode);
//  break;
//case db::pg_Code_fr_result:
//  DrawList(NULL);
//  ugsp_maveko_.SetMsg(mvm_CodeResult);
//  SetHudEnd();
//  SetHudItemView_GetLog(FALSE, FALSE, FALSE);
//  break;

//case db::pg_Invite:
//  DrawList(&uglistInvite_);
//  ugsp_maveko_.SetMsg(mvm_Invite);
//  break;




//void TLyHmMaveko::MvPage_Code_fr_input()
//{
//  if (frame_->IsHudCancel())
//  {
//    ChangePage(pg_Code);//もどる
//  }
//  else if (frame_->IsHudOK())//確定ボタンにかえる
//  {
//    inputcode_ = frame_->HudOutputString();
//    //POSTフレンドコード
//    Post_FrCode();
//  }
//}
//void TLyHmMaveko::MvPage_Code_fr_decide()
//{
//  if (ugbtn_back_->IsAct())
//  {
//    ChangePage(pg_Code);//もどる
//  }
//  else if (ugbtn_ok_->IsAct())//確定ボタンにかえる
//  {
//    //POSTフレンドコード確定
//    Post_FrCodeDecide();
//  }
//}
//void TLyHmMaveko::MvPage_Code_fr_result()
//{
//  //アイテム獲得ボタン
//  if (IsHudItemView())
//  {
//    ChangePage(pg_Code);//もどる
//  }
//}


////ゲームに＿しょうたいする
//void TLyHmMaveko::MvPage_Invite()
//{
//  if (ugbtn_back_->IsAct())
//  {
//    ChangePage(pg_Code);//もどる
//  }
//}

////POSTフレンドコード
//void TLyHmMaveko::Post_FrCode()
//{
//  StartConnect(cn_FrCode);
//  pmgEO_->mgPo_.poHome_.Ready_frcode(inputcode_);
//  if (pmgEO_->stSeq_.IsNoSv()) Connect_FrCode();
//}
////POSTフレンドコード確定
//void TLyHmMaveko::Post_FrCodeDecide()
//{
//  StartConnect(cn_FrCodeDecide);
//  pmgEO_->mgPo_.poHome_.Ready_frcodedecide(inputcode_);
//  if (pmgEO_->stSeq_.IsNoSv()) Connect_FrCodeDecide();
//}


////接続フレンドコード
//void TLyHmMaveko::Connect_FrCode()
//{
//  ChangePage(pg_Code_fr_decide);
//}
////接続フレンドコード
//void TLyHmMaveko::Connect_FrCodeDecide()
//{
//  ChangePage(pg_Code_fr_result);
//}


//case pg_Code: MvPage_Code(); break;
//case pg_Code_fr_input: MvPage_Code_fr_input(); break;
//case pg_Code_fr_decide: MvPage_Code_fr_decide(); break;
//case pg_Code_fr_result: MvPage_Code_fr_result(); break;
//case pg_Code_cp_input: MvPage_Code_cp_input(); break;
//case pg_Code_cp_result: MvPage_Code_cp_result(); break;
//case pg_Invite: MvPage_Invite(); break;


}