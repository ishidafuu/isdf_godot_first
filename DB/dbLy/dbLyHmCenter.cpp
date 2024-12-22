#include "dbLyHmCenter.h"
#include "dbSnHome.h"
//#include "dbUILayer.h"

namespace db
{
  const s32  DOCKNUM = 4;

  enum enPage
  {
    pg_Walking,
    pg_Selected,
    pg_AddDock,
    pg_QDock,
    pg_InDock,
    pg_OutDock,
    pg_Maruka,
	pg_OutMaruka,
    pg_BBA,
    //pg_TutoGousei,
    pg_Error,
    pg_Out,
    pg_Idle,
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

  enum enConnect
  {
    cn_CheckMaruka,
    cn_AddDock,
    cn_QuickGousei,
    //cn_Reset,
  };

  const s32 HMC_START_PX = 97 + 18;
  const s32 HMC_START_PY = 158 + 35;

  //コンストラクタ
  TLyHmCenter::TLyHmCenter(TSnHome* pScene) :TLyHmGen(pScene)
    , ugkantoku_(this)
    , ugcap_(this)
    , ugbln_bba_(this)
    , ugbln_maruka_(this)
    , ugcl_maruka_(this)
    , ugcl_bba_(this)
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

  TLyHmCenter::~TLyHmCenter()
  {
    for (s32 i = 0; i < DOCKNUM; ++i)
    {
      Freemem(ugdock_[i]);
    }
  }

  void TLyHmCenter::SetupCenter(s32 centerNo)
  {
	  // ここでショップ内ジャンプのパラメータを取得する
	  INTVECTOR v;
	  switch (centerNo)
	  {
		  default:
			  break;
		  // ばばあ
		  case 2:
			  ugkantoku_.z_MoveWork_Full(ds_BBA, 172, 98 + 60, 2, 197);

			  //監督
			  ugkantoku_.SetPos_Full(172, 98 + 60);
			  ugkantoku_.SetMType(enKantokuMotionType::kmt_St);

			  // キャプテン
			  ugcap_.SetWalkType(cwt_MYWALK);
			  ugcap_.SetPos_Full(172 - 26, 98 + 60);
			  ugcap_.SetWalkType(cwt_FOLLOW);

			  SetFoot_None();

			  ChangePage(pg_BBA);
			  break;
	  }

	  // ジャンプが終わったらジャンプデータはクリアしておく
	  pmgEO_->stJump_.ClearJumpAreaData();
  }

  void TLyHmCenter::DoLoad()
  {
    //合成ドック
    for (s32 i = 0; i < DOCKNUM; ++i)
    {
      ugdock_[i]->MakeGroup(field_, hmgrp_->center_shutter_, hmgrp_->center_dockdoor_, i);
    }

    RefreshDock();

    ugcl_maruka_.MakeGroup(field_, ct_Maruka);
    ugcl_maruka_.SetPos_Full(97, 160);

    ugcl_bba_.MakeGroup(field_, ct_BBA);
    ugcl_bba_.SetDepth(UIDP_01BGOBJ);
    ugcl_bba_.SetPos_Full(197, 124);

    //ドア
    ugdoor_bba_.MakeGroup(field_, hmgrp_->center_bbadoor_, 187 - mid::midFullDotL_Hf(), 96 - mid::midFullDotS_Hf());
    ugdoor_bba_.GetField()->g_SetRevDepthLv(1);
    //ugdoor_bba_.SetDraw(FALSE);

    //バルーン
    ugbln_bba_.MakeGroup_Balloon(field_, "しゅーとおばば");
    ugbln_bba_.SetPos_Full(172, 98);

    ugbln_maruka_.MakeGroup_Balloon(field_, "あずかり");
    ugbln_maruka_.SetPos_Full(92, 98);

    ugsoukonum_have_.MakeGroup(field_, FALSE);
    ugsoukonum_have_.SetPos_Full(76, 70);
    ugsoukonum_inv_.MakeGroup(field_, TRUE);
    ugsoukonum_inv_.SetPos_Full(116, 70);

    //トラック
    track_ = MakeObj();
    track_->o_SetParent(field_);
    track_->o_SetObjGrp(hmgrp_->center_track_);
    track_->z_SetPos_Full(0, 112 - 16);

    pole_ = MakeObj();
    pole_->o_SetParent(field_);
    pole_->o_SetObjGrp(hmgrp_->center_pole_);
    pole_->z_SetPos_Full(171, 56);

    //監督
    ugkantoku_.MakeGroup(field_, FALSE);
    ugkantoku_.SetPos_Full(HMC_START_PX, HMC_START_PY);
    ugkantoku_.SetDraw(TRUE);
    s32 limX = -mid::midGetDotL_Hf();
    s32 limW = mid::midGetDotL_Hf();
    ugkantoku_.GetField()->g_SetLimX(limX, limW);

    // キャプテン
    ugcap_.MakeGroup(field_);
    ugcap_.SetPos_Full(HMC_START_PX - 26, HMC_START_PY);
    ugcap_.SetKantoku(&ugkantoku_);
    ugcap_.SetWalkType(cwt_FOLLOW);
    ugcap_.GetField()->g_SetLimX(limX, limW);

    //まるかうんそうのひょうじ
    //RefreshMaruka();
  }

