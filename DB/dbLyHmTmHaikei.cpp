#include "dbLyHmTmHaikei.h"
#include "dbSnHome.h"

namespace db
{

  enum enPage
  {
    pg_Haikei,
    pg_SetHaikei,
  };

  enum enConnect
  {
    cn_SetHaikei,
  };

  //コンストラクタ
  TLyHmTmHaikei::TLyHmTmHaikei(TSnHome* pScene) :TLyHmGen(pScene)
    , ugfilter_(this)
	, ugHaikei_(this)
  {
  }
  TLyHmTmHaikei::~TLyHmTmHaikei()
  {
  }

  void TLyHmTmHaikei::DoLoad()
  {
    ugfilter_.MakeGroup(field_);
    ugfilter_.OffFilter(2, FALSE);

	ugHaikei_.MakeGroup(field_);
	ugHaikei_.SetDraw(FALSE);

#ifdef __K_DEBUG_USER_DATA__
	kdebug::DebugUDManager::GetInstance()->GetAllBG();
#endif
  }
  //入店時の処理
  void TLyHmTmHaikei::DoReady()
  {
	  ugHaikei_.SetDraw(TRUE);
	  ugHaikei_.Ready();
    ugbtn_back_->SetFont("もどる");
    ugbtn_ok_->SetFont("ホームコート");
    ugbtn_back_->SetDraw(TRUE);
    ugbtn_ok_->SetDraw(FALSE);
  }
  void TLyHmTmHaikei::DoDown()
  {
	  ugHaikei_.Down();
    pScene_->lyHmTeam_->Ready();
  }

  void TLyHmTmHaikei::DoChangePage(s32 pageno)
  {
    switch ((enPage)pageno)
    {
    case db::pg_Haikei:
	  ugHaikei_.SetDraw(TRUE);
	  ugbtn_back_->SetDraw(TRUE);
	  break;
    case db::pg_SetHaikei:
      ugbtn_ok_->SetDraw(FALSE);
      ugbtn_back_->SetDraw(FALSE);
      ugfilter_.OnFilter(2, FALSE, TRUE);
      break;
    }
  }

  void TLyHmTmHaikei::MvPage_Haikei()
  {
	  mid_haikei_ = ugHaikei_.GetHaikeiID();

	  if (ugbtn_back_->IsAct())//おーけー
	  {
		  Down();
		  return;
	  }
	  else if (ugbtn_ok_->IsAct() && (mid_haikei_ != NGNUM))//おーけー
	  {
		  // 持ってる
		  if (pmgEO_->mgCoU_.IsHaikei(mid_haikei_))
		  {
			  Post_SetHaikei();
		  }
		  // 持ってない
		  else
		  {
			  // ガチャ屋にとばす
			  Down();

			  // ここに飛び先データを取ってきて入れる
			  INTVECTOR v;
			  v.push_back(footer_id_shop);	// ショップタブ
			  v.push_back(2);	// ガチャショップ
			  v.push_back(2); // なつかしBOX
			  s32 mid_nabox = pmgEO_->mgDt_.dtCourt_.GetDtLine(mid_haikei_, enDtCourt_mid_nabox);
			  v.push_back(mid_nabox);
			  pmgEO_->stJump_.SetJumpAreaData(v);
			  pmgEO_->stJump_.SetFromTab(footer_id_team);
			  pScene_->lyHmBar_->JumpFooter(v[0]);
			  return;
		  }
	  }
	  else if (ugHaikei_.GetActRes() == enUGHaikeiAct_Select)
	  {
		  // 持ってる
		  if (pmgEO_->mgCoU_.IsHaikei(mid_haikei_))
		  {
			  ugbtn_ok_->SetFont("ホームコート");
			  ugbtn_ok_->SetDraw(TRUE);
		  }
		  // 持ってない
		  else
		  {
			  ugbtn_ok_->SetFont("ガチャでかう");
			  ugbtn_ok_->SetDraw(TRUE);
		  }
	  }
  }
  void TLyHmTmHaikei::MvPage_SetHaikei()
  {
    if (ugfilter_.IsAct())//おーけー
    {
      pmgUI_->LoadCourt(mid_haikei_);//背景
      ChangePage(pg_Haikei);
    }
  }

  void TLyHmTmHaikei::Post_SetHaikei()
  {
    if (mid::midIsDXL())
    {
#ifdef __K_DEBUG_USER_DATA__
		kdebug::DebugUDManager::GetInstance()->SetHomeHaikei(mid_haikei_);
#endif
      Connect_SetHaikei();
      return;
    }

    StartConnect(cn_SetHaikei);
    pmgEO_->mgPo_.poHmTeam_.Ready_sethaikei(mid_haikei_);

  }
  void TLyHmTmHaikei::Connect_SetHaikei()
  {
    //背景切り替え
    ChangePage(pg_SetHaikei);
  }

  //接続完了
  void TLyHmTmHaikei::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_SetHaikei: Connect_SetHaikei(); break;
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

  void TLyHmTmHaikei::DoUpdate(BOOL draw_f)
  {
	  ugHaikei_.Update(draw_f);
    ugfilter_.Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Haikei:MvPage_Haikei(); break;
      case pg_SetHaikei:MvPage_SetHaikei(); break;
      }
    }
  }
}
