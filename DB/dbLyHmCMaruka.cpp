#include "dbLyHmCMaruka.h"
#include "dbSnHome.h"

namespace db
{
  enum enLayerPhase
  {
    lp_Wait,
    lp_Shopping,
    lp_OutStore,
  };

  enum enPage
  {
    pg_Visit,
    pg_SelectSozai,
    pg_Out,
  };

  enum enConnect
  {
    cn_ReadyMaruka,
    cn_Maruka,
    cn_OutMaruka,
  };

  //コンストラクタ
  TLyHmCMaruka::TLyHmCMaruka(TSnHome* pScene)
    :TLyHmGen(pScene)
    , ugmaruka_(this)
    , uglistMaruka_(this)
  {
    stCMaruka_.Init();
  }

  TLyHmCMaruka::~TLyHmCMaruka()
  {
  }

  void TLyHmCMaruka::DoLoad()
  {
    //まるかフィールド全生成
    ugmaruka_.MakeGroup(field_, &pScene_->lyHmCenter_->ugkantoku_,
						&pScene_->lyHmCenter_->ugcap_,
                        &pScene_->lyHmCenter_->ugcl_maruka_,
                        &pScene_->lyHmCenter_->ugdoor_bba_);

    //リスト
    uglistMaruka_.MakeGroup(field_, FALSE);

  }

