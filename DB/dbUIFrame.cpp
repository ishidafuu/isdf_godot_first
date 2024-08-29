#include "dbUIFrame.h"
#include "dbUGHud.h"
#include "dbUGFilter.h"
#include "dbUGHeadder.h"
#include "dbUGFooter.h"
#include "dbLyBarGen.h"
namespace db
{
  //コンストラクタ
  TUIFrame::TUIFrame(TScene* pScene) :TUIGenLayer(pScene)//, ughud_(this, pmgEO, pmgUI)
  {
    stLy_.Init();
    ughud_ = new TUGHud(this);
    ugheadder_ = NULL;
    ugfooter_ = NULL;
  }

  TUIFrame::~TUIFrame()
  {
    Freemem(ughud_);
  }

  void TUIFrame::SetHedder(TLyBarGen* lybar)
  {
    ugheadder_ = &lybar->ugheadder_;
    ugfooter_ = &lybar->ugfooter_;
    ughud_->SetHedder(ugheadder_, ugfooter_);
    lybar->SetFooterPointer();//Barもレイヤとしてヘッダフッタのポインタセット
  }

  void TUIFrame::SetHedderEnable()
  {
    if (ugheadder_ != NULL) ugheadder_->SetEnable();
    if (ugfooter_ != NULL) ugfooter_->SetEnable();
  }
  void TUIFrame::SetHedderDisable()
  {
    if (ugheadder_ != NULL) ugheadder_->SetDisable();
    if (ugfooter_ != NULL) ugfooter_->SetDisable();
  }

  BOOL TUIFrame::IsAni()
  {
    return ughud_->IsAni();
  }
  BOOL TUIFrame::IsMsg()
  {
    return ughud_->IsMsg();
  }
  BOOL TUIFrame::IsPrev()
  {
    return ughud_->IsPrev();
  }
  
  void TUIFrame::SetAniStart()
  {
    ughud_->SetAniStart();
    pmgUI_->SetModal(TRUE);
  }
  void TUIFrame::SetAniStart_Login()
  {
    ughud_->SetAniStart_Login();
    pmgUI_->SetModal(TRUE);
  }
  void TUIFrame::SetAniStart_Auth()
  {
    ughud_->SetAniStart_Auth();
    pmgUI_->SetModal(TRUE);
  }
  void TUIFrame::SetAniEnd()
  {
    ughud_->SetAniEnd();
    pmgUI_->SetModal(FALSE);
  }

  void TUIFrame::EndConnectNG(s32 code, const char* msg)
  {
    //メッセージボックスを出して終了
    ughud_->SetErrorMessage(code, msg);
    pmgUI_->SetModal(TRUE);//HUDの中でやる 全体に影響アルのでやっぱレイヤで管理
  }

  void TUIFrame::SetHudMsg(BOOL cancel_f, BOOL tsh_f, STRVECTOR* strvec)
  {
    pmgUI_->SetModal(TRUE);
    ughud_->SetMessage(cancel_f, tsh_f, strvec);
  }
  void TUIFrame::SetHudMsgTodd(STRVECTOR* strvec, s32 num, BOOL get_f)
  {
    pmgUI_->SetModal(TRUE);
    ughud_->SetMessageTodd(strvec, num, get_f);
  }
  void TUIFrame::SetHudMsgStar(STRVECTOR* strvec, s32 num)
  {
    pmgUI_->SetModal(TRUE);
    ughud_->SetMessageStar(strvec, num);
  }
  void TUIFrame::SetHudMsgStarSupply(s32 mid_supply, BOOL quse_f)
  {
    pmgUI_->SetModal(TRUE);
    ughud_->SetMessageStarSupply(mid_supply, quse_f);
  }
  void TUIFrame::SetHudMsgStarGacha(s32 mid_gacha)
  {
    pmgUI_->SetModal(TRUE);
    ughud_->SetMessageStarGacha(mid_gacha);
  }
  void TUIFrame::SetHudMsgStarNaBox(s32 mid_nabox, BOOL otona_f)
  {
    pmgUI_->SetModal(TRUE);
    ughud_->SetMessageStarNaBox(mid_nabox, otona_f);
  }
  void TUIFrame::SetHudFilter(BOOL draw_f)
  {
	  ughud_->ugitemview_.SetDrawFilter(draw_f);
  }
  void TUIFrame::SetHudGKErrorMsg()
  {
    pmgUI_->SetModal(TRUE);
    ughud_->SetGKErrorMessage();
  }
  void TUIFrame::SetMsgEnd()
  {
    if (ughud_->IsMsg())
    {
      pmgUI_->SetModal(FALSE);//モーダル状態終了
      ughud_->SetMsgEnd();//けす
    }
  }
  void TUIFrame::SetItemViewEnd()
  {
    if (ughud_->IsItemView())
    {
      pmgUI_->SetModal(FALSE);//モーダル状態終了
      ughud_->SetItemViewEnd();//けす
    }
  }
  void TUIFrame::SetItemViewDraw(BOOL draw_f)
  {
	  pmgUI_->SetModal(draw_f);//モーダル状態終了
	  ughud_->SetItemViewDraw(draw_f);//けす
  }
  void TUIFrame::SetPrevEnd()
  {
    if (ughud_->IsPrev())
    {
      pmgUI_->SetModal(FALSE);//モーダル状態終了
      ughud_->SetPrevEnd();//けす
			ughud_->SetMixPrevEnd();
    }
  }

