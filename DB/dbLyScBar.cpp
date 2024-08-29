#include "dbLyScBar.h"
#include "dbSnScenario.h"

namespace db
{

  //コンストラクタ
  TLyScBar::TLyScBar(TSnScenario* pScene) :TLyBarGen(pScene, pScene->frame_)
  {
    pScene_ = pScene;
    scgrp_ = pScene->scgrp_;
    stScBar_.Init();
  }

  TLyScBar::~TLyScBar()
  {

  }
  void TLyScBar::DoOpenHeader()
  {

  }
  void TLyScBar::DoCloseHeader()
  {
    SetFoot_BtnHd();
    switch (stScBar_.nowLayer_)
    {
    case db::scl_Map: pScene_->lyMap_->ResetBtn();break;
    case db::scl_Enemy:pScene_->lyEnemy_->ResetBtn(); break;
    case db::scl_TSL:pScene_->lyTSL_->RetrunHedder(); break;
    }
  }
  void TLyScBar::DoJumpHeader()
  {
    if (pScene_->lyTechno_->IsReady()) pScene_->lyTechno_->Down();
    if (pScene_->lyProfile_->IsReady()) pScene_->lyProfile_->Down();
  }
  void TLyScBar::DoOpenTechno()
  {
    pScene_->lyTechno_->Ready();
  }
  void TLyScBar::DoOpenProfile()
  {
    pScene_->lyProfile_->Ready();
  }
  void TLyScBar::DoMoveFilter()
  {

  }
  void TLyScBar::DoMoveFilterAll()
  {
    switch (stScBar_.nowLayer_)
    {
    case scl_Home:pScene_->GoToHome(); break;
    case scl_VSFilter: MoveLayer(scl_VS); break;
    case scl_Shiai:pScene_->GoToShiai(); break;
    }
  }
  void TLyScBar::DoBackBtn()
  {
    switch (stScBar_.nowLayer_)
    {
    case scl_Map: MoveLayer(scl_Home); break;//ホームへ
    case scl_Enemy: MoveLayer(scl_Map); break;//マップへ
    //case scl_TSL:
    //{
    //  SetHudMsg(TRUE, FALSE, "しあい＿かいひ", "スタミナは＿しょうひされます", "よろしいですか？");
    //}
    //  break;//マップへ
    }
  }
  void TLyScBar::MoveLayer(enScLayer mvlayer)
  {
    switch (mvlayer)
    {
    case scl_Map://マップ選択に戻る
      pScene_->lyEnemy_->Wait(TRUE);
      pScene_->lyTSL_->Wait(TRUE);
      pScene_->lyMap_->Return();
      //SetFoot_Btn();
      break;
    case scl_Enemy://敵選択
      pScene_->lyTSL_->Wait(TRUE);
      pScene_->lyEnemy_->RedayEnemy(pmgEO_->stSeq_.GetMidMapTip(), pmgEO_->stSeq_.GetLNoMapTip());
      break;
    case scl_TSL://チーム選択
      pScene_->lyEnemy_->Wait(TRUE);
      pScene_->lyMap_->Wait(TRUE);
      pScene_->lyTSL_->Ready();
      break;
    case scl_VSFilter: //ＶＳフィルタ
      SetFilterAll(FALSE);
      break;
    case scl_VS: //ＶＳ
      pScene_->lyTSL_->Wait(TRUE);
      pScene_->lyVS_->Ready();
      //ugheadder_.SetDraw(FALSE);
      //ugfooter_.SetDraw(FALSE);
      ugfilter_.OffFilter(2, FALSE);
      SetNoneBar(TRUE);
      break;
    case scl_Shiai://試合へ
      SetFilterAll(FALSE);
      break;
    case scl_Home://ホームへ
      SetFilterAll(FALSE);
      break;
    }

    //レイヤ変更
    stScBar_.nowLayer_ = mvlayer;
  }

}