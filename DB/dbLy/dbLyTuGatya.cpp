//utf8
#include "dbLyTuGatya.h"
#include "dbSnTutorial.h"
//=========================================================================
// 
// チュートリアル：ガチャ
//
//=========================================================================
namespace db
{
  enum enPage
  {
    pg_Init,

    pg_GachaInfo,
    pg_SelectTypeG,
    pg_SelectGacha,
	pg_CheckGacha,
    pg_DecideGacha,
    pg_GachaRoll,
    pg_GachaResult,		// 結果のウィンドウ
	pg_GachaResult2,	// 結果へのセリフ

    pg_NaBoxInfo,
    pg_SelectTypeN,
    pg_SelectNaBox,
    pg_SelectNaBoxNum,
    pg_DecideNaBox,
    pg_NaBoxRoll,
    pg_NaBoxResult,
	pg_NaBoxResult2,

    pg_LastInfo,
    pg_Out,
  };

  enum enGatyaType
  {
    gt_Star,
    gt_Box,
    gt_Todd,
    gtEND,
  };

  enum enConnect
  {
    cn_tutogatya,
    cn_tutonabox,
  };

  //-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
  TLyTuGatya::TLyTuGatya(TSnTutorial* pScene)
    :TLyTuBase(pScene)
    , ugsp_gacha_(this)
    , uglistMenu_(this)
    , uglistGacha_(this)
    , uglistNaBox_(this)
    , uglistNaBoxNum_(this)
  {
    stGacha_.Init();
  }

  //-----------------------------------------------------------------
  // デストラクタ
  //-----------------------------------------------------------------
  TLyTuGatya::~TLyTuGatya()
  {
  }

