#include "dbLyTuScBar.h"
#include "dbSnScenario.h"


namespace db
{

  //コンストラクタ
  TLyTuScBar::TLyTuScBar(TSnScenario* pScene) 
	  :TLyBarGen(pScene, pScene->frame_)
	  ,ugserifu_(this)
	  ,ugfinger_(this)
	  , ugfinger2_(this)
	  , ugfinger3_(this)
  {
    pScene_ = pScene;
    scgrp_ = pScene->scgrp_;
    stScBar_.Init();
  }

  TLyTuScBar::~TLyTuScBar()
  {

  }
  void TLyTuScBar::DoOpenHeader()
  {

  }
  void TLyTuScBar::DoJumpHeader()
  {
    if (pScene_->lyTechno_->IsReady()) pScene_->lyTechno_->Down();
    if (pScene_->lyProfile_->IsReady()) pScene_->lyProfile_->Down();
  }
  void TLyTuScBar::DoCloseHeader()
  {
    SetFoot_BtnHd();
    switch (stScBar_.nowLayer_)
    {
    case db::tscl_Map: pScene_->lyTuMap_->ResetBtn();break;
    case db::tscl_Enemy:pScene_->lyTuEnemy_->ResetBtn(); break;
    case db::tscl_TSL:pScene_->lyTuTSL_->ResetBtn(); break;
    }
  }
  void TLyTuScBar::DoOpenTechno()
  {
    pScene_->lyTechno_->Ready();
  }
  void TLyTuScBar::DoOpenProfile()
  {
    pScene_->lyProfile_->Ready();
  }
  void TLyTuScBar::DoMoveFilter()
  {

  }
  void TLyTuScBar::DoMoveFilterAll()
  {
    switch (stScBar_.nowLayer_)
    {
    case tscl_Home:pScene_->GoToHome(); break;
    case tscl_VSFilter: MoveLayer(tscl_VS); break;
    case tscl_Shiai:pScene_->GoToShiai(); break;
    }
  }
  void TLyTuScBar::DoBackBtn()
  {
    switch (stScBar_.nowLayer_)
    {
    case tscl_Map: MoveLayer(tscl_Home); break;//ホームへ
    case tscl_Enemy: MoveLayer(tscl_Map); break;//マップへ
    //case tscl_TSL:
    //{
    //  SetHudMsg(TRUE, FALSE, "しあい＿かいひ", "スタミナは＿しょうひされます", "よろしいですか？");
    //}
    //  break;//マップへ
    }
  }
  void TLyTuScBar::DoUpdate(BOOL draw_f)
  {
    TLyBarGen::DoUpdate(draw_f);
    ugserifu_.Update(draw_f);	//セリフ
    ugfinger_.Update(draw_f);
	ugfinger2_.Update(draw_f);
	ugfinger3_.Update(draw_f);
  }
  void TLyTuScBar::MoveLayer(enTuScLayer mvlayer)
  {
    switch (mvlayer)
    {
    case tscl_Map://マップ選択に戻る
      pScene_->lyTuEnemy_->Wait(TRUE);
      pScene_->lyTuTSL_->Wait(TRUE);
      //SetFoot_Btn();
      break;
    case tscl_Enemy://敵選択
      pScene_->lyTuTSL_->Wait(TRUE);
      pScene_->lyTuEnemy_->RedayEnemy(pmgEO_->stSeq_.GetMidMapTip(), pmgEO_->stSeq_.GetLNoMapTip());
      break;
    case tscl_TSL://チーム選択
      pScene_->lyTuEnemy_->Wait(TRUE);
      pScene_->lyTuMap_->Wait(TRUE);
      pScene_->lyTuTSL_->Ready();
      break;

    case tscl_VSFilter: //ＶＳフィルタ
      SetFilterAll(FALSE);
      break;
    case tscl_VS: //ＶＳ
      pScene_->lyTuTSL_->Wait(TRUE);
      pScene_->lyTuVS_->Ready();
      //ugheadder_.SetDraw(FALSE);
      //ugfooter_.SetDraw(FALSE);
      ugfilter_.OffFilter(2, FALSE);
      SetNoneBar(TRUE);
      break;

    case tscl_Shiai://試合へ
      SetFilterAll(FALSE);
      break;
    case tscl_Home://ホームへ
      SetFilterAll(FALSE);
      break;
    }

    //レイヤ変更
    stScBar_.nowLayer_ = mvlayer;
  }


  void TLyTuScBar::DoReady(void)
  {
	  ugserifu_.MakeGroup(field_, 2, enSerifuFrame_Wide);
	  ugserifu_.SetParent(field_);
	  ugserifu_.SetPos(-90, 30);
	  ugserifu_.SetDraw(FALSE);
	  ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);

	  // チュートリアル用指
	  ugfinger_.MakeGroup(field_);
	  ugfinger_.SetDepth(UIDP_07BUTTONTOP);
	  ugfinger_.SetDraw(FALSE);
	 
	  ugfinger2_.MakeGroup(field_);
    ugfinger2_.SetDepth(UIDP_07BUTTONTOP);
	  ugfinger2_.SetDraw(FALSE);

	  ugfinger3_.MakeGroup(field_);
    ugfinger3_.SetDepth(UIDP_07BUTTONTOP);
	  ugfinger3_.SetDraw(FALSE);
  }
}