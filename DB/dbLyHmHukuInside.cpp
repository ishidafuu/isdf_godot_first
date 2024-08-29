#include "dbLyHmHukuInside.h"
#include "dbSnHome.h"
#include "dbUGPrev.h"
namespace db
{

  enum enPage
  {
    pg_Visit,
    pg_Select,
    pg_Decide,
    pg_TousenGrade,
    pg_Tousen,
    pg_TousenEnd,
    pg_Cnt,
    pg_CntEnd,
	pg_Prev,	// プレビュー
    pg_Roll,
    pg_Result,
    pg_Out,
  };

  enum enConnect
  {
    cn_Huku,
    cn_HukuCnt,
    cn_Tousen,
  };

  //コンストラクタ
  TLyHmHukuInside::TLyHmHukuInside(TSnHome* pScene)
    :TLyHmGen(pScene)
    , ugsp_Huku_(this)
    , uglistSpHuku_(this)
    , uglistHukuCnt_(this)
    , uglistTousenGrade_(this)
    , uglistTousen_(this)
    , ugprof_(this)
  {
    stHuku_.Init();
  }

  TLyHmHukuInside::~TLyHmHukuInside()
  {

  }

  void TLyHmHukuInside::ClearRecords()
  {
  }

  // メニューの中から開始
  // 配列数→階層数
  // 配列の中身→メニュー番号
  void TLyHmHukuInside::SetupMenu(void)
  {
    INTVECTOR v = pmgEO_->stJump_.GetJumpData();
	s32 msgNo = NGNUM;
	  for (s32 i = 0; i < v.size(); i++)
	  {
		  if (v[i] <= 0){ break; }

		  // 1階層目
		  if (i == 0)
		  {
			  s32 recordidx = v[i]-1;
			  uglistSpHuku_.SetListEnd(recordidx);
			  stHuku_.mid_huku_ = uglistSpHuku_.records_[recordidx]->GetCommonID();//福引きＭＩＤ
			  //福引きグループ
			  stHuku_.mid_hukugrp_ = pmgEO_->mgCoM_.mdm_Huku_.GetPKDt(stHuku_.mid_huku_, mdm_huku_id_w_hukugrp);
			  ChangePage(pg_Decide);

			  msgNo = hkmt_Jump;
		  }
	  }
	  if (msgNo != NGNUM)
	  {
		  ugsp_Huku_.SetMsg((enHukuMsgType)msgNo);
	  }
  }


  void TLyHmHukuInside::DoLoad()
  {
    //ショップフィールド全生成
    ugsp_Huku_.MakeGroup();
    uglistSpHuku_.MakeGroup(field_, FALSE);
    uglistHukuCnt_.MakeGroup(field_, FALSE);
    uglistTousenGrade_.MakeGroup(field_, FALSE);
    uglistTousen_.MakeGroup(field_, FALSE);
    
    ugprof_.MakeGroup(field_, TRUE);
    ugprof_.SetProfMe();

    listvec_.clear();
    listvec_.push_back(&uglistSpHuku_);
    listvec_.push_back(&uglistHukuCnt_);
    listvec_.push_back(&uglistTousen_);
    listvec_.push_back(&uglistTousenGrade_);


    //当選者
    const s32 GRADENUM = 4;
    const std::string GRADE[] =
    {
      "１とう",
      "２とう",
      "３とう",
      "４とう",
    };

    for (s32 i = 0; i < GRADENUM; ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)uglistTousenGrade_.MakeObj_Record();
      s32 grade = RARITYNUM - i - 1;
      newrecord->SetCommonID(grade);//等級
      newrecord->SetName(GRADE[i].c_str());
      newrecord->ugname_.ugicon_.SetIconCupHuku(grade);
    }

