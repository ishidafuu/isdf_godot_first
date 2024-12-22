#include "dbLyHmHome.h"
#include "dbSnHome.h"
#ifdef __K_MAKE__
#include "dbUGVisitManager.h"
#endif

namespace db
{
  enum enPage
  {
	pg_LonginBonus,
    pg_Walking,
    pg_Selected,
    pg_InMenu,
    pg_OutMami,	// まみ、画面外へ
    pg_Help,
    pg_Idle,
  };

  enum enHmHome
  {
    hh_GetLog,//かくとくりれき
    hh_DelLog,//しょうひりれき
    hh_Challenge,//ちゃれんじりすと
    hh_Reminder,//りまいんだー
    hh_Supply,//もちものりすと
    hh_Mobage,//もばげーさーくる
    hh_Syoutai,//しょうたい
    hh_Help,//へるぷ
    hh_Message,//めっせーじ　
    hh_GuildInfo,//ギルド案内所へのリンク　
    hh_Guild,//ギルドへのショートカット　
    hh_Campain,//何かのキャンペーン
    hhEND,
  };

  enum enConnect
  {
    cn_CheckMami,
    cn_Event,
  };

  enum enSetupHomeNo
  {
	  shn_None = 0,		// 間違って飛ばないように0にはとび先なしをセット
	  shn_Hasebe,
  };

  const s32 MAMIY = 136;
  const s32 MAMIYF = MAMIY - mid::midFullDotS_Hf();

  const s32 MAMI_OFFSETX = 20;
  const s32 MABEKO_OFFSETX = 56;
  const s32 HASEBE_OFFSETX = -50;

  //コンストラクタ
  TLyHmHome::TLyHmHome(TSnHome* pScene) :TLyHmGen(pScene)
    , ugkantoku_(this)
    , ugcap_(this)
    , ugbln_mami_(this)
    , ugbln_mabeko_(this)
    , ugmami_(this)
    , ugmabeko_(this)
    , uglogbo_(this)
    , uginfo_(this)
    , ugBanner_(this)
	, ugbln_hasebe_(this)
	, ughasebe_(this)
	, ugTmBoard_(this)
  {
    mamichecked_f_ = FALSE;
  }

  TLyHmHome::~TLyHmHome()
  {
#ifdef __K_MAKE__
    TUGVisitManager* pvm = TUGVisitManager::GetInstance();
    if (pvm)
    {
      pvm->Destroy();
      pvm = NULL;
    }
#endif
  }

  void TLyHmHome::SetOutMami(void)
  {
    // 真実どっかいく
    //ugbln_mami_.SetDraw(FALSE);	// ここでメニューボタンを非表示
    ugmami_.SetMirror(FALSE);
    f32 x = 0;
    f32 y = 0;
    ugmami_.GetField()->z_GetPos(&x, &y);
    ugmami_.z_MoveWork(0, x + 54, y, 2, cmk_Right);		// 柱裏まで移動
    ugmami_.SetMType(gmt_Wk);
    SetFoot_None();
    ChangePage(pg_OutMami);
  }

  void TLyHmHome::RefreshTopTeam(void)
  {
	  ugTmBoard_.SetTopTeamDt();
  }

