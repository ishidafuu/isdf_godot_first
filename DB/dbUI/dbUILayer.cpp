#include "dbUILayer.h"
#include "dbUIFrame.h"
#include "dbUGHeadder.h"
#include "dbUGFooter.h"
namespace db
{
  //コンストラクタ
  TUILayer::TUILayer(TScene* pScene, TUIFrame* frame) :TUIGenLayer(pScene)//, ughud_(this, pmgEO, pmgUI)
  {
    stLy_.Init();
    stLy_.pageNo_ = NGNUM;//CLOSE
    frame_ = frame;
    SetFooterPointer();//ポインタセット
  }

  TUILayer::~TUILayer()
  {
  }

  void TUILayer::SetFooterPointer()//ポインタセット
  {
    if (frame_->ugfooter_)
    {
      ugbtn_back_ = &frame_->ugfooter_->ugbtn_back_;
      ugbtn_ok_ = &frame_->ugfooter_->ugbtn_ok_;
      ugbtn_mid_ = &frame_->ugfooter_->ugbtn_mid_;
      ugbtn_prev_ = &frame_->ugfooter_->ugbtn_prev_;
      ugbtn_next_ = &frame_->ugfooter_->ugbtn_next_;
      ugbtn_tsh_ = &frame_->ugfooter_->ugbtn_tsh_;
      ugbtn_mgk_ = &frame_->ugfooter_->ugbtn_mgk_;
      ugbtn_fkt_ = &frame_->ugfooter_->ugbtn_fkt_;
      ugr_star_ = &frame_->ugheadder_->ugr_star_;
      ugr_todd_ = &frame_->ugheadder_->ugr_todd_;
      ugr_huku_ = &frame_->ugheadder_->ugr_huku_;
      ugr_gacha_ = &frame_->ugheadder_->ugr_gacha_;
    }
    else
    {
      ugbtn_back_ = NULL;
      ugbtn_ok_ = NULL;
      ugbtn_mid_ = NULL;
      ugbtn_prev_ = NULL;
      ugbtn_next_ = NULL;
      ugbtn_tsh_ = NULL;
      ugbtn_mgk_ = NULL;
      ugbtn_fkt_ = NULL;
      ugr_star_ = NULL;
      ugr_huku_ = NULL;
      ugr_todd_ = NULL;
      ugr_gacha_ = NULL;
    }
  }

  void TUILayer::Load()//読み込み
  {
    //ここでフレームのロード読んでたので多重ロードされてた
    field_ = MakeObj();
    DoLoad();//継承先で定義
  }
  void TUILayer::Ready()
  {
    stLy_.ready_f_ = TRUE;
    if (stLy_.hedderlayer_f_ == FALSE)
    {
      SetFoot_Btn();//基本的にはボタン、タブにすべきならDoReady内で指定
    }
    else
    {
      frame_->ugfooter_->HideBtn(TRUE);
    }
    DoReady();
    ChangePage(0);
  }
  void TUILayer::Down()
  {
    if (IsReady() == FALSE) return;
    DoDown();
    stLy_.pageNo_ = NGNUM;
    stLy_.ready_f_ = FALSE;
  }
  void TUILayer::Wait(BOOL wait_f)
  {
    stLy_.wait_f_ = wait_f;
  }

