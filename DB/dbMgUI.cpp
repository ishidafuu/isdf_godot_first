#include "dbMgUI.h"
#include "dbUILayer.h"
#include "dbUIFrame.h"
#include "dbUIObj.h"
namespace db
{

  //コンストラクタ
  TMgUI::TMgUI(TMgEngineObj* pmgEO) : pmgEO_(pmgEO)
  {
    settex_f_ = FALSE;
    modal_f_ = FALSE;
    action_c_ = 0;
    needReload_f_ = FALSE;
    waitSceneMove_f_ = FALSE;
    //bgtype_ = bg_None;
  }

  TMgUI::~TMgUI()
  {
    if (settex_f_)
    {
      Freemem(pgr_UIBG_);//メニュー背景
      Freemem(pgr_UIFilter_);//スクリプト用フィルタ
      bmo_UIFilter_.DeleteTex();
      bmo_UIBG_.DeleteTex();
      FreeEvIcon();


      for (s32 i = 0; i < pgr_uggrp_.size(); ++i) Freemem(pgr_uggrp_[i]);
      for (s32 i = 0; i < bmo_uggrp_.size(); ++i)
      {
        bmo_uggrp_[i]->DeleteTex();
        Freemem(bmo_uggrp_[i]);
      }

      for (s32 i = 0; i < pgr_uggrpart_.size(); ++i) Freemem(pgr_uggrpart_[i]);
      for (s32 i = 0; i < bmo_uggrpart_.size(); ++i)
      {
        bmo_uggrpart_[i]->DeleteTex();
        Freemem(bmo_uggrpart_[i]);
      }
    }
  }

  void TMgUI::FreeEvIcon()
  {
    for (s32 i = 0; i < pgr_evicon_.size(); ++i) Freemem(pgr_evicon_[i]);
    for (s32 i = 0; i < bmo_evicon_.size(); ++i)
    {
      bmo_evicon_[i]->DeleteTex();
      Freemem(bmo_evicon_[i]);
    }
    pgr_evicon_.clear();
    bmo_evicon_.clear();
  }

  //初期化
  void TMgUI::Init(enSceneName scenename)
  {
    //レイヤの解放は宣言元で行う
    layers_.clear();
    scenename_ = scenename;
    modal_f_ = FALSE;
    action_c_ = 0;
    waitSceneMove_f_ = FALSE;

    if (scenename == snEND) uiobjlist_.clear();

    //フォント情報ポインタ解放
    pmgEO_->mgFont_.FreeFontSt();

    if (settex_f_ == FALSE)
    {
      //オブジェフィルター（スクリプトなどから呼ぶ用）
      lib_rgb::TRGB ofcol;
      ofcol.SetCol(0xFF, 0xFF, 0xFF);
      mid::midBltGradationTexture(&bmo_UIFilter_, ofcol, ofcol);
      //スクリプトフィルタ
      pgr_UIFilter_ = new base::TGrp(&(pmgEO_->mgOrder_), &(bmo_UIFilter_));
      //メニュー背景
      pgr_UIBG_ = new base::TGrBG(&(pmgEO_->mgOrder_), &(bmo_UIBG_), &pmgEO_->mgCamera_);

      //UG用画像読み込み
      for (s32 i = 0; i < UGGrpEND; ++i)
      {
        bmo_uggrp_.push_back(new mid::TBmpObj());
        mid::midLoadCI(bmo_uggrp_[i], "ug", UGGRP_STR[i].c_str());
        pgr_uggrp_.push_back(new base::TGrp(&(pmgEO_->mgOrder_), bmo_uggrp_[i]));
      }

      //UG用画像読み込み
      for (s32 i = 0; i < UGGrPartEND; ++i)
      {
        s32 www = (i * 2);
        s32 hhh = (www + 1);
        bmo_uggrpart_.push_back(new mid::TBmpObj());
        mid::midLoadCI(bmo_uggrpart_[i], "ugp", UGGRPART_STR[i].c_str());
        pgr_uggrpart_.push_back(new base::TGrPart(&(pmgEO_->mgOrder_), bmo_uggrpart_[i], UGGRPART_SIZE[www], UGGRPART_SIZE[hhh]));
      }


      //イベントアイコン読み込み
      LoadEvIcon();


      settex_f_ = TRUE;
    }
  }

  void TMgUI::LoadEvIcon()
  {
    //解放
    FreeEvIcon();

    const s32 EVICON = 4;

    for (s32 i = 0; i < EVICON; ++i)
    {
      bmo_evicon_.push_back(new mid::TBmpObj());
      std::string stricon = "ev_icon" + lib_str::IntToStr1b(i, 1);
      mid::midLoadCI(bmo_evicon_[i], "event", stricon.c_str());
      pgr_evicon_.push_back(new base::TGrp(&(pmgEO_->mgOrder_), bmo_evicon_[i]));
    }
  }

  void TMgUI::SetUIFrame(TUIFrame* pframe)
  {
    frame_ = pframe;
  }
  void TMgUI::SetUILayer(TUILayer* pui)//ＵＩレイヤセット
  {
    layers_.push_back(pui);
  }
  void TMgUI::FreeUILayer()//ＵＩレイヤセットされたものを全て解放
  {
    //waitSceneMove_f_を建てることでシナリオにうつるときのuiobjlist_開放をカット
    waitSceneMove_f_ = TRUE;
    for (s32 i = 0; i < layers_.size(); ++i) Freemem(layers_[i]);
    waitSceneMove_f_ = FALSE;
  }