  void TLyHmHome::DoLoad()
  {


    for (s32 i = 0; i < WINDOWNUM; ++i)
    {
      shadow_[i] = MakeObj();
      shadow_[i]->o_SetObjGrp(hmgrp_->home_shadow_);
      shadow_[i]->o_SetParent(field_);
      shadow_[i]->g_SetAlp(TRUE, 0x9A);

      window_[i] = MakeObj();
      window_[i]->o_SetObjGrp(hmgrp_->home_window_);
      window_[i]->o_SetParent(field_);
      window_[i]->g_SetOvOvj(shadow_[0]);
      window_[i]->g_SetAlp(TRUE, 0x30);
    }
    shadow_[0]->z_SetPos_Full(72 + 0, 44);
    shadow_[1]->z_SetPos_Full(72 + 104, 44);
    shadow_[2]->z_SetPos_Full(72 + 200, 44);
    window_[0]->z_SetPos_Full(75 + 0, 44);
    window_[1]->z_SetPos_Full(75 + 105, 44);
    window_[2]->z_SetPos_Full(75 + 201, 44);


    floorB_ = MakeObj();
    floorB_->o_SetObjGrPart(hmgrp_->home_bg_);
    floorB_->g_SetCenter(TRUE);
    floorB_->z_SetPos(0, -4);
    floorB_->o_SetParent(field_);
    floorB_->g_SetOvOvj(window_[0]);

    //boardL_ = MakeObj();
    //boardL_->o_SetObjGrp(hmgrp_->home_board_);
    //boardL_->o_SetParent(floorB_);
    //boardL_->z_SetPos_Full(117, 90);

    //boardR_ = MakeObj();
    //boardR_->o_SetObjGrp(hmgrp_->home_board_);
    //boardR_->o_SetParent(floorB_);
    //boardR_->z_SetPos_Full(277, 90);

    floorF_ = MakeObj();
    floorF_->o_SetObjGrPart(hmgrp_->home_bg_);
    floorF_->g_SetPartNo(1);
    floorF_->g_SetCenter(TRUE);
    floorF_->o_SetParent(floorB_);
    floorF_->g_SetCharLayer(MAMIYF + 1);

    //flower_ = MakeObj();
    //flower_->o_SetObjGrp(hmgrp_->home_flower_);
    //flower_->o_SetParent(floorF_);
    //flower_->z_SetPos_Full(200, 162);
    //flower_->g_SetCharLayer(flower_->stSO_.stZahyou_.y_ + hmgrp_->home_flower_->GetHeight() - 2);

    //challenge_ = MakeObj();
    //challenge_->o_SetObjGrp(hmgrp_->home_challenge_);
    //challenge_->o_SetParent(floorF_);
    //challenge_->z_SetPos_Full(196, 164);
    //challenge_->g_SetCharLayer(challenge_->stSO_.stZahyou_.y_ + hmgrp_->home_challenge_->GetHeight());

    //tv_ = MakeObj();
    //tv_->o_SetObjGrpPart(hmgrp_->home_tv_);
    //tv_->o_SetParent(challenge_);
    //tv_->z_SetPos(51, 9);
    //tv_->a_AddAnivec(0, 2, 4, at_Loop);//砂嵐
    //tv_->a_SetAniNo(0, TRUE);


    ugmabeko_.MakeGroup(field_, 10, FALSE);
    ugmabeko_.SetMirror(TRUE);
	ugmabeko_.SetPos_Full(167 + MABEKO_OFFSETX, MAMIY);

    ugmami_.MakeGroup(field_, 11, FALSE);
    ugmami_.SetPos_Full(279 + MAMI_OFFSETX, MAMIY);
    ugmami_.SetMirror(TRUE);

	ughasebe_.MakeGroup(field_, 12, FALSE);
	ughasebe_.SetMirror(FALSE);
	ughasebe_.SetPos_Full(167 + HASEBE_OFFSETX, MAMIY + 10);

    ugbln_mabeko_.MakeGroup_Balloon(field_, "うんえい");
	ugbln_mabeko_.SetPos_Full(124 + 32 + 12 + MABEKO_OFFSETX, 58 + 16);

    ugbln_mami_.MakeGroup_Balloon(field_, "あんない");
    ugbln_mami_.SetPos_Full(260 + 32 + MAMI_OFFSETX, 58 + 16);

	ugbln_hasebe_.MakeGroup_Balloon(field_, "チーム");
	ugbln_hasebe_.SetPos_Full(124 + 32 + 30 + HASEBE_OFFSETX, 58 + 16);

    ugkantoku_.MakeGroup(field_, FALSE);
    ugkantoku_.SetPos_Full(97 + 18, 158 + 35);

    // バナー
    ugBanner_.MakeGroup(field_);
    // パラメータ読み込みをここで

    //#ifdef __K_DEBUG_ROOT__
    //	TUGBannerParam param;
    //	param.SetupDebugParam1();
    //	ugBanner_.SetParam(param);
    //	param.SetupDebugParam2();
    //	ugBanner_.SetParam(param);
    //	param.SetupDebugParam3();
    //	ugBanner_.SetParam(param);
    //#else
    // データ読み込み
    s32 lineNum = pmgEO_->mgCoInfo_.mdm_banner_.GetLineNum();
    for (s32 i = 0; i < lineNum; i++)
    {
      TUGBannerParam param;
      STRVECTOR strVec;
      INTVECTOR animVec;
      INTVECTOR switchVec;
      s64 st_date = pmgEO_->mgCoInfo_.mdm_banner_.GetLineDt(i, enmdm_Banner::mdm_banner_st_date);
      s64 ed_date = pmgEO_->mgCoInfo_.mdm_banner_.GetLineDt(i, enmdm_Banner::mdm_banner_ed_date);

      //#ifdef __K_DEBUG_ROOT__
      //		st_date = mid::midNowTime() - 100;
      //		ed_date = mid::midNowTime() + 1000;
      //#endif

      param.SetBannerID(pmgEO_->mgCoInfo_.mdm_banner_.GetLineDt(i, enmdm_Banner::mdm_banner_pk));

      strVec.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetLineDtStr(i, enmdm_BannerStr::mdm_banner_text0_c128));
      strVec.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetLineDtStr(i, enmdm_BannerStr::mdm_banner_text1_c128));
      strVec.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetLineDtStr(i, enmdm_BannerStr::mdm_banner_text2_c128));
      param.SetText(strVec);

      animVec.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetLineDt(i, enmdm_Banner::mdm_banner_textAni0));
      animVec.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetLineDt(i, enmdm_Banner::mdm_banner_textAni1));
      animVec.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetLineDt(i, enmdm_Banner::mdm_banner_textAni2));
      param.SetTextDispType(animVec);

      switchVec.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetLineDt(i, enmdm_Banner::mdm_banner_switchA));
      switchVec.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetLineDt(i, enmdm_Banner::mdm_banner_switchB));
      switchVec.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetLineDt(i, enmdm_Banner::mdm_banner_switchC));
      param.SetDispChangeType(switchVec);

      param.SetDate(st_date, ed_date);

      ugBanner_.SetParam(param);
    }
    //#endif
    ugBanner_.SetTab();

    // キャプテン
    ugcap_.SetupMgCharGrp();
    ugcap_.MakeGroup(field_);
    ugcap_.SetPos_Full(97 + 18 - 26, 158 + 35);
    ugcap_.SetKantoku(&ugkantoku_);

    uglogbo_.MakeGroup(field_);
    uglogbo_.SetPos_Full(167 + 6 + 75 + MAMI_OFFSETX, MAMIY - 12);
    uglogbo_.Refresh();

    uginfo_.MakeGroup(field_);
	uginfo_.SetPos_Full(167 + 6 - 32 + MABEKO_OFFSETX, MAMIY - 12);

	// 所持最強チーム表示
	ugTmBoard_.MakeGroup(field_);
	ugTmBoard_.SetPos_Full(124 + 32+ 2 + HASEBE_OFFSETX, 88);