  //-----------------------------------------------------------------
  // ロード
  //-----------------------------------------------------------------
  void TLyTuGatya::DoLoad()
  {
    //ショップフィールド全生成
    ugsp_gacha_.MakeGroup();

    uglistMenu_.MakeGroup(field_, FALSE);
    uglistGacha_.MakeGroup(field_, FALSE);

    uglistNaBox_.MakeGroup(field_, FALSE);
    uglistNaBoxNum_.MakeGroup(field_, FALSE);

    listvec_.clear();
    listvec_.push_back(&uglistMenu_);
    listvec_.push_back(&uglistGacha_);
    listvec_.push_back(&uglistNaBox_);
    listvec_.push_back(&uglistNaBoxNum_);

    //トップメニュー
    for (s32 i = 0; i < gtEND; ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)uglistMenu_.MakeObj_Record();
      switch (i)
      {
      case gt_Star: newrecord->SetNameIcon("スターガチャ", ugi_StarGacha); break;
      case gt_Box: newrecord->SetNameIcon("なつかしＢＯＸ", ugi_NaBox); break;
      case gt_Todd: newrecord->SetNameIcon("トッドガチャ", ugi_Gacha); break;
      }
    }
  }

  //-----------------------------------------------------------------
  // 準備
  //-----------------------------------------------------------------
  void TLyTuGatya::DoReady()
  {
    // 基底クラスのDoReadyを呼び出す
    base::DoReady();
    pmgEO_->mgNSF_.Play(nsf_shop_gatya);//BGM
  }

  //-----------------------------------------------------------------
  // ページ切り替え
  //-----------------------------------------------------------------
  void TLyTuGatya::DoChangePage(s32 pageno)
  {
    stGacha_.tutomsgstep_ = 0;


    const s32 FNGBX = 88;
    const s32 FNGBY = -32;
    const s32 FNGDY = 24;
    const s32 FNGOKY = 52;

    switch ((enPage)pageno)
    {
    case db::pg_Init:
      break;
    case db::pg_GachaInfo:
      DrawList(&uglistMenu_);
      ugsp_gacha_.SetMsg(gamt_TuGachaVisit);
      break;
    case db::pg_SelectTypeG:
      DrawList(&uglistMenu_);
      uglistMenu_.records_[0]->SetDisable(TRUE);
      uglistMenu_.records_[1]->SetDisable(TRUE);
      uglistMenu_.records_[2]->SetDisable(FALSE);
      ugsp_gacha_.SetMsg(gamt_TuReady);
      ugfinger_.SetMotionTouch(FNGBX, (FNGBY + (FNGDY * 2)));
      ugfinger_.SetDraw(TRUE);
      break;
    case db::pg_SelectGacha:
      DrawList(&uglistGacha_);
      ugfinger_.SetMotionTouch(FNGBX, (FNGBY + (FNGDY * 1)));
      ugfinger_.SetDraw(TRUE);
      ugsp_gacha_.SetMsg(gamt_TuSelectTodd);
      ugsp_gacha_.uggacha_.SetGachaType(enUGGachaType_GachaTodd);
      uglistGacha_.Refresh(TRUE);//トッドガチャ
      uglistGacha_.ChangeListEnd(FALSE);
      ugr_todd_->Refresh();//トッドガチャ
      break;
	case db::pg_CheckGacha:
		DrawList(&uglistGacha_);
		ugfinger_.SetMotionTouch(FNGBX, FNGOKY);
		ugsp_gacha_.SetMsg(gamt_TuCheckTodd);
		uglistGacha_.ChangeListEnd(TRUE);
		ugr_todd_->Refresh();//トッドガチャ
		break;
    case db::pg_DecideGacha:
		DrawList(NULL);
		ugfinger_.SetDraw(FALSE);
		ugsp_gacha_.SetMsg(gamt_TuDecideTodd);
		SetHudEnd();
		break;
    case db::pg_GachaRoll:
      ugsp_gacha_.SetMsg(gamt_TuGacha);
      break;
    case db::pg_GachaResult:
      DrawList(NULL);
      SetHudItemView_GetLog(FALSE, FALSE, TRUE);
	  CheckGetLogTeam();
      break;
	case db::pg_GachaResult2:
	  DrawList(NULL);
	  ugsp_gacha_.SetMsg(gamt_TuBuy);
	  break;

    case db::pg_NaBoxInfo:
      DrawList(NULL);
      break;
    case db::pg_SelectTypeN:
      DrawList(&uglistMenu_);
      uglistMenu_.records_[0]->SetDisable(TRUE);
      uglistMenu_.records_[1]->SetDisable(FALSE);
      uglistMenu_.records_[2]->SetDisable(TRUE);
      ugsp_gacha_.SetMsg(gamt_TuNReady);
      ugfinger_.SetMotionTouch(FNGBX, (FNGBY + (FNGDY * 1)));
      ugfinger_.SetDraw(TRUE);
      break;
    case db::pg_SelectNaBox:
      DrawList(&uglistNaBox_);
      uglistNaBox_.Refresh();
      ugr_star_->Refresh();
      ugsp_gacha_.SetMsg(gamt_TuNSelectNaBox);
      ugfinger_.SetMotionTouch(FNGBX, (FNGBY + (FNGDY * 0)));
      ugfinger_.SetDraw(TRUE);
      break;
    case db::pg_SelectNaBoxNum:
      DrawList(&uglistNaBoxNum_);
      uglistNaBoxNum_.Refresh(stGacha_.mid_gacha_);
      uglistNaBoxNum_.ChangeListEnd(FALSE);
      ugr_star_->Refresh();
      ugsp_gacha_.SetMsg(gamt_TuNSelectNaBoxNum);
      ugfinger_.SetMotionTouch(FNGBX, (FNGBY + (FNGDY * 2)));
      break;
    case db::pg_DecideNaBox:
      DrawList(&uglistNaBoxNum_);
      uglistNaBoxNum_.SetListEnd(TRUE);//おとながい
      uglistNaBoxNum_.ChangeListEnd(TRUE);
      ugr_star_->Refresh();
      ugsp_gacha_.SetMsg(gamt_TuNDecideNaBox);
      ugfinger_.SetMotionTouch(FNGBX, (FNGOKY));
      break;
    case db::pg_NaBoxRoll:
      DrawList(NULL);
      ugfinger_.SetDraw(FALSE);
      ugsp_gacha_.SetMsg(gamt_TuNNaBox);
      SetHudEnd();
      break;
    case db::pg_NaBoxResult:
      DrawList(NULL);
      SetHudItemView_GetLog(FALSE, FALSE, TRUE);
      break;
	case db::pg_NaBoxResult2:
	  DrawList(NULL);
	  ugsp_gacha_.SetMsg(gamt_TuNBuy);
	  break;
    case db::pg_LastInfo:
      DrawList(NULL);
      ugsp_gacha_.SetMsg(gamt_TuLastInfo);
      ugbtn_back_->SetFont("もどる");
      ugbtn_back_->SetDraw(TRUE);
      break;
    case db::pg_Out:
      ugsp_gacha_.SetMsg(gamt_TuOut);
      UndrawBtn();
      break;
    default:
      break;
    }
    //stLy_.pageNo_ = pageno;
  }
  
  //-----------------------------------------------------------------
  // 接続
  //-----------------------------------------------------------------
  void TLyTuGatya::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_tutogatya: Connect_TuGatya(); break;
        case cn_tutonabox: Connect_TuNaBox(); break;
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
  // 更新
  //-----------------------------------------------------------------
  void TLyTuGatya::DoUpdate(BOOL draw_f)
  {
    base::DoUpdate(draw_f);

    switch (stLy_.pageNo_)
    {
    case pg_Out:
      ugsp_gacha_.Update(draw_f);
      break;
    default:
      uglistMenu_.Update(draw_f);
      uglistGacha_.Update(draw_f);
      uglistNaBox_.Update(draw_f);
      uglistNaBoxNum_.Update(draw_f);
      ugsp_gacha_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Init: MvPage_Init(); break;

      case pg_GachaInfo: MvPage_GachaInfo(); break;//次へのメッセージ
      case pg_SelectTypeG: MvPage_SelectTypeG(); break;//ガチャタイプ選択
      case pg_SelectGacha: MvPage_SelectGacha(); break;//ガチャ回数選択
	  case pg_CheckGacha: MvPage_CheckGacha(); break;//決定画面
      case pg_DecideGacha: MvPage_DecideGacha(); break;//決定画面
      case pg_GachaRoll: MvPage_GachaRoll(); break;//ガチャ
      case pg_GachaResult: MvPage_GachaResult(); break;//ガチャ結果
	  case pg_GachaResult2: MvPage_GachaResult2(); break;//ガチャ結果2

      case pg_NaBoxInfo: MvPage_NaBoxInfo(); break;//次へのメッセージ
      case pg_SelectTypeN: MvPage_SelectTypeN(); break;//ガチャタイプ選択
      case pg_SelectNaBox: MvPage_SelectNaBox(); break;//
      case pg_SelectNaBoxNum: MvPage_SelectNaBoxNum(); break;//大人選択
      case pg_DecideNaBox: MvPage_DecideNaBox(); break;//決定画面
      case pg_NaBoxRoll: MvPage_NaBoxRoll(); break;//ガチャ
      case pg_NaBoxResult: MvPage_NaBoxResult(); break;//ガチャ結果
	  case pg_NaBoxResult2: MvPage_NaBoxResult2(); break;//ガチャ結果2

      case pg_LastInfo: MvPage_LastInfo(); break;//最後メッセージ
      case pg_Out: MvPage_Out(); break;
      }
    }

