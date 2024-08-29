#include "dbLyHmShopInside.h"
#include "dbSnHome.h"

namespace db
{

  enum enPage
  {
    pg_Visit,
    pg_Select,
    pg_Decide,
    pg_Out,
  };

  enum enConnect
  {
    cn_Buy,
  };

  //コンストラクタ
  TLyHmShopInside::TLyHmShopInside(TSnHome* pScene)
    :TLyHmGen(pScene)
    , ugshop_(this)
    , uglistSyouhin_(this)
  {
    stSI_.Init();
  }

  TLyHmShopInside::~TLyHmShopInside()
  {

  }

  // メニューの中から開始
  // 配列数→階層数
  // 配列の中身→メニュー番号
  void TLyHmShopInside::SetupMenu(void)
  {
    INTVECTOR v = pmgEO_->stJump_.GetJumpData();

	  for (s32 i = 0; i < v.size(); i++)
	  {
		  if (v[i] <= 0){ break; }

		  // 1階層目
		  if (i == 0)
		  {
			  s32 no = v[i]-1;

			  s64 mid_syouhin = uglistSyouhin_.records_[no]->GetCommonID();
			  stSI_.mid_supply_ = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_syouhin, mdm_spsyouhin_itemID);
			  stSI_.itemIconNo_ = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(stSI_.mid_supply_, mdm_supply_iconNo);
			  //リスト末端
			  uglistSyouhin_.uglistend_.SetDataSpSyouhin(mid_syouhin);
			  uglistSyouhin_.ChangeListEnd(TRUE);
			  ugbtn_tsh_->SetDraw(TRUE);
			  ugshop_.SetMsg(spmt_Order);
			  ChangePage(pg_Decide);
		  }
	  }
  }

  void TLyHmShopInside::ClearRecords()
  {
  }

  void TLyHmShopInside::DoLoad()
  {
    //ショップフィールド全生成
    ugshop_.MakeGroup();
    uglistSyouhin_.MakeGroup(field_, FALSE);
    uglistSyouhin_.Refresh(enShopType_Supply);
  }

  //入店時の処理
  void TLyHmShopInside::DoReady()
  {
    stSI_.Init();
    pmgEO_->mgNSF_.Play(nsf_shop_in);//BGM
    ugshop_.Ready();
    uglistSyouhin_.SetDraw(TRUE);
    uglistSyouhin_.ChangeListEnd(FALSE);
    //ugbtn_back_->SetFont("もどる");
	ugbtn_back_->SetFont("みせをでる");
    
  }
  void TLyHmShopInside::DoDown()
  {
    pScene_->lyHmShop_->OutShop();
    pmgEO_->mgNSF_.Play(nsf_home);//BGM
  }
  void TLyHmShopInside::DoChangePage(s32 pageno)
  {
    
    switch (pageno)
    {
    case db::pg_Visit:
      break;
    case db::pg_Select:
      ugbtn_back_->SetFont("みせをでる");
      ugbtn_back_->SetDraw(TRUE);
      ugbtn_tsh_->SetDraw(TRUE);
      uglistSyouhin_.SetDraw(TRUE);
      ugr_star_->Refresh();
      break;
    case db::pg_Decide:
      ugbtn_back_->SetFont("もどる");
      ugbtn_back_->SetDraw(TRUE);
      ugbtn_tsh_->SetDraw(TRUE);
      uglistSyouhin_.SetDraw(TRUE);
      ugr_star_->Refresh();
      break;
    case db::pg_Out:
      stSI_.msgset_f_ = FALSE;
      stSI_.wait_c_ = 60;
      ugshop_.SetMsg(spmt_Out);
      UndrawBtn();
      break;
    default:
      break;
    }
  }
  void TLyHmShopInside::ActListSyouhin()
  {
    s64 mid_syouhin = uglistSyouhin_.GetCommonID();
    stSI_.mid_supply_ = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_syouhin, mdm_spsyouhin_itemID);
    stSI_.itemIconNo_ = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(stSI_.mid_supply_, mdm_supply_iconNo);
    //リスト末端
    uglistSyouhin_.SetListEnd(uglistSyouhin_.GetActIndex());
    uglistSyouhin_.ChangeListEnd(TRUE);
    ugbtn_tsh_->SetDraw(TRUE);
    ugshop_.SetMsg(spmt_Order);
    ChangePage(pg_Decide);
  }

  void TLyHmShopInside::MvPage_Visit()
  {
    if (ugbtn_back_->IsAct())
    {
      OutShop();
    }
    else if (ugshop_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      ugshop_.SetMsg(spmt_Menu);
      ChangePage(pg_Select);
    }
    else if (uglistSyouhin_.IsAct())
    {
      ActListSyouhin();
    }
  }

  void TLyHmShopInside::MvPage_Select()
  {
    if (ugbtn_back_->IsAct())
    {
      OutShop();
    }
    else if (uglistSyouhin_.IsAct())
    {
      ActListSyouhin();
    }
  }

  void TLyHmShopInside::MvPage_Decide()
  {
    if (ugbtn_back_->IsAct())
    {
      uglistSyouhin_.ChangeListEnd(FALSE);
      ChangePage(pg_Select);
    }
    else if (uglistSyouhin_.uglistend_.IsAct())//関数に
    {
      //スター消費メッセージ
      SetHudMsgStarSupply(stSI_.mid_supply_, FALSE);
    }
    else if (IsHudOK())
    {
      Post_Buy();
    }
  }
  void TLyHmShopInside::MvPage_Out()
  {
    lib_num::AprTo0(&stSI_.wait_c_);
    if (stSI_.wait_c_ == 0)
    {
      Down();
    }
  }
  void TLyHmShopInside::Post_Buy()
  {
	if (pmgEO_->stSeq_.IsNoSv())
	{
		Connect_Buy();
	}
	else
	{
		StartConnect(cn_Buy);
		pmgEO_->mgPo_.poHmShop_.Ready_buy(stSI_.mid_supply_);
	}
  }
  void TLyHmShopInside::Connect_Buy()
  {
    ugshop_.SetMsg(spmt_Buy);
    ugshop_.DrawBuyIcon(stSI_.itemIconNo_);//アイテム表示
    uglistSyouhin_.ChangeListEnd(FALSE);
    SetHudEnd();
    ChangePage(pg_Visit);
  }

  //接続完了
  void TLyHmShopInside::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_Buy: Connect_Buy(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        //通信エラー
        ChangePage(pg_Select);
      }
    }
    else
    {
      //通信エラー
      //通信エラー
      ChangePage(pg_Select);
    }
  }

  void TLyHmShopInside::DoUpdate(BOOL draw_f)
  {
    switch (stLy_.pageNo_)
    {
    case pg_Out: 
      ugshop_.Update(draw_f);
      break;
    default:
      uglistSyouhin_.Update(draw_f);
      ugshop_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
        switch (stLy_.pageNo_)
        {
        case pg_Visit: MvPage_Visit(); break;
        case pg_Select: MvPage_Select(); break;
        case pg_Decide: MvPage_Decide(); break;
        case pg_Out: MvPage_Out(); break;
        }
    }
  }

  void TLyHmShopInside::OutShop()
  {
    stSI_.msgset_f_ = FALSE;
    stSI_.wait_c_ = 60;
    ugshop_.SetMsg(spmt_Out);
    ChangePage(pg_Out);
  }

}