  //入店時の処理
  void TLyHmCMaruka::DoReady()
  {
    stCMaruka_.Init();
    ugmaruka_.Ready();
    //uglistMaruka_.Refresh(0);
    uglistMaruka_.SetDraw(FALSE);
    ugbtn_back_->SetFont("もどる");
    ugbtn_prev_->SetFont("まえのページ");
    ugbtn_next_->SetFont("つぎのページ");
    ugbtn_ok_->SetFont("うけとる");
    ugbtn_back_->SetDraw(FALSE);
    pmgEO_->stSeq_.PushNeedOmakase();//おまかせリフレッシュフラグ


  }
  void TLyHmCMaruka::DoDown()
  {
    //pScene_->lyHmCenter_->Ready();//退出


	// ここでうけとれるものがあるかチェックする
	//s32 uketoriNum = pmgEO_->mgCoU_.mdlog_Maruka_.GetLineNum();
    if (pmgEO_->stSeq_.IsDrawMaruka() == FALSE)
	{
		pScene_->lyHmCenter_->SetOutMaruka();
	}
    else
    {
      pScene_->lyHmCenter_->Ready();//退出
    }
  }
  void TLyHmCMaruka::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case pg_Visit:
      //uglistMaruka_.SetDraw(TRUE);
      break;
    case pg_SelectSozai:
      stCMaruka_.logpage_ = 0;
      uglistMaruka_.SetDraw(TRUE);
      uglistMaruka_.Refresh(stCMaruka_.logpage_, stCMaruka_.endpage_);
      ugmaruka_.SetMsg(enMarukaMsg_ShotRcv);
      ugbtn_back_->SetDraw(TRUE);
      ugbtn_ok_->SetDraw(FALSE);
      break;
    case pg_Out:
      ugmaruka_.SetMsg(enMarukaMsg_Out);
      UndrawBtn();
      break;
    default:
      break;
    }
  }

  //入店
  void TLyHmCMaruka::MvPage_Visit()
  {
    if (ugmaruka_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      stCMaruka_.logpage_ = 0;
      Post_ReadyMaruka();
    }
  }
  //メニュー
  void TLyHmCMaruka::MvPage_SelectSozai()
  {
    ugbtn_ok_->SetDraw(uglistMaruka_.GetSelectNum() > 0);

    //出る
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Out);
    }
    else if ((stCMaruka_.logpage_ > 0) && ugbtn_prev_->IsAct())//前ページ
    {
      //アイテム＿かくとくりれき
      --stCMaruka_.logpage_;
      Post_Maruka();
    }
    else if (ugbtn_next_->IsAct())//次ページ
    {
      //アイテム＿かくとくりれき
      ++stCMaruka_.logpage_;
      Post_Maruka();
    }
    else if (ugbtn_ok_->IsAct() && (uglistMaruka_.GetSelectNum() > 0))
    {
      outidvec_ = uglistMaruka_.GetIDOutSozai();

      //POST：素材受け取り
      Post_OutMaruka();
    }

  }
  void TLyHmCMaruka::MvPage_OutStore()
  {
    if (ugmaruka_.IsAct())
    {
      Down();
    }
  }
  //POSTの結果から最終ページ番号取得
  void TLyHmCMaruka::SetEndPage()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  stCMaruka_.endpage_ = 0;
	  }
	  else
	  {
	    stCMaruka_.endpage_ = pmgEO_->mgPo_.GetValue("endpage");
	  }
  }
  void TLyHmCMaruka::SwitchLogPageBtn()
  {
    ugbtn_prev_->SetDraw((stCMaruka_.logpage_ > 0));
    ugbtn_next_->SetDraw((stCMaruka_.logpage_ < stCMaruka_.endpage_));
  }

  //POSTかくとくログ（最初）
  void TLyHmCMaruka::Post_ReadyMaruka()
  {
	if (pmgEO_->stSeq_.IsNoSv())
	{
		Connect_ReadyMaruka();
	}
	else
	{
		StartConnect(cn_ReadyMaruka);
		//通信はページの更新と変わらない
		pmgEO_->mgPo_.poHmCenter_.Ready_maruka(stCMaruka_.logpage_);
	}
  }
  //POSTかくとくログページの更新
  void TLyHmCMaruka::Post_Maruka()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_Maruka();
	  }
	  else
	  {
		StartConnect(cn_Maruka);
		pmgEO_->mgPo_.poHmCenter_.Ready_maruka(stCMaruka_.logpage_);
	  }
  }
  //POST：素材受け取り
  void TLyHmCMaruka::Post_OutMaruka()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_OutMaruka();
	  }
	  else
	  {
		StartConnect(cn_OutMaruka);
		pmgEO_->mgPo_.poHmCenter_.Ready_outmaruka(&outidvec_);
	  }
  }
  //接続獲得ログ（最初）
  void TLyHmCMaruka::Connect_ReadyMaruka()
  {
    SetEndPage();
    //１ページ目と全体数読み込めたら表示ページに移る
    SwitchLogPageBtn();
    ChangePage(pg_SelectSozai);
  }
  //接続獲得ログ
  void TLyHmCMaruka::Connect_Maruka()
  {
    //リストリフレッシュ
    uglistMaruka_.Refresh(stCMaruka_.logpage_, stCMaruka_.endpage_);
    SwitchLogPageBtn();
    SetHudEnd();
  }

  //接続：素材受け取り
  void TLyHmCMaruka::Connect_OutMaruka()
  {
    stCMaruka_.logpage_ = 0;
    pScene_->lyHmCenter_->RefreshSoukoNum();
    SetEndPage();
    SwitchLogPageBtn();
    uglistMaruka_.Refresh(stCMaruka_.logpage_, stCMaruka_.endpage_);
    ugmaruka_.SetMsg(enMarukaMsg_Thanks);

    pmgEO_->stSeq_.SetDrawFlag(
      pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_mami_f),
      pmgEO_->mgCoU_.mdre_Refresh_.GetLineDt(0, mdre_refresh_maruka_f));
  }

  //接続完了
  void TLyHmCMaruka::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_ReadyMaruka: Connect_ReadyMaruka(); break;
        case cn_Maruka: Connect_Maruka(); break;
        case cn_OutMaruka: Connect_OutMaruka(); break;
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
      ChangePage(pg_Out);
    }
  }

  //lp_Wait,
  void TLyHmCMaruka::DoUpdate(BOOL draw_f)
  {
    switch (stLy_.pageNo_)
    {
    case pg_Out:
      ugmaruka_.Update(draw_f);
      break;
    default:
      ugmaruka_.Update(draw_f);
      uglistMaruka_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Visit: MvPage_Visit(); break;
      case pg_SelectSozai: MvPage_SelectSozai(); break;
      case pg_Out: MvPage_OutStore(); break;
      }
    }
  }

}
