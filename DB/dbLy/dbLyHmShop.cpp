#include "dbLyHmShop.h"
#include "dbSnHome.h"
//#include "dbUILayer.h"
#include "dbUGTipsManager.h"

namespace db
{
  enum enPage
  {
    pg_Walking,
    pg_Selected,
    pg_InShop,
    pg_GotoShop,
    pg_OutShop,
	pg_ShowTips,
    pg_Idle,
  };

  const s32 JKNUM = 1;

  const s32 DOORBY = 144;
  const s32 DOORBX[] = { 88, 232, 366 };

  enum enTouchPos
  {
	  touchPos_shop_item = 0,
	  touchPos_shop_gacya,
	  touchPos_shop_huku,

	  touchPos_Tips_00,
	  touchPos_Tips_01,
	  touchPos_Tips_02,

	  touchPos_Max,
  };


  //コンストラクタ
  TLyHmShop::TLyHmShop(TSnHome* pScene) :TLyHmGen(pScene),
    ugkantoku_(this)
    , ugcap_(this)
    , ugbln_item_(this)
    , ugbln_gacha_(this)
    , ugbln_huku_(this)
  {
    stHmShop_.Init();
    for (s32 i = 0; i < JKNUM; ++i) ugjk_.push_back(new TUGChGirl(this));
    for (s32 i = 0; i < SHOPNUM; ++i) ugdoor_.push_back(new TUGDoor(this));
  }

  TLyHmShop::~TLyHmShop()
  {
    for (s32 i = 0; i < JKNUM; ++i) Freemem(ugjk_[i]);
    for (s32 i = 0; i < SHOPNUM; ++i) Freemem(ugdoor_[i]);

	TUGTipsManager* ptm = TUGTipsManager::GetInstance();
	if (ptm)
	{
		ptm->Destroy();
		ptm = NULL;
	}
  }
  