  void TUILayer::ChangePage(s32 pageno)
  {
    if (IsReady() == FALSE) return;
    Wait(FALSE);
    UndrawCredit();
    stLy_.lastpageNo_ = stLy_.pageNo_;
    DoChangePage(pageno);
    stLy_.pageNo_ = pageno;
  }
  void TUILayer::ChangeLastPage()
  {
    ChangePage(stLy_.lastpageNo_);
  }
  void TUILayer::SetFoot_Tab()
  {
    if (frame_->ugheadder_) frame_->ugheadder_->SetEnable();//結局ヘッダも連動してる
    if (frame_->ugfooter_) frame_->ugfooter_->SetFoot_Tab();
  }
  void TUILayer::SetFoot_Btn()
  {
    if (frame_->ugheadder_) frame_->ugheadder_->SetDisable();//結局ヘッダも連動してる
    if (frame_->ugfooter_) frame_->ugfooter_->SetFoot_Btn();
  }
  void TUILayer::SetFoot_BtnHd()
  {
    if (frame_->ugheadder_) frame_->ugheadder_->SetEnable();//結局ヘッダも連動してる
    if (frame_->ugfooter_) frame_->ugfooter_->SetFoot_Btn();
  }
  void TUILayer::SetFoot_None()
  {
    if (frame_->ugheadder_) frame_->ugheadder_->SetDisable();
    if (frame_->ugfooter_)  frame_->ugfooter_->SetFoot_None();
  }
  void TUILayer::SetHeadderModal(BOOL modal_f)
  {
    if (frame_->ugheadder_) frame_->ugheadder_->SetModal(modal_f);
    //if (frame_->ugfooter_) frame_->ugfooter_->SetFoot_Btn();
  }
  void TUILayer::UndrawBtn()
  {
    if (frame_->ugfooter_)  frame_->ugfooter_->UndrawBtn();
    if (frame_->ugheadder_) frame_->ugheadder_->UndrawCredit();
  }
  void TUILayer::UndrawCredit()
  {
    if (frame_->ugheadder_) frame_->ugheadder_->UndrawCredit();
  }

  BOOL TUILayer::IsCheckOut()
  {
    return (stLy_.checkout_ != coNone);
  }
  void TUILayer::StartCheckOut(enCOType cotype)
  {
    //通信番号をセットしてインジケータ回す
    stLy_.checkout_ = cotype;
    frame_->SetAniStart();//モーダル
  }
  BOOL TUILayer::IsConnect()
  {
    return (stLy_.connect_ != NGNUM);
  }
  BOOL TUILayer::IsDisconnect()
  {
    return (stLy_.connect_ == NGNUM);
  }
  void TUILayer::StartConnect(s32 connectNo)
  {
    //通信番号をセットしてインジケータ回す
    stLy_.connect_ = connectNo;
    frame_->SetAniStart();//モーダル
  }
  void TUILayer::EndConnectOK()
  {
    //    SetHudEnd();
    if (frame_->IsAni()) frame_->SetAniEnd();//モーダル
    stLy_.connect_ = NGNUM;
    stLy_.checkout_ = coNone;
  }
  void TUILayer::EndConnectNG(s32 code)
  {
    //メッセージボックスを出して終了
    frame_->EndConnectNG(code, "");
    stLy_.connect_ = NGNUM;
    stLy_.checkout_ = coNone;
  }
  void TUILayer::EndConnectNG(s32 code, const char* msg)
  {
    //メッセージボックスを出して終了
    frame_->EndConnectNG(code, msg);
    stLy_.connect_ = NGNUM;
    stLy_.checkout_ = coNone;
  }

  void TUILayer::SetHudMsg(BOOL cancel_f, BOOL tsh_f, STRVECTOR* strvec)
  {
    frame_->SetHudMsg(cancel_f, tsh_f, strvec);
  }
  void TUILayer::SetHudMsgTodd(STRVECTOR* strvec, s32 num, BOOL get_f)
  {
    frame_->SetHudMsgTodd(strvec, num, get_f);
    ugr_todd_->Refresh();
  }
  void TUILayer::SetHudMsgStar(STRVECTOR* strvec, s32 num)
  {
    frame_->SetHudMsgStar(strvec, num);
    ugr_star_->Refresh();
  }
  void TUILayer::SetHudMsgStarSupply(s32 mid_supply, BOOL quse_f)
  {
    frame_->SetHudMsgStarSupply(mid_supply, quse_f);
    ugr_star_->Refresh();
  }
  void TUILayer::SetHudMsgStarGacha(s32 mid_gacha)
  {
    ugr_star_->Refresh();
    frame_->SetHudMsgStarGacha(mid_gacha);
  }
  void TUILayer::SetHudMsgStarNaBox(s32 mid_nabox, BOOL otona_f)
  {
    frame_->SetHudMsgStarNaBox(mid_nabox, otona_f);
    ugr_star_->Refresh();
  }
  void TUILayer::SetHudGKErrorMsg()
  {
    frame_->SetHudGKErrorMsg();
  }
  void TUILayer::SetHudFilter(BOOL draw_f)
  {
    frame_->SetHudFilter(draw_f);
  }
  void TUILayer::SetHudEnd()
  {
    frame_->SetMsgEnd();
    frame_->SetItemViewEnd();
    frame_->SetPrevEnd();
  }
  void TUILayer::SetPrevEnd()
  {
	  frame_->SetPrevEnd();
  }
  void TUILayer::SetHudDraw(BOOL draw_f)
  {
	  frame_->SetItemViewDraw(draw_f);
  }