#ifdef __K_DEBUG_ROOT__
    kdebug::DebugText::GetInstance()->SetColor(kdebug::COL_RED);
    kdebug::DebugText::GetInstance()->SetDebugText(10, 30, "ガチャ説明");
#endif
  }
  //-----------------------------------------------------------------
  // 終了
  //-----------------------------------------------------------------
  void TLyTuGatya::DoDown()
  {
    pScene_->lyTuShop_->OutShop();
    pmgEO_->mgNSF_.Play(nsf_home);//BGM
  }

  void TLyTuGatya::MvPage_Init()
  {
    //すでにガチャをクリアしている
//    if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_Gatya))
//    {
//      ChangePage(pg_NaBoxInfo);
//      ugsp_gacha_.SetMsg(gamt_TuNNaBoxVisit);//中断したときは普通にvisit
//    }
//    else
    if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Gatya) == FALSE)
    {
      ChangePage(pg_GachaInfo);
    }
    else
    {
      ChangePage(pg_NaBoxInfo);
      ugsp_gacha_.SetMsg(gamt_TuNNaBoxVisit);//中断したときは普通にvisit
    }
  }
  //ガチャ説明
  void TLyTuGatya::MvPage_GachaInfo()
  {
    if (ugsp_gacha_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      switch (stGacha_.tutomsgstep_)
      {
      case 0:ugsp_gacha_.SetMsg(gamt_TuGachaInfo); break;
      case 1:ChangePage(pg_SelectTypeG); break;
      }
      ++stGacha_.tutomsgstep_;
    }
  }
  //ガチャタイプ選択
  void TLyTuGatya::MvPage_SelectTypeG()
  {
    if (uglistMenu_.IsAct()) ChangePage(pg_SelectGacha);
  }
  //ガチャ個数選択
  void TLyTuGatya::MvPage_SelectGacha()
  {
    if (uglistGacha_.IsAct())//レコード選択
    {
      s32 recordidx = uglistGacha_.GetActIndex();
      uglistGacha_.SetListEnd(recordidx);
      stGacha_.mid_gacha_ = uglistGacha_.GetCommonID();//ガチャＭＩＤ
      ChangePage(pg_CheckGacha);
    }
  }
  //ガチャ買う確認画面
  void TLyTuGatya::MvPage_CheckGacha()
  {
	  if (uglistGacha_.uglistend_.IsActRes(enUGListEndAct_ok))
	  {
		  Post_TuGatya();//ガチャ開始通信
	  }
  }
  //ガチャ買う画面
  void TLyTuGatya::MvPage_DecideGacha()
  {
	  if (ugsp_gacha_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
	  {
		  ChangePage(pg_GachaRoll);
		  SetHudEnd();
	  }
  }
  //ガチャロール
  void TLyTuGatya::MvPage_GachaRoll()
  {
    //ガチャアニメーション終了
    if (ugsp_gacha_.IsActRes(enUGSpGachaAct_CupsuleOut))
    {
      SetHudItemView_GachaGet(ugsp_gacha_.GetActIndex());
    }
    else if (ugsp_gacha_.IsActRes(enUGSpGachaAct_GachaEnd))
    {
      ChangePage(pg_GachaResult);
    }
    //画面タップ
    else if (field_->sc_IsCamEnter2())
    {
      ugsp_gacha_.TapGachaField();
    }
  }
  //ガチャ結果
  void TLyTuGatya::MvPage_GachaResult()
  {
    if (IsHudItemView())
    {
		ChangePage(pg_GachaResult2);
    }
  }
  //ガチャ結果2
  void TLyTuGatya::MvPage_GachaResult2()
  {
	  if (ugsp_gacha_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
	  {
		  ChangePage(pg_NaBoxInfo);
		  ugsp_gacha_.SetMsg(gamt_TuNNaBoxInfo);//継続したときは普通にInfo
	  }
  }
  //なつかし説明
  void TLyTuGatya::MvPage_NaBoxInfo()
  {
    if (ugsp_gacha_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      switch (stGacha_.tutomsgstep_)
      {
      case 0:ugsp_gacha_.SetMsg(gamt_TuNNaBoxInfo2); break;
      case 1:ChangePage(pg_SelectTypeN); break;
      }
      ++stGacha_.tutomsgstep_;
    }
  }
  //ガチャタイプ選択
  void TLyTuGatya::MvPage_SelectTypeN()
  {
    if (uglistMenu_.IsAct()) ChangePage(pg_SelectNaBox);
  }
  //なつかし選択
  void TLyTuGatya::MvPage_SelectNaBox()
  {
    if (uglistNaBox_.IsAct())//レコード選択
    {
      stGacha_.mid_gacha_ = uglistNaBox_.GetCommonID();//なつかしＭＩＤ
      ChangePage(pg_SelectNaBoxNum);
    }
  }
  //なつかしの通常／大人買い選択
  void TLyTuGatya::MvPage_SelectNaBoxNum()
  {
    if (uglistNaBoxNum_.IsAct())//レコード選択
    {
      ChangePage(pg_DecideNaBox);
      //ugsp_gacha_.SetMsg(gamt_DecideNaBox);
    }
  }
  //なつかし買う画面
  void TLyTuGatya::MvPage_DecideNaBox()
  {
    if (uglistNaBoxNum_.uglistend_.IsActRes(enUGListEndAct_ok))
    {
      //通信に入る前に筐体のリフレッシュを掛ける
      ugsp_gacha_.NaBoxRefresh(stGacha_.mid_gacha_);
      Post_TuNaBoxOtona();//なつかしおとな開始通信
    }
  }
  //なつかしロール
  void TLyTuGatya::MvPage_NaBoxRoll()
  {
    //ガチャアニメーション終了
    if (ugsp_gacha_.IsAct())
    {
      ChangePage(pg_NaBoxResult);
    }
    //画面タップ
    else if (field_->sc_IsCamEnter())
    {
      ugsp_gacha_.TapBoxGachaField();
    }
  }
  //なつかし結果
  void TLyTuGatya::MvPage_NaBoxResult()
  {
    if (IsHudItemView())
    {
		ChangePage(pg_NaBoxResult2);
    }
  }
  //なつかし結果2
  void TLyTuGatya::MvPage_NaBoxResult2()
  {
	  if (ugsp_gacha_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
	  {
		  ChangePage(pg_LastInfo);
	  }
  }
  //おわりのことば
  void TLyTuGatya::MvPage_LastInfo()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Out);
    }
  }
  //退出
  void TLyTuGatya::MvPage_Out()
  {
    lib_num::AprTo0(&stGacha_.wait_c_);
    if (stGacha_.wait_c_ == 0)
    {
      Down();
    }
  }

  // チーム取得時にステータス更新を行う関数
  void TLyTuGatya::CheckGetLogTeam(void)
  {
	  s32 num = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineNum();
	  LONGVECTOR checkEndList;
	  checkEndList.clear();
	  for (s32 i = 0; i < num; i++)
	  {
		  // チームじゃなければ次へ
		  enItemType itemtype = (enItemType)pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_itemType);
		  if (itemtype != enItemType_Team){ continue; }

		  s64 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_itemID);
		  BOOL isCheckEnd = FALSE;
		  for (s32 j = 0; j < checkEndList.size(); j++)
		  {
			  // すでにステータス更新済みチーム
			  if (checkEndList[j] == itemid)
			  {
				  isCheckEnd = TRUE;
				  break;
			  }
		  }
		  // 更新済みチームだったら次へ
		  if (isCheckEnd){ continue; }

		  // チームのステータス更新
		  pmgEO_->RefreshStateTbl(itemid);
		  checkEndList.push_back(itemid);
	  }
  }

  //-----------------------------------------------------------------
  // ガチャ送信
  //-----------------------------------------------------------------

  void TLyTuGatya::Post_TuGatya()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_TuGatya();
    }
    else
    {
      // ここに送信処理を書く
      StartConnect(cn_tutogatya);
      pmgEO_->mgPo_.poTuto_.Ready_Gacha(stGacha_.mid_gacha_);
    }
  }
  //-----------------------------------------------------------------
  // なつかし送信
  //-----------------------------------------------------------------
  void TLyTuGatya::Post_TuNaBoxOtona()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_TuNaBox();
    }
    else
    {
      // ここに送信処理を書く
      StartConnect(cn_tutonabox);
      pmgEO_->mgPo_.poHmShop_.Ready_nabox_otona(stGacha_.mid_gacha_);
    }
  }
  //-----------------------------------------------------------------
  // ガチャ接続
  //-----------------------------------------------------------------
  void TLyTuGatya::Connect_TuGatya()
  {
    // メッセージを
    //ガチャアニメーションへ移行
    ChangePage(pg_DecideGacha);
    SetHudEnd();

#ifdef __K_DEBUG_USER_DATA__
    kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_Gatya);
#endif
  }
  //-----------------------------------------------------------------
  // なつかし接続
  //-----------------------------------------------------------------
  void TLyTuGatya::Connect_TuNaBox()
  {
    //ガチャアニメーションへ移行
    ChangePage(pg_NaBoxRoll);
    SetHudEnd();
#ifdef __K_DEBUG_USER_DATA__
    kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_Gatya);
#endif
  }

  //リストの描画
  void TLyTuGatya::DrawList(TUGLiGen* drawlist)
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

    ugbtn_back_->SetDraw(FALSE);
  }
}