  void TLyHmShop::DoLoad()
  {
    //監督
    ugkantoku_.MakeGroup(field_, FALSE);
    ugkantoku_.SetPos_Full(97 + 18, 158 + 35);

    ugcap_.MakeGroup(field_);	// キャプテン
    ugcap_.SetPos_Full(97 + 18 - 26, 158 + 35);
    ugcap_.SetKantoku(&ugkantoku_);

    //女子高生
    for (s32 i = 0; i < JKNUM; ++i) ugjk_[i]->MakeGroup(field_, i, TRUE);
    ugjk_[0]->SetWander(448 + 18, -40 + 18, 140 + 35);
    //ugjk_[1]->SetWander(-40 + 18, 448 + 18, 168 + 35);

    //店
    for (s32 i = 0; i < SHOPNUM; ++i)
    {
      shop_[i] = MakeObj();
      shop_[i]->o_SetParent(field_);
      shop_[i]->o_SetObjGrp(hmgrp_->shop_front_[i]);
      shop_[i]->b_SetBtn(TRUE);
    }
    shop_[0]->z_SetPos_Full(24, 56);
    shop_[1]->z_SetPos_Full(152, 64);
    shop_[2]->z_SetPos_Full(320, 56);
    //ドア
    ugdoor_[0]->MakeGroup(shop_[0], hmgrp_->shop_door_[0], 40, 40);
    ugdoor_[1]->MakeGroup(shop_[1], hmgrp_->shop_door_[1], 56, 32);
    ugdoor_[2]->MakeGroup(shop_[2], hmgrp_->shop_door_[2], 32, 40);

    //バルーン
    ugbln_item_.MakeGroup_Balloon(field_, "アイテム");
    ugbln_item_.SetPos_Full(56 + 32, 77 + 16);

    ugbln_gacha_.MakeGroup_Balloon(field_, "ガチャ");
    ugbln_gacha_.SetPos_Full(200 + 32, 77 + 16);

    ugbln_huku_.MakeGroup_Balloon(field_, "ふくびき");
    ugbln_huku_.SetPos_Full(328 + 32, 77 + 16);

    //ugbln_nazo_.MakeGroup_Balloon(field_, "なぞのみせ");
    //ugbln_nazo_.SetPos_Full(274 + 32, 156 + 14);

	TUGTipsManager* ptm = TUGTipsManager::Create(this, field_);
	if (ptm)
	{
		ptm->Setup();
	}
  }
  void TLyHmShop::DoReady()
  {
    // 最後に使ったチームが変わっているかもなのでキャプテン更新
    ugcap_.SetupMgCharGrp();
    SetFoot_Tab();
  }
  void TLyHmShop::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case db::pg_Walking:
      SetFoot_Tab();
      break;
    case db::pg_Selected:
      SetFoot_None();
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
	case db::pg_ShowTips:
		break;
    case db::pg_Idle:
      break;
    default:
      break;
    }
  }

  void TLyHmShop::SetupShop(s32 shopNo)
  {
    s32 shopdtpos = (shopNo - 1);

	  // 監督
    ugkantoku_.SetPos_Full(DOORBX[shopdtpos], DOORBY);
	  ugkantoku_.SetDraw(FALSE);
	  // キャプテン
    ugcap_.SetPos_Full(DOORBX[shopdtpos], DOORBY);
	  ugcap_.SetDraw(FALSE);
	  ugcap_.SetWalkType(cwt_MYWALK);

    stHmShop_.inshopNo_ = shopdtpos;

	  // ここでショップ内ジャンプのパラメータを取得する
	  INTVECTOR v;
	  switch (shopNo)
	  {
		  // ショップ
		  case 1:
			  pScene_->lyHmShopInside_->Ready();
			  pScene_->lyHmShopInside_->SetupMenu();
			  break;
			  // ガチャ
		  case 2:
			  pScene_->lyHmGachaInside_->Ready();
			  pScene_->lyHmGachaInside_->SetupMenu();
			  break;
		  // ふくびき
		  case 3:
			  pScene_->lyHmHukuInside_->Ready();
			  pScene_->lyHmHukuInside_->SetupMenu();
			  break;
	  }
	  // ジャンプが終わったらジャンプデータはクリアしておく
    pmgEO_->stJump_.ClearJumpAreaData();
	  ChangePage(pg_GotoShop);
  }
  void TLyHmShop::OutShop()
  {
    SetFoot_None();
    ugdoor_[stHmShop_.inshopNo_]->OpenDoor();
    subPage_ = 0;
    ChangePage(pg_OutShop);
  }
  void TLyHmShop::Idle()
  {
    ChangePage(pg_Idle);
  }

  void TLyHmShop::MvPage_Walking()
  {
	  // 歩いてるキャラを選択した
	  TUGTipsManager* tm = TUGTipsManager::GetInstance();
	  if (tm->IsSelect())
	  {
		  s32 no = tm->GetSelectNo();
		  f32 personX = tm->GetPersonPosX(no);
		  f32 personY = tm->GetPersonPosY(no);
		  f32 x = 0;
		  f32 y = 0;
		  ugkantoku_.GetField()->z_GetPos(&x, &y);
		  tm->SetStopPerson(no);
		  if (x > personX)
		  {
			  isMirror_ = TRUE;
			  ugkantoku_.z_MoveWork(touchPos_Tips_00 + no, personX + 32, personY, 2);
			  ChangePage(pg_Selected);
			  //tm->SetPersonFrameOutL(no);
		  }
		  else
		  {
			  isMirror_ = FALSE;
			  ugkantoku_.z_MoveWork(touchPos_Tips_00 + no, personX - 32, personY, 2);
			  ChangePage(pg_Selected);
			  //tm->SetPersonFrameOutR(no);
		  }
		  tipsPersonNo_ = no;
		  tm->SetDrawTipsBtnALL(FALSE);
		  return;
	  }

    //後ほど統合ボタンに変える
    if (shop_[0]->b_IsDecide())
    {
      ugkantoku_.z_MoveWork_Full(0, DOORBX[0], DOORBY, 2);
      ChangePage(pg_Selected);
    }
    else if (shop_[1]->b_IsDecide())
    {
      ugkantoku_.z_MoveWork_Full(1, DOORBX[1], DOORBY, 2);
      ChangePage(pg_Selected);
    }
    else if (shop_[2]->b_IsDecide())
    {
      ugkantoku_.z_MoveWork_Full(2, DOORBX[2], DOORBY, 2);
      ChangePage(pg_Selected);
    }
  }
  void TLyHmShop::MvPage_Selected()
  {
    if (ugkantoku_.IsActRes(enUGChGenAct_MoveStop))
    {
      switch (ugkantoku_.stChar_.moveNo_)
      {
      case 0:
      case 1:
      case 2:
        ChangePage(pg_InShop);
        break;
	  case touchPos_Tips_00:
	  case touchPos_Tips_01:
	  case touchPos_Tips_02:
		  {
			  s32 no = ugkantoku_.stChar_.moveNo_ - touchPos_Tips_00;
			  TUGTipsManager::GetInstance()->ShowTips(no, !isMirror_);
			  ugkantoku_.SetMirror(isMirror_);
			  ChangePage(pg_ShowTips);
		  }
		  break;
      }
    }
  }
  void TLyHmShop::MvPage_InShop()
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
        //ドア開く
        switch (stHmShop_.inshopNo_)
        {
        case 0:
          //店内へ
          pScene_->lyHmShopInside_->Ready();
          ChangePage(pg_GotoShop);
          break;
        case 1:
          //店内へ
          pScene_->lyHmGachaInside_->Ready();
          ChangePage(pg_GotoShop);
          break;
        case 2:
          //店内へ
          pScene_->lyHmHukuInside_->Ready();
          ChangePage(pg_GotoShop);
          break;
        default:
          //まだできてない
          OutShop();
          break;
        }
      }
      break;
    }
  }
  void TLyHmShop::MvPage_GotoShop()
  {

  }
  void TLyHmShop::MvPage_OutShop()
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
        SetFoot_Tab();
        ChangePage(pg_Walking);
      }
      break;
    }
  }
  void TLyHmShop::MvPage_ShowTips(void)
  {
	  TUGTipsManager* tm = TUGTipsManager::GetInstance();
	  if (tm->IsEndTips(tipsPersonNo_))
	  {
		  tm->CloseTips();
		  tm->SetDrawTipsBtnALL(TRUE);
		  if (ugkantoku_.IsMirror())
		  {
			  tm->SetPersonFrameOutL(tipsPersonNo_);
		  }
		  else
		  {
			  tm->SetPersonFrameOutR(tipsPersonNo_);
		  }
		  tipsPersonNo_ = NGNUM;
		  ChangePage(pg_Walking);
	  }
  }
  void TLyHmShop::MvPage_Idle()
  {

  }

  void TLyHmShop::DoUpdate(BOOL draw_f)
  {
    switch (stLy_.pageNo_)
    {
    case db::pg_GotoShop:
      //何も処理しない
      break;
    case db::pg_OutShop:
    case db::pg_Selected:
    case db::pg_InShop:
	case db::pg_ShowTips:
		TUGTipsManager::GetInstance()->Update(draw_f);
		ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      for (s32 i = 0; i < JKNUM; ++i) ugjk_[i]->Update(draw_f);
      for (s32 i = 0; i < SHOPNUM; ++i)
      {
        shop_[i]->Update(draw_f, TRUE);
        ugdoor_[i]->Update(draw_f);
      }
      break;
    case db::pg_Idle:
      for (s32 i = 0; i < SHOPNUM; ++i) shop_[i]->Update(draw_f, FALSE);
      break;
    default:
		TUGTipsManager::GetInstance()->Update(draw_f);
		ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      for (s32 i = 0; i < JKNUM; ++i) ugjk_[i]->Update(draw_f);
      for (s32 i = 0; i < SHOPNUM; ++i) shop_[i]->Update(draw_f, FALSE);
      ugbln_item_.Update(draw_f);
      ugbln_gacha_.Update(draw_f);
      ugbln_huku_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      if (pScene_->lyHmBar_->IsBarAction()) return;
      switch (stLy_.pageNo_)
      {
      case db::pg_Walking:MvPage_Walking(); break;
      case db::pg_Selected:MvPage_Selected(); break;
      case db::pg_InShop:MvPage_InShop(); break;
      case db::pg_GotoShop:MvPage_GotoShop(); break;
      case db::pg_OutShop:MvPage_OutShop(); break;
	  case db::pg_ShowTips:MvPage_ShowTips(); break;
      case db::pg_Idle:MvPage_Idle(); break;
      }
    }
  }


}
