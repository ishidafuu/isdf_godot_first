#include "dbLyShResult.h"
#include "dbSnShiai.h"

namespace db
{
  enum enPage
  {
    pg_Result,
    pg_VSHumResult,
    pg_TourResult,
    pg_TutoRetroResult,
    pg_TutoSmartResult,
	pg_TimeAttackResult,
    pg_Out,
  };

  enum enConnect
  {
    cn_result,
    cn_vshumresult,
    cn_tourresult,
    cn_firsttutoresult,
	cn_timeattackresult,
  };

  //コンストラクタ
  TLyShResult::TLyShResult(TSnShiai* pScene)
    :TLyShGen(pScene)
    , ugresult_(this)
	, mid_team_drop_(NGNUM)
  {
  }

  TLyShResult::~TLyShResult()
  {
  }

  void TLyShResult::DoLoad()
  {
    field_->g_SetDepth(UIDP_02BGOBJHI);//手前

    filter_ = MakeObj();
    filter_->o_SetParent(field_);
    filter_->o_SetObjFilterGrp(mid::midFullDotL(), mid::midFullDotS(), 0x80, 0, 0, 0);
    filter_->g_SetARGB(TRUE, 0x80, 0x00, 0x00, 0x00);
    filter_->g_SetCenter(TRUE);
    //filter_->z_SetPos_SafeR(0, 0);

    ugresult_.MakeGroup(field_);
  }

  void TLyShResult::DoReady()
  {
	  mid_team_drop_ = NGNUM;

    //結果オブジェの準備
	  if (pmgEO_->stShiai_.IsTimeAttack())
	  {
		  ugresult_.RefreshTimeAttack();
	  }
	  else
	  {
		  ugresult_.Refresh();
	  }

    //レトロのチュートは何も表示しない
    if (pmgEO_->stSeq_.IsFirstTuto())
    {
      ugresult_.SetDraw(FALSE);
    }
#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
	// 対人戦
	else if (pmgEO_->stShiai_.IsVSHum())
	{
		s32 myside = mid::midGetMyIdx();

		s64 id_drop = NGNUM;
		// 対人
		if (!pmgEO_->stShiai_.IsDisConnect())	// 切れてない
		{
			// 勝ち
			if( (pScene_->pGame_->IsWin() && myside == SIDE0) ||
				(!pScene_->pGame_->IsWin() && myside == SIDE1) )
			{
				id_drop = pmgEO_->mgCoU_.mdVSDrop_.GetLineDt(0, enmdVSDrop::mdvsdrop_id_m_vsdrop_hw);
			}
			// 負け
			else
			{
				id_drop = pmgEO_->mgCoU_.mdVSDrop_.GetLineDt(0, enmdVSDrop::mdvsdrop_id_m_vsdrop_hl);
			}
		}
		// 対COM
		else
		{
			// 勝ち
			if ((pScene_->pGame_->IsWin() && myside == SIDE0) ||
				(!pScene_->pGame_->IsWin() && myside == SIDE1))
			{
				id_drop = pmgEO_->mgCoU_.mdVSDrop_.GetLineDt(0, enmdVSDrop::mdvsdrop_id_m_vsdrop_cw);
			}
			// 負け
			else
			{
				id_drop = pmgEO_->mgCoU_.mdVSDrop_.GetLineDt(0, enmdVSDrop::mdvsdrop_id_m_vsdrop_cl);
			}
		}
		s64 itemType = pmgEO_->mgCoM_.mdm_VSDrop_.GetPKDt(id_drop, enmdm_VSDrop::mdm_vsdrop_itemType);
		s64 itemID   = pmgEO_->mgCoM_.mdm_VSDrop_.GetPKDt(id_drop, enmdm_VSDrop::mdm_vsdrop_itemID);
		s64 itemNum  = pmgEO_->mgCoM_.mdm_VSDrop_.GetPKDt(id_drop, enmdm_VSDrop::mdm_vsdrop_itemNum);
		SetHudItemView_Drop( itemType, itemID, itemNum, NGNUM, NGNUM, FALSE);

		

		// チームだったらあとでステータス更新する為に覚えておく
		if(itemType == enItemType_Team)
		{
			mid_team_drop_ = itemID;
		}
	}
#endif

	//すでに確定していたやつを表示
	else if ((pmgEO_->stDrop_.IsDrop() == FALSE))
	{
		//アイテムが手に入らないとき
		ugbtn_back_->SetFont("もどる");
		ugbtn_back_->SetDraw(TRUE);
	}
    else
    {
		BOOL isDraw = FALSE;
		if (pmgEO_->stShiai_.IsTimeAttack())
		{
			
			if (pScene_->pGame_->IsWin())
			{
				isDraw = TRUE;
			}
		}
		else
		{
			isDraw = TRUE;
		}

		if (isDraw)
		{
			s64 itemType = pmgEO_->stDrop_.GetItemType(FALSE);
			s64 itemID = pmgEO_->stDrop_.GetItemID(FALSE);
			s64 itemNum = pmgEO_->stDrop_.GetItemNum(FALSE);

			SetHudItemView_Drop(
				itemType,
				itemID,
				itemNum,
				pmgEO_->stDrop_.GetTokkouNum(),
				pmgEO_->stDrop_.GetTokkouMag(),
				FALSE);

			// チームだったらここでステータス計算
			if (itemType == enItemType_Team)
			{
				// チームのステータス更新
				pmgEO_->RefreshStateTbl(itemID);
			}
		}
		else
		{
			//アイテムが手に入らないとき
			ugbtn_back_->SetFont("もどる");
			ugbtn_back_->SetDraw(TRUE);
		}
    }

  }
  void TLyShResult::DoDown()
  {
  }
  void TLyShResult::DoChangePage(s32 pageno)
  {

  }
  void TLyShResult::SwitchPostResult()
  {
    if (pmgEO_->stShiai_.IsVSHum())
    {
      Post_VSHumResult();
    }
    else if (pmgEO_->stShiai_.IsTour())
    {
      Post_TourResult();
    }
    else if (pmgEO_->stSeq_.IsFirstTuto())
    {
      Post_TutoResult();
    }
	else if (pmgEO_->stShiai_.IsTimeAttack())
	{
		Post_TimeAttackResult();
	}
    else
    {
      Post_Result();
    }
  }
  void TLyShResult::MvPage_Result()
  {
    //キャンセルボタンによって選択番号のマップを中心にするように移動、
    //パネル選択状態に状態遷移
    if (pmgEO_->stSeq_.IsFirstTuto() 
      //|| IsTuto()
      || ugbtn_back_->IsAct() 
      || IsHudItemView() 
      || IsHudOK())//まけたとき
    {
      SwitchPostResult();
    }
  }