#ifdef __K_MAKE__
    TUGVisitManager* pvm = TUGVisitManager::Create(this, field_);
    if (pvm)
    {
      pvm->SetupVisitChara();
    }
#endif
  }
  void TLyHmHome::DoReady()
  {
    // 最後に使ったチームが変わっているかもなのでキャプテン更新
    ugcap_.SetupMgCharGrp();
    SetFoot_Tab();
    uginfo_.Refresh();
    mamichecked_f_ = FALSE;
    uglogbo_.Refresh();
    //RefreshHome();
    //pScene_->lyHmBar_->CreateFinger();

	ugTmBoard_.SetTopTeamDt();

#if FALSE
    // RefreshHome()で表示、非表示やってるのでここはコメントアウト

    // まみのところから出てきた時以外
    if (stLy_.pageNo_ != pg_InMenu)
    {
      // ここでうけとれるものがあるかチェックする
      s32 uketoriNum = 1;

      // まみとバルーンを表示するかチェック
      BOOL isDraw = FALSE;
      if (uketoriNum != 0){ isDraw = TRUE; }
      ugmami_.SetDraw(isDraw);
      ugbln_mami_.SetDraw(isDraw);
  }
#endif
}
  void TLyHmHome::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
	case db::pg_LonginBonus:
		break;
    case db::pg_Walking:
      break;
    case db::pg_Selected:
      SetFoot_None();
      break;
    case db::pg_InMenu:
      break;
    case db::pg_OutMami:
      break;
    case db::pg_Help:
      break;
    case db::pg_Idle:
      break;
    default:
      break;
    }
  }

  void TLyHmHome::SetupHome(s32 moveNo)
  {
		switch(moveNo)
		{
		case shn_Hasebe:
			ugkantoku_.z_MoveWork_Full(2, 131 + 18 + 40 + HASEBE_OFFSETX, 110 + 35 + 2, 2, 124 + 32);
			ChangePage(pg_Selected);
			break;
		}
  }

  void TLyHmHome::RefreshHome()
  {
    ugbln_mami_.SetDraw(pmgEO_->stSeq_.IsDrawMami());
    ugmami_.SetDraw(pmgEO_->stSeq_.IsDrawMami());
	ugmami_.SetPos_Full(279 + MAMI_OFFSETX, MAMIY);
    ugmami_.SetMirror(TRUE);
    uginfo_.Refresh();
    ugBanner_.Refresh();
    uglogbo_.Refresh();
  }

  void TLyHmHome::MvPage_LoginBonus()
  {
	  if (uglogbo_.IsLogobo() || uglogbo_.IsLogoboSp())
	  {
		  ugkantoku_.z_MoveWork_Full(1, 233 + 18, 110 + 35, 2, 260 + 32);
		  ChangePage(pg_Selected);
	  }
	  else
	  {
		  ChangePage(pg_Walking);
	  }
  }

  void TLyHmHome::MvPage_Walking()
  {
    if ((mamichecked_f_ == FALSE)
      && pmgEO_->stSeq_.IsNeedCheck() 
      && (pmgEO_->stSeq_.IsNoSv() == FALSE))
    {
      
      Post_CheckMami();
    }
    else if (pmgEO_->stSeq_.PopNeedEvent())//イベント更新フラグチェック＆たたみ
    {
      Post_Event();
    }
    else if (ugbln_mabeko_.IsAct())
    {
		ugkantoku_.z_MoveWork_Full(0, 131 + 18 + MABEKO_OFFSETX, 110 + 35, 2, 124 + 32);
	  ChangePage(pg_Selected);
    }
    else if (ugbln_mami_.IsAct())
    {
		ugkantoku_.z_MoveWork_Full(1, 233 + 18 + MAMI_OFFSETX, 110 + 35, 2, 260 + 32);
	  ChangePage(pg_Selected);
    }
	else if (ugbln_hasebe_.IsAct())
	{
		ugkantoku_.z_MoveWork_Full(2, 131 + 18 + 40 + HASEBE_OFFSETX, 110 + 35 + 2, 2, 124 + 32);
		ChangePage(pg_Selected);
	}
    else if (ugBanner_.IsAct())
    {
      // 一応クリアしておく
      pmgEO_->stJump_.ClearJumpAreaData();

      s32 id = ugBanner_.GetBannerID();
#ifdef __K_DEBUG_ROOT__
      //------------------------
      // ここでパラメータを取得
      INTVECTOR v;
      switch (id)
      {
	  case 0:
		v.push_back(0);	// ホームタブ
		v.push_back(1);	// はせべ
		break;

      case 1:
        v.push_back(4);	// ショップタブ
        v.push_back(0);	// ガチャショップ
        v.push_back(0); // スターガチャ
        v.push_back(0); // 1スターガチャ
        break;
      case 2:
        v.push_back(4);	// ショップタブ
        v.push_back(1);	// ガチャショップ
        v.push_back(2); // なつかしＢＯＸ
        break;
      case 3:
        v.push_back(4);	// ショップタブ
        v.push_back(2);	// ふくびきショップ
        v.push_back(0); // なにか
        break;
      }
      pmgEO_->stJump_.SetJumpAreaData(v);
      //------------------------
#else
      //------------------------
      // ここでパラメータを取得
      INTVECTOR v;
      v.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetPKDt(id, enmdm_Banner::mdm_banner_jumpNo0));
      v.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetPKDt(id, enmdm_Banner::mdm_banner_jumpNo1));
      v.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetPKDt(id, enmdm_Banner::mdm_banner_jumpNo2));
      v.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetPKDt(id, enmdm_Banner::mdm_banner_jumpNo3));
      v.push_back(pmgEO_->mgCoInfo_.mdm_banner_.GetPKDt(id, enmdm_Banner::mdm_banner_jumpNo4));
      pmgEO_->stJump_.SetJumpAreaData(v);
      //------------------------
