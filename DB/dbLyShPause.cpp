#include "dbLyShPause.h"
#include "dbSnShiai.h"

namespace db
{

  enum enPage
  {
    pg_pause,
    //pg_Idle,
  };

  enum enConnect
  {
    cn_retire,
    cn_vshumretire,
    cn_tourretire,
	cn_timeattackretire,
  };

  //コンストラクタ
  TLyShPause::TLyShPause(TSnShiai* pScene) 
    :TLyShGen(pScene)
  {
  }
  
  TLyShPause::~TLyShPause()
  {
  }

  void TLyShPause::DoLoad()
  {
    field_->g_SetDepth(UIDP_02BGOBJHI);//手前

    filter_ = MakeObj();
    filter_->o_SetParent(field_);
    filter_->o_SetObjFilterGrp(mid::midFullDotL(), mid::midFullDotS(), 0x80, 0, 0, 0);
    filter_->g_SetARGB(TRUE, 0x80, 0x00, 0x00, 0x00);
    filter_->g_SetCenter(TRUE);
  }
  void TLyShPause::DoReady()
  {
    STRVECTOR strvec = { "しあいを＿ほうきします", "よろしいですか？" };
    SetHudMsg(TRUE, FALSE, &strvec);
  }
  void TLyShPause::DoDown()
  {
  }
  void TLyShPause::DoChangePage(s32 pageno)
  {

  }
  void TLyShPause::MvPage_Pause()
  {
    if  (IsHudOK())
    {
      //リタイヤ
      if (pmgEO_->stShiai_.IsTour())
      {
        Post_TourRetire();
      }
      else if (pmgEO_->stShiai_.IsVSHum())
      {
        Post_VSHumRetire();
      }
	  else if (pmgEO_->stShiai_.IsTimeAttack())
	  {
		  Post_TimeAttackRetire();
	  }
      else
      {
        Post_Retire();
      }
    }
    else if (IsHudCancel())
    {
      //試合に戻る
      Down();
      pScene_->lyShBar_->MoveLayer(shl_Play);
    }
  }
  //POST：リタイヤ
  void TLyShPause::Post_Retire()
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
  //POST：VSリタイヤ
  void TLyShPause::Post_VSHumRetire()
  {
    StartConnect(cn_vshumretire);
    pmgEO_->mgPo_.poHmShiai_.Ready_vshumretire(mid::midGetSyncErrorCode() + (mid::midGetGKErrorCode() * 10),
                                               mid::midGetNGNum(),
                                               mid::midGetMyBM(),
                                               mid::midGetEnBM(),
                                               pmgEO_->stShiai_.GetCtrl());//win:0 lose : 1 draw : 2
    
    
//    pmgEO_->mgPo_.poHmShiai_.Ready_vshumretire(mid::midGetSyncErrorCode(),
//                                               mid::midGetNGNum(),
//                                               mid::midGetMyBM(),
//                                               mid::midGetEnBM(),
//                                               pmgEO_->stShiai_.GetCtrl());
  }
  //POST：かちぬきリタイヤ
  void TLyShPause::Post_TourRetire()
  {
    StartConnect(cn_tourretire);
    pmgEO_->mgPo_.poHmShiai_.Ready_tourretire();
  }
  //POST：タイムアタックリタイヤ
  void TLyShPause::Post_TimeAttackRetire()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_TimeAttackRetire();
	  }
	  else
	  {
		  StartConnect(cn_timeattackretire);
		  pmgEO_->mgPo_.poHmShiai_.Ready_tmatretire();
	  }
  }

  //接続：リタイヤ
  void TLyShPause::Connect_Retire()
  {
    Down();
    pScene_->lyShBar_->MoveLayer(shl_Map);
    SetHudEnd();
  }
  //接続：VSリタイヤ
  void TLyShPause::Connect_VSHumRetire()
  {
    Down();
    pScene_->lyShBar_->MoveLayer(shl_Home);
    SetHudEnd();
  }
  //接続：かちぬきリタイヤ
  void TLyShPause::Connect_TourRetire()
  {
    Down();
    pScene_->lyShBar_->MoveLayer(shl_Home);
    SetHudEnd();
  }
  //接続：タイムアタックリタイヤ
  void TLyShPause::Connect_TimeAttackRetire()
  {
	  Down();
	  pScene_->lyShBar_->MoveLayer(shl_EventHome);
	  SetHudEnd();
  }
  //接続完了
  void TLyShPause::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_retire: Connect_Retire(); break;
        case cn_vshumretire: Connect_VSHumRetire(); break;
        case cn_tourretire: Connect_TourRetire(); break;
		case cn_timeattackretire: Connect_TimeAttackRetire(); break;
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


  void TLyShPause::DoUpdate(BOOL draw_f)
  {
    filter_->Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_pause:MvPage_Pause(); break;
      }
    }
  }
}