  void TLyShResult::MvPage_Out()
  {

  }

  //POST：結果
  void TLyShResult::Post_Result()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Result();
    }
    else
    {
      StartConnect(cn_result);
      pmgEO_->mgPo_.poScenario_.Ready_result(
        pmgEO_->stResult_.GetVsResult(),
        pmgEO_->stResult_.GetKillnum(),
        pmgEO_->stResult_.GetDeathnum(),
        pmgEO_->stResult_.GetCleattime(),
        pmgEO_->stResult_.GetMaxdam(),
        pmgEO_->stResult_.GetTotaldam(),
        pmgEO_->stShiai_.GetCtrl());
    }
  }
  //対戦リザルト
  void TLyShResult::Post_VSHumResult()
  {
    StartConnect(cn_vshumresult);
    pmgEO_->mgPo_.poHmShiai_.Ready_vshumresult(
      pmgEO_->stResult_.GetVsResult(),
      pmgEO_->stResult_.GetKillnum(),
      pmgEO_->stResult_.GetDeathnum(),
      pmgEO_->stResult_.GetCleattime(),
      pmgEO_->stResult_.GetMaxdam(),
      pmgEO_->stResult_.GetTotaldam(),
      pmgEO_->stResult_.GetErrorCode(),
      pmgEO_->stShiai_.GetCtrl());
  }
  //かちぬきリザルト
  void TLyShResult::Post_TourResult()
  {
    StartConnect(cn_tourresult);
    pmgEO_->mgPo_.poHmShiai_.Ready_tourresult(
      pmgEO_->stResult_.GetVsResult(),
      pmgEO_->stResult_.GetKillnum(),
      pmgEO_->stResult_.GetDeathnum(),
      pmgEO_->stResult_.GetCleattime(),
      pmgEO_->stResult_.GetMaxdam(),
      pmgEO_->stResult_.GetTotaldam(),
      pmgEO_->stShiai_.GetCtrl());
  }
  //タイムアタックリザルト
  void TLyShResult::Post_TimeAttackResult()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_TimeAttackResult();
	  }
	  else
	  {
		  StartConnect(cn_timeattackresult);
		  pmgEO_->mgPo_.poHmShiai_.Ready_tmatresult(
			  pmgEO_->stResult_.GetVsResult(),
			  pmgEO_->stResult_.GetKillnum(),
			  pmgEO_->stResult_.GetDeathnum(),
			  pmgEO_->stResult_.GetCleattime(),
			  pmgEO_->stResult_.GetMaxdam(),
			  pmgEO_->stResult_.GetTotaldam(),
			  pmgEO_->stShiai_.GetCtrl());
	  }
  }
  void TLyShResult::Post_TutoResult()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