  BOOL TUILayer::IsHudOK()
  {
    return frame_->IsHudOK();
  }
  BOOL TUILayer::IsHudCancel()
  {
    return frame_->IsHudCancel();
  }
  BOOL TUILayer::IsHudItemView()
  {
    return frame_->IsHudItemView();
  }
  BOOL TUILayer::IsHudPrevBack()
  {
    return frame_->IsHudPrevBack();
  }
  BOOL TUILayer::IsHudPrevMix()
  {
	  return frame_->IsHudPrevMix();
  }
  BOOL TUILayer::IsHudPrevMixClear()
  {
	  return frame_->IsHudPrevMixClear();
  }
  BOOL TUILayer::IsHudPrevHit()
  {
    return frame_->IsHudPrevHit();
  }
  BOOL TUILayer::IsHudIntro(void)
  {
	  return frame_->IsHudIntro();
  }
  TUGItemViewParam TUILayer::GetHudItemView(void)
  {
	  return frame_->GetHudItemView();
  }
	/* 2015/06/19 Koike Added */
	BOOL TUILayer::IsHudPrevHaveMix()
	{
		return frame_->IsHudPrevHaveMix();
	}
	BOOL TUILayer::IsHudMixPrevUndo()
	{
		return frame_->IsHudMixPrevUndo();
	}
	BOOL TUILayer::IsHudMixPrevBall_0()
	{
		return frame_->IsHudMixPrevBall_0();
	}
	BOOL TUILayer::IsHudMixPrevBall_1()
	{
		return frame_->IsHudMixPrevBall_1();
	}
	BOOL TUILayer::IsHudMixPrevBall_2()
	{
		return frame_->IsHudMixPrevBall_2();
	}
	BOOL TUILayer::IsHudMixPrevBall_3()
	{
		return frame_->IsHudMixPrevBall_3();
	}
	BOOL TUILayer::IsHudMixPrevBall_4()
	{
		return frame_->IsHudMixPrevBall_4();
	}
	BOOL TUILayer::IsHudMixPrevBall_5()
	{
		return frame_->IsHudMixPrevBall_5();
	}
	BOOL TUILayer::IsHudMixPrevBall_6()
	{
		return frame_->IsHudMixPrevBall_6();
	}
	BOOL TUILayer::IsHudMixPrevBall_7()
	{
		return frame_->IsHudMixPrevBall_7();
	}
	BOOL TUILayer::IsHudMixPrevSelectBall()
	{
		return frame_->IsHudMixPrevSelectBall();
	}
	BOOL TUILayer::IsHudMixPrevSelectShot()
	{
		return frame_->IsHudMixPrevSelectShot();
	}
	BOOL TUILayer::IsHudMixPrevCancel()
	{
		return frame_->IsHudMixPrevCancel();
	}