    ResetObj();
  }

  //入店時の処理
  void TLyHmHukuInside::DoReady()
  {
    stHuku_.Init();
    pmgEO_->mgNSF_.Play(nsf_shop_huku);//BGM
    ugsp_Huku_.Ready();
    uglistSpHuku_.Refresh();
    ugbtn_back_->SetFont("もどる");
    ugbtn_mid_->SetFont("ふくびきなかみ");
    ugbtn_ok_->SetFont("とうせんしゃ");
    DrawList(NULL);
  }
  void TLyHmHukuInside::DoDown()
  {
    pScene_->lyHmShop_->OutShop();
    pmgEO_->mgNSF_.Play(nsf_home);//BGM
  }

  //リストを全て閉じる
  void TLyHmHukuInside::DrawList(TUGLiGen* drawlist)
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
    ResetObj();
  }
  //ボタンを全て閉じる
  void TLyHmHukuInside::ResetObj()
  {
    ugprof_.SetDraw(FALSE);
    ugbtn_mid_->SetDraw(FALSE);
    ugbtn_ok_->SetDraw(FALSE);
    ugbtn_back_->SetDraw(TRUE);
  }

  void TLyHmHukuInside::DoChangePage(s32 pageno)
  {

    
    switch ((enPage)pageno)
    {
    case db::pg_Visit:
      DrawList(&uglistSpHuku_);
	  ugbtn_back_->SetFont("みせをでる");
      break;
    case db::pg_Select:
      DrawList(&uglistSpHuku_);
	  ugbtn_back_->SetFont("みせをでる");
      //uglistSpHuku_.Refresh();
      uglistSpHuku_.ChangeListEnd(FALSE);
      ugr_huku_->Refresh();
      break;
    case db::pg_Decide:
      DrawList(&uglistSpHuku_);
      uglistSpHuku_.ChangeListEnd(TRUE);
      ugbtn_mid_->SetDraw(TRUE);
      ugbtn_ok_->SetDraw(TRUE);
      ugr_huku_->Refresh();
	  ugbtn_mid_->SetFont("ふくびきなかみ");
	  ugbtn_back_->SetFont("もどる");
      break;
    case db::pg_TousenGrade:
      DrawList(&uglistTousenGrade_);

      break;
    case db::pg_Tousen:
      DrawList(&uglistTousen_);
      pmgEO_->mgCoU_.MakeChestEnemy();//敵アイコン作成
      uglistTousen_.Refresh_Huku();
      uglistTousen_.ChangeListEnd(FALSE);
      if (uglistTousen_.records_.size() > 0)
      {
        ugprof_.SetDraw(TRUE);
        ugprof_.SetProfEnemy(0);
      }
      break;
    case db::pg_TousenEnd:
      uglistTousen_.SetListEnd(uglistTousen_.GetActIndex());
      uglistTousen_.ChangeListEnd(TRUE);
      break;
    case db::pg_Cnt:
      DrawList(&uglistHukuCnt_);
      uglistHukuCnt_.Refresh(stHuku_.mid_hukugrp_);
      ugbtn_back_->SetDraw(TRUE);
	  ugbtn_mid_->SetDraw(FALSE);
      break;
	case db::pg_CntEnd:
		{
			DrawList(NULL);
			ugbtn_back_->SetDraw(FALSE);

			enItemType itemtype = (enItemType)pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(selectNo_, mdm_hukucnt_itemType);
			if (itemtype == enItemType_Sozai)
			{
				ugbtn_mid_->SetFont("プレビュー", FC2_normal);
				ugbtn_mid_->SetDraw(TRUE);
			}
		}
	  break;
  case db::pg_Prev:
  {
    TPrevParam param;
		param.nonehavemixbtn_f = TRUE;
    DrawList(NULL);
    SetHudPrev(param);
    SetHudPrevChangeShot(prevvec_);
    //ugbtn_back_->SetDraw(FALSE);
  }

		break;
    case db::pg_Roll:
      DrawList(NULL);
      ugbtn_back_->SetDraw(FALSE);
      pmgEO_->stSeq_.PushNeedOmakase();//おまかせリフレッシュフラグ
      break;
    case db::pg_Result:
      DrawList(NULL);
      SetHudItemView_GetLog(FALSE, FALSE, TRUE);
      break;
    case db::pg_Out:
      UndrawBtn();
      break;
    default:
      break;
    }
  }

  void TLyHmHukuInside::ActListSpHuku()
  {
    s32 recordidx = uglistSpHuku_.GetActIndex();
    uglistSpHuku_.SetListEnd(recordidx);
    stHuku_.mid_huku_ = uglistSpHuku_.GetCommonID();//福引きＭＩＤ
    //福引きグループ
    stHuku_.mid_hukugrp_ = pmgEO_->mgCoM_.mdm_Huku_.GetPKDt(stHuku_.mid_huku_, mdm_huku_id_w_hukugrp);
    ChangePage(pg_Decide);
  }

  void TLyHmHukuInside::MvPage_Visit()
  {
    if (ugbtn_back_->IsAct())
    {
      OutShop();
    }
    else if (ugsp_Huku_.ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      ChangePage(pg_Select);
      ugsp_Huku_.SetMsg(hkmt_Menu);
    }
    else if (uglistSpHuku_.IsAct())//レコード選択
    {
      ActListSpHuku();
    }
  }

  //ガチャタイプ選択
  void TLyHmHukuInside::MvPage_Select()
  {
    if (ugbtn_back_->IsAct())
    {
      OutShop();
    }
    else if (uglistSpHuku_.IsAct())//レコード選択
    {
      ActListSpHuku();
    }
  }

  //ガチャ買う画面
  void TLyHmHukuInside::MvPage_Decide()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Select);
    }
    else if (ugbtn_ok_->IsAct())//とうせんしゃ
    {
      //Post_Tousen();//当選者ログを取りに行く
      ChangePage(pg_TousenGrade);
    }
    else if (ugbtn_mid_->IsAct())//ふくびきなかみ
    {
      //ChangePage(pg_Cnt);
      Post_HukuCnt();//ガチャ開始通信
    }
    else if (uglistSpHuku_.uglistend_.IsActRes(enUGListEndAct_ok))
    {
      Post_Huku();//ガチャ開始通信
    }
  }

  //当選ランク
  void TLyHmHukuInside::MvPage_TousenGrade()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Decide);
    }
    else if (uglistTousenGrade_.IsAct())//レコード選択
    {
      stHuku_.grade_ = uglistTousenGrade_.GetCommonID();//等級
      stHuku_.tousenpage_ = 0;//ページ
      Post_Tousen();//ガチャ開始通信
    }
  }

  //当選者表示
  void TLyHmHukuInside::MvPage_Tousen()
  {
    if (ugbtn_back_->IsAct())
    {
      //監督画像が共用なので元に戻す
      ugprof_.SetProfMe();
      ChangePage(pg_TousenGrade);
    }
    else if (uglistTousen_.IsAct())//レコード選択
    {
      //表示書き換え//変数更新（テクスチャ読み込み）
      ugprof_.SetProfEnemy(uglistTousen_.GetActIndex());
      //ChangePage(pg_TousenEnd);
    }
  }
  //当選者ＥＮＤ
  void TLyHmHukuInside::MvPage_TousenEnd()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Tousen);
    }
  }
  //確率中身
  void TLyHmHukuInside::MvPage_Cnt()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Decide);
    }
    else if (uglistHukuCnt_.IsAct())//レコード選択
    {
		selectNo_ = uglistHukuCnt_.GetCommonID();
      SetHudItemView_HukuCnt(selectNo_);
      ChangePage(pg_CntEnd);
    }
  }
  //確率ＥＮＤ
  void TLyHmHukuInside::MvPage_CntEnd()
  {
    //if (ugbtn_back_->IsAct())
    if (IsHudItemView())
    {
      ChangePage(pg_Cnt);
    }
	else if (ugbtn_mid_->IsAct())
	{
		prevvec_.clear();
		TSozaiData sozai;
		enItemType itemtype = (enItemType)pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(selectNo_, mdm_hukucnt_itemType);
		s64 sozaiID = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(selectNo_, mdm_hukucnt_itemID);
		sozai.SetMidSozai(sozaiID);
		prevvec_.push_back(sozai);
		SetHudEnd();
		ChangePage(pg_Prev);
	}
  }
  // プレビュー
  void TLyHmHukuInside::MvPage_Preview()
  {
	if (IsHudPrevBack())//おーけー
	{
		SetHudItemView_HukuCnt(selectNo_);
		ChangePage(pg_CntEnd);
	}
	else if (IsHudPrevMix())
	{
		SetHudPrevMixSozai(prevvec_[0].GetSozaiR(), TRUE);
	}
	else if (IsHudPrevMixClear())
	{
		SetHudPrevMixSozai(prevvec_[0].GetSozaiR(), FALSE);
	}
  }
  //ガチャロール
  void TLyHmHukuInside::MvPage_Roll()
  {
    ////ガチャアニメーション終了
    //if (ugsp_Huku_.IsAct())
    //{
    //  ChangePage(pg_Result);
    //}
    //ガチャアニメーション終了
    if (ugsp_Huku_.IsActRes(enUGSpHukuAct_CupsuleOut))
    {
      SetHudItemView_GachaGet(ugsp_Huku_.GetActIndex());
    }
    else if (ugsp_Huku_.IsActRes(enUGSpHukuAct_HukuEnd))
    {
      ChangePage(pg_Result);
    }
    //画面タップ
    else if (field_->sc_IsCamEnter2())
    {
      ugsp_Huku_.TapHukuField();
    }
  }

  void TLyHmHukuInside::MvPage_Result()
  {
    if (IsHudItemView())
    {
      ChangePage(pg_Select);
      ugsp_Huku_.SetMsg(hkmt_Order2);
      ugbtn_back_->SetDraw(TRUE);
    }
  }

  void TLyHmHukuInside::MvPage_Out()
  {
    lib_num::AprTo0(&stHuku_.wait_c_);
    if (stHuku_.wait_c_ == 0)
    {
      Down();
    }
  }

  //POST：ガチャ
  void TLyHmHukuInside::Post_Huku()
  {
    StartConnect(cn_Huku);
    pmgEO_->mgPo_.poHmShop_.Ready_huku(stHuku_.mid_huku_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_Huku();
  }
  //POST：ふくびきなかみ
  void TLyHmHukuInside::Post_HukuCnt()
  {
	  if (!pmgEO_->stSeq_.IsNoSv())
	  {
		  StartConnect(cn_HukuCnt);
		  pmgEO_->mgPo_.poHmShop_.Ready_hukucnt(stHuku_.mid_hukugrp_);
	  }
	  else
	  {
		  Connect_HukuCnt();
	  }
  }
  //POST：当選者取得
  void TLyHmHukuInside::Post_Tousen()
  {
    StartConnect(cn_Tousen);
    pmgEO_->mgPo_.poHmShop_.Ready_tousenhuku(stHuku_.mid_hukugrp_, stHuku_.grade_, stHuku_.tousenpage_);

    if (pmgEO_->stSeq_.IsNoSv()) Connect_Tousen();
  }
  //通信：ガチャ
  void TLyHmHukuInside::Connect_Huku()
  {
    //ガチャアニメーションへ移行
    ChangePage(pg_Roll);
    ugsp_Huku_.SetMsg(hkmt_Huku);
    SetHudEnd();
    //まるかのこうしん
    pmgEO_->stSeq_.PushNeedMaruka();
  }
  //通信：当選者
  void TLyHmHukuInside::Connect_HukuCnt()
  {
    //当選者表示ページへ移行
    ChangePage(pg_Cnt);
    SetHudEnd();
  }
  //通信：当選者
  void TLyHmHukuInside::Connect_Tousen()
  {
    //当選者表示ページへ移行
    ChangePage(pg_Tousen);
    SetHudEnd();
  }

  void TLyHmHukuInside::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_Huku: Connect_Huku(); break;
        case cn_HukuCnt: Connect_HukuCnt(); break;
        case cn_Tousen: Connect_Tousen(); break;
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

  void TLyHmHukuInside::DoUpdate(BOOL draw_f)
  {
    switch (stLy_.pageNo_)
    {
    case pg_Out:
      ugsp_Huku_.Update(draw_f);
      break;
    default:
      uglistSpHuku_.Update(draw_f);
      uglistHukuCnt_.Update(draw_f);
      uglistTousenGrade_.Update(draw_f);
      uglistTousen_.Update(draw_f);
      ugsp_Huku_.Update(draw_f);
      ugprof_.Update(draw_f);
      break;
    }
    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Visit: MvPage_Visit(); break;//品物選択
      case pg_Select: MvPage_Select(); break;//ガチャタイプ選択
      case pg_Decide: MvPage_Decide(); break;//決定画面
      case pg_TousenGrade: MvPage_TousenGrade(); break;//当選者
      case pg_Tousen: MvPage_Tousen(); break;//当選者
      case pg_TousenEnd: MvPage_TousenEnd(); break;//当選者エンド
      case pg_Cnt: MvPage_Cnt(); break;//確率中身
      case pg_CntEnd: MvPage_CntEnd(); break;//ボックス中身
	  case pg_Prev:MvPage_Preview(); break;//プレビュー
      case pg_Roll: MvPage_Roll(); break;//ガチャ
      case pg_Result: MvPage_Result(); break;//ガチャ結果
      case pg_Out: MvPage_Out(); break;
      }
    }
  }

  void TLyHmHukuInside::OutShop()
  {
    stHuku_.msgset_f_ = FALSE;
    stHuku_.wait_c_ = 60;
    ugsp_Huku_.SetMsg(hkmt_Out);
    ChangePage(pg_Out);
  }

}