#ifdef __K_DEBUG_USER_DATA__
      kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_GameSousa);
#endif
      Connect_FirstTutoResult();
    }
    else
    {
      StartConnect(cn_firsttutoresult);
      pmgEO_->mgPo_.poTuto_.Ready_SetBit(enTutoBitType_GameSousa);
    }
  }

  //接続：結果
  void TLyShResult::Connect_Result()
  {
	  // チームのステータス更新
	  if (mid_team_drop_ != NGNUM)
	  {
		  pmgEO_->RefreshStateTbl(mid_team_drop_);
	  }

	  // この中身が入ってる時はランキングの時
	  if (pmgEO_->mgCoU_.mdre_Ranker_.GetLineNum())
	  {
		  pScene_->lyShBar_->MoveLayer(shl_Rank);
	  }
	  else
	  {
		  pScene_->lyShBar_->MoveLayer(shl_Map);
	  }

    //獲得ログにコンプアイテムが含まれる
    for (s32 i = 0; i < pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_getType) == GT_QUESTCOMP)
      {
        pmgEO_->stDrop_.PushCmp();
        break;
      }
    }

    SetHudEnd();
    ChangePage(pg_Out);
  }
  void TLyShResult::Connect_VSHumResult()
  {
    pScene_->lyShBar_->MoveLayer(shl_Home);//ホームに戻る
    SetHudEnd();
    ChangePage(pg_Out);
  }
  void TLyShResult::Connect_TourResult()
  {
    //pScene_->lyShBar_->MoveLayer(shl_Home);//ホームに戻る
	  pScene_->lyShBar_->MoveLayer(shl_EventHome);//ホームに戻る
    SetHudEnd();
    ChangePage(pg_Out);
  }
  void TLyShResult::Connect_FirstTutoResult()
  {
    pScene_->lyShBar_->MoveLayer(shl_Home);//ホームに戻る
    //pScene_->GoToTutorial();//チュートリアルに戻る
    SetHudEnd();
    ChangePage(pg_Out);
  }
  void TLyShResult::Connect_TimeAttackResult()
  {
	  // この中身が入ってる時はランキングの時
	  if (pmgEO_->mgCoU_.mdre_Ranker_.GetLineNum())
	  {
		  pScene_->lyShBar_->MoveLayer(shl_Rank);
	  }
	  else
	  {
		  pScene_->lyShBar_->MoveLayer(shl_EventHome);//ホームに戻る
	  }
	  SetHudEnd();
	  ChangePage(pg_Out);
  }

  //接続完了
  void TLyShResult::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_result: Connect_Result(); break;
        case cn_vshumresult: Connect_VSHumResult(); break;
        case cn_tourresult: Connect_TourResult(); break;
        case cn_firsttutoresult: Connect_FirstTutoResult(); break;
		case cn_timeattackresult: Connect_TimeAttackResult(); break;
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


  //パネルが選択された
  void TLyShResult::DoUpdate(BOOL draw_f)
  {
    filter_->Update(draw_f);
    ugresult_.Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Result: MvPage_Result(); break;
      case pg_Out: MvPage_Out(); break;
      }
    }
  }

}