#endif
      s32 tabNo = pmgEO_->stJump_.GetJumpTabNo();
      // 無効な値、ホームタブ時は飛ばない
      if (tabNo <= footer_id_home || tabNo > footer_id_event)
      {
		// ヘルプ
		if (tabNo == footer_id_help)
        {
          s32 cateNo = pmgEO_->stJump_.GetJumpCategoryNo();
          s32 helpNo = pmgEO_->stJump_.GetJumpData()[0];
          if (cateNo < 0){ cateNo = 0; }
          //5-helpType-その中の番号(0指定でhelpTypeリスト)
          if (helpNo == 0)
          {
            mid::midModalPanel_HelpList(&pmgEO_->mgCoM_.mdm_Help_.model_, cateNo);//パネル表示
          }
          else
          {
            mid::midModalPanel_HelpSingle(&pmgEO_->mgCoM_.mdm_Help_.model_, cateNo, (helpNo - 1));//パネル表示
          }
		}
		// ホーム
		else if(tabNo == footer_id_home)
		{
			s32 cateNo = pmgEO_->stJump_.GetJumpCategoryNo();
			SetupHome(cateNo);
		}

        pmgEO_->stJump_.ClearJumpAreaData();
        return;
      }

	  pmgEO_->stJump_.SetFromTab(footer_id_home);
      pScene_->lyHmBar_->JumpFooter(tabNo);
    }

    mamichecked_f_ = TRUE;

  }
  void TLyHmHome::MvPage_Selected()
  {
	  if (ugkantoku_.IsAct())
	  {
		  if (ugkantoku_.GetActRes() == enUGChGenAct_MoveStop)
		  {
			  //ドア開く
			  switch (ugkantoku_.stChar_.moveNo_)
			  {
			  default:
			  case 0://まべこ
				  {
					  pScene_->lyHmMaveko_->Ready();
					  f32 kantokuX = 0.0f;
					  f32 kantokuY = 0.0f;
					  ugkantoku_.GetField()->z_GetPos(&kantokuX, &kantokuY);

					  f32 mavekoX = 0.0f;
					  f32 mavekoY = 0.0f;
					  ugmabeko_.GetField()->z_GetPos(&mavekoX, &mavekoY);

					  if (kantokuX > mavekoX)
					  {
						  ugkantoku_.SetMirror(TRUE);
					  }
					  else
					  {
						  ugkantoku_.SetMirror(FALSE);
					  }
				  }
				  break;
			  case 1://まみ
				  pScene_->lyHmMami_->Ready();
				  ugkantoku_.SetMirror(FALSE);
				  break;
			  case 2:// はせべ
				  pScene_->lyHmHasebe_->Ready();
				  ugkantoku_.SetMirror(TRUE);
				  break; 
			  }
			  ChangePage(pg_InMenu);
		  }
	  }
  }
  void TLyHmHome::MvPage_InMenu()
  {

  }
  void TLyHmHome::MvPage_OutMami()
  {
    // 真実画面外へ(柱の後ろへ)
    if (ugmami_.GetActRes() == enUGChGenAct_MoveStop)
    {
      ugmami_.SetDraw(FALSE);
      ugbln_mami_.SetDraw(FALSE);
      Ready();
    }
  }
  void TLyHmHome::MvPage_Help()
  {
    //モーダルパネルが消えたらメニューに戻る
    if (mid::midIsDrawModalPanel() == FALSE)
    {
      ChangePage(pg_Walking);//メニューに戻る
    }
  }
  void TLyHmHome::MvPage_Idle()
  {

  }
  //POST：まみチェック
  void TLyHmHome::Post_CheckMami()
  {
    StartConnect(cn_CheckMami);
    pmgEO_->mgPo_.poHome_.Ready_checkdraw(pmgEO_->stSeq_.PopNeedMami(), pmgEO_->stSeq_.PopNeedMaruka());
  }
  //POST：イベント更新
  void TLyHmHome::Post_Event()
  {
    StartConnect(cn_Event);
    pmgEO_->mgPo_.poHmShiai_.Ready_refreshevent();
  }
  void TLyHmHome::Connect_CheckMami()
  {
    pmgEO_->stSeq_.SetDrawFlag(
      pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_mami_f),
      pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_maruka_f));
    if (pmgEO_->mgCoInfo_.IsNewestInfo()) pmgEO_->stSeq_.PushNewInfo();

    RefreshHome();
  }
  void TLyHmHome::Connect_Event()
  {

  }
  //接続完了
  void TLyHmHome::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_CheckMami:Connect_CheckMami(); break;
        case cn_Event:Connect_Event(); break;
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
  void TLyHmHome::Idle()
  {
    ChangePage(pg_Idle);
  }

  void TLyHmHome::DoUpdate(BOOL draw_f)
  {
    floorB_->Update(draw_f);
    floorF_->Update(draw_f);
    //boardL_->Update(draw_f);
    //boardR_->Update(draw_f);
    //flower_->Update(draw_f);
    for (s32 i = 0; i < WINDOWNUM; ++i)
    {
      shadow_[i]->Update(draw_f);
      window_[i]->Update(draw_f);
    }

	ugTmBoard_.Update(draw_f);

    switch (stLy_.pageNo_)
    {
	case pg_LonginBonus:
		ugkantoku_.Update(draw_f);
		ugcap_.Update(draw_f);
		ugmami_.Update(draw_f);
		ugmabeko_.Update(draw_f);
		ughasebe_.Update(draw_f);
		uglogbo_.Update(draw_f);
		uginfo_.Update(draw_f);
		break;
    case pg_Selected:
      ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      ugmami_.Update(draw_f);
      ugmabeko_.Update(draw_f);
	  ughasebe_.Update(draw_f);
      uglogbo_.Update(draw_f);
      uginfo_.Update(draw_f);
      break;
    case pg_InMenu:
      ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      uglogbo_.Update(draw_f);
      uginfo_.Update(draw_f);
      break;
    case pg_OutMami:
      ugmami_.Update(draw_f);
      ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      uglogbo_.Update(draw_f);
      uginfo_.Update(draw_f);
      break;
    case pg_Help:
      ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      uglogbo_.Update(draw_f);
      uginfo_.Update(draw_f);
      break;
    case pg_Idle:
      break;
    default:
      ugbln_mabeko_.Update(draw_f);
      ugbln_mami_.Update(draw_f);
	  ugbln_hasebe_.Update(draw_f);
      ugkantoku_.Update(draw_f);
      ugcap_.Update(draw_f);
      ugmami_.Update(draw_f);
      ugmabeko_.Update(draw_f);
	  ughasebe_.Update(draw_f);
      uglogbo_.Update(draw_f);
      uginfo_.Update(draw_f);
      ugBanner_.Update(draw_f);
#ifdef __K_MAKE__
      TUGVisitManager* pvm = TUGVisitManager::GetInstance();
      if (pvm)
      {
        pvm->Update(draw_f);
      }
#endif
      break;
    }

    if (draw_f == FALSE)
    {
      if (pScene_->lyHmBar_->IsBarAction()) return;
      switch (stLy_.pageNo_)
      {
	  case pg_LonginBonus:MvPage_LoginBonus(); break;
      case pg_Walking:MvPage_Walking(); break;
      case pg_Selected:MvPage_Selected(); break;
      case pg_InMenu:MvPage_InMenu(); break;
      case pg_Help:MvPage_Help(); break;
      case pg_OutMami:MvPage_OutMami(); break;
      case pg_Idle:MvPage_Idle(); break;
      }
    }
  }


}