  //背景読み込み
  void TMgUI::LoadBG(const char* filename)
  {
    mid::midLoadCI_NoColKey(pgr_UIBG_->pbmo_, "bg", filename);
  }
  //背景読み込み
  void TMgUI::LoadCourt(s64 mid_court)
  {
    if (mid_court == 0) return;
    std::string courtname = "c_" + pmgEO_->mgDt_.dtCourt_.GetStrPK(mid_court, enDtCourtStr_filename);
    mid::midLoadCI(pgr_UIBG_->pbmo_, "court", courtname.c_str());

    mid::midLog("LoadCourt");
    //if (mid::midIsDXL())
    //{
    //  std::string courtname = "c_" + pmgEO_->mgDt_.dtCourt_.GetStrPK(mid_court, enDtCourtStr_filename);
    //  mid::midLoadCI(pgr_UIBG_->pbmo_, "court", courtname.c_str());
    //}
    //else
    //{
    //  s64 mid_haikei = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei);
    //  if (mid_haikei == 0) mid_haikei = 1;
    //  std::string courtname = "c_" + pmgEO_->mgDt_.dtCourt_.GetStrPK(mid_haikei, enDtCourtStr_filename);
    //  mid::midLoadCI_NoColKey(pgr_UIBG_->pbmo_, "court", courtname.c_str());
    //}

  }

  void TMgUI::LoadLayer()
  {
    //フレームのロード
    frame_->Load();
    //レイヤのロード
    for (s32 i = 0; i < layers_.size(); ++i) layers_[i]->Load();
  }
  void TMgUI::ReloadTex()//テクスチャ再読み込み（アンドロイドむけ）
  {
    //テクスチャリロード検知
    if (mid::midIsNeedReload() == FALSE) return;



    pmgEO_->mgFont_.SwitchFontSpriteSt();
    for (auto& item : uiobjlist_) item->ReloadTex();

    lib_rgb::TRGB ofcol;
    ofcol.SetCol(0xFF, 0xFF, 0xFF);
    mid::midLog("midBltGradationTexture");
    mid::midBltGradationTexture(&bmo_UIFilter_, ofcol, ofcol);
    mid::midLog("bmo_UIBG_.ReloadTex();");
    bmo_UIBG_.ReloadTex();
    mid::midLog("bmo_uggrp_.ReloadTex();");
    for (auto& item : bmo_uggrp_) item->ReloadTex();
    mid::midLog("bmo_uggrpart_.ReloadTex();");
    for (auto& item : bmo_uggrpart_) item->ReloadTex();
    mid::midLog("bmo_evicon_.ReloadTex();");
    for (auto& item : bmo_evicon_) item->ReloadTex();
    mid::midLog("uiobjlist_.ReloadTex();");



    for (s32 i = 0; i < layers_.size(); ++i)
    {
      layers_[i]->ReloadTex();
    }

    pmgEO_->mgGrp_.ReloadTex(scenename_);


    mid::midSetNeedReload(FALSE);
  }
  void TMgUI::SetWaitSceneMove()
  {
    waitSceneMove_f_ = TRUE;
  }
  BOOL TMgUI::IsWaitSceneMove()//シーン移動待ち
  {
    return waitSceneMove_f_;
  }
  //グラフィックの取得
  base::TGrp* TMgUI::GetUGGrp(enUGGrp gtype)
  {
    return pgr_uggrp_[gtype];
  }

  base::TGrPart* TMgUI::GetUGGrPart(enUGGrPart gptype)
  {
    return pgr_uggrpart_[gptype];
  }

  base::TGrp* TMgUI::GetEvIcon(s32 iconno)
  {
    return pgr_evicon_[iconno];
  }

  void TMgUI::SetModal(BOOL modal_f)//モーダル窓
  {
    modal_f_ = modal_f;
  }
  BOOL TMgUI::IsModal()//モーダル窓
  {
    return modal_f_;
  }


  void TMgUI::SetConnect(BOOL connect_f)//モーダル窓
  {
    connect_f_ = connect_f;
  }
  BOOL TMgUI::IsConnect()//モーダル窓
  {
    return connect_f_;
  }
  void TMgUI::SetAction()//現フレームでアクションが発生した（２つ以上発生しないようにする）
  {
    action_c_ = 1;
  }
  BOOL TMgUI::IsAction()//現フレームでアクションが発生した
  {
    return (action_c_ > 0);
  }

  s32 TMgUI::GetDepth()
  {
    return depth_c_;// ++;
  }
  void TMgUI::AddUIObj(TUIObj* obj)//再構築用
  {
    if (mid::midIsADR()) uiobjlist_.push_back(obj);
  }
  void TMgUI::RemoveUIObj(TUIObj* obj)//再構築用
  {
    if (mid::midIsADR())
    {
      if (IsWaitSceneMove() == FALSE) uiobjlist_.remove(obj);
    }
  }
  //更新
  void TMgUI::Update()
  {

//    //テクスチャリロード検知
//    if (mid::midIsNeedReload())
//    {
//      //pmgEO_->mgSound_.RestartBGM();
//
////      ReloadTex();
//      ReloadTex();
//      mid::midSetNeedReload(FALSE);
//    }

    lib_num::AprTo0(&action_c_);//アクション発生フラグリセット
    //レイヤの更新
    frame_->Update(FALSE);
    for (s32 i = 0; i < layers_.size(); ++i) layers_[i]->Update(FALSE);
  }
  //描画
  void TMgUI::Draw(BOOL bg_f)
  {
    depth_c_ = 0;
    //レイヤの描画
    frame_->Update(TRUE);
    for (s32 i = 0; i < layers_.size(); ++i) layers_[i]->Update(TRUE);
    //背景描画
    if (bg_f) pgr_UIBG_->OrderDrawBG(FALSE, FALSE);
  }

}