  void TLyHmCenter::DoReady()
  {
    // 最後に選んだチームが変わっている可能性があるので
    // 再度設定しなおす
    ugcap_.SetupMgCharGrp();

    //まるかうんそうのひょうじ
    RefreshMaruka();
  }

  void TLyHmCenter::DoChangePage(s32 pageno)
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
    case db::pg_Walking:
      SetFoot_Tab();//タブ表示
      SetDockWait(FALSE);
      break;
    case db::pg_Selected:
      SetFoot_None();
      break;
    case db::pg_AddDock:
    {
      SetFoot_None();
      s32 dockprice = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(SPL_DOCKINV, mdm_supply_price);
      STRVECTOR strvec = { "けんきゅうしつを＿ついかします", "よろしいですか？" };
      SetHudMsgStar(&strvec, dockprice);
    }
    break;
    case db::pg_QDock:
    {
      SetFoot_None();
      s32 rest = (ugdock_[stCenter_.dockNo_]->stDock_.resttime_ / 10) + 1;
      s32 quickprice = (pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(SPL_GOUSEI, mdm_supply_price) * rest);
      STRVECTOR strvec = { "ミックスを＿かんりょうさせます", "よろしいですか？" };
      SetHudMsgStar(&strvec, quickprice);
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
      ugkantoku_.z_MoveWork_Full(ds_Init, HMC_START_PX, HMC_START_PY, 2);
      ugkantoku_.GetField()->g_SetLimX(limX, limW);
      ugcap_.SetParent(field_);
      ugcap_.SetPos_Full(224 + 2 + 20, 48 + (3 * 40) + 40);
      ugcap_.SetDraw(TRUE);
      ugcap_.SetWalkType(cwt_FOLLOW);
      ugcap_.GetField()->g_SetLimX(limX, limW);
      break;
    }
    case db::pg_Maruka:
      stCenter_.dockNo_ = NGNUM;
      SetDockWait(TRUE);
      pScene_->lyHmCMaruka_->Ready();
      break;
	case db::pg_OutMaruka:
		break;
    case db::pg_BBA:
      stCenter_.dockNo_ = NGNUM;
      SetDockWait(TRUE);
      pScene_->lyHmCBBA_->Ready();
      break;
      //case db::pg_TutoGousei:
      //  stCenter_.dockNo_ = NGNUM;
      //  SetDockWait(TRUE);
      //  pScene_->lyHmCTuto_->Ready();
      //  break;
    case db::pg_Out:
      break;
    case db::pg_Idle:
      break;
    default:
      break;
    }
  }

  void TLyHmCenter::SetDockWait(BOOL wait_f)
  {
    for (s32 i = 0; i < DOCKNUM; ++i)
    {
      if (stCenter_.dockNo_ == i) continue;
      ugdock_[i]->SetWait(wait_f);
    }
  }
  void TLyHmCenter::RefreshSoukoNum()
  {
    //倉庫表示リフレッシュ
    ugsoukonum_have_.Refresh();
    ugsoukonum_inv_.Refresh();
  }
  // ドックから出る時に呼んでもらう
  void TLyHmCenter::OutDock(void)
  {
    isLockPage_ = FALSE;
    ChangePage(pg_OutDock);
  }

  // まるか運送画面外へ
  void TLyHmCenter::SetOutMaruka(void)
  {
	  // 真実どっかいく
	  ugbln_maruka_.SetDraw(FALSE);	// ここでメニューボタンを非表示
	  ugcl_maruka_.SetMirror(TRUE);
	  f32 x = 0;
	  f32 y = 0;
	  ugcl_maruka_.GetField()->z_GetPos(&x, &y);
	  ugcl_maruka_.z_MoveWork(0, x - 100, y, 2, cmk_Left);		// 画面外まで移動
	  ugcl_maruka_.SetMType(cmt_Walk);

	  SetFoot_None();
	  ChangePage(pg_OutMaruka);
	  subPage_ = 0;
  }

  void TLyHmCenter::Idle()
  {
    ChangePage(pg_Idle);
  }


  //ドック更新
  void TLyHmCenter::RefreshDock()
  {
    s32 opendocknum = pmgEO_->mgCoU_.mdDock_.GetLineNum();
    if (pmgEO_->stSeq_.IsNoSv())  opendocknum = 2;

    for (s32 i = 0; i < DOCKNUM; ++i)
    {
      if (i < opendocknum)
        ugdock_[i]->SetEnable(pmgEO_->mgCoU_.mdDock_.GetLineDt(i, mddock_pk), FALSE);
      else
        ugdock_[i]->SetDisable((i == opendocknum));
    }
  }
  void TLyHmCenter::RefreshMaruka()
  {
    ugbln_maruka_.SetDraw(pmgEO_->stSeq_.IsDrawMaruka());
    track_->g_SetDraw(pmgEO_->stSeq_.IsDrawMaruka());
    ugcl_maruka_.SetDraw(pmgEO_->stSeq_.IsDrawMaruka());
  }

  void TLyHmCenter::MvPage_Walking()
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

	if (pmgEO_->stSeq_.IsNeedCheck() && !pmgEO_->stSeq_.IsNoSv())//イベント更新フラグチェック＆たたみ
    {
      Post_CheckMaruka();
    }
    else if (ugbln_bba_.IsAct())//ばばあ
    {
      ugkantoku_.z_MoveWork_Full(ds_BBA, 172, 98 + 60, 2, 197);
      ChangePage(pg_Selected);
    }
    else if (ugbln_maruka_.IsAct())//まるか
    {
      ugkantoku_.z_MoveWork_Full(ds_Maruka, 97 + 32, 160, 2, 97);
      ChangePage(pg_Selected);
    }
    else
    {
      for (s32 i = 0; i < DOCKNUM; ++i)
      {
        if (ugdock_[i]->IsAct())
        {
          switch (ugdock_[i]->GetDockState())
          {
          case enDockState_Empty: //空
            //case enDockState_Opening: //空
          case enDockState_Finish: //合成完了
            ugkantoku_.z_MoveWork_Full(ds_dock1 + i, 224 + 40 + 20, 48 + (3 * 40) + 40, 2, 224 + 40 + 1);
            ChangePage(pg_Selected);
            break;
          case enDockState_Gousei: //合成中
            stCenter_.dockNo_ = i;
            ChangePage(pg_QDock);
            break;
          case enDockState_NextOpen://新ドック解放
            stCenter_.dockNo_ = i;
            ChangePage(pg_AddDock);
            break;
          }
        }
      }
    }
  }
  void TLyHmCenter::MvPage_Selected()
  {
    switch (ugkantoku_.stChar_.moveNo_)
    {
    case ds_BBA:
      if (!ugkantoku_.IsActRes(enUGChGenAct_MoveStop)){ return; }
      ChangePage(pg_BBA);
      break;

    case ds_Maruka:
      if (!ugkantoku_.IsActRes(enUGChGenAct_MoveStop)){ return; }
      ChangePage(pg_Maruka);
      break;

    case ds_dock1:
    case ds_dock2:
    case ds_dock3:
    case ds_dock4:
    {
      if (!ugkantoku_.IsActRes(enUGChGenAct_MoveStop)){ return; }
      stCenter_.dockNo_ = ugkantoku_.stChar_.moveNo_ - ds_dock1;
      switch (ugdock_[stCenter_.dockNo_]->GetDockState())
      {
      case enDockState_Empty: //空
        //case enDockState_Opening: //空
        if (pmgEO_->mgCoU_.IsIntoGousei() || pmgEO_->stSeq_.IsNoSv())
        {
          ChangePage(pg_InDock);
          pScene_->lyHmCDock_->ReadyDock(ugdock_[stCenter_.dockNo_], stCenter_.dockNo_);
        }
        else
        {
          STRVECTOR strvec = { "ミックスできる", "シュートが＿ありません" };
          SetHudMsg(FALSE, FALSE, &strvec);
          ChangePage(pg_OutDock);
        }
        break;
      case enDockState_Gousei: //合成中
        ChangePage(pg_QDock);
        break;
      case enDockState_Finish: //合成完了
        ChangePage(pg_InDock);
        pScene_->lyHmCDock_->ReadyDock(ugdock_[stCenter_.dockNo_], stCenter_.dockNo_);
        break;
      case enDockState_NextOpen://新ドック解放
        ChangePage(pg_AddDock);
        break;
      }
    }
    break;
    }
  }
  void TLyHmCenter::MvPage_AddDock()
  {
    if (IsHudOK())
    {
      Post_AddDock();//ドック追加
    }
    else if (IsHudCancel())//きゃんせる
    {
      ChangePage(pg_Walking);
      //ChangePage(pg_OutDock);
    }
  }
  void TLyHmCenter::MvPage_QDock()
  {
    if (IsHudOK())
    {
      Post_QuickGousei();//急速合成
    }
    else if (IsHudCancel())//きゃんせる
    {
      ChangePage(pg_Walking);
      //ChangePage(pg_OutDock);
    }
  }
  void TLyHmCenter::MvPage_InDock()
  {

  }
  void TLyHmCenter::MvPage_OutDock()
  {
    if (!ugkantoku_.IsActRes(enUGChGenAct_MoveStop)){ return; }
    ChangePage(pg_Walking);
  }
  void TLyHmCenter::MvPage_Maruka()
  {

  }
  void TLyHmCenter::MvPage_OutMaruka()
  {
	  if (subPage_ == 0)
	  {
		  // おじさん画面外へ
		  if (ugcl_maruka_.GetActRes() == enUGChGenAct_MoveStop)
		  {
			  ugcl_maruka_.SetDraw(FALSE);
			  ugbln_maruka_.SetDraw(FALSE);

			  // トラック画面外へ
			  f32 tx = 0;
			  f32 ty = 0;
			  track_->z_GetPos(&tx, &ty);
			  s32 width = track_->b_GetAutoSize().width_;
			  track_->z_MoveSetspd(tx-width, ty, 2.0f);

			  subPage_++;			  
		  }
	  }
	  else if (subPage_ == 1)
	  {
		  if (track_->stSO_.stZahyou_.stopping_f_)
		  {
			  track_->g_SetDraw(FALSE);
			  Ready();
			  subPage_ = 0;
		  }
	  }
  }
  void TLyHmCenter::MvPage_BBA()
  {

  }
  void TLyHmCenter::MvPage_Error()
  {
    if (IsHudOK())
    {
      //ChangePage(pg_Walking);
      ChangePage(pg_OutDock);
    }
  }
  void TLyHmCenter::MvPage_Out()
  {

  }
  void TLyHmCenter::MvPage_Idle()
  {

  }

  //POST：まるかチェック
  void TLyHmCenter::Post_CheckMaruka()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_CheckMaruka();
	  }
	  else
	  {
		  StartConnect(cn_CheckMaruka);
		  pmgEO_->mgPo_.poHome_.Ready_checkdraw(pmgEO_->stSeq_.PopNeedMami(), pmgEO_->stSeq_.PopNeedMaruka());
	  }
  }
  //POST：ドック追加
  void TLyHmCenter::Post_AddDock()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_AddDock();
    }
    else
    {
      StartConnect(cn_AddDock);
      pmgEO_->mgPo_.poHmCenter_.Ready_adddock();
    }
  }
  //POST：急速
  void TLyHmCenter::Post_QuickGousei()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_QuickGousei();
    }
    else
    {
      StartConnect(cn_QuickGousei);
      pmgEO_->mgPo_.poHmCenter_.Ready_quickgousei(ugdock_[stCenter_.dockNo_]->GetIDDock());
    }
  }

  //接続：急速
  void TLyHmCenter::Connect_CheckMaruka()
  {
    
    pmgEO_->stSeq_.SetDrawFlag(
      pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_mami_f),
      pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_maruka_f));

    if (pmgEO_->mgCoInfo_.IsNewestInfo()) pmgEO_->stSeq_.PushNewInfo();

    //まるかうんそうのひょうじ
    RefreshMaruka();
  }

  //接続：ドック追加
  void TLyHmCenter::Connect_AddDock()
  {
    //最後に追加されたドックID
    s32 lastdock = (pmgEO_->mgCoU_.mdDock_.GetLineNum() - 1);
    s64 id_dock_last = pmgEO_->mgCoU_.mdDock_.GetLineDt(lastdock, mddock_pk);
    ugdock_[stCenter_.dockNo_]->SetEnable(id_dock_last, TRUE);

    //次の空いてないドック指定
    s32 opendocknum = pmgEO_->mgCoU_.mdDock_.GetLineNum();
    if (opendocknum < DOCKNUM)
    {
      ugdock_[opendocknum]->SetDisable(TRUE);
    }
    ChangePage(pg_Walking);
    //ChangePage(pg_OutDock);
  }
  //接続：急速
  void TLyHmCenter::Connect_QuickGousei()
  {
    ugdock_[stCenter_.dockNo_]->SetFinish();
    ChangePage(pg_Walking);
    //ChangePage(pg_OutDock);
  }

  //接続完了
  void TLyHmCenter::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_CheckMaruka: Connect_CheckMaruka(); break;
        case cn_AddDock: Connect_AddDock(); break;
        case cn_QuickGousei: Connect_QuickGousei(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        //ChangePage(pg_Error);
        ChangePage(pg_Walking);
      }
    }
    else
    {
      //ChangePage(pg_Error);
      ChangePage(pg_Walking);
    }
  }

  //lp_Walking
  void TLyHmCenter::DoUpdate(BOOL draw_f)
  {
    track_->Update(draw_f);//トラック
    pole_->Update(draw_f);//ポール
    ugdoor_bba_.Update(draw_f);
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
      ugcl_bba_.Update(draw_f);
      for (s32 i = 0; i < DOCKNUM; ++i) ugdock_[i]->Update(draw_f);
      break;
    case db::pg_AddDock:
    case db::pg_QDock:
    case db::pg_Maruka:
    case db::pg_BBA:
      //case db::pg_TutoGousei:
    case db::pg_Idle:
    case db::pg_Out:
      for (s32 i = 0; i < DOCKNUM; ++i) ugdock_[i]->Update(draw_f);
	  break;
	case db::pg_OutMaruka:
		ugkantoku_.Update(draw_f);
		ugcap_.Update(draw_f);
		for (s32 i = 0; i < DOCKNUM; ++i) ugdock_[i]->Update(draw_f);
		ugcl_maruka_.Update(draw_f);
		break;
    default:
      ugdoor_bba_.Update(draw_f);
      ugbln_maruka_.Update(draw_f);//まるかうんそう
      ugbln_bba_.Update(draw_f);//BBA
      ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      ugcl_maruka_.Update(draw_f);
      ugcl_bba_.Update(draw_f);
      for (s32 i = 0; i < DOCKNUM; ++i) ugdock_[i]->Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      if (pScene_->lyHmBar_->IsBarAction()) return;
      switch (stLy_.pageNo_)
      {
      case db::pg_Walking:MvPage_Walking(); break;
      case db::pg_Selected:MvPage_Selected(); break;
      case db::pg_AddDock:MvPage_AddDock(); break;
      case db::pg_QDock:MvPage_QDock(); break;
      case db::pg_InDock:MvPage_InDock(); break;
      case db::pg_OutDock:MvPage_OutDock(); break;
      case db::pg_Maruka:MvPage_Maruka(); break;
	  case db::pg_OutMaruka:MvPage_OutMaruka(); break;
      case db::pg_BBA:MvPage_BBA(); break;
      case db::pg_Out:MvPage_Out(); break;
      case db::pg_Error:MvPage_Error(); break;
      case db::pg_Idle:MvPage_Idle(); break;
      }
    }
  }


}
