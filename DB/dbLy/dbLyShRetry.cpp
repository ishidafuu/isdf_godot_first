#include "dbLyShRetry.h"
#include "dbSnShiai.h"

namespace db
{
  const s32 TMNUM = 3;

  enum enPage
  {
    pg_Retry,
    pg_Error,
    pg_Out,
  };

  enum enConnect
  {
    cn_retry,
    cn_retrystar,
	  cn_retire,
	  cn_retiretimeattack,
  };

  //コンストラクタ
  TLyShRetry::TLyShRetry(TSnShiai* pScene) 
    :TLyShGen(pScene)
  {
    msgwait_c_ = 0;
  }
  
  TLyShRetry::~TLyShRetry()
  {
  }

  void TLyShRetry::DoLoad()
  {
    field_->g_SetDepth(UIDP_02BGOBJHI);//手前

    filter_ = MakeObj();
    filter_->o_SetParent(field_);
    filter_->o_SetObjFilterGrp(mid::midFullDotL(), mid::midFullDotS(), 0x80, 0, 0, 0);
    filter_->g_SetARGB(TRUE, 0x80, 0x00, 0x00, 0x00);
    filter_->g_SetCenter(TRUE);
  }

  void TLyShRetry::DoReady()
  {
    MsgRetry();
  }
  void TLyShRetry::DoDown()
  {
  }
  void TLyShRetry::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case db::pg_Retry:
      msgwait_c_ = 10;
      break;
    case db::pg_Error:
      break;
    case db::pg_Out:
      break;
    default:
      break;
    }
  }
  void TLyShRetry::MsgRetry()
  {
    //リトライ確認メッセージ
    s32 tanenum = pmgEO_->mgCoU_.GetSupplyNum_FKT();
    if (tanenum > 0)
    {
      std::string strnum = "もっているかず：";
      strnum += lib_str::IntToStr(tanenum);
      STRVECTOR strvec = { "ふっかつのたねを＿しようします", "よろしいですか？", strnum};
      SetHudMsg(TRUE, FALSE, &strvec);
    }
    else
    {
      SetHudMsgStarSupply(SPL_FKT, TRUE);
    }
  }
  void TLyShRetry::MvPage_Retry()
  {
    if (msgwait_c_ > 0)
    {
      --msgwait_c_;
      if (msgwait_c_ == 0) MsgRetry();
    }
    else
    {
      if (IsHudOK())
      {
        if (pmgEO_->mgCoU_.GetSupplyNum_FKT() > 0)
        {
          Post_Retry();//復活
        }
        else
        {
          Post_RetryStar();//復活
        }
      }
      else if (IsHudCancel())
      {
		  if (pmgEO_->stShiai_.IsTimeAttack())
		  {
			  Post_TimeAttackRetire();
		  }
		  else
		  {
			  Post_Retire();//あきらめる
		  }
      }
    }

  }
  void TLyShRetry::MvPage_Error()
  {
    if (IsHudOK())
    {
      ChangePage(pg_Retry);
    }
  }
  void TLyShRetry::MvPage_Out()
  {
  
  }

  //POST：リトライ
  void TLyShRetry::Post_Retry()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Retry();
    }
    else
    {
      StartConnect(cn_retry);
      pmgEO_->mgPo_.poScenario_.Ready_retry();
    }
  }
  //POST：リトライスターしよう
  void TLyShRetry::Post_RetryStar()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_RetryStar();
    }
    else
    {
      StartConnect(cn_retrystar);
      pmgEO_->mgPo_.poScenario_.Ready_retrystar();
    }
  }
  //POST：リタイヤ
  void TLyShRetry::Post_Retire()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Retire();
    }
    else
    {
      StartConnect(cn_retire);
      pmgEO_->mgPo_.poScenario_.Ready_retire(pmgEO_->stShiai_.GetCtrl());
    }
  }
  //POST：タイムアタックリタイヤ
  void TLyShRetry::Post_TimeAttackRetire()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_TimeAttackRetire();
	  }
	  else
	  {
		  StartConnect(cn_retiretimeattack);
		  pmgEO_->mgPo_.poHmShiai_.Ready_tmatretire();
	  }
  }

  //接続：リトライ
  void TLyShRetry::Connect_Retry()
  {
    Down();
    SetHudEnd();
    pScene_->lyShBar_->MoveLayer(shl_Play);
  }

  //接続：リトライスター
  void TLyShRetry::Connect_RetryStar()
  {
    Down();
    SetHudEnd();
    pScene_->lyShBar_->MoveLayer(shl_Play);
  }

  //接続：リタイヤ
  void TLyShRetry::Connect_Retire()
  {
    Down();
    pScene_->lyShBar_->MoveLayer(shl_Map);
    SetHudEnd();
  }
  //接続：タイムアタックリタイヤ
  void TLyShRetry::Connect_TimeAttackRetire()
  {
	  Down();
	  pScene_->lyShBar_->MoveLayer(shl_EventHome);
	  SetHudEnd();
  }

  //接続完了
  void TLyShRetry::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_retry: Connect_Retry(); break;
        case cn_retrystar: Connect_RetryStar(); break;
        case cn_retire: Connect_Retire(); break;
        }
      }
      else
      {
        switch (stLy_.connect_)
        {
        case cn_retry:
        case cn_retrystar:
          //エラーメッセージ
          EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
          ChangePage(pg_Error);
          break;
        default:
          //エラーメッセージ
          EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
          break;
        }
      }
    }
    else
    {
      switch (stLy_.connect_)
      {
      case cn_retry:
      case cn_retrystar:
        ChangePage(pg_Error);
        break;
      }

    }
  }


  void TLyShRetry::DoUpdate(BOOL draw_f)
  {
    filter_->Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Retry: MvPage_Retry(); break;
      case pg_Error: MvPage_Error(); break;
      case pg_Out: MvPage_Out(); break;
      }
    }
  }


}