  void TUILayer::SetHudItemView_GetLog(BOOL msgok_f, BOOL leftpos_f, BOOL filter_f)
  {
	frame_->SetHudItemView_GetLog(msgok_f, leftpos_f, filter_f);
  }
  void TUILayer::SetHudItemView_GetLogSingle(s32 logno, BOOL msgok_f, BOOL leftpos_f)
  {
    frame_->SetHudItemView_GetLogSingle(logno, msgok_f, leftpos_f);
  }
  void TUILayer::SetHudItemView_GachaCnt(s64 mid_gachacnt)
  {
    frame_->SetHudItemView_GachaCnt(mid_gachacnt);
  }
  void TUILayer::SetHudItemView_GachaInit()
  {
    frame_->SetHudItemView_GachaInit();
  }
  void TUILayer::SetHudItemView_GachaGet(s64 logno)
  {
    frame_->SetHudItemView_GachaGet(logno);
  }
  void TUILayer::SetHudItemView_NaBoxCnt(s64 mid_naboxcnt)
  {
    frame_->SetHudItemView_NaBoxCnt(mid_naboxcnt);
  }
  void TUILayer::SetHudItemView_HukuCnt(s64 mid_hukucnt)
  {
    frame_->SetHudItemView_HukuCnt(mid_hukucnt);
  }
  void TUILayer::SetHudItemView_Reward(s64 mid_ranking, s32 rank, BOOL msgok_f, BOOL leftpos_f, BOOL filter_f)
  {
	  frame_->SetHudItemView_Reward(mid_ranking, rank, msgok_f, leftpos_f, filter_f);
  }
  void TUILayer::SetHudItemView_View(s64 itemType, s64 itemID, s64 itemNum, BOOL msgok_f, BOOL leftpos_f)
  {
	  frame_->SetHudItemView_View(itemType, itemID, itemNum, msgok_f, leftpos_f);
  }
  void TUILayer::SetHudItemView_Shot(SOZAIDTVECTOR sozai, BOOL leftpos_f)
  {
	  frame_->SetHudItemView_Shot(sozai, leftpos_f);
  }
  void TUILayer::SetHudItemView_SozaiList(SOZAIDTVECTOR sozai, BOOL leftpos_f)
  {
	  frame_->SetHudItemView_SozaiList(sozai, leftpos_f);
  }
  void TUILayer::SetHudItemView_Drop(s64 itemType, s64 itemID, s64 itemNum, s32 tokkouNum, s32 tokkouMag, BOOL cmp_f)
  {
    frame_->SetHudItemView_Drop(itemType, itemID, itemNum, tokkouNum, tokkouMag, cmp_f);
  }
  //void TUILayer::SetHudPrev(s64 id_team, s64 id_shot, BOOL backbtn_f)
  //{
  //  frame_->SetHudPrev(id_team, id_shot, backbtn_f);
  //}
  void TUILayer::SetHudPrev(const TPrevParam& param)
  {
    frame_->SetHudPrev(param);
  }
  void TUILayer::SetHudPrev_Mixed(LONGVECTOR* id_shot_mixvec)
  {
    frame_->SetHudPrev_Mixed(id_shot_mixvec);
  }
  void TUILayer::SetHudPrevChangeShot(const SOZAIDTVECTOR& sozaivec)
  {
    frame_->SetHudPrevChangeShot(sozaivec);
  }
	/* 2015/06/19 Koike Added */
	void TUILayer::SetHudPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec)
	{
		frame_->SetHudPrev_Have_Mixed(id_shot_mixvec);
	}
  void TUILayer::SetHudPrevMixSozai(s64 id_sozai, BOOL isRndSet)
  {
	  frame_->SetHudPrevMixSozai(id_sozai, isRndSet);
  }
  void TUILayer::SetHudPrevMixShot(s64 id_shot, BOOL isRndSet)
  {
	  frame_->SetHudPrevMixShot(id_shot, isRndSet);
  }

	/* 2015/07/02 Koike Added */
	void TUILayer::SetHudMixPrev(const TMixPrevParam& param)
	{
		frame_->SetHudMixPrev(param);
	}
	void TUILayer::SetHudMixPrev_Mixed(LONGVECTOR* id_shot_mixvec)
	{
		frame_->SetHudMixPrev_Mixed(id_shot_mixvec);
	}
	void TUILayer::SetHudMixPrevChangeShot(const SOZAIDTVECTOR& sozaivec)
	{
		frame_->SetHudMixPrevChangeShot(sozaivec);
	}
	/* 2015/06/19 Koike Added */
	void TUILayer::SetHudMixPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec)
	{
		frame_->SetHudMixPrev_Have_Mixed(id_shot_mixvec);
	}
	void TUILayer::SetHudMixPrevMixSozai(s64 id_sozai, BOOL isRndSet)
	{
		frame_->SetHudMixPrevMixSozai(id_sozai, isRndSet);
	}
	void TUILayer::SetHudMixPrevMixShot(s64 id_shot, BOOL isRndSet)
	{
		frame_->SetHudMixPrevMixShot(id_shot, isRndSet);
	}
	void TUILayer::SetHudMixPrevTouchMode(LONGVECTOR* id_shot_mixvec, s32 iconNo)
	{
		frame_->SetHudMixPrevTouchMode(id_shot_mixvec, iconNo);
	}
	void TUILayer::SetHudMixPrevSelectShot(s64 id_shot)
	{
		frame_->SetHudMixPrevSelectShot(id_shot);
	}
	LONGVECTOR TUILayer::GetHudMixPrevChangeVec()
	{
		return frame_->GetHudMixPrevChangeVec();
	}
	void TUILayer::SetHudResetField(BOOL fieldflg)
	{
		frame_->SetHudResetField(fieldflg);
	}
	//---------------------------------------------------------------------------

  void TUILayer::SetHudVSWait_Search()
  {
    frame_->SetHudVSWait_Search();
  }
  void TUILayer::SetHudVSWait_EnWait()
  {
    frame_->SetHudVSWait_EnWait();
  }
  void TUILayer::SetHudVSWaitEnd()
  {
    frame_->SetHudVSWaitEnd();
  }
  void TUILayer::SetHudCount(s32 count)
  {
    frame_->SetHudCount(count);
  }
  void TUILayer::SetHudCountEnd()
  {
    frame_->SetHudCountEnd();
  }


  BOOL TUILayer::IsMove()
  {
    return (stLy_.pageNo_ >= 0);
  }
  BOOL TUILayer::IsReady()
  {
    return stLy_.ready_f_;
  }

  //更新
  void TUILayer::Update(BOOL draw_f)
  {
    if (IsReady() == FALSE) return;
    if (stLy_.wait_f_) return;

    if ((draw_f 
      || (mid::midGetRequestState() == mid::rqNone)) 
      //&& (stLy_.gototitle_f_ == FALSE)
      )
    {
      field_->Update(draw_f);
      if (draw_f 
        || stLy_.barlayer_f_ 
        ||(stLy_.gototitle_f_ == FALSE))
      {
        DoUpdate(draw_f);
      }
    }

    if (draw_f == FALSE)
    {
      //チェックアウト
      if (IsCheckOut())
      {
        enRequestState res = rqNone;

        switch (stLy_.checkout_)
        {
        case coUser:res = pmgEO_->mgCoU_.Connecting(); break;
        case coInfo:res = pmgEO_->mgCoInfo_.Connecting(); break;
        case coWorld:res = pmgEO_->mgCoU_.Connecting(); break;
        case coClmaster:res = pmgEO_->mgCoClmaster_.Connecting(); break;
        case coSvmaster:res = pmgEO_->mgCoSvmaster_.Connecting(); break;
        case coBannerBmp:res = pmgEO_->mgCoBannerBmp_.Connecting(); break;
        case coEventBmp:res = pmgEO_->mgCoEventBmp_.Connecting(); break;
          //default: assart(0); break;
        }

        if (res == rqOK)
        {
          DoCheckOut(TRUE);//継承先の通信成功を呼び出す
          EndConnectOK();//とりあえず全て表示は消す。必要あればメッセージボックスをDoConnectOK内で出す
        }
        else if (res == rqNG)
        {
          DoCheckOut(FALSE);//継承先の通信失敗を呼び出す
          EndConnectNG(NGNUM);//一括して通信エラーメッセージボックス
        }

      }
      else if (IsConnect())
      {
        //通信
        enRequestState res = pmgEO_->mgPo_.Connecting();

        if (res == rqOK)
        {
          if (pmgEO_->mgPo_.IsGotoTitle())
          {
            EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());

            stLy_.gototitle_f_ = TRUE;

            //とりあえず０ページに戻す（ぎんぎんゼットなど連コイン状態になる）
            ChangePage(0);
            //GotoTitle();
          }
          else
          {
            DoConnect(TRUE);//継承先の通信成功を呼び出す
            EndConnectOK();//とりあえず全て表示は消す。必要あればメッセージボックスをDoConnectOK内で出す
          }
        }
        else if (res == rqNG)
        {
          //リトライする
          if (pmgEO_->mgPo_.IsNeedRetry())
          {
            pmgEO_->mgPo_.RetryConnect();
          }
          else
          {
            //リトライしてもだめ
            DoConnect(FALSE);//継承先の通信失敗を呼び出す
            EndConnectNG(NGNUM);//一括して通信エラーメッセージボックス
          }
        }
      }
      else
      {
        //メッセージボックス押されたらHUD完全終了
        //        if (frame_->IsAct()) SetHudEnd();
      }

      if (frame_->IsAct())
      {
        SetHudEnd();

        //タイトルに戻る
        if (stLy_.gototitle_f_) GotoTitle();
      }

    }

  }

}