  BOOL TUIFrame::IsHudOK()
  {
    return (ughud_->IsActRes(enUGHudAct_OK) || ughud_->IsActRes(enUGHudAct_InputOK));
  }
  BOOL TUIFrame::IsHudCancel()
  {
    return (ughud_->IsActRes(enUGHudAct_Cancel) || ughud_->IsActRes(enUGHudAct_InputCancel));
  }
  BOOL TUIFrame::IsHudPrevBack()
  {
    return ughud_->IsActRes(enUGHudAct_PrevBack);
  }
  BOOL TUIFrame::IsHudPrevMix()
  {
	  return ughud_->IsActRes(enUGHudAct_PrevMix);
  }
  BOOL TUIFrame::IsHudPrevMixClear()
  {
	  return ughud_->IsActRes(enUGHudAct_PrevMixClear);
  }
  BOOL TUIFrame::IsHudPrevHit()
  {
    return ughud_->IsActRes(enUGHudAct_PrevHit);
  }
  BOOL TUIFrame::IsHudItemView()
  {
    return ughud_->IsActRes(enUGHudAct_ItemView);
  }
	/* 2015/06/19 Koike Added */
	BOOL TUIFrame::IsHudPrevHaveMix()
	{
		return ughud_->IsActRes(enUGHudAct_PrevHaveMix);
	}
	BOOL TUIFrame::IsHudMixPrevUndo()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevUndo);
	}
	BOOL TUIFrame::IsHudMixPrevBall_0()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevBall0);
	}
	BOOL TUIFrame::IsHudMixPrevBall_1()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevBall1);
	}
	BOOL TUIFrame::IsHudMixPrevBall_2()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevBall2);
	}
	BOOL TUIFrame::IsHudMixPrevBall_3()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevBall3);
	}
	BOOL TUIFrame::IsHudMixPrevBall_4()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevBall4);
	}
	BOOL TUIFrame::IsHudMixPrevBall_5()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevBall5);
	}
	BOOL TUIFrame::IsHudMixPrevBall_6()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevBall6);
	}
	BOOL TUIFrame::IsHudMixPrevBall_7()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevBall7);
	}
	BOOL TUIFrame::IsHudMixPrevSelectBall()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevSelect);
	}
	BOOL TUIFrame::IsHudMixPrevSelectShot()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevSelectOK);
	}
	BOOL TUIFrame::IsHudMixPrevCancel()
	{
		return ughud_->IsActRes(enUGHudAct_MixPrevCancel);
	}

	
  BOOL TUIFrame::IsHudIntro(void)
  {
	  return ughud_->ugitemview_.ugbtn_intro_.IsAct();
  } 
  TUGItemViewParam TUIFrame::GetHudItemView(void)
  {
	  return ughud_->ugitemview_.viewItemParam_;
  }
  void TUIFrame::SetHudItemView_GetLog(BOOL msgok_f, BOOL leftpos_f, BOOL filter_f)
	{
    pmgUI_->SetModal(TRUE);//モーダル
		ughud_->SetItemView_GetLog(msgok_f, leftpos_f, filter_f);
  }
  void TUIFrame::SetHudItemView_GetLogSingle(s32 logno, BOOL msgok_f, BOOL leftpos_f)
  {
    pmgUI_->SetModal(TRUE);//モーダル
    ughud_->SetItemView_GetLogSingle(logno, msgok_f, leftpos_f);
  }
  void TUIFrame::SetHudItemView_GachaCnt(s64 mid_gachacnt)
  {
    pmgUI_->SetModal(TRUE);//モーダル
    ughud_->SetItemView_GachaCnt(mid_gachacnt);
  }
  void TUIFrame::SetHudItemView_GachaInit()
  {
    ughud_->SetItemView_GachaInit();
  }
  void TUIFrame::SetHudItemView_GachaGet(s32 logno)
  {
    //pmgUI_->SetModal(TRUE);//モーダル
    ughud_->SetItemView_GachaGet(logno);
  }
  void TUIFrame::SetHudItemView_NaBoxCnt(s64 mid_naboxcnt)
  {
    pmgUI_->SetModal(TRUE);//モーダル
    ughud_->SetItemView_NaBoxCnt(mid_naboxcnt);
  }
  void TUIFrame::SetHudItemView_HukuCnt(s64 mid_hukucnt)
  {
    pmgUI_->SetModal(TRUE);//モーダル
    ughud_->SetItemView_HukuCnt(mid_hukucnt);
  }
  void TUIFrame::SetHudItemView_Reward(s64 mid_ranking, s32 rank, BOOL msgok_f, BOOL leftpos_f, BOOL filter_f)
  {
	  pmgUI_->SetModal(TRUE);//モーダル
	  ughud_->SetItemView_Reward(mid_ranking, rank, msgok_f, leftpos_f, filter_f);
  }
  void TUIFrame::SetHudItemView_View(s64 itemType, s64 itemID, s64 itemNum, BOOL msgok_f, BOOL leftpos_f)
  {
    pmgUI_->SetModal(TRUE);//モーダル
    ughud_->SetItemView_View(itemType, itemID, itemNum, msgok_f, leftpos_f);
  }
  void TUIFrame::SetHudItemView_Shot(SOZAIDTVECTOR sozai, BOOL leftpos_f)
  {
	  pmgUI_->SetModal(TRUE);//モーダル
	  ughud_->SetItemView_Shot(sozai, leftpos_f);
  }
  void TUIFrame::SetHudItemView_SozaiList(SOZAIDTVECTOR sozai, BOOL leftpos_f)
  {
	  pmgUI_->SetModal(TRUE);//モーダル
	  ughud_->SetItemView_SozaiList(sozai, leftpos_f);
  }
  void TUIFrame::SetHudItemView_Drop(s64 itemType, s64 itemID, s64 itemNum, s32 tokkouNum, s32 tokkouMag, BOOL comp_f)
  {
    pmgUI_->SetModal(TRUE);//モーダル
    ughud_->SetItemView_Drop(itemType, itemID, itemNum, tokkouNum, tokkouMag, comp_f);
  }
  ////プレビュー
  //void TUIFrame::SetHudPrev(s64 id_team, s64 id_shot, BOOL backbtn_f)
  //{
  //  pmgUI_->SetModal(TRUE);//モーダル
  //  ughud_->SetPrev(id_team, id_shot, backbtn_f);
  //}
  void TUIFrame::SetHudPrev(const TPrevParam& param)
  {
	  pmgUI_->SetModal(TRUE);//モーダル
	  ughud_->SetPrev(param);
  }
  //プレビュー
  void TUIFrame::SetHudPrev_Mixed(LONGVECTOR* id_shot_mixvec)
  {
    pmgUI_->SetModal(TRUE);//モーダル
    ughud_->SetPrev_Mixed(id_shot_mixvec);
  }
 
  void TUIFrame::SetHudPrevChangeShot(const SOZAIDTVECTOR& sozaivec)
  {
    ughud_->SetPrevChangeShot(sozaivec);
  }

	/* 2015/06/19 Koike Added */
	void TUIFrame::SetHudPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec)
	{
		pmgUI_->SetModal(TRUE);//モーダル
		ughud_->SetPrev_Have_Mixed(id_shot_mixvec);
	}

  void TUIFrame::SetHudPrevMixSozai(s64 id_sozai, BOOL isRndSet)
  {
	  ughud_->SetPrevMixSozai(id_sozai, isRndSet);
  }
  void TUIFrame::SetHudPrevMixShot(s64 id_shot, BOOL isRndSet)
  {
	  ughud_->SetPrevMixShot(id_shot, isRndSet);
  }

	/* 2015/06/30 Koike Added */
	void TUIFrame::SetHudMixPrev(const TMixPrevParam& param)
	{
		pmgUI_->SetModal(TRUE);//モーダル
		ughud_->SetMixPrev(param);
	}
	void TUIFrame::SetHudMixPrev_Mixed(LONGVECTOR* id_shot_mixvec)
	{
		pmgUI_->SetModal(TRUE);//モーダル
		ughud_->SetMixPrev_Mixed(id_shot_mixvec);
	}

	void TUIFrame::SetHudMixPrevChangeShot(const SOZAIDTVECTOR& sozaivec)
	{
		ughud_->SetMixPrevChangeShot(sozaivec);
	}
	void TUIFrame::SetHudMixPrev_Have_Mixed(LONGVECTOR* id_shot_mixvec)
	{
		pmgUI_->SetModal(TRUE);//モーダル
		ughud_->SetMixPrev_Have_Mixed(id_shot_mixvec);
	}
	void TUIFrame::SetHudMixPrevMixSozai(s64 id_sozai, BOOL isRndSet)
	{
		ughud_->SetMixPrevMixSozai(id_sozai, isRndSet);
	}
	void TUIFrame::SetHudMixPrevMixShot(s64 id_shot, BOOL isRndSet)
	{
		ughud_->SetMixPrevMixShot(id_shot, isRndSet);
	}
	s32 TUIFrame::SetHudMixPrevTouchMode(LONGVECTOR* id_shot_mixvec, s32 iconNo)
	{
		return ughud_->SetMixPrevTouchMode(id_shot_mixvec, iconNo);
	}
	void TUIFrame::SetHudMixPrevSelectShot(s64 id_shot)
	{
		ughud_->SetMixPrevSlectShot(id_shot);
	}
	LONGVECTOR TUIFrame::GetHudMixPrevChangeVec()
	{
		return ughud_->GetMixPrevChangeVec();
	}
	void TUIFrame::SetHudResetField(BOOL fieldflg)
	{
		ughud_->ResetField(fieldflg);
	}

	//-------------------------------------------------------

  void TUIFrame::SetHudInput(s32 outw, BOOL alponly_f)
  {
    pmgUI_->SetModal(TRUE);//モーダル
    ughud_->SetInput(outw, alponly_f);
  }
  void TUIFrame::SetHudInput(s32 outw, BOOL alponly_f, INTVECTOR defmcvec)
  {
    pmgUI_->SetModal(TRUE);//モーダル
    ughud_->SetInput(outw, alponly_f, defmcvec);
  }
  void TUIFrame::SetHudVSWait_Search()
  {
    ughud_->SetVSWait_Search();
  }
  void TUIFrame::SetHudVSWaitEnd()
  {
    ughud_->SetVSWaitEnd();
  }
  void TUIFrame::SetHudVSWait_EnWait()
  {
    ughud_->SetVSWait_EnWait();
  }
  void TUIFrame::SetHudCount(s32 count)
  {
    ughud_->SetCount(count);
  }
  void TUIFrame::SetHudCountEnd()
  {
    ughud_->SetCountEnd();
  }

  std::string TUIFrame::HudOutputString()
  {
    return ughud_->OutputString();
  }
  BOOL TUIFrame::IsAct()
  {
    return (ughud_->IsActRes(enUGHudAct_OK)
      || ughud_->IsActRes(enUGHudAct_Cancel) 
      || ughud_->IsActRes(enUGHudAct_InputOK)
      || ughud_->IsActRes(enUGHudAct_InputCancel));
  }

  //とりあえず仮定義、継承先でオーバーロードする
  void TUIFrame::Load()//読み込み
  {
    field_ = MakeObj();
    ughud_->MakeGroup(field_);
    DoLoad();//継承先で定義
  }
  //更新
  void TUIFrame::Update(BOOL draw_f)
  {
    field_->Update(draw_f);
    ughud_->Update(draw_f);

    DoUpdate(draw_f);

    if (draw_f == FALSE)
    {
      if (ughud_->IsActRes(enUGHudAct_ItemView))
      {
        //ヘッダ解除とは限らない
        pmgUI_->SetModal(FALSE);//モーダルは解除
      }
      else if (ughud_->IsActRes(enUGHudAct_PrevBack))
      {
        pmgUI_->SetModal(FALSE);//モーダルは解除
      }
      else if (ughud_->IsActRes(enUGHudAct_InputOK) || ughud_->IsActRes(enUGHudAct_InputCancel))
      {
        pmgUI_->SetModal(FALSE);//モーダルは解除
      }
    }
  }
}
